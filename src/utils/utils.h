#include <string>
#include <vector>

std::string string_printf(const char *format, ...);

std::string string_printf_vector(const std::vector<double> &vec);

void debug_printf(const char *format, ...);