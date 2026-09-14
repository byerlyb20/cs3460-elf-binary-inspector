#include <cstdint>

struct ElfHeader {
std::uint16_t type{};
std::uint64_t entry{};
std::uint64_t program_header_offset{};
std::uint16_t program_header_entry_size{};
std::uint16_t program_header_count{};
std::uint64_t section_header_offset{};
std::uint16_t section_header_entry_size{};
std::uint16_t section_header_count{};
};