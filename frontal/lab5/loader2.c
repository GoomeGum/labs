#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

void print_phdr_info(Elf64_Phdr *phdr, int index) {
    printf("Program header %d:\n", index);
    printf("  Type: %d\n", phdr->p_type);
    printf("  Offset: 0x%lx\n", phdr->p_offset);
    printf("  Virtual address: 0x%lx\n", phdr->p_vaddr);
    printf("  Physical address: 0x%lx\n", phdr->p_paddr);
    printf("  File size: %lu\n", phdr->p_filesz);
    printf("  Memory size: %lu\n", phdr->p_memsz);
    printf("  Flags: %d\n", phdr->p_flags);
    printf("  Align: %lu\n", phdr->p_align);
}

int checkProtectionFlags(int p_flags) {
    int prot = 0;
    if (p_flags & PF_R) prot |= PROT_READ;
    if (p_flags & PF_W) prot |= PROT_WRITE;
    if (p_flags & PF_X) prot |= PROT_EXEC;
    return prot;
}

void load_segment(int fd, Elf64_Phdr *phdr) {
    size_t padding = phdr->p_vaddr % phdr->p_align;
    size_t memszTotal = phdr->p_memsz + padding;
    void* vaddr = (void*)(phdr->p_vaddr - padding);
    void* elfFileMapped = mmap(vaddr, memszTotal, checkProtectionFlags(phdr->p_flags), MAP_PRIVATE | MAP_FIXED, fd, phdr->p_offset);
    if (elfFileMapped == MAP_FAILED) {
        perror("mmap");
        return;
    } else {
        print_phdr_info(phdr, 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    Elf64_Ehdr ehdr;
    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
        perror("read");
        close(fd);
        return 1;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF file\n");
        close(fd);
        return 1;
    }

    for (int i = 0; i < ehdr.e_phnum; i++) {
        Elf64_Phdr phdr;
        lseek(fd, ehdr.e_phoff + i * sizeof(phdr), SEEK_SET);
        if (read(fd, &phdr, sizeof(phdr)) != sizeof(phdr)) {
            perror("read");
            close(fd);
            return 1;
        }
        load_segment(fd, &phdr);
    }

    close(fd);
    return 0;
}