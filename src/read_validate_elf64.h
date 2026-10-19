#include <filesystem>
#include "elf_dtypes.h"
#include "file_buffer.h"

void print_elf_header(const ElfHeader& header);
bool read_validate_elf64(FileBuffer& file, ElfHeader& elf_header_out);
void read_section_name_table(FileBuffer& file, ElfHeader& header, std::vector<Section>& sections_out);
void read_program_headers(FileBuffer& file, ElfHeader& header, std::vector<Segment>& segments_out);

