/*!
 * Copyright (c) 2017 by Contributors
 * \file c_api.cc
 * \author Philip Cho
 * \brief C API of fasst_svmlight_loader
 */

#include <fast_svmlight_loader/c_api.h>
#include <fast_svmlight_loader/data.h>
#include <fast_svmlight_loader/logging.h>
#include <dmlc/thread_local.h>
#include <memory>
#include <algorithm>
#include "./c_api_error.h"

using namespace fast_svmlight_loader;

namespace {

/*! \brief entry to to easily hold returning information */
struct FastSVMLightLoaderThreadLocalEntry {
  /*! \brief result holder for returning string */
  std::string ret_str;
};

// define threadlocal store for returning information
using FastSVMLightLoaderThreadLocalStore
  = dmlc::ThreadLocalStore<FastSVMLightLoaderThreadLocalEntry>;

}  // namespace anonymous

int FastSVMLightLoaderDMatrixCreateFromFile(const char* path,
                                            int nthread,
                                            int verbose,
                                            DMatrixHandle* out) {
  API_BEGIN();
  *out = static_cast<DMatrixHandle>(DMatrix::Create(path, nthread, verbose));
  API_END();
}

int FastSVMLightLoaderDMatrixGetDimension(DMatrixHandle handle,
                                          size_t* out_num_row,
                                          size_t* out_num_col,
                                          size_t* out_nelem) {
  API_BEGIN();
  const DMatrix* dmat = static_cast<DMatrix*>(handle);
  *out_num_row = dmat->num_row;
  *out_num_col = dmat->num_col;
  *out_nelem = dmat->nelem;
  API_END();
}

int FastSVMLightLoaderDMatrixGetArrays(DMatrixHandle handle,
                                       const float** out_data,
                                       const uint32_t** out_col_ind,
                                       const size_t** out_row_ptr,
                                       const float** out_labels,
                                       const float** out_weights) {
  API_BEGIN();
  const DMatrix* dmat_ = static_cast<DMatrix*>(handle);
  *out_data = &dmat_->data[0];
  *out_col_ind = &dmat_->col_ind[0];
  *out_row_ptr = &dmat_->row_ptr[0];
  *out_labels = (dmat_->labels.empty()) ? NULL : &dmat_->labels[0];
  *out_weights = (dmat_->weights.empty()) ? NULL : &dmat_->weights[0];
  API_END();
}

int FastSVMLightLoaderDMatrixFree(DMatrixHandle handle) {
  API_BEGIN();
  delete static_cast<DMatrix*>(handle);
  API_END();
}

int FastSVMLightLoaderRegisterLogCallback(void (*callback)(const char*)) {
  API_BEGIN();
  LogCallbackRegistry* registry = LogCallbackRegistryStore::Get();
  registry->Register(callback);
  API_END();
}
