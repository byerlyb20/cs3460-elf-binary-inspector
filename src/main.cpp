#include <filesystem>
#include "file_buffer.h"
#include "elf_header.h"
#include "read_validate_elf64.h"

int main(int argc, char* argv[])
{
    if (argc != 1) {
        return 1;
    }

    const std::filesystem::path& path = std::filesystem::path(argv[0]);

    FileBuffer file = FileBuffer(path);
    ElfHeader elf_header;

    if (read_validate_elf64(file, elf_header)) {
        print_elf_header(elf_header);
    } else {
        return 1;
    }

    return 0;
}
