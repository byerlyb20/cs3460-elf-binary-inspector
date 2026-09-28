#include <filesystem>
#include "elf_header.h"
#include "file_buffer.h"

void print_elf_header(const ElfHeader& header);
bool read_validate_elf64(FileBuffer& file, ElfHeader& elf_header);
