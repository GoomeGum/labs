#include<stdio.h>

#include<stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEBUG_ON "+D"
#define DEBUG_OFF "-D" 
#define PLUS_KEY "+E"
#define MINUS_KEY "-E"
#define READ_FILE "-i"
#define OUT_FILE "-o"


void setDebugTrue(bool*);
void setDebugFalse(bool*);
void printErr(bool*,const char*);
void readInput(int*,FILE*,FILE*);
void handleARGV(int argc, const char**,char**);
char encode(char c,int shift);
int* createKey(const char*);
FILE* createFile(const char*,char*);

int main(int argc, char const *argv[])
{
	int *key = malloc(sizeof(int));  
	*key = 0;                        
	FILE *infile = stdin;   
	FILE *outfile = stdout;
	bool debug = false;
	bool* debugPtr = &debug;

	for(int i = 1; i<argc;i++){
		printErr(debugPtr,argv[i]);
		//activate debug
		if (strcmp(argv[i], DEBUG_ON) == 0){
			setDebugTrue(debugPtr);
		}
		//disable debug
		else if(strcmp(argv[i], DEBUG_OFF)==0){
			setDebugFalse(debugPtr);
		}
		//encode +E -E
		else if ((strncmp(argv[i], PLUS_KEY,strlen(PLUS_KEY)) == 0) || (strncmp(argv[i], MINUS_KEY,strlen(MINUS_KEY)) == 0)) {
			if (key != NULL) {
        		free(key);
    		}
    		key = createKey(argv[i]);
		}
		//set readfile
		else if((strncmp(argv[i],READ_FILE,strlen(READ_FILE))==0)){
			if (infile != stdin) {
        		fclose(infile);
    		}

			infile =createFile(argv[i], "r");
		}
		//set outfile
		else if((strncmp(argv[i],OUT_FILE,strlen(OUT_FILE))==0)){
			if (outfile != stdout) {
        		fclose(outfile);
    		}
			outfile = createFile(argv[i], "w");
		}
	}
	readInput(key,infile,outfile);

	free(key);
	if(infile!=stdin)
		fclose(infile);
	if(outfile!=stdout)
		fclose(outfile);
	return 0;
}

int* createKey(const char* data){
	int size = sizeof(int)*((strlen(data)-2)+1);
	int mul = (data[0] == '-') ? -1 : 1;
	int* key = (int*)malloc(size);
	if (!key) {
        fprintf(stderr,"Memory allocation failed!\n");
        exit(1);
    }
    int length = strlen(data) - 2;
    for (int i = 0; i < length; i++) {
        if (data[i + 2] < '0' || data[i + 2] > '9') {
        	fprintf(stderr,"Invalid character in numeric part: %c\n", data[i + 2]);
            free(key);
            exit(1);
        }
        key[i] = mul*(data[i + 2] - '0');
        
    }
    key[size-1] = '\0';
    return key; // will be free at the main
}

void readInput(int* key,FILE *infile,FILE *outfile){

	int i = 0,size =0; 
	while(key[size]!='\0')
		size++;
	while (1) {
    	char c = fgetc(infile); 
    	if (c==EOF)
    		break;
    	c = encode(c,key[i]);
    	i = i+1;
    	i %= size;
    	fputc(c, outfile);      
    }        
}

FILE* createFile(const char* data,char* mode){
	int size = sizeof(char)*((strlen(data)-2)+1);
	char* fileName = (char*)malloc(size);
	if (!fileName) {
        fprintf(stderr,"Memory allocation failed!\n");
        exit(1);
    }
    for(int i =0;i<size-1;i++)
    	fileName[i] = data[i+2];
    fileName[size - 1] = '\0';
    FILE* file = fopen(fileName,mode);
	if (file == NULL) {
            fprintf(stderr,"Error: Could not open file %s in %s mode\n",fileName,mode);
			free(fileName);
            exit(1);
    }
    free(fileName);

    return file; 
}
void printErr(bool* debugPtr, const char* c){

	if(*debugPtr && strcmp(c, DEBUG_ON) != 0){
		fprintf(stderr, "stderr: %s\n", c);
	}
}

void setDebugFalse(bool* debugPtr){
	*debugPtr = false;
		
}

void setDebugTrue(bool* debugPtr){
	*debugPtr = true;
}

char encode(char c, int code) {
    if (c >= 'a' && c <= 'z') {
        return (char)('a' + (c - 'a' + code) % 26); 
    }
    if (c >= 'A' && c <= 'Z') {
        return (char)('A' + (c - 'A' + code) % 26); 
    }
    if (c >= '0' && c <= '9') {
        return (char)('0' + (c - '0' + code) % 10); 
	}
    return c;
}

