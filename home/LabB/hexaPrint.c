#include <stdio.h>
#include <stdlib.h>

#define CHUNK_SIZE 16  // Define X as 16 bytes per chunk

// Function to print bytes in hexadecimal format
void PrintHex(unsigned char *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]); // Print each byte in 2-digit hexadecimal
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    unsigned char buffer[CHUNK_SIZE];
    size_t bytesRead;

    // Read and print chunks of the file
    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {
        PrintHex(buffer, bytesRead);
    }

    if (ferror(file)) {
        perror("Error reading file");
        fclose(file);
        return EXIT_FAILURE;
    }

    fclose(file);
    return EXIT_SUCCESS;
}
