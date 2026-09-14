#include <filesystem>
#include "elf_header.h"

void print_elf_header(ElfHeader& header);
bool read_validate_elf64(FileBuffer& file, ElfHeader& elf_header);
