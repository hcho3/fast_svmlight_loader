/*!
 * Copyright (c) 2017 by Contributors
 * \file data.h
 * \author Philip Cho
 * \brief Input data structure of fast_svmlight_loader
 */
#ifndef FAST_SVMLIGHT_LOADER_DATA_H_
#define FAST_SVMLIGHT_LOADER_DATA_H_

#include <dmlc/data.h>

namespace fast_svmlight_loader {

/*! \brief a simple data matrix in CSR (Compressed Sparse Row) storage */
struct DMatrix {
  /*! \brief feature values */
  std::vector<float> data;
  /*! \brief feature indices */
  std::vector<uint32_t> col_ind;
  /*! \brief pointer to row headers; length of [num_row] + 1 */
  std::vector<size_t> row_ptr;
  /*! \brief label of each instance */
  std::vector<float> labels;
  /*! \brief weight of each instance */
  std::vector<float> weights;
  /*! \brief number of rows */
  size_t num_row;
  /*! \brief number of columns */
  size_t num_col;
  /*! \brief number of nonzero entries */
  size_t nelem;

  /*!
   * \brief clear all data fields
   */
  inline void Clear() {
    data.clear();
    row_ptr.clear();
    col_ind.clear();
    labels.clear();
    weights.clear();
    row_ptr.resize(1, 0);
    num_row = num_col = nelem = 0;
  }
  /*!
   * \brief construct a new DMatrix from a text file with SVMLight format
   * \param filename name of file; the file must comply with SVMLight format
   * \param nthread number of threads to use
   * \param verbose whether to produce extra messages
   * \return newly built DMatrix
   */
  static DMatrix* Create(const char* filename, int nthread, int verbose);
};

}  // namespace fast_svmlight_loader

#endif  // FAST_SVMLIGHT_LOADER_DATA_H_
