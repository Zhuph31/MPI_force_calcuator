#include "utils.h"

#include <gflags/gflags.h>
#include <stdarg.h>

DEFINE_bool(debug_log, false, "");

std::string string_printf(const char *format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  std::vsprintf(buffer, format, args);
  va_end(args);
  return std::string(buffer);
}

std::string string_printf_vector(const std::vector<double> &vec) {
  std::string str;
  int pos = 0;
  for (double each : vec) {
    str += string_printf("%d:%e\n", pos++, each);
  }
  return str;
}

void debug_printf(const char *format, ...) {
  if (FLAGS_debug_log) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}
