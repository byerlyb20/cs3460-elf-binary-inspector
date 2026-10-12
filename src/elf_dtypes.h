#ifndef ELF_DTYPES_H
#define ELF_DTYPES_H

#include <cstdint>
#include <string>

struct ElfHeader {
    std::uint16_t type{};
    std::uint64_t entry{};
    std::uint64_t program_header_offset{};
    std::uint16_t program_header_entry_size{};
    std::uint16_t program_header_count{};
    std::uint64_t section_header_offset{};
    std::uint16_t section_header_entry_size{};
    std::uint16_t section_header_count{};
    std::uint16_t section_name_table_index{};
};

struct Section {
    std::uint32_t index{};
    std::string name;
    std::uint64_t offset{}, size{};
};

#endif
