/*!
 * Copyright (c) 2017 by Contributors
 * \file logging.cc
 * \author Philip Cho
 * \brief logging facility for fast_svmlight_loader
 */

#include <dmlc/logging.h>
#include <fast_svmlight_loader/logging.h>

// Override logging mechanism
void dmlc::CustomLogMessage::Log(const std::string& msg) {
  const fast_svmlight_loader::LogCallbackRegistry* registry
    = fast_svmlight_loader::LogCallbackRegistryStore::Get();
  auto callback = registry->Get();
  callback(msg.c_str());
}
