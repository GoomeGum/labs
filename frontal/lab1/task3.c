#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CARRAY_SIZE 5

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i =0; i < array_length;i++)
      mapped_array[i] = f(array[i]);
  return mapped_array;
}


char my_get(char c) {
    return fgetc(stdin);
}

char cprt(char c) {
    if (c >= 0x20 && c <= 0x7E) {
        putchar((int)c);   
    } else {
        putchar((int)'.'); 
    }
    putchar((int)'\n');   
    return c;        
}

char encrypt(char c) {
    if (c >= 0x20 && c <= 0x7E) {
        return c + 1; 
    }
    return c; 
}

char decrypt(char c) {
    if (c >= 0x21 && c <= 0x7F) {
        return c - 1; 
    }
    return c; 
}

char xprt(char c) {
    printf("%02X\n", c);  
    return c;             
}

char dprt(char c) {
    printf("%d\n", c);  
    return c;           
}

struct fun_desc
{
    char* name;
    char (*fun)(char);
};

struct fun_desc menu_items[] = {
    {"my_get",my_get},
    {"cprt",cprt},
    {"encrypt",encrypt},
    {"decrypt",decrypt},
    {"xprt",xprt},
    {"dprt",dprt},
    {NULL,NULL}};


void menu(){
    int menu_size = 0;
    char* carray = (char*)malloc(CARRAY_SIZE* sizeof(char)) ;
    if (carray == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    } 
    for (int i = 0; i < CARRAY_SIZE; i++) {
        carray[i] = '\0';
    }
    while(menu_items[menu_size].name !=NULL){
            menu_size++;
    }
    int choice;
        while (1) {
        printf("Select operation from the following menu:\n");
        for (int i = 0; i < menu_size; i++) {
            printf("%d. %s\n", i + 1, menu_items[i].name);
        }
        printf("%d. Exit\n", menu_size + 1);

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("\nExiting program.\n");
            break;
        }

        if (choice >= 1 && choice <= menu_size) {
            printf("Within bounds\n");
            carray = map(carray, 5, menu_items[choice - 1].fun);
        } else if (choice == menu_size + 1) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Not within bounds\n");
            break;
        }
    }

    free(carray); 
}

int main(int argc, char **argv) {
    menu();
    return 0;
}
