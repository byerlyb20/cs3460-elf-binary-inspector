#ifndef FILE_BUFFER_H
#define FILE_BUFFER_H

#include <filesystem>
#include <span>

class FileBuffer {
public:
    explicit FileBuffer(const std::filesystem::path& path);
    std::span<const std::byte> bytes();
private:
    std::vector<std::byte> data_;
};

#endif
