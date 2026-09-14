#include <filesystem>
#include "file_buffer.h"
#include "elf_header.h"
#include "read_validate_elf64.h"

int main(int argc, char* argv[])
{
    // TODO: Read path from command line args
    const std::filesystem::path& path;

    FileBuffer file = FileBuffer(path);
    ElfHeader elf_header;

    if (read_validate_elf64(file, elf_header)) {
        print_elf_header(elf_header);
    }

    // TODO: Call read_validate_elf64 and print_elf_header
    return 0;
}
