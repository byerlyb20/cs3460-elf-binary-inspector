#ifndef BINARY_READER_H
#define BINARY_READER_H

#include <cstddef>

bool in_range(std::size_t offset,
    std::size_t length,
    std::size_t size);

#endif
