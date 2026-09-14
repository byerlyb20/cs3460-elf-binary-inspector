#include "file_buffer.h"

class FileBuffer {
    
    explicit FileBuffer(const std::filesystem::path& path) {
        // TODO: Read the file into memory and store in data_ vector
    }

    std::span<const std::byte> bytes() const { return {data_.data(), data_.size()}; }
    std::vector<std::byte> data_;
};
