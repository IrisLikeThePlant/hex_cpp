#include "value.h"

#include <cstdio>

void ValueArray::write(const Value value) {
    values.push_back(value);
}

size_t ValueArray::count() const {
    return values.size();
}

Value ValueArray::get_value_at(const int offset) const {
    return values[offset];
}

void ValueArray::print_value(const int offset) const {
    std::printf("%g", values[offset]);
}

void print_value(const Value value) {
    std::printf("%g", value);
}
