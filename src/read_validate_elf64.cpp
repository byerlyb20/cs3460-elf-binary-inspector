#include "read_validate_elf64.h"
#include "elf.h"
#include <cstring>
#include "file_buffer.h"
#include "binary_reader.h"
#include <iostream>
#include <cstdio>

std::string elf_type_text(uint32_t type) {
    switch (type) {
        case ET_NONE:
        return "NONE";
        break;
        case ET_REL:
        return "REL";
        break;
        case ET_EXEC:
        return "EXEC";
        break;
        case ET_DYN:
        return "DYN";
        break;
        case ET_CORE:
        return "CORE";
        break;
        case ET_NUM:
        return "NUM";
        break;
    }
    return std::format("0x{:x}", type);
}

std::string permission_text(std::uint32_t flags) {
    std::string text = "---";
    if ((flags & PF_R) != 0) text[0] = 'R';
    if ((flags & PF_W) != 0) text[1] = 'W';
    if ((flags & PF_X) != 0) text[2] = 'X';
    return text;
}

std::string pheader_type_text(uint32_t type) {
    switch (type) {
        case PT_NULL:
        return "NULL      ";
        break;
        case PT_LOAD:
        return "LOAD      ";
        break;
        case PT_DYNAMIC:
        return "DYNAMIC   ";
        break;
        case PT_INTERP:
        return "INTERP    ";
        break;
        case PT_NOTE:
        return "NOTE      ";
        break;
        case PT_PHDR:
        return "PHDR      ";
        break;
        case PT_TLS:
        return "TLS       ";
        break;
    }
    return std::format("0x{:08x}", type);
}

void print_elf_header(
    const ElfHeader& header,
    const std::vector<Section>& sections,
    const std::vector<Segment>& segments
) {
    // TODO: Print all fields in header
    printf(R"(File: %s
ELF64 | little-endian | x86-64 | %s | Entry: 0x%06llx
Program headers: offset=0x%06llx, entry_size=%d, count=%d
Section headers: offset=0x%06llx, entry_size=%d, count=%d

)",
        "sample.elf",
        elf_type_text(header.type).c_str(),
        header.entry,
        header.program_header_offset,
        header.program_header_entry_size,
        header.program_header_count,
        header.section_header_offset,
        header.section_header_entry_size,
        header.section_header_count
    );
    printf("Sections\nINDEX   NAME            OFFSET        SIZE_BYTES\n");

    for (Section item: sections) {
        printf("%-3d     %-16s0x%06llx      %llu\n",
            item.index,
            item.name.c_str(),
            item.offset,
            item.size
        );
    }

    printf("\nProgram headers\nIDX   TYPE            OFFSET      VADDR      FILE_B    MEM_B   R/W/X   ALIGN\n");

    for (Segment item: segments) {
        printf("%-3d   %s      0x%06llx    0x%06llx   %-4llu      %-4llu    %s     0x%06llx\n",
            item.index,
            pheader_type_text(item.type).c_str(),
            item.offset,
            item.virtual_address,
            item.file_size,
            item.memory_size, 
            permission_text(item.flags).c_str(),
            item.alignment
        );
    }

}

bool read_validate_elf64(FileBuffer& file, ElfHeader& elf_header_out) {
    auto bytes = file.bytes();
    if (bytes.size() < sizeof(Elf64_Ehdr)) {
        // invalid/truncated input
        fprintf(stderr, "read_validate_elf64: invalid/truncated input\n");
        return false;
    }

    Elf64_Ehdr raw{};
    std::memcpy(&raw, bytes.data(), sizeof(raw));

    if (raw.e_ident[EI_MAG0] != ELFMAG0 ||
        raw.e_ident[EI_MAG1] != ELFMAG1 ||
        raw.e_ident[EI_MAG2] != ELFMAG2 ||
        raw.e_ident[EI_MAG3] != ELFMAG3) {
        // not an ELF file
        fprintf(stderr, "read_validate_elf64: not an ELF file\n");
        return false;
    }
    if (raw.e_ident[EI_CLASS] != ELFCLASS64 ||
        raw.e_ident[EI_DATA] != ELFDATA2LSB ||
        raw.e_ident[EI_VERSION] != EV_CURRENT ||
        raw.e_machine != EM_X86_64) {
        // unsupported ELF target
        fprintf(stderr, "read_validate_elf64: unsupported ELF target\n");
        return false;
    }

    elf_header_out.type = raw.e_type;
    elf_header_out.entry = raw.e_entry;
    elf_header_out.program_header_offset = raw.e_phoff;
    elf_header_out.program_header_entry_size = raw.e_phentsize;
    elf_header_out.program_header_count = raw.e_phnum;
    elf_header_out.section_header_offset = raw.e_shoff;
    elf_header_out.section_header_entry_size = raw.e_shentsize;
    elf_header_out.section_header_count = raw.e_shnum;
    elf_header_out.section_name_table_index = raw.e_shstrndx;

    return true;
}

// Code given in Milestone 2
// Ordinary helper, defined outside the parsing function.
std::string read_name(const std::string& names, std::size_t start) {
    if (names.empty() && start == 0) return {};
    if (start >= names.size())
        throw std::runtime_error("Invalid section-name offset");
    const std::size_t end = names.find('\0', start);
    if (end == std::string::npos)
        throw std::runtime_error("Unterminated section name");
    return names.substr(start, end - start);
}



void read_section_name_table(
    FileBuffer& file,
    ElfHeader& header,
    std::vector<Section>& sections_out
) {
    std::span<const std::byte> bytes = file.bytes();

    // Locate and validate the table (code given in Milestone 2)

    const std::uint64_t offset = header.section_header_offset;
    const std::size_t count = header.section_header_count;
    const std::size_t stride = header.section_header_entry_size;
    if (stride != sizeof(Elf64_Shdr) || offset > bytes.size() ||
        count > (bytes.size() - offset) / stride
    ) {
        throw std::runtime_error("Invalid section header table");
    }
    std::vector<Elf64_Shdr> section_headers;
    for (std::size_t i = 0; i < count; ++i) {
        Elf64_Shdr raw{};
        std::memcpy(&raw, bytes.data() + offset + i * stride, sizeof(raw));
        if (raw.sh_type != SHT_NULL && raw.sh_type != SHT_NOBITS &&
            raw.sh_size != 0 &&
            !in_range(raw.sh_offset, raw.sh_size, bytes.size())
        ) {
            throw std::runtime_error("Invalid section data range");
        }
        section_headers.push_back(raw);
    }

    // END provided code

    // For the section name table index at e_shstrndx: check the index,
    // require SHT_STRTAB, and validate that header's sh_offset/sh_size

    if (header.section_name_table_index >= count) {
        throw std::runtime_error("Invalid section name table index");
    }

    if (header.section_name_table_index == SHN_UNDEF) {
        for (uint32_t i = 0; const Elf64_Shdr& section_header : section_headers) {
            Section section = {
                .index = i,
                .name = "",
                .offset = section_header.sh_offset,
                .size = section_header.sh_size
            };
            sections_out.push_back(section);

            ++i;
        }
    } else {
        Elf64_Shdr name_table_header = section_headers[header.section_name_table_index];
        
        if (name_table_header.sh_type != SHT_STRTAB) {
            throw std::runtime_error("Invalid section type for section name table");
        }

        // Do we need to "validate that header's sh_offset/sh_size" here? Looks like
        // the provided code already does that above

        // Copy the bytes from the section name table section into a std::string named names
        const char* name_table = (const char*) bytes.data() + name_table_header.sh_offset;
        std::string names(name_table, name_table_header.sh_size);

        // Create each Section from its original index, resolved name, sh_offset, and sh_size
        for (uint32_t i = 0; const Elf64_Shdr& section_header : section_headers) {
            Section section = {
                .index = i,
                .name = read_name(names, section_header.sh_name),
                .offset = section_header.sh_offset,
                .size = section_header.sh_size
            };
            sections_out.push_back(section);

            ++i;
        }
    }
}


void read_program_headers(
    FileBuffer& file,
    ElfHeader& header,
    std::vector<Segment>& segments_out
){
    std::span<const std::byte> bytes = file.bytes();

    // offset, count, and stride now describe the program-header table.
    const std::uint64_t offset = header.program_header_offset;
    const std::size_t count = header.program_header_count;
    const std::size_t stride = header.program_header_entry_size;

    // e_phentsize can be zero if there is no program header table
    if ((stride != 0 && stride != sizeof(Elf64_Phdr)) ||
        offset > bytes.size() ||
        count > (bytes.size() - offset) / stride
    ) {
        throw std::runtime_error("Invalid program header table");
    }

    for (std::size_t i = 0; i < count; ++i) {
        Elf64_Phdr raw{};
        std::memcpy(&raw, bytes.data() + offset + i * stride, sizeof(raw));

        if (
            raw.p_type != PT_NULL && raw.p_filesz != 0 &&
            !in_range(raw.p_offset, raw.p_filesz, bytes.size())
        )
            throw std::runtime_error("Invalid segment data range");

        Segment s;
        s.index = static_cast<std::uint32_t>(i);
        s.type = raw.p_type;
        s.flags = raw.p_flags;
        s.offset = raw.p_offset;
        s.virtual_address = raw.p_vaddr;
        s.file_size = raw.p_filesz;
        s.memory_size = raw.p_memsz;
        s.alignment = raw.p_align;

        if (s.offset > bytes.size() || s.file_size > bytes.size() - offset) {
            throw std::runtime_error("Invalid program header");
        }

        segments_out.push_back(s);
    }
}
