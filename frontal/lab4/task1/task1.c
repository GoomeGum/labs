#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 128
#define MEM_BUF_SIZE 10000

typedef struct
{
    char debug_mode;
    char file_name[MAX_FILENAME];
    int unit_size;
    unsigned char mem_buf[MEM_BUF_SIZE];
    size_t mem_count;
    char display_mode; // 0 for decimal, 1 for hexadecimal
} state;

void toggle_debug_mode(state *s)
{
    if (s->debug_mode)
    {
        s->debug_mode = 0;
        printf("Debug flag now off\n");
    }
    else
    {
        s->debug_mode = 1;
        printf("Debug flag now on\n");
    }
}

void toggle_display_mode(state *s)
{
    if (s->display_mode)
    {
        s->display_mode = 0;
        printf("Display flag now off, decimal representation\n");
    }
    else
    {
        s->display_mode = 1;
        printf("Display flag now on, hexadecimal representation\n");
    }
}

void set_file_name(state *s)
{
    printf("Enter file name: ");
    if (fgets(s->file_name, MAX_FILENAME, stdin))
    {
        s->file_name[strcspn(s->file_name, "\n")] = '\0'; // Remove newline character
        if (s->debug_mode)
        {
            fprintf(stderr, "Debug: file name set to '%s'\n", s->file_name);
        }
    }
}

void set_unit_size(state *s)
{
    int size;
    printf("Enter unit size (1, 2, or 4): ");
    if (scanf("%d", &size) == 1 && (size == 1 || size == 2 || size == 4))
    {
        s->unit_size = size;
        if (s->debug_mode)
        {
            fprintf(stderr, "Debug: set size to %d\n", size);
        }
    }
    else
    {
        printf("Invalid unit size\n");
    }
    while (getchar() != '\n')
        ; // Clear input buffer
}

void load_into_memory(state *s)
{
    if (strcmp(s->file_name, "") == 0)
    {
        printf("Error: File name is empty.\n");
        return;
    }

    FILE *file = fopen(s->file_name, "rb");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    char input[100];
    unsigned int location;
    int length;

    printf("Please enter <location> <length>: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %d", &location, &length) != 2)
    {
        printf("Invalid input\n");
        fclose(file);
        return;
    }

    if (s->debug_mode)
    {
        fprintf(stderr, "Debug: file_name='%s', location=0x%X, length=%d\n", s->file_name, location, length);
    }

    size_t bytes_to_read = length * s->unit_size;
    if (fseek(file, location, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return;
    }

    size_t bytes_read = fread(s->mem_buf, 1, bytes_to_read, file);
    if (bytes_read < bytes_to_read)
    {
        if (feof(file))
        {
            printf("Warning: Reached end of file. Only %zu units loaded.\n", bytes_read / s->unit_size);
        }
        else
        {
            perror("Error reading file");
        }
    }
    else
    {
        printf("Loaded %zu units into memory\n", bytes_read / s->unit_size);
    }

    s->mem_count = bytes_read;
    fclose(file);
}

void display_file(state *s)
{
    if (strcmp(s->file_name, "") == 0)
    {
        printf("Error: File name is empty.\n");
        return;
    }

    FILE *file = fopen(s->file_name, "rb");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    char input[100];
    unsigned int offset;
    int length;

    printf("Enter file offset and length: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %d", &offset, &length) != 2)
    {
        printf("Invalid input\n");
        fclose(file);
        return;
    }

    if (fseek(file, offset, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return;
    }

    printf(s->display_mode ? "Hexadecimal\n===========\n" : "Decimal\n=======\n");
    for (int i = 0; i < length; i++)
    {
        unsigned int value = 0;
        fread(&value, s->unit_size, 1, file);
        if (s->display_mode)
        {
            printf(s->unit_size == 1 ? "%#hhx\n" : s->unit_size == 2 ? "%#hx\n"
                                                                     : "%#x\n",
                   value);
        }
        else
        {
            printf(s->unit_size == 1 ? "%#hhd\n" : s->unit_size == 2 ? "%#hd\n"
                                                                     : "%#d\n",
                   value);
        }
    }

    fclose(file);
}

void display_memory(state *s)
{
    char input[100];
    unsigned int addr;
    int length;

    printf("Enter address and length: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %d", &addr, &length) != 2)
    {
        printf("Invalid input\n");
        return;
    }

    unsigned char *start = addr ? (unsigned char *)addr : s->mem_buf;

    printf(s->display_mode ? "Hexadecimal\n===========\n" : "Decimal\n=======\n");
    for (int i = 0; i < length; i++)
    {
        unsigned int value = 0;
        memcpy(&value, start + i * s->unit_size, s->unit_size);
        if (s->display_mode)
        {
            printf(s->unit_size == 1 ? "%#hhx\n" : s->unit_size == 2 ? "%#hx\n"
                                                                     : "%#x\n",
                   value);
        }
        else
        {
            printf(s->unit_size == 1 ? "%#hhd\n" : s->unit_size == 2 ? "%#hd\n"
                                                                     : "%#d\n",
                   value);
        }
    }
}

void save_into_file(state *s)
{
    if (strcmp(s->file_name, "") == 0)
    {
        printf("Error: File name is empty.\n");
        return;
    }

    char input[100];
    unsigned int source_address, target_location;
    int length;

    printf("Please enter <source-address> <target-location> <length>: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %x %d", &source_address, &target_location, &length) != 3)
    {
        printf("Invalid input\n");
        return;
    }

    unsigned char *src_ptr = source_address ? (unsigned char *)source_address : s->mem_buf;
    size_t bytes_to_write = length * s->unit_size;

    FILE *file = fopen(s->file_name, "r+b");
    if (!file)
    {
        perror("Error opening file for writing");
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);

    if (target_location > file_size)
    {
        printf("Error: Target location exceeds file size.\n");
        fclose(file);
        return;
    }

    if (fseek(file, target_location, SEEK_SET) != 0)
    {
        perror("Error seeking in file");
        fclose(file);
        return;
    }

    size_t bytes_written = fwrite(src_ptr, 1, bytes_to_write, file);
    if (bytes_written < bytes_to_write)
    {
        perror("Error writing to file");
    }
    else
    {
        printf("Wrote %zu units to file\n", bytes_written / s->unit_size);
    }

    fclose(file);
}

void memory_modify(state *s)
{
    char input[100];
    unsigned int location;
    unsigned int value;

    printf("Please enter <location> <val>: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%x %x", &location, &value) != 2)
    {
        printf("Invalid input\n");
        return;
    }

    if (location + s->unit_size > MEM_BUF_SIZE)
    {
        printf("Error: Location out of bounds.\n");
        return;
    }

    if (s->debug_mode)
    {
        fprintf(stderr, "Debug: location=0x%X, value=0x%X\n", location, value);
    }

    memcpy(s->mem_buf + location, &value, s->unit_size);
    printf("Memory modified at location 0x%X with value 0x%X\n", location, value);
}

void quit(state *s)
{
    if (s->debug_mode)
    {
        printf("quitting\n");
    }
    exit(0);
}

typedef struct
{
    char *name;
    void (*func)(state *);
} menu_option;

void print_debug_info(state *s)
{
    if (s->debug_mode)
    {
        fprintf(stderr, "Debug: unit_size=%d, file_name='%s', mem_count=%zu, display_mode=%s\n",
                s->unit_size, s->file_name, s->mem_count, s->display_mode ? "hexadecimal" : "decimal");
    }
}

void display_menu(menu_option *menu)
{
    printf("Choose action:\n");
    for (int i = 0; menu[i].name != NULL; i++)
    {
        printf("%d-%s\n", i, menu[i].name);
    }
}

int main()
{
    state s = {0, "", 1, {0}, 0, 0};
    menu_option menu[] = {
        {"Toggle Debug Mode", toggle_debug_mode},
        {"Set File Name", set_file_name},
        {"Set Unit Size", set_unit_size},
        {"Load Into Memory", load_into_memory},
        {"Toggle Display Mode", toggle_display_mode},
        {"File Display", display_file},
        {"Memory Display", display_memory},
        {"Save Into File", save_into_file},
        {"Memory Modify", memory_modify},
        {"Quit", quit},
        {NULL, NULL}};

    while (1)
    {
        print_debug_info(&s);
        display_menu(menu);
        printf("Option: ");

        int choice;
        if (scanf("%d", &choice) == 1)
        {
            while (getchar() != '\n')
                ; // Clear input buffer
            if (choice >= 0 && menu[choice].func != NULL)
            {
                menu[choice].func(&s);
            }
            else
            {
                printf("Invalid option\n");
            }
        }
        else
        {
            printf("Invalid input\n");
            while (getchar() != '\n')
                ; // Clear input buffer
        }
    }

    return 0;
}
