#include <elf.h>
#include <cstring>

auto bytes = file.bytes();
if (bytes.size() < sizeof(Elf64_Ehdr)) {
// invalid/truncated input
}

Elf64_Ehdr raw{};
std::memcpy(&raw, bytes.data(), sizeof(raw));

if (raw.e_ident[EI_MAG0] != ELFMAG0 ||
    raw.e_ident[EI_MAG1] != ELFMAG1 ||
    raw.e_ident[EI_MAG2] != ELFMAG2 ||
    raw.e_ident[EI_MAG3] != ELFMAG3) {
    // not an ELF file
}
if (raw.e_ident[EI_CLASS] != ELFCLASS64 ||
    raw.e_ident[EI_DATA] != ELFDATA2LSB ||
    raw.e_ident[EI_VERSION] != EV_CURRENT ||
    raw.e_machine != EM_X86_64) {
    // unsupported ELF target
}

