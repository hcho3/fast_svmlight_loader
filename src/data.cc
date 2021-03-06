/*!
 * Copyright (c) 2017 by Contributors
 * \file data.h
 * \author Philip Cho
 * \brief Input data structure of fast_svmlight_loader
 */

#include <fast_svmlight_loader/data.h>
#include <fast_svmlight_loader/omp.h>
#include <memory>
#include <limits>
#include <cstdint>

namespace fast_svmlight_loader {

DMatrix*
DMatrix::Create(const char* filename, int nthread, int verbose) {
  std::unique_ptr<dmlc::Parser<uint32_t>> parser(
    dmlc::Parser<uint32_t>::Create(filename, 0, 1, "libsvm"));
  const int max_thread = omp_get_max_threads();
  nthread = (nthread == 0) ? max_thread : std::min(nthread, max_thread);

  DMatrix* dmat = new DMatrix();
  dmat->Clear();
  auto& data_ = dmat->data;
  auto& col_ind_ = dmat->col_ind;
  auto& row_ptr_ = dmat->row_ptr;
  auto& num_row_ = dmat->num_row;
  auto& num_col_ = dmat->num_col;
  auto& nelem_ = dmat->nelem;
  auto& labels_ = dmat->labels;
  auto& weights_ = dmat->weights;

  std::vector<size_t> max_col_ind(nthread, 0);
  parser->BeforeFirst();
  while (parser->Next()) {
    const dmlc::RowBlock<uint32_t>& batch = parser->Value();
    if (batch.label != nullptr) {
      for (const dmlc::real_t* p = batch.label;
           p < batch.label + batch.size; ++p) {
        labels_.push_back(static_cast<float>(*p));
      }
    }
    if (batch.weight != nullptr) {
      for (const dmlc::real_t* p = batch.weight;
            p < batch.weight + batch.size; ++p) {
        weights_.push_back(static_cast<float>(*p));
      }
    }
    num_row_ += batch.size;
    nelem_ += batch.offset[batch.size];
    const size_t top = data_.size();
    data_.resize(top + batch.offset[batch.size] - batch.offset[0]);
    col_ind_.resize(top + batch.offset[batch.size] - batch.offset[0]);
    CHECK_LT(static_cast<int64_t>(batch.offset[batch.size]),
             std::numeric_limits<int64_t>::max());
    #pragma omp parallel for schedule(static) num_threads(nthread)
    for (int64_t i = static_cast<int64_t>(batch.offset[0]);
                 i < static_cast<int64_t>(batch.offset[batch.size]); ++i) {
      const int tid = omp_get_thread_num();
      const uint32_t index = batch.index[i];
      const float fvalue = (batch.value == nullptr) ? 1.0f :
                           static_cast<float>(batch.value[i]);
      const size_t offset = top + i - batch.offset[0];
      data_[offset] = fvalue;
      col_ind_[offset] = index;
      max_col_ind[tid] = std::max(max_col_ind[tid],
                                  static_cast<size_t>(index));
    }
    const size_t rtop = row_ptr_.size();
    row_ptr_.resize(rtop + batch.size);
    CHECK_LT(static_cast<int64_t>(batch.size),
             std::numeric_limits<int64_t>::max());
    #pragma omp parallel for schedule(static) num_threads(nthread)
    for (int64_t i = 0; i < static_cast<int64_t>(batch.size); ++i) {
      row_ptr_[rtop + i]
        = row_ptr_[rtop - 1] + batch.offset[i + 1] - batch.offset[0];
    }
    if (verbose > 0) {
      LOG(INFO) << num_row_ << " rows read into memory";
    }
  }
  num_col_ = *std::max_element(max_col_ind.begin(), max_col_ind.end()) + 1;
  // one label and weight per instance
  if (!labels_.empty()) {
    CHECK_EQ(labels_.size(), num_row_);
  }
  if (!weights_.empty()) {
    CHECK_EQ(weights_.size(), num_row_);
  }
  return dmat;
}

}  // namespace fast_svmlight_loader
