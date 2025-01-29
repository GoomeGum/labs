#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 128
#define MEM_BUF_SIZE 10000

typedef struct {
    char debug_mode;
    char file_name[MAX_FILENAME];
    int unit_size;
    unsigned char mem_buf[MEM_BUF_SIZE];
    size_t mem_count;
} state;

void toggle_debug_mode(state* s) {
    if (s->debug_mode) {
        s->debug_mode = 0;
        printf("Debug flag now off\n");
    } else {
        s->debug_mode = 1;
        printf("Debug flag now on\n");
    }
}

void set_file_name(state* s) {
    printf("Enter file name: ");
    if (fgets(s->file_name, MAX_FILENAME, stdin)) {
        s->file_name[strcspn(s->file_name, "\n")] = '\0'; // Remove newline character
        if (s->debug_mode) {
            fprintf(stderr, "Debug: file name set to '%s'\n", s->file_name);
        }
    }
}

void set_unit_size(state* s) {
    int size;
    printf("Enter unit size (1, 2, or 4): ");
    if (scanf("%d", &size) == 1 && (size == 1 || size == 2 || size == 4)) {
        s->unit_size = size;
        if (s->debug_mode) {
            fprintf(stderr, "Debug: set size to %d\n", size);
        }
    } else {
        printf("Invalid unit size\n");
    }
    while (getchar() != '\n'); // Clear input buffer
}

void quit(state* s) {
    if (s->debug_mode) {
        printf("quitting\n");
    }
    exit(0);
}

void not_implemented_yet(state* s) {
    printf("Not implemented yet\n");
}

typedef struct {
    char* name;
    void (*func)(state*);
} menu_option;

void print_debug_info(state* s) {
    if (s->debug_mode) {
        fprintf(stderr, "Debug: unit_size=%d, file_name='%s', mem_count=%zu\n", 
                s->unit_size, s->file_name, s->mem_count);
    }
}

void display_menu(menu_option* menu) {
    printf("Choose action:\n");
    for (int i = 0; menu[i].name != NULL; i++) {
        printf("%d-%s\n", i, menu[i].name);
    }
}

int main() {
    state s = {0, "", 1, {0}, 0};
    menu_option menu[] = {
        {"Toggle Debug Mode", toggle_debug_mode},
        {"Set File Name", set_file_name},
        {"Set Unit Size", set_unit_size},
        {"Load Into Memory", not_implemented_yet},
        {"Toggle Display Mode", not_implemented_yet},
        {"File Display", not_implemented_yet},
        {"Memory Display", not_implemented_yet},
        {"Save Into File", not_implemented_yet},
        {"Memory Modify", not_implemented_yet},
        {"Quit", quit},
        {NULL, NULL}
    };

    while (1) {
        print_debug_info(&s);
        display_menu(menu);
        printf("Option: ");

        int choice;
        if (scanf("%d", &choice) == 1) {
            while (getchar() != '\n'); // Clear input buffer
            if (choice >= 0 && menu[choice].func != NULL) {
                menu[choice].func(&s);
            } else {
                printf("Invalid option\n");
            }
        } else {
            printf("Invalid input\n");
            while (getchar() != '\n'); // Clear input buffer
        }
    }

    return 0;
}
