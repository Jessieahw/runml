#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <stdbool.h>
int identifierCount = 0;
// int functionDefined = 0;
// char functions[];,
int functionCounts =0;
char identifiers[50][13];

// for identifiers
int isValidIdentifier(const char str[]) {
    
    int length = strlen(str);
    // if (length>50){
    //     return 0;
    // }
    if(length<1 || length>12){
        return 0;
    }

    for (int i=0; i<length;i++){
        if(!isalpha(str[i])||!islower(str[i])){
            return 0;
        }
    }

    return 1;
}

int isValidNumber(const char str[]){

    int length = strlen(str);
    int dotCount = 0;
    
    // if (length == 0) return 0;

    for (int i = 0; i < length; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1){
                return 0;
            }
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    
    return 1;

}

void checkSyntax(const char filename[]) {
    FILE   *file = fopen(filename, "r");

//  CHECK IF ANYTHING WENT WRONG
    if(file == NULL) {
        fprintf(stderr, "Cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[256];

    while( fgets(line, sizeof line, file) != NULL ) {  
        int len = strlen(line);
        if (line[len-1] == '\n') {
            line[len-1] = '\0';  // Remove newline character
            // len--;  // Decrement length after removing \n
        }
        if (line[len-1] == '\r') {
            line[len-1] = '\0';  // Remove carriage return if it exists
        }
    // line[len-1] = '\0';
        // for comment or empty
        char *comment = strchr(line,'#');
        if (comment!= NULL){
            *comment = '\0';
            continue;
        }
        else if (strlen(line)==0){
            continue;
        }
      int hasFunction = strncmp(line,"function",8);
        if(hasFunction==0){//has function: function multiply a b
            //                             012345678901234567890
            // functionDefined=1;
            // int indexSpaces[50];
            // indexSpaces[0]=8;
            int i = 9;
            // char identifiers[50];
            // int identifierCount=0;
            while(line[i]!='\0'){// not the end of the line
              // }
                while (line[i] == ' ') {// finding where is the first space
                    i++;
                }
                // functionCounts++;
                strncpy(identifiers[functionCounts], line + 9, i - 9);
                identifiers[functionCounts][i-9]='\0';
                functionCounts++;
                int start = i;
                if (functionCounts >= 50) {
                    fprintf(stderr, "Error: Too many function identifiers\n");
                    exit(EXIT_FAILURE);
                }
                // Find the end of the current identifier
                while (line[i] != ' ' && line[i] != '\0') {
                    i++;
                }

                int length = i - start; // get the length of the identifier
                if (length > 0) {
                    // Extract the identifier
                    char identifier[length + 1];
                    strncpy(identifier, line + start, length);
                    identifier[length] = '\0';

                    // Validate the identifier
                    if (isdigit(identifier[0])&& !isValidNumber(identifier)){
                        // if(!isValidNumber(identifier)){
                            fprintf(stderr, "! Error: Invalid identifier '%s'\n", identifier);
                            exit(EXIT_FAILURE);
                        }
                        //if it's digit,need to check one more time
                    else if (!isValidIdentifier(identifier)) {
                        fprintf(stderr, "! Error: Invalid identifier '%s'\n", identifier);
                        exit(EXIT_FAILURE);
                    }
                    identifierCount++;
                    if(identifierCount>50){
                        fprintf(stderr, "! Error: identifier exceed'%s'\n", identifier);
                        exit(EXIT_FAILURE);
                    }
                    
                }
            
        }}
        // else if()
    // return 1;
        }fclose(file);
}
            

void translateCode(const char *filename){
    printf("Starting translation...\n");
    
    // printf("k");
    FILE *newFile;
    char newName[20]; // 3 (ml-) + 7 (pid) + 2 (.c) + 1 (null character) = 13
    printf("Generating filename with getpid...\n");
    sprintf(newName,"ml-%d.c",getpid());
   
    printf("Generated filename: %s\n", newName);
    printf("about to start: %s",newName);
    // fileName=newName;
    newFile = fopen(newName,"w");
    if (newFile == NULL) {
        fprintf(stderr, "Error: Failed to open file '%s' for writing.\n", newName);
        perror("fopen");
        exit(EXIT_FAILURE);  // Terminate program if file can't be opened
    }
    printf("File opened successfully: %s\n", newName);
    // printf("about to start");
fprintf(newFile,"#include <stdlib.h>\n");

fprintf(newFile,"#include <stdio.h>\n");
fprintf(newFile,"#include <math.h>\n");
printf("About to write: int main() { \n");
fprintf(newFile,"\n int main() { \n");     

    // if( newFile == NULL) {
    //     fprintf(stderr,"cannot open '%s'\n", newName);
    //     exit(EXIT_FAILURE);
    // }
    printf("f");
    FILE *file = fopen(filename,"r");
    if( file == NULL) {
        fprintf(stderr,"cannot open '%s'\n", filename);
        fclose(newFile);
        exit(EXIT_FAILURE);
    }

    char line[256];
    char vaName[50][50];
int count = 0;
bool used[50] = {false};

    while (fgets(line, sizeof(line), file)) {
        printf("f");
        bool inFunction=false;

        int len = strlen(line);
        if (line[len-1] == '\n') {
            line[len-1] = '\0';  // Remove newline character
            // len--;  // Decrement length after removing \n
        }
        if (line[len-1] == '\r') {
            line[len-1] = '\0';  // Remove carriage return if it exists
        }
            if(strstr(line,"#")){
                continue;
            }

            if(strstr(line,"function")){
                inFunction=true;
                char fName[50];
                char pName[50][50];
                // int count=0;
                int start=9;
                int findNext=9;
                int findSpace=9;
                int countSpace=0;
                while(line[findNext]!=' '){//function printsum a b\0
                    findNext++;           // 0123456789012345678901
                }
                while(line[findSpace]!='\0'){
                    if(line[findSpace]==' '){
                        countSpace++;
                    }
                    findSpace++;
                }//findspace = 20, countspace=2
                while(line[findNext]!=' '){//function printsum a b\0
                    findNext++;           // 0123456789012345678901
                }//findnext = 17
                strncpy(fName,line+start,findNext-start);//17-9
                fName[findNext-start]='\0';
                findNext++;//18
                start = findNext;//18
                for(int i =0; i<countSpace;i++){

                    while(line[findNext]!=' '&&line[findNext]!='\0' ){
                                               //function printsum a b\0
                    findNext++;               // 0123456789012345678901
                    }//19  //21
                    strncpy(pName[i],line+start,findNext-start);
                    pName[i][findNext-start]='\0';
                    start = findNext+1;//20
                    findNext++;//20
                }
                fprintf(newFile,"void %s(",fName);
                for(int i=0;i<countSpace;i++){
                    fprintf(newFile,"double %s",pName[i]);
                    if(i<countSpace-1){
                        fprintf(newFile,", ");
                    }
                }
                fprintf(newFile,") {\n");
                
            }
            if(line[0]=='\t'){

            }
            if(strstr(line,"return")){

                char expression[256];
                char *start = strstr(line,"return ")+7;
                strncpy(expression,start,sizeof(expression)-1);
                expression[sizeof(expression)-1]='\0';
                fprintf(newFile,"return %s;\n",expression);
            }
            if(strstr(line,"<-")){
                char name[50];
                char value[50];
                const char *start = line;
                int index;
                char *place = strstr(line,"<-");
                index = place-start;
                if(index-1<sizeof(name)){
                    strncpy(name,line,index-1);
                    name[index-1]='\0';
                }else{
                    fprintf(stderr, "Error: identifier is too long.\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(value,place+3);
                fprintf(newFile,"double %s = %.6f;\n",name,atof(value));
                if(count<50){
                    strncpy(vaName[count],name,sizeof(name));
                count++;
                }else{
                    fprintf(stderr, "Too many variables.\n");
                    exit(EXIT_FAILURE);
                }
                 }
            if (strstr(line, "print")) {
            char expression[256];
            char *place = strstr(line,"print ");
            char *start = place+6;
            strncpy(expression,start,sizeof(expression)-1);
            expression[sizeof(expression)-1]='\0';

            // store it in a variable called printing, float. later in the c file check the type?
            fprintf(newFile,"double toPrint = %s;\n",expression);
            fprintf(newFile,"if(nearbyint(toPrint)==toPrint){\n");
            fprintf(newFile,"printf(\"%%.f\\n\",toPrint);");
            fprintf(newFile,"}\n");
            fprintf(newFile,"else{\n");
            fprintf(newFile,"printf(\"%%.6f\\n\",toPrint);");
            fprintf(newFile,"}\n");
            for(int i =0;i<count;i++){
                if(strstr(line,vaName[i])){
                used[i]=true;
                }
            }
        }
        }


for(int i =0;i<count;i++){
    if(!used[i]){
        // printf("%d",count);
            fprintf(newFile,"(void)%s;\n",vaName[i]);
        }
}


fprintf(newFile,"return 0;\n");
fprintf(newFile,"}\n"); 
fclose(file);
fclose(newFile);

char command[256];
snprintf(command, sizeof(command), "cc -std=c11 -Wall -Werror %s -o Newrunml", newName);
int run = system(command);  // system() only takes one argument (a string)
if (run == -1) {
    fprintf(stderr, "Error running command: %s\n", command);
    exit(EXIT_FAILURE);
}

char command2[256];
snprintf(command2, sizeof(command2), "./Newrunml");
int run2 = system(command2);
if (run2 == -1) {
    fprintf(stderr, "Error running command: %s\n", command);
    exit(EXIT_FAILURE);
}
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    checkSyntax(argv[1]);
    // printf("f");
    // printf("Filename provided: %s\n", argv[1]);
 
printf("Filename provided: %s\n", argv[1]);
// printf("f");
    translateCode(argv[1]);
    return EXIT_SUCCESS;
}
