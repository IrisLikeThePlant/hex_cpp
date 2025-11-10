#pragma once

#include <vector>

#include "common.h"

typedef double Value;

class ValueArray {
    std::vector<Value> values;

public:
    ValueArray() = default;
    ~ValueArray() {
        values.clear();
    }

    void write(Value value);
    void print_value(int offset) const;
    [[nodiscard]] size_t count() const;
    [[nodiscard]] Value get_value_at(int offset) const;
};