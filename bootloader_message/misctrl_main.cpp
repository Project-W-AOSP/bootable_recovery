/*
 * Copyright (C) 2024 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <bootloader_message/bootloader_message.h>
#include <log/log.h>

#include <string>

#include <cstdio>

static void log(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
static void log(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);

  va_list vb;
  va_copy(vb, va);

  __android_log_vprint(ANDROID_LOG_ERROR, "misctrl", fmt, va);
  va_end(va);

  vfprintf(stderr, fmt, vb);
  fprintf(stderr, "\n");
  va_end(vb);
}

static int check_reserved_space() {
  bool empty;
  std::string err;
  bool success = CheckReservedSystemSpaceEmpty(&empty, &err);
  if (!success) {
    log("Could not read reserved space: %s", err.c_str());
    return 1;
  }
  log("System reserved space empty? %d.", empty);

  if (!err.empty()) {
    constexpr size_t kPrintChunkSize = 256;
    for (size_t i = 0; i < err.size(); i += kPrintChunkSize) {
      log("DATA: %s", err.substr(i, kPrintChunkSize).c_str());
    }
  }

  return empty ? 0 : 1;
}

int main() {
  return check_reserved_space();
}
