#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <elf.h>

#define MAX_FILES 2

struct ELF_File {
    int fd;
    void *map_start;
    size_t file_size;
    char filename[256];
};

struct ELF_File elf_files[MAX_FILES] = {{-1, NULL, 0, ""}, {-1, NULL, 0, ""}};
int debug_mode = 0;

void toggle_debug_mode() {
    debug_mode = !debug_mode;
    printf("Debug mode is now %s.\n", debug_mode ? "ON" : "OFF");
}

void cleanup_file(int index) {
    if (elf_files[index].map_start) {
        munmap(elf_files[index].map_start, elf_files[index].file_size);
        elf_files[index].map_start = NULL;
    }
    if (elf_files[index].fd != -1) {
        close(elf_files[index].fd);
        elf_files[index].fd = -1;
    }
    elf_files[index].file_size = 0;
    elf_files[index].filename[0] = '\0';
}

void examine_elf_file() {
    char filename[256];
    printf("Enter ELF file name: ");
    scanf("%255s", filename);

    for (int i = 0; i < MAX_FILES; i++) {
        if (elf_files[i].fd == -1) {
            int fd = open(filename, O_RDONLY);
            if (fd < 0) {
                perror("Failed to open file");
                return;
            }

            off_t file_size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);

            void *map_start = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (map_start == MAP_FAILED) {
                perror("Failed to map file");
                close(fd);
                return;
            }

            Elf32_Ehdr *header = (Elf32_Ehdr *)map_start;

            if (memcmp(header->e_ident, ELFMAG, SELFMAG) != 0) {
                printf("Not an ELF file.\n");
                munmap(map_start, file_size);
                close(fd);
                return;
            }

            elf_files[i].fd = fd;
            elf_files[i].map_start = map_start;
            elf_files[i].file_size = file_size;
            strncpy(elf_files[i].filename, filename, sizeof(elf_files[i].filename) - 1);

            printf("Magic: %c%c%c\n", header->e_ident[EI_MAG1], header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
            printf("Data encoding: %s\n", header->e_ident[EI_DATA] == ELFDATA2LSB ? "Little Endian" : "Big Endian");
            printf("Entry point: 0x%x\n", header->e_entry);
            printf("Section header offset: %d\n", header->e_shoff);
            printf("Number of section headers: %d\n", header->e_shnum);
            printf("Size of section header entry: %d\n", header->e_shentsize);
            printf("Program header offset: %d\n", header->e_phoff);
            printf("Number of program headers: %d\n", header->e_phnum);
            printf("Size of program header entry: %d\n", header->e_phentsize);

            if (debug_mode) {
                printf("[DEBUG] File %s mapped at address %p\n", filename, map_start);
            }

            return;
        }
    }

    printf("Cannot open more than %d ELF files at once.\n", MAX_FILES);
}

void print_section_names() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (elf_files[i].fd == -1) {
            printf("No ELF file loaded in slot %d.\n", i);
            continue;
        }

        Elf32_Ehdr *header = (Elf32_Ehdr *)elf_files[i].map_start;
        Elf32_Shdr *section_headers = (Elf32_Shdr *)(elf_files[i].map_start + header->e_shoff);
        const char *section_names = (char *)(elf_files[i].map_start + section_headers[header->e_shstrndx].sh_offset);

        printf("File: %s\n", elf_files[i].filename);

        for (int j = 0; j < header->e_shnum; j++) {
            printf("[%2d] %s 0x%08x %6d %6d %d\n",
                   j,
                   &section_names[section_headers[j].sh_name],
                   section_headers[j].sh_addr,
                   section_headers[j].sh_offset,
                   section_headers[j].sh_size,
                   section_headers[j].sh_type);
        }

        if (debug_mode) {
            printf("[DEBUG] shstrndx: %d, section header offset: %d\n", header->e_shstrndx, header->e_shoff);
        }
    }
}

void print_symbols() {
    for (int i = 0; i < MAX_FILES; i++) {
        if (elf_files[i].fd == -1) {
            printf("No ELF file loaded in slot %d.\n", i);
            continue;
        }

        Elf32_Ehdr *header = (Elf32_Ehdr *)elf_files[i].map_start;
        Elf32_Shdr *section_headers = (Elf32_Shdr *)(elf_files[i].map_start + header->e_shoff);
        const char *section_names = (char *)(elf_files[i].map_start + section_headers[header->e_shstrndx].sh_offset);

        printf("File: %s\n", elf_files[i].filename);

        for (int j = 0; j < header->e_shnum; j++) {
            if (section_headers[j].sh_type == SHT_SYMTAB || section_headers[j].sh_type == SHT_DYNSYM) {
                Elf32_Sym *symbol_table = (Elf32_Sym *)(elf_files[i].map_start + section_headers[j].sh_offset);
                int symbol_count = section_headers[j].sh_size / sizeof(Elf32_Sym);
                const char *string_table = (char *)(elf_files[i].map_start + section_headers[section_headers[j].sh_link].sh_offset);

                if (debug_mode) {
                    printf("[DEBUG] Symbol table index: %d, size: %d, symbol count: %d\n", j, section_headers[j].sh_size, symbol_count);
                }

                for (int k = 0; k < symbol_count; k++) {
                    const char *symbol_name = &string_table[symbol_table[k].st_name];
                    const char *section_name = (symbol_table[k].st_shndx < header->e_shnum)
                                                   ? &section_names[section_headers[symbol_table[k].st_shndx].sh_name]
                                                   : "UND";

                    printf("[%2d] 0x%08x %d %s %s\n",
                           k,
                           symbol_table[k].st_value,
                           symbol_table[k].st_shndx,
                           section_name,
                           symbol_name);
                }
            }
        }
    }
}

void check_files_for_merge() {
    if (elf_files[0].fd == -1 || elf_files[1].fd == -1) {
        printf("Two ELF files must be loaded to check for merge.\n");
        return;
    }

    Elf32_Ehdr *header1 = (Elf32_Ehdr *)elf_files[0].map_start;
    Elf32_Ehdr *header2 = (Elf32_Ehdr *)elf_files[1].map_start;
    Elf32_Shdr *section_headers1 = (Elf32_Shdr *)(elf_files[0].map_start + header1->e_shoff);
    Elf32_Shdr *section_headers2 = (Elf32_Shdr *)(elf_files[1].map_start + header2->e_shoff);

    // Locate symbol tables
    Elf32_Shdr *symtab1 = NULL, *symtab2 = NULL;
    for (int i = 0; i < header1->e_shnum; i++) {
        if (section_headers1[i].sh_type == SHT_SYMTAB) {
            symtab1 = &section_headers1[i];
            break;
        }
    }
    for (int i = 0; i < header2->e_shnum; i++) {
        if (section_headers2[i].sh_type == SHT_SYMTAB) {
            symtab2 = &section_headers2[i];
            break;
        }
    }

    if (!symtab1 || !symtab2) {
        printf("Feature not supported: One or both files lack a symbol table.\n");
        return;
    }

    Elf32_Sym *symbols1 = (Elf32_Sym *)(elf_files[0].map_start + symtab1->sh_offset);
    Elf32_Sym *symbols2 = (Elf32_Sym *)(elf_files[1].map_start + symtab2->sh_offset);
    const char *strtab1 = (char *)(elf_files[0].map_start + section_headers1[symtab1->sh_link].sh_offset);
    const char *strtab2 = (char *)(elf_files[1].map_start + section_headers2[symtab2->sh_link].sh_offset);

    int count1 = symtab1->sh_size / sizeof(Elf32_Sym);
    int count2 = symtab2->sh_size / sizeof(Elf32_Sym);

    // Check symbols from file 1 against file 2
    for (int i = 1; i < count1; i++) { // Skip the first dummy symbol
        const char *name1 = &strtab1[symbols1[i].st_name];
        int found = 0;

        for (int j = 1; j < count2; j++) { // Skip the first dummy symbol
            const char *name2 = &strtab2[symbols2[j].st_name];
            if (strcmp(name1, name2) == 0) {
                found = 1;
                if (symbols1[i].st_shndx != SHN_UNDEF && symbols2[j].st_shndx != SHN_UNDEF) {
                    printf("Symbol %s multiply defined.\n", name1);
                }
                break;
            }
        }

        if (!found && symbols1[i].st_shndx == SHN_UNDEF) {
            printf("Symbol %s undefined.\n", name1);
        }
    }

    // Check symbols from file 2 against file 1
    for (int j = 1; j < count2; j++) { // Skip the first dummy symbol
        const char *name2 = &strtab2[symbols2[j].st_name];
        int found = 0;

        for (int i = 1; i < count1; i++) { // Skip the first dummy symbol
            const char *name1 = &strtab1[symbols1[i].st_name];
            if (strcmp(name2, name1) == 0) {
                found = 1;
                break;
            }
        }

        if (!found && symbols2[j].st_shndx == SHN_UNDEF) {
            printf("Symbol %s undefined.\n", name2);
        }
    }
}

void merge_elf_files() {
    printf("Merge ELF Files is not implemented yet.\n");
}

void quit() {
    for (int i = 0; i < MAX_FILES; i++) {
        cleanup_file(i);
    }
    printf("Exiting. Goodbye!\n");
    exit(0);
}

typedef struct {
    const char *name;
    void (*func)();
} MenuOption;

MenuOption menu[] = {
    {"Toggle Debug Mode", toggle_debug_mode},
    {"Examine ELF File", examine_elf_file},
    {"Print Section Names", print_section_names},
    {"Print Symbols", print_symbols},
    {"Check Files for Merge", check_files_for_merge},
    {"Merge ELF Files", merge_elf_files},
    {"Quit", quit}
};

int main() {
    int choice;
    size_t menu_size = sizeof(menu) / sizeof(MenuOption);

    while (1) {
        printf("\nChoose action:\n");
        for (size_t i = 0; i < menu_size; i++) {
            printf("%lu-%s\n", i, menu[i].name);
        }

        printf("Choice: ");
        if (scanf("%d", &choice) != 1 || choice < 0 || choice >= menu_size) {
            printf("Invalid choice. Try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        menu[choice].func();
    }

    return 0;
}
