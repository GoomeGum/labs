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


FILE* outFile;
int main(int argc, char const *argv[])
{
    if(argc!=2){
        printf("Usage: %s <output file>\n",argv[0]);
        exit(1);
    }
    outFile = fopen(argv[1],"w");
    if (outFile == NULL) {
        perror("Error opening output file");
        exit(1);
    }
    link* head = NULL;
    while(true){
        printMenu(head);
    }
	return 0;
}



void printMenu(link* head){
    int menu_size = 0;
    while(menu_items[menu_size].name !=NULL){
            menu_size++;
    }
    int choice;
        while (1) {
        printf("Select operation from the following menu:\n");
        for (int i = 0; i < menu_size; i++) {
            printf("%d. %s\n", i + 1, menu_items[i].name);
        }
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("\nExiting program.\n");
            break;
        }

        if (choice >= 1 && choice <= menu_size) {
            printf("\n");
            map(&head,menu_items[choice - 1].fun);
        }
        else {
            printf("Not within bounds\n");
            printf("\n");
            break;
        }
    }
}

void map(link** head, void (*f)(link**))
{
    f(head);
}

void loadSignatures(link** head){
    char  fileName[MAX_FILE_NAME];
    FILE* inFile = NULL;
    
    printf("Please enter file name\n");
    if (scanf("%s", fileName) != 1) {
        perror("Error reading input\n");
        exit(1); 
    }
    
    inFile = fopen(fileName, "r");
    if (inFile == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    isLitleEndian(inFile);
    while(!feof(inFile)){
        virus* virusTemp = readVirus(inFile);
        if(virusTemp->SigSize == 0){
            freeVirus(virusTemp);
            break;
        }
        *head = list_append(*head,virusTemp);
    }
    fclose(inFile);
}

void quit(link** head)
{
    printf("quitting\n");
    list_free(*head);
    fclose(outFile);
    exit(0);
}

virus* readVirus(FILE* inFile){
    virus* virusTemp = (virus*)malloc(sizeof(virus));
    virusTemp->sig = NULL;
    virusTemp->SigSize = 0;
    virusTemp->virusName[0] = '\0';
    readSignatureSize(inFile,virusTemp);
    if(virusTemp->SigSize == 0){
        return virusTemp;
    }
    readVirusName(inFile,virusTemp);
    readSignature(inFile,virusTemp);
    return virusTemp;
}

void readSignatureSize(FILE *inFile, virus* virus) {
    unsigned char bytes[2];
    if (fread(bytes, 1, 2, inFile) != 2) {
        virus->SigSize = 0;
        return;
    }
    if (isLitle) {
        virus->SigSize =  bytes[0] | (bytes[1] << 8);  
    } else {
        virus ->SigSize = bytes[1] | (bytes[0] << 8); 
    }
}

void isLitleEndian(FILE* inFile){
	char magic[4];
    fread(magic, 1, 4, inFile);
    isLitle = (magic[0] == 'V' && magic[1] == 'I' && magic[2] == 'R' && magic[3] == 'L');
}

void readVirusName(FILE* inFile, virus* virus) {
    fread(virus->virusName, 1, MAX_VIRUS_NAME , inFile);
    virus->virusName[MAX_VIRUS_NAME - 1] = '\0';
}

void readSignature(FILE* inFile, virus* virus) {
    virus->sig = (unsigned char*)malloc(virus->SigSize);
    fread(virus->sig, 1, virus->SigSize, inFile);
}

void printVirus(virus* virus, FILE* output) {
    if(output == NULL){
        printf("Error opening file\n redirecting to stdout");
        output = stdout;
    }
    fprintf(output, "\n");
    fprintf(output, "Virus name: %s\n", virus->virusName);
    fprintf(output, "Virus size: %d\n", virus->SigSize);
    fprintf(output, "Signature: \n");

    for (int i = 0; i < virus->SigSize; i++) {
        fprintf(output, "%02X ", virus->sig[i]);
        fflush(output);
    }
    
}

void freeVirus(virus* v) {
    free(v->sig);  
    v->sig = NULL; 
    free(v);
}

link* list_append(link* head,virus* v)
{
    if(head == NULL){
        link* newLink = (link*)malloc(sizeof(link));
        newLink->vir = v;
        newLink->nextLink = NULL;
        return newLink;
    }
    link* runner = head;
    while(runner->nextLink != NULL){
        runner = runner->nextLink;
    }
    link* newLink = (link*)malloc(sizeof(link));
    newLink->vir = v;
    runner->nextLink = newLink;
    newLink->nextLink = NULL;
    return head;
}

void list_print_wrapper(link** head ){
    if(*head==NULL){
        printf("list is empty\n");
        return;
    }

    list_print(*head,outFile);
}

void list_print(link* head,FILE* outFileTemp){
    link* runner = head;
    while(runner != NULL){
        printVirus(runner->vir, outFileTemp);
        runner = runner->nextLink;
    }

}

void list_free(link* head){
    while(head!=NULL){
        link* temp = head;
        head = head->nextLink;
        freeVirus(temp->vir);
        free(temp);
    }
}

void neutralize_virus(char *fileName, int signatureOffset){
    char retInstruction = 0xC3;
    char buffer[BUFFER_SIZE];
    FILE *inFile = NULL;
    inFile = fopen(fileName, "rb+");//open file for reading and writing without createing a new file in case of missing file
    if (inFile == NULL) {
        perror("Error opening the suspected file\n");
        exit(1);
    }

    unsigned int bytesRead = fread(buffer, 1, BUFFER_SIZE, inFile);
    if (bytesRead == 0) {
        printf("Failed to read the file or the file is empty.\n");
        fclose(inFile);
        return;
    }
    if (signatureOffset < BUFFER_SIZE){
        fseek(inFile, signatureOffset, SEEK_SET);
        fwrite(&retInstruction, 1, 1, inFile);
        fclose(inFile);
    } 
    else {
        perror("Index out of bounds.\n");
    }
}

void checkVirus(link** head){
    char fileName[MAX_FILE_NAME]={'\0'};
    char buffer[BUFFER_SIZE]={'\0'};
    FILE *inFile = NULL;

    printf("Please enter the suspected file name to scan for viruses: ");
    if (scanf("%s", fileName) != 1) {
        perror("Error reading input\n");
        exit(1);
    }

    inFile = fopen(fileName, "rb");
    if (inFile == NULL) {
        perror("Error opening the suspected file\n");
        exit(1);
    }

    unsigned int bytesRead = fread(buffer, 1, BUFFER_SIZE, inFile);
    if (bytesRead == 0) {
        printf("Failed to read the file or the file is empty.\n");
        fclose(inFile);
        return;
    }
    fclose(inFile);
    detect_virus(buffer, bytesRead, *head);
}

void detect_virus(char *buffer, unsigned int size, link *head) {
    // Traverse the virus list
    link *current = head;
    while (current != NULL) {
        virus *virus = current->vir;
        unsigned int virus_size = virus->SigSize;
        unsigned int compare_size = (size < virus_size) ? size : virus_size;

        for (unsigned int i = 0; i <= size - compare_size; i++) {
            if (memcmp(&buffer[i], virus->sig, compare_size) == 0) {
                printf("Virus detected at byte: %d\n", i);
                printf("Virus name: %s\n", virus->virusName);
                printf("Virus signature size: %d\n", virus->SigSize);
                
            }
        }
        current = current->nextLink; 
    }
}

void fixFile(link** head){
    char fileName[MAX_FILE_NAME];
    int signatureOffset;

    printf("Please enter the  file name to fix: ");
    if (scanf("%s", fileName) != 1) {
        perror("Error reading input\n");
        exit(1);
    }
    printf("Please enter the signature offset: ");
    if (scanf("%d", &signatureOffset) != 1) {
        perror("Error reading input\n");
        exit(1);
    }
    neutralize_virus(fileName, signatureOffset);
}