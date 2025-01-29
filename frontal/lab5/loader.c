#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

// Function to iterate over program headers and apply a callback
int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *, int, int), int arg) {
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)map_start;

    // Ensure it's a valid ELF file
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Error: Not an ELF file\n");
        return -1;
    }

    // Iterate over program headers
    for (int i = 0; i < ehdr->e_phnum; ++i) {
        Elf32_Phdr *phdr = (Elf32_Phdr *)((char *)map_start + ehdr->e_phoff + i * ehdr->e_phentsize);
        func(phdr, i, arg);
    }

    return 0;
}

// Helper function to print protection flags for mmap
void print_protection_flags(uint32_t p_flags) {
    printf("PROT_");
    printf("%s", (p_flags & PF_R) ? "READ " : "");
    printf("%s", (p_flags & PF_W) ? "WRITE " : "");
    printf("%s", (p_flags & PF_X) ? "EXEC " : "");
}

// Callback function to print program header mapping details
void print_mapping_details(Elf32_Phdr *phdr, int index, int fd) {
    // Only consider PT_LOAD segments
    if (phdr->p_type != PT_LOAD) {
        return;
    }

    // Print the header details
    printf("Segment %d:\n", index);
    printf("  Type: LOAD\n");
    printf("  Offset: 0x%06x\n", phdr->p_offset);
    printf("  Virtual Address: 0x%08x\n", phdr->p_vaddr);
    printf("  Physical Address: 0x%08x\n", phdr->p_paddr);
    printf("  File Size: 0x%05x\n", phdr->p_filesz);
    printf("  Memory Size: 0x%05x\n", phdr->p_memsz);
    printf("  Alignment: 0x%x\n", phdr->p_align);

    // Compute and print mmap protection flags
    printf("  Protection Flags: ");
    print_protection_flags(phdr->p_flags);
    printf("\n");

    // Print mmap mapping flags (assuming MAP_PRIVATE for executable files)
    printf("  Mapping Flags: MAP_PRIVATE | MAP_FIXED\n\n");
}

// Function to load program headers into memory
void load_phdr(Elf32_Phdr *phdr, int fd, int arg) {
    // Only consider PT_LOAD segments
    if (phdr->p_type != PT_LOAD) {
        return;
    }

    // Compute mmap parameters
    void *vaddr = (void *)(phdr->p_vaddr & 0xfffff000);
    size_t padding = phdr->p_vaddr & 0xfff;
    size_t mem_size = phdr->p_memsz + padding;
    off_t offset = phdr->p_offset & 0xfffff000;

    int prot = 0;
    if (phdr->p_flags & PF_R) prot |= PROT_READ;
    if (phdr->p_flags & PF_W) prot |= PROT_WRITE;
    if (phdr->p_flags & PF_X) prot |= PROT_EXEC;

    // Perform mmap
    void *mapped = mmap(vaddr, mem_size, prot, MAP_PRIVATE | MAP_FIXED, fd, offset);
    if (mapped == MAP_FAILED) {
        perror("Error mapping segment");
        exit(1);
    }

    // Zero out the padding
    if (phdr->p_memsz > phdr->p_filesz) {
        void *zero_start = (void *)((char *)mapped + phdr->p_filesz + padding);
        memset(zero_start, 0, phdr->p_memsz - phdr->p_filesz);
    }

    printf("Successfully mapped segment at virtual address 0x%08x\n", phdr->p_vaddr);
}

int main(int argc, char *argv[]) {
    printf("test");
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf_filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // Open the ELF file
    FILE* fd = open(filename, O_RDONLY);
    if (!fd) {
        perror("Error opening file");
        return 1;
    }

    
    fseek(fd,0,SEEK_END);
    size_t fileSize = ftell(fd);
    // Map the ELF file into memory
    
    void *map_start = mmap(NULL, fileSize, PROT_READ | PROT_EXEC | PROT_WRITE ,MAP_PRIVATE,fileno(fd), 0);
    if (map_start == MAP_FAILED) {
        perror("Error mapping file into memory");
        close(fd);
        return 1;
    }
    // Print program header mapping details
    printf("\nProgram Headers:\n");
    foreach_phdr(map_start, print_mapping_details, fileno(fd));

    // Load segments into memory
    printf("\nLoadiang Segments:\n");
    foreach_phdr(map_start, load_phdr, fd);

    // Prepare to execute the loaded program (optional, uncomment to use startup)
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)map_start;
    extern int startup(int argc, char **argv, void (*start)());
    startup(argc - 1, argv + 1, (void *)(ehdr->e_entry));

    // Unmap the file from memory (file must remain open during execution)
    if (munmap(map_start, fileSize) != 0) {
        perror("Error unmapping file from memory");
    }

    // Close the file
    close(fd);
   

    return 0;
}
