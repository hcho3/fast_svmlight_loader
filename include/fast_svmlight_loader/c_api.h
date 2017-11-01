/*!
 * Copyright (c) 2017 by Contributors
 * \file c_api.h
 * \author Philip Cho
 * \brief C API of fast_svmlight_loader
 */

/* Note: Make sure to use slash-asterisk form of comments in this file
   (like this one). Do not use double-slash (//). */

#ifndef FAST_SVMLIGHT_LOADER_C_API_H_
#define FAST_SVMLIGHT_LOADER_C_API_H_

#ifdef __cplusplus
#define FAST_SVMLIGHT_LOADER_EXTERN_C extern "C"
#include <cstdio>
#include <cstdint>
#else
#define FAST_SVMLIGHT_LOADER_EXTERN_C
#include <stdio.h>
#include <stdint.h>
#endif

/* special symbols for DLL library on Windows */
#if defined(_MSC_VER) || defined(_WIN32)
#define FAST_SVMLIGHT_LOADER_DLL FAST_SVMLIGHT_LOADER_EXTERN_C \
                                __declspec(dllexport)
#else
#define FAST_SVMLIGHT_LOADER_DLL FAST_SVMLIGHT_LOADER_EXTERN_C
#endif

/*!
 * \addtogroup opaque_handles
 * opaque handles
 * \{
 */
/*! \brief handle to a data matrix */
typedef void* DMatrixHandle;
/*! \} */

/*!
 * \defgroup dmatrix
 * Data matrix interface
 * \{
 */
/*!
 * \brief create DMatrix from a text file with SVMLight format
 * \param path file path; the file must comply with SVMLight format
 * \param nthread number of threads to use
 * \param verbose whether to produce extra messages
 * \param out the created DMatrix
 * \return 0 for success, -1 for failure
 */
FAST_SVMLIGHT_LOADER_DLL int FastSVMLightLoaderDMatrixCreateFromFile(
                               const char* path,
                               int nthread,
                               int verbose,
                               DMatrixHandle* out);

/*!
 * \brief get dimensions of a DMatrix
 * \param handle handle to DMatrix
 * \param out_num_row used to set number of rows
 * \param out_num_col used to set number of columns
 * \param out_nelem used to set number of nonzero entries
 * \return 0 for success, -1 for failure
 */
FAST_SVMLIGHT_LOADER_DLL int FastSVMLightLoaderDMatrixGetDimension(
                               DMatrixHandle handle,
                               size_t* out_num_row,
                               size_t* out_num_col,
                               size_t* out_nelem);

/*!
 * \brief extract three arrays (data, col_ind, row_ptr) that define a DMatrix.
 * \param handle handle to DMatrix
 * \param out_data used to save pointer to array containing feature values
 * \param out_col_ind used to save pointer to array containing feature indices
 * \param out_row_ptr used to save pointer to array containing pointers to
 *                    row headers
 * \param out_labels used to save pointer to array containing labels
 *                   (nullptr if no label is provided)
 * \param out_weights used to save pointer to array containing instance weights
 *                    (nullptr if no weight is provided)
 * \return 0 for success, -1 for failure
 */
FAST_SVMLIGHT_LOADER_DLL int FastSVMLightLoaderDMatrixGetArrays(
                               DMatrixHandle handle,
                               const float** out_data,
                               const uint32_t** out_col_ind,
                               const size_t** out_row_ptr,
                               const float** out_labels,
                               const float** out_weights);

/*!
 * \brief delete DMatrix from memory
 * \param handle handle to DMatrix
 * \return 0 for success, -1 for failure
 */
FAST_SVMLIGHT_LOADER_DLL int FastSVMLightLoaderDMatrixFree(DMatrixHandle handle);
/*! \} */

/*!
 * \brief display last error; can be called by multiple threads
 * Note. Each thread will get the last error occured in its own context.
 * \return error string
 */
FAST_SVMLIGHT_LOADER_DLL const char* FastSVMLightLoaderGetLastError();

/*!
 * \brief register callback function for LOG(INFO) messages -- helpful messages
 *        that are not errors.
 * Note: this function can be called by multiple threads. The callback function
 *       will run on the thread that registered it
 * \return 0 for success, -1 for failure
 */
FAST_SVMLIGHT_LOADER_DLL int FastSVMLightLoaderRegisterLogCallback(
                                                 void (*callback)(const char*));

#endif  /* FAST_SVMLIGHT_LOADER_C_API_H_ */
