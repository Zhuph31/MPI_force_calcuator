#include "utils.h"
#include <stdarg.h>

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
  for (double each : vec) {
    str += string_printf("%lf,", each);
  }
  return str;
}