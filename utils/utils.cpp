#include "utils.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string Utils::to_string_with_precision(double value, int precision) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}
