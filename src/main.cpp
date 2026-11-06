#include <filesystem>
#include "file_buffer.h"
#include "elf_dtypes.h"
#include "read_validate_elf64.h"
#include <format>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << std::format("Got {} arguments, expected 1.\n", argc - 1);
        return 1;
    }

    const std::filesystem::path& path = std::filesystem::path(argv[1]);

    FileBuffer file = FileBuffer(path);
    ElfHeader elf_header;

    try {
        read_validate_elf64(file, elf_header);

        std::vector<Section> sections;
        read_section_name_table(file, elf_header, sections);

        std::vector<Segment> segments;
        read_program_headers(file, elf_header, segments);

        std::cout << std::format("File: {}\n", argv[1]);
        print_elf_header(elf_header, sections, segments);
    } catch (const std::exception& e) {
        std::cerr << std::format("Reading ELF file failed with exception:\n\n{}\n", e.what());
    }

    return 0;
}
