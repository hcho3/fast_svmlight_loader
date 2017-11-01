/*!
 *  Copyright (c) 2017 by Contributors
 * \file c_api_error.cc
 * \author Philip Cho
 * \brief C error handling
 */
#include <dmlc/thread_local.h>
#include "./c_api_error.h"

struct FastSVMLightLoaderAPIErrorEntry {
  std::string last_error;
};

typedef dmlc::ThreadLocalStore<FastSVMLightLoaderAPIErrorEntry> \
                                                FastSVMLightLoaderAPIErrorStore;

const char* FastSVMLightLoaderGetLastError() {
  return FastSVMLightLoaderAPIErrorStore::Get()->last_error.c_str();
}

void FastSVMLightLoaderAPISetLastError(const char* msg) {
  FastSVMLightLoaderAPIErrorStore::Get()->last_error = msg;
}
