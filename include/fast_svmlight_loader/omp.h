/*!
 * Copyright 2017 by Contributors
 * \file omp.h
 * \brief compatiblity wrapper for systems that don't support OpenMP
 * \author Philip Cho
 */
#ifndef FAST_SVMLIGHT_LOADER_OMP_H_
#define FAST_SVMLIGHT_LOADER_OMP_H_

#ifdef FAST_SVMLIGHT_LOADER_OPENMP_SUPPORT
#include <omp.h>
#else
inline int omp_get_thread_num() {
  return 0;
}

inline int omp_get_max_threads() {
  return 1;
}
#endif

#endif  // FAST_SVMLIGHT_LOADER_OMP_H_
