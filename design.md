# Technical Report: Refactoring
## 0. Overview
The document presents the 
## 1. Requirements

<!--
Briefly describe the major functional requirements of your project.
- Identify what the system should do.
- Make each requirement clear and testable.
- Use requirement IDs (e.g., R1, R2, R3) for reference.
-->

| ID | Requirement |
|---|---|
| R1 | Reads and validates `Elf64_Ehdr` for magic number, class, data endianness, version, and machine architecture (supports little-endian, x86-64). |
| R2 | Creates an `ElfHeader` object from a valid ELF binary with all fields populated. |
| R3 | Reads section size and offset for all section headers present in binary. |
| R4 | Reads section names from the section name table for all section headers present in binary, failing safely with empty strings if the section name table index is `SHN_UNDEF`. |
| R5 | Reads program headers from the program header table, validating the table's offset, count, and entry size against the file bounds. |
| R6 | Reads program type, offset, virtual address, file size, and memory size. |
| R7 | Populates a segment struct with values read from the program header table. |
| R8 | Prints values from `ElfHeader` object. |
| R9 | Prints values from all populated section structs. |
| R10 | Prints values from all populated segment structs. |

## System Design

<!--
Describe how you plan to organize your project using C++ classes. Your design should include:

- **Major classes:** Explain the responsibility, important data members, and main functions/interfaces of each class.
- **Class relationships:** Provide a UML class diagram or equivalent diagram showing how the classes are related (e.g., composition, inheritance, or usage).
- **Class interactions:** Explain how the classes work together to implement the major system functionality.

Include representative C++ class declarations where helpful.

Focus on explaining **how your design satisfies the requirements**, rather than simply listing classes and functions.
-->

```mermaid
classDiagram
  class FileBuffer {
    +FileBuffer(path path)
    +bytes() span~byte~
    -vector~byte~ data
  }
  class ElfBinary {
    +ElfHeader header
    +vector~Section~ sections
    +vector~Segment~ segments
    +get_section_name_table() SectionNameTable
  }
  class ElfHeader {
    +uint16_t type
    +uint64_t entry
    +uint64_t program_header_offset
    +uint16_t program_header_entry_size
    +uint16_t program_header_count
    +uint64_t section_header_offset
    +uint16_t section_header_entry_size
    +uint16_t section_header_count
    +uint16_t section_name_table_index
  }
  class Section {
    +uint32_t index
    +uint64_t offset
    +uint64_t size
    +get_name() string
    +get_body(FileBuffer body) span~byte~
  }
  class SectionNameTable {
    +get_section_name(int index) string
  }
  class Segment {
    +uint32_t index
    +uint32_t type
    +uint32_t flags
    +uint64_t offset
    +uint64_t virtual_address
    +uint64_t file_size
    +uint64_t memory_size
    +uint64_t alignment
  }
  class ElfFactory {
    +read_elf_from_file(span~byte~ file) ElfBinary
  }
  class ElfHeaderFactory {
    +read_header_from_file(span~byte~ file) ElfHeader
  }
  class SectionFactory {
    +create_section_from_shdr(span~byte~ shdr) Section
  }
  class SegmentFactory {
    +create_segment_from_shdr(span~byte~ shdr) Segment
  }
  class ElfBinaryLogger {
    +print(ElfBinary elf, span~byte~ file)
  }
  ElfBinary "1" *-- "1" ElfHeader : header
  ElfBinary "1" *-- "*" Section : sections
  ElfBinary "1" *-- "*" Segment : segments
  Section <|-- SectionNameTable
  ElfFactory ..> ElfHeaderFactory
  ElfFactory ..> SectionFactory
  ElfFactory ..> SegmentFactory
```

## Test Design

<!--
Design tests to verify the major requirements and system functionality. For each test, provide:

- **Target:** The requirement or component being tested.
- **Input/Setup:** The input or conditions required.
- **Expected Result:** The expected behavior or output.

Include normal cases and important error/edge cases.

Briefly explain how your planned tests cover the major requirements.

You only need to design the tests in this milestone. They will be implemented and executed in the final milestone.
-->
