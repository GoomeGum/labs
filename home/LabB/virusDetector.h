#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h> 

#define MAX_VIRUS_NAME 16
#define MAX_FILE_NAME 100   
#define BUFFER_SIZE 10240  // 10 KB buffer

typedef struct virus {
unsigned short SigSize;
char virusName[MAX_VIRUS_NAME];
unsigned char* sig;
} virus;

typedef struct link {
    struct link *nextLink;
    virus *vir;
}link;


struct fun_desc
{
    char* name;
    void (*fun)();
};



/*virus functions*/
void loadSignatures(link**);
void quit(link**);
virus* readVirus(FILE*); //system
void printVirus(virus*, FILE*); //system
void detect_virus(char *buffer, unsigned int size, link *virus_list);//system
void isLitleEndian(FILE*);
void readSignatureSize(FILE*,virus*);
void readVirusName(FILE*, virus*);
void readSignature(FILE*, virus*);
void freeVirus(virus*);
bool isLitle = false;


/*link functions*/

struct link* list_append(link*,virus*); //system
void list_print(link*,FILE*); //system
void list_print_wrapper(link**); 
void list_free(link*); //system
void checkVirus(link**);
void neutralize_virus(char *fileName, int signatureOffset);
void fixFile(link**);

/*menu*/
struct fun_desc menu_items[] = {
    {"Load signatures",loadSignatures},
    {"Print signatures",list_print_wrapper},
    {"Detect viruses",checkVirus},
    {"Fix file",fixFile},
    {"Quit",quit},
    {NULL,NULL}};

void printMenu(link* head);
void map(link**, void (*f) (link**));