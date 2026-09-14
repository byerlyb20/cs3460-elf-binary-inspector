#include <cstddef>

bool in_range(std::size_t offset,
    std::size_t length,
    std::size_t size) {
return offset <= size && length <= size - offset;
}