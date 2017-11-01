/*!
 * Copyright 2017 by Contributors
 * \file logging.h
 * \brief logging facility for fast_svmlight_loader
 * \author Philip Cho
 */
#ifndef FAST_SVMLIGHT_LOADER_LOGGING_H_
#define FAST_SVMLIGHT_LOADER_LOGGING_H_

#include <dmlc/thread_local.h>

namespace fast_svmlight_loader {

class LogCallbackRegistry {
 public:
  using Callback = void (*)(const char*);
  inline void Register(Callback log_callback) {
    this->log_callback_ = log_callback;
  }
  inline Callback Get() const {
    return log_callback_;
  }
 private:
  Callback log_callback_;
};

using LogCallbackRegistryStore = dmlc::ThreadLocalStore<LogCallbackRegistry>;

}  // namespace fast_svmlight_loader

#endif  // FAST_SVMLIGHT_LOADER_LOGGING_H_
