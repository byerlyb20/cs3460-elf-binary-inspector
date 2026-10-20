#include <filesystem>
#include "file_buffer.h"
#include "elf_dtypes.h"
#include "read_validate_elf64.h"
#include <cstdio>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Got %d arguments, expected 1.\n", argc - 1);
        return 1;
    }

    const std::filesystem::path& path = std::filesystem::path(argv[1]);

    FileBuffer file = FileBuffer(path);
    ElfHeader elf_header;

    if (read_validate_elf64(file, elf_header)) {
        std::vector<Section> sections;
        read_section_name_table(file, elf_header, sections);

        std::vector<Segment> segments;
        read_program_headers(file, elf_header, segments);

        printf("File: %s\n", argv[1]);
        print_elf_header(elf_header, sections, segments);
    } else {
        return 1;
    }

    return 0;
}
