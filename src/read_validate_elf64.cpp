#include "read_validate_elf64.h"
#include <elf.h>
#include <cstring>
#include "file_buffer.h"
#include <iostream> 

void print_elf_header(const ElfHeader& header) {
    // TODO: Print all fields in header
    std::cout << "----ELF HEADER ----\n";
    std::cout << "Type:                        " << header.type << "\n";
    std::cout << "Entry:                       " << header.entry << "\n";
    std::cout << "Program header offset:       " << header.program_header_offset << "\n";
    std::cout << "Program header entry size:   " << header.program_header_entry_size << "\n";
    std::cout << "Program header count:        " << header.program_header_count << "\n";
    std::cout << "Section header offset:       " << header.section_header_offset << "\n";
    std::cout << "Section header entry size:   " << header.section_header_entry_size << "\n";
    std::cout << "Section header count:        " << header.section_header_count << "\n";
}

bool read_validate_elf64(FileBuffer& file, ElfHeader& elf_header) {
    auto bytes = file.bytes();
    if (bytes.size() < sizeof(Elf64_Ehdr)) {
        // invalid/truncated input
        return false;
    }

    Elf64_Ehdr raw{};
    std::memcpy(&raw, bytes.data(), sizeof(raw));

    if (raw.e_ident[EI_MAG0] != ELFMAG0 ||
        raw.e_ident[EI_MAG1] != ELFMAG1 ||
        raw.e_ident[EI_MAG2] != ELFMAG2 ||
        raw.e_ident[EI_MAG3] != ELFMAG3) {
        // not an ELF file
        return false;
    }
    if (raw.e_ident[EI_CLASS] != ELFCLASS64 ||
        raw.e_ident[EI_DATA] != ELFDATA2LSB ||
        raw.e_ident[EI_VERSION] != EV_CURRENT ||
        raw.e_machine != EM_X86_64) {
        // unsupported ELF target
        return false;
    }

    // TODO: copy values into ElfHeader

    return true;
}
