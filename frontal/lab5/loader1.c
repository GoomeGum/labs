#include <stdio.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int iterate_phdr(void *map_start, void (*func)(Elf32_Phdr *,int), int arg);
void display_phdr_info(Elf32_Phdr *phdr, int index);
const char *retrieve_p_type_string(Elf32_Word type);
void display_mmap_prot_flags(Elf32_Phdr *phdr, int arg);
Elf32_Word validateMappingFlags(Elf32_Word flag);
int validateProtectionFlags(Elf32_Word flag);
void process_phdr(Elf32_Phdr *phdr, int fd);
int startup(int argc, char **argv, void (*start)());


int iterate_phdr(void *map_start, void (*func)(Elf32_Phdr *,int), int arg){
    Elf32_Ehdr *elf_header = (Elf32_Ehdr *)map_start;
    Elf32_Phdr *program_header;
    for (int i = 0; i < elf_header->e_phnum; i++) {
        program_header = (Elf32_Phdr *)(map_start + elf_header->e_phoff + i * sizeof(Elf32_Phdr));
        func(program_header, arg);
    }
    return 0;
}

const char *retrieve_p_type_string(Elf32_Word type) {
    if (type == PT_PHDR)   return "PHDR";
    if (type == PT_INTERP) return "INTERP";
    if (type == PT_LOAD)   return "LOAD";
    if (type == PT_DYNAMIC)return "DYNAMIC";
    if (type == PT_NOTE)   return "NOTE";
    if (type == PT_SHLIB)  return "SHLIB";
    if (type == PT_NULL)   return "NULL";
    return "UNKNOWN";
}

void display_phdr_info(Elf32_Phdr *phdr, int index) {
    
    printf("%-8s 0x%06x 0x%08x 0x%08x 0x%06x 0x%06x ",
        retrieve_p_type_string(phdr->p_type),
        phdr->p_offset,
        phdr->p_vaddr,
        phdr->p_paddr,
        phdr->p_filesz,
        phdr->p_memsz);

    
    printf("%c%c%c 0x%08x 0x%05x  0x%x\n",
           (phdr->p_flags & PF_R) ? 'R' : ' ',
           (phdr->p_flags & PF_W) ? 'W' : ' ',
           (phdr->p_flags & PF_X) ? 'E' : ' ',
           phdr->p_align, validateProtectionFlags(phdr->p_flags), validateMappingFlags(phdr->p_flags));

    
}


void display_mmap_prot_flags(Elf32_Phdr *phdr, int arg)
{
    int prot = 0;
    if (phdr->p_flags & PF_R)
        prot |= PROT_READ;
    if (phdr->p_flags & PF_W)
        prot |= PROT_WRITE;
    if (phdr->p_flags & PF_X)
        prot |= PROT_EXEC;

    printf("0x%x\n", prot);
}

Elf32_Word validateMappingFlags(Elf32_Word flag){  
    if (flag == 0x1)       
        return PROT_EXEC;
    if (flag == 0x2)       
        return MAP_SHARED;
    if (flag == 0x3)       
        return MAP_SHARED;
    if (flag == 0x4)       
        return MAP_PRIVATE;
    if (flag == 0x5)      
        return MAP_PRIVATE;
    if (flag == 0x6)      
        return MAP_SHARED;
    if (flag == 0x7)       
        return MAP_SHARED; 
    return 0;
}

int validateProtectionFlags(Elf32_Word flag){
    if (flag == 0x1)       
        return PROT_EXEC;
    if (flag == 0x2)     
        return PROT_WRITE;
    if (flag == 0x3)       
        return PROT_WRITE | PROT_EXEC;
    if (flag == 0x4)       
        return PROT_READ;
    if (flag == 0x5)      
        return PROT_READ | PROT_EXEC;
    if (flag == 0x6)       
        return PROT_READ | PROT_WRITE;
    if (flag == 0x7)       
        return PROT_READ | PROT_WRITE | PROT_EXEC; 
    return PROT_NONE;
}

void process_phdr(Elf32_Phdr *phdr, int fd){
    if (phdr->p_type != PT_LOAD) {
        return;  
    }

    void* vaddr = (void*)(phdr->p_vaddr & 0xfffff000);
    int padding = phdr->p_vaddr & 0xfff;
    int offset = phdr->p_offset & 0xfffff000;
    size_t memszTotal = phdr->p_memsz + padding;
    void* elfFileMaped = mmap(vaddr, memszTotal, validateProtectionFlags(phdr->p_flags), MAP_PRIVATE | MAP_FIXED , fd, offset); 
    if(elfFileMaped == MAP_FAILED){
        perror("cannot map file");
        return;
    }
    else{
        display_phdr_info(phdr, 0);
    }
}


int main(int argc, char *argv[]) {

    if (argc < 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        perror("cannot open file, exiting");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    void *map_start = mmap(NULL, filesize, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_PRIVATE, fileno(file), 0);
    printf("Type\tOffset\tVirualtAddr\tPhysicalAddr\tFileSize\tMemSiz\tFlag\tAlign\tProtFlag mapFlag\n");
    iterate_phdr(map_start, process_phdr, fileno(file));

    Elf32_Ehdr *elf_header = (Elf32_Ehdr *)map_start;

    startup(argc-1, argv+1, (void *)(elf_header->e_entry));


    fclose(file);
    return 0;

}