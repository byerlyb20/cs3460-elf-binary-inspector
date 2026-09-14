
class FileBuffer {
public:
    explicit FileBuffer(const std::filesystem::path& path);
    std::span<const std::byte> bytes() const { return {data_.data(), data_.size()}; }
private:
    std::vector<std::byte> data_;
};