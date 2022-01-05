/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_LITE_TOOLS_LOGGING_H_
#define TENSORFLOW_LITE_TOOLS_LOGGING_H_

// LOG and CHECK macros for tflite tooling.

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#undef ERROR
#endif

namespace tflite {
namespace logging {
// A wrapper that logs to stderr.
//
// Used for TFLITE_LOG and TFLITE_BENCHMARK_CHECK macros.
class LoggingWrapper {
 public:
  enum class LogSeverity : int {
    INFO = 0,
    WARN = 1,
    ERROR = 2,
    FATAL = 3,
  };
  LoggingWrapper(LogSeverity severity)
      : severity_(severity), should_log_(true) {}
  LoggingWrapper(LogSeverity severity, bool log)
      : severity_(severity), should_log_(log) {}
  std::stringstream& Stream() { return stream_; }
  ~LoggingWrapper() {  // This prints out to cout when loggingwrapper goes out of the scope
    if (should_log_) {
      std::ofstream ofs;
      // For Android
      //ofs.open("/sdcard/output.txt", std::ios::app);

      // For Desktop
      ofs.open("/Users/heeh/Github/tensorflow-cmd/output.txt", std::ios::app);
      //std::cout << "hello world!\n";
      switch (severity_) {
        case LogSeverity::INFO:
        case LogSeverity::WARN:
          ofs << stream_.str() << std::endl;
          break;
        case LogSeverity::ERROR:
          ofs << stream_.str() << std::endl;
          break;
        case LogSeverity::FATAL:
          ofs << stream_.str() << std::endl;
          std::flush(ofs);
          std::abort();
          break;
      }
      ofs.close();
    }
  }

 private:
  std::stringstream stream_;
  LogSeverity severity_;
  bool should_log_;
};
}  // namespace logging
}  // namespace tflite

#define TFLITE_LOG(severity)                                  \
  tflite::logging::LoggingWrapper(                            \
      tflite::logging::LoggingWrapper::LogSeverity::severity) \
      .Stream()

#define TFLITE_MAY_LOG(severity, should_log)                                \
  tflite::logging::LoggingWrapper(                                          \
      tflite::logging::LoggingWrapper::LogSeverity::severity, (should_log)) \
      .Stream()

#define TFLITE_TOOLS_CHECK(condition) TFLITE_MAY_LOG(FATAL, !(condition))

#define TFLITE_TOOLS_CHECK_EQ(a, b) TFLITE_TOOLS_CHECK((a) == (b))

#endif  // TENSORFLOW_LITE_TOOLS_LOGGING_H_
