#include "file_buffer.h"
#include <fstream>
    
FileBuffer::FileBuffer(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    data_.resize(size);
    std::vector<char> buffer(size);
    file.read(reinterpret_cast<char*>(data_.data()), size);
}

std::span<const std::byte> FileBuffer::bytes() { return {data_.data(), data_.size()}; }
