// CITS2002 Project 1 2024
// Student 1:23673705 Jessie Ou
// Platform:Apple

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

int identifierCount = 0;
char identifiers[100][100]; //for variable identifiers
char records[50][50]; //for function names
char records_count[50]; //for parameter count for each function
int count = 0; // for number of functions
int arguments[50]; //to store argument values
int arguments_count = 0; // for number of arguments passed
int process_id = -1; // for process ID for creating unique filenames

// structure to store function name and the number of parameters
typedef struct {
    char name[50];
    int paramCount;  
} FunctionRecord;

// function to get expected parameter count of a function, knowing the name of the function
int getExpectedParamCount(const char *functionName, int recordCount){
    for (int i = 0;i< recordCount;i++){
        if (strcmp(records[i],functionName) == 0){
            return records_count[i];
        }
    }
    return -1;
}

// function to check if a variable was declared before
bool existVariable(const char *varName, char identifier[100][100], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(identifier[i], varName) == 0) {
            return true;
        }
    }
    return false;
}

// function to check if a function exists
bool existFunction(const char *varName, char record[50][50], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(record[i], varName) == 0) {
            return true;
        }
    }
    return false;
}

// function to skip leading whitespace including tab
char *skip_leading_whitespace(char *str) {
    while (isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

// function to check if an identifier is valid
int isValidIdentifier(char *token) {
    int length = strlen(token);
    if(length<1 || length>12){
        return 0;
    }
    for (int i=0; i<length;i++){       
        if(!islower(token[i])){
            return 0;
        }
    }
    return 1;
}

// function to check if a number is valid real number
int isValidNumber(char *token){
    int length = strlen(token);
    int dotCount = 0;
    for (int i = 0; i < length; i++) {
        if (token[i] == '.') {
            dotCount++;
            if (dotCount > 1){ // if it has two dot, then it is invalid
                return 0;
            }
        } else if (!isdigit(token[i])) {
            return 0;
        }
    }
    return 1;
}

// function to validate a line
void checkLine(char *line){
    line[strlen(line) - 1] = '\0'; 
    char *token, *token2,*token3,*token4;
    char *saveptr1, *saveptr2,*saveptr3,*saveptr4;
    char *varName;
    int index=0;
    // if it has assignment operator
    if(strstr(line,"<-")!=NULL){
        // split it into the token based on the assignment operator
        token3=strtok_r(line,"<-",&saveptr3);
        if (line[0] !='\t'){
            char *Newtoken3= skip_leading_whitespace(token3);
            while(token3!=NULL && index==0){
                if(strstr(Newtoken3," ") != NULL ){
                    // split it again based on the space to get the variable name
                    token4=strtok_r(Newtoken3," ",&saveptr4);
                    while(token4!=NULL){
                        varName=token4;
                        token4=strtok_r(NULL," ",&saveptr4);
                    }
                }
                else{
                    varName=Newtoken3; // get the variable name
                }
                token3=strtok_r(NULL,"<-",&saveptr3);
                index++;
            }
            strcpy(identifiers[identifierCount++],varName); // store the identifiers
            if(identifierCount>50){
                fprintf(stderr, "! Error: the number of identifiers exceed 50\n");
                exit(EXIT_FAILURE);
            }

        }
    }
    // split the line based on space
    token = strtok_r(line, " ", &saveptr1);
    while (token != NULL){
        char *newToken = skip_leading_whitespace(token);
        if (strcmp(newToken, "print") == 0 || strcmp(newToken, "return") == 0 || strpbrk(line, "+-*/") != NULL)
        {
            token = strtok_r(NULL, " ", &saveptr1);
            continue;
        }
        // split it again based on brackets and commas
        token2 = strtok_r(newToken, "(,)", &saveptr2);
        while (token2 != NULL)
        {
            if (isdigit(token2[0]))
            {
                if (!isValidNumber(token2))
                {
                    fprintf(stderr, "! Error: Invalid number '%s'\n", token2);
                    exit(EXIT_FAILURE);
                }
            }
            if (isalpha(token2[0]))
            {
                if (!isValidIdentifier(token2))
                {
                    fprintf(stderr, "! Error: Invalid identifier '%s'\n", token2);
                    exit(EXIT_FAILURE);
                }
            }
            token2 = strtok_r(NULL, "(,)", &saveptr2);
        }
        // check is the current token a value or variable name
        if (isdigit(newToken[0]))
        {
            if (!isValidNumber(newToken))
            {
                fprintf(stderr, "! Error: Invalid number '%s'\n", newToken);
                exit(EXIT_FAILURE);
            }
        }
        if (isalpha(newToken[0]))
        {
            if (!isValidIdentifier(newToken))
            {
                fprintf(stderr, "! Error: Invalid identifier '%s'\n", newToken);
                exit(EXIT_FAILURE);
            }
        }
        token = strtok_r(NULL, " ", &saveptr1);
    }
}

// function to check if the current line contains "print"
bool is_exact_print(const char *line) {
    const char *pos = strstr(line, "print");
    while (pos != NULL) {
        // make sure "print" is not part of a identifier
        if ((pos[5] == '\0' || !isalnum((unsigned char)pos[5])) && 
            (pos == line || !isalnum((unsigned char)pos[-1]))) {   
            return true;  
        }
        pos = strstr(pos + 1, "print");
    }
    return false; 
}

// function to check syntax, which will pass lines into the checkLine 
void checkSyntax(const char filename[]) {
    FILE   *file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[1000];
    while( fgets(line, sizeof line, file) != NULL ) {  
        if (strstr(line,"#")!= NULL || strlen(line)==0){
            continue;
        }
        checkLine(line);
    }
    fclose(file);
}

// function to translate ML into C and execute the translated file, and delete it at the end
void translateCode(const char *filename){
    FILE *newFile;
    char newName[13]; 
    process_id = getpid();
    sprintf(newName,"ml-%d.c",process_id);
    newFile = fopen(newName,"w");
    fprintf(newFile,"#include <stdlib.h>\n");
    fprintf(newFile,"#include <stdio.h>\n");
    fprintf(newFile,"#include <math.h>\n");
    // for command-line arguments
    for (int i = 0; i< arguments_count;i++){
        fprintf(newFile,"double arg");
        fprintf(newFile,"%d",i);
        fprintf(newFile," = ");
        fprintf(newFile,"%d",arguments[i]);
        fprintf(newFile,";\n");
    }
    // define custom_print to print real-number
    fprintf(newFile,"\n");
    fprintf(newFile, "void custom_print(double value) {\n");
    fprintf(newFile, "    double int_part, frac_part;\n");
    fprintf(newFile, "    frac_part = modf(value, &int_part);\n");
    fprintf(newFile, "    if (frac_part == 0.0) {\n");
    fprintf(newFile, "        printf(\"%%.0f\\n\", value);\n"); // only print the integer part
    fprintf(newFile, "    } else {\n");
    fprintf(newFile, "        printf(\"%%.6f\\n\", value);\n"); // print it with six decimal places
    fprintf(newFile, "    }\n");
    fprintf(newFile, "}\n");
    FILE *file = fopen(filename,"r");
    if( file == NULL) {
        fprintf(stderr,"cannot open '%s'\n", filename);
        fclose(newFile);
        exit(EXIT_FAILURE);
    }
    char line[256];
    int function_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if(line[0]=='#'){ // ignore comments
            continue;
        }
        if (strstr(line,"function") != NULL){
            function_count++; // counting how many functions are there
        }
    }
    // reset the pointer back to the beginner, start translating
    rewind(file);
    bool main_added = false; // check if main is added or not
    int function_number = 0;
    // go through each line
    while (fgets(line, sizeof(line), file) != NULL) {
        if(line[0]=='#'){
            continue;
        }
        // when all the functions are added, put main function
        if (function_number == function_count && !main_added){
            main_added = true;
            fprintf(newFile,"\n int main() { \n");
        }
        line[strcspn(line, "\n")] = 0; 
        if (line[0]=='#' || line[0]=='\t') {
            continue;
        }
        // start to translate the function
        else if (strstr(line,"function") != NULL){
            char parameter_names[100][100]; 
            int param_count = 0;

            char linebuffer[10000];
            char function_identifier[100][100];
            int local_count = 0;
            
            //define multiple pointers variable to perform strtok on later
            char *token3;
            char *token4;
            char *saveptr3;
            char *saveptr4;

            char *varName;

            int index = 0;

            snprintf(linebuffer,sizeof(linebuffer)," ");     

            bool hasReturn = false; 

            function_number++; 

            char line_copy[1000];
            strcpy(line_copy, line); // make a copy of the line
            char *token = strtok(line_copy, " "); // split it based on space
            
            int function_index = 0; // plan to use index to translate 
            while (token != NULL) {
                if (function_index == 1){ // translate the function name which has index 1
                    strcpy(records[count], token);
                    strcat(linebuffer,token);
                    strcat(linebuffer," (");
                    strcpy(identifiers[identifierCount++],token); // to store function name because it is also identifiers
                    if (identifierCount > 50){
                          fprintf(stderr, "! Error: the number of identifiers exceed 50\n");
                        exit(EXIT_FAILURE);
                    }
                }   
                if (function_index == 2){ // translate the first parameter
                    strcat(linebuffer,"double ");
                    strcat(linebuffer,token);
                    records_count[count]++;
                    // to store parameters, for checking whether it is declared or added in the later part
                    strcpy(function_identifier[local_count++],token);
                    strcpy(parameter_names[param_count++], token);
                }
                if (function_index > 2){ // translate some more parameters
                    strcat(linebuffer,",double ");
                    strcat(linebuffer,token);
                    records_count[count]++;
                    strcpy(function_identifier[local_count++],token); 
                    strcpy(parameter_names[param_count++], token);
                }
                function_index++; // increment to looping through each token based on the index
                token = strtok(NULL, " "); 
            }
            count++; // increment record ccount for functions
            strcat(linebuffer,"){\n");
            // start to translate the function body
            long place = ftell(file);
            while (fgets(line, sizeof(line), file) != NULL) {
                if(line[0]=='#'){
                    continue;
                }
                if (line[0]!='\t'){
                    fseek(file,place,SEEK_SET);
                    // break it when there is no tab at the beginning of the line which means the function ends
                    break;
                }
                // if printing is needed, execute custom_print to achieve it
                if (strstr(line,"print") != NULL){
                    char line_copy2[1000];
                    strcpy(line_copy2, line); // make a copy of the line
                    char *token = strtok(line_copy2, " "); // split it based on the space
                    strcat(linebuffer,"custom_print(");
                    while (token != NULL) {
                        char *trimmed_token = skip_leading_whitespace(token);
                        size_t len = strlen(trimmed_token);
                        if (len > 0 && trimmed_token[len - 1] == '\n') {
                            trimmed_token[len - 1] = '\0';
                        }
                        if (strcmp(trimmed_token,"print") == 0){
                            token = strtok(NULL, " "); 
                                continue;
                        }
                        else if (isalpha(trimmed_token[0])){ 
                            // if the token starts with letter, then check whether it is declared as identifiers or parameters before
                            if(existVariable(trimmed_token, function_identifier,100) || existVariable(trimmed_token, identifiers,100)||existVariable(trimmed_token,parameter_names,100))  {
                                // if it is declared before, add it to translated code
                                strcat(linebuffer,trimmed_token);
                            }
                            else{
                                // if it is not declared, set it to 0.0
                                strcat(linebuffer,"0.0"); 
                            }
                        }
                        else{
                            strcat(linebuffer,trimmed_token);
                        }
                        token = strtok(NULL, " "); 
                    }
                    strcat(linebuffer,");\n");
                }
                // translate the assignments and return part
                else if (strstr(line,"<-") != NULL){
                    char *token = strtok(line, " ");
                    token3=strtok_r(line,"<-",&saveptr3);
                    char *Newtoken3= skip_leading_whitespace(token3);
                    while(token3!=NULL && index==0){
                        if(strstr(Newtoken3," ") != NULL){
                            token4=strtok_r(Newtoken3," ",&saveptr4);
                            while(token4!=NULL){
                                varName=token4;
                                token4=strtok_r(NULL," ",&saveptr4);
                                strcpy(function_identifier[local_count++],token4);
                            }
                        }
                        else{
                            varName=Newtoken3;
                            strcpy(function_identifier[local_count++],varName);
                        }
                        token3=strtok_r(NULL,"<-",&saveptr3);
                        index++;
                    }
                    strcat(linebuffer,"double ");
                    int index = 0;
                    while (token != NULL) {
                        if (index == 0){
                            strcat(linebuffer,token);
                            strcat(linebuffer," = ");
                        }
                        if (index > 1){
                            strcat(linebuffer,token);
                        }
                        index++;
                        token = strtok(NULL, " ");
                    }
                    strcat(linebuffer,";\n");
                }
                else if(strstr(line,"return") != NULL){
                    hasReturn = true;
                    char *token = strtok(line," ");
                    while (token != NULL){
                        char *trimmed_token = skip_leading_whitespace(token);
                        size_t len = strlen(trimmed_token);
                        if (len > 0 && trimmed_token[len - 1] == '\n') {
                            trimmed_token[len - 1] = '\0';
                        }
                        if(strcmp(trimmed_token,"return") == 0){
                            strcat(linebuffer,"return ");
                             token = strtok(NULL," ");
                             continue;
                        }
                        // if it is declared before, use it. Or else set it to 0.0
                        else if (isalpha(trimmed_token[0])){
                            if (existVariable(trimmed_token, parameter_names, param_count) ||
                                existVariable(trimmed_token, identifiers, identifierCount)||
                                existVariable(trimmed_token, function_identifier, 50)) {
                                strcat(linebuffer, trimmed_token); 
                            } else {
                                strcat(linebuffer, "0.0");
                            }
                        }
                        else{
                            strcat(linebuffer,trimmed_token);
                        }
                        strcat(linebuffer," ");
                        token = strtok(NULL," ");
                    }
                    strcat(linebuffer,";\n");
                }
            }
                fprintf(newFile,"double ");
            fprintf(newFile,"%s",linebuffer);
            // if there is no return statement in the function, return 0.0
            if(!hasReturn){
                fprintf(newFile,"return 0.0;");
            }
            fprintf(newFile,"\n}\n");
        }
        else if (is_exact_print(line)){
            //for the print statements that are outside of the functions

            char str[100];
            strcpy(str,line);
            char *token = strtok(line, " ,()");
            fprintf(newFile,"custom_print(");
            int count_comma = 0;
            char *ptr = str;

            //count the commas for using it identify how many parameters a function uses
            while (*ptr != '\0') {
                if (*ptr == ','){
                    count_comma++;
                }
                ptr++;  
            }
            int count_identifier = 0;
            int functionDepth = 0; // for making sure it is inside a function
            int expectedParamCount = 0;
            while (token != NULL) {
                if (strcmp(token,"print") != 0){    
                    if(existFunction(token,records,50)){ //check if the print statement include function call
                        // count how many parameters are there in the function, so that we can handle the close bracket when the function is used
                        expectedParamCount = getExpectedParamCount(token,50); 
                        fprintf(newFile,"%s",token);
                        fprintf(newFile,"(");
                        functionDepth++; 
                        count_identifier = 0;
                    }
                    else if  (isdigit(token[0])){ // if it is a digit, we know it should be written as it is and check for comma's existence
                    if (count_identifier > 0 && count_comma > 0){
                            count_comma--;
                            fprintf(newFile," , "); 
                        }
                        fprintf(newFile,"%s",token);
                        count_identifier++;
                    }
                    else if (!isalpha(token[0])){ //handling different signs such as +,-
                        fprintf(newFile,"%s",token);
                    }
                    else if (!existVariable(token,identifiers,50)){ //check for existence of variables or replace with 0.0
                        if (count_identifier > 0 && count_comma > 0){
                            count_comma--;
                            fprintf(newFile," , ");
                        }
                        fprintf(newFile,"%s","0.0");
                        count_identifier++;
                    }
                    else{ 
                        if (count_identifier > 0 && count_comma > 0){
                            count_comma--;
                            fprintf(newFile," , ");
                        }
                        fprintf(newFile,"%s",token);
                        count_identifier++;
                    }
                }
                // add the close bracket for a function when it is used
                if (functionDepth > 0 && count_identifier >= expectedParamCount) {
                    fprintf(newFile, ")");
                    functionDepth--;
                    count_identifier = 0;  
                    expectedParamCount = 0; 
                }
                token = strtok(NULL, " ,()"); 
            }
            //add remaining empty brackets
            while (functionDepth > 0) { 
                fprintf(newFile, ")");
                functionDepth--;
            }
            fprintf(newFile,");\n");
        }
        else if (strstr(line,"<-") != NULL){ //for all of the assignments, put it in directly
            char *token = strtok(line, " ");
            fprintf(newFile,"double ");
            int index = 0;
            while (token != NULL) {
                if (index == 0){
                    fprintf(newFile,"%s",token);
                    fprintf(newFile," = ");
                }
                if (index == 2){
                    fprintf(newFile,"%s",token);
                    fprintf(newFile,";\n");
                    strcpy(identifiers[identifierCount++],token);
                }
                index++;
                token = strtok(NULL, " ");
            }
        }
        else{ //for situations where the function call is start of the line
            for(int i = 0;i < count; i++){
                if(strstr(line,records[i])){
                    if (strstr(line," ") == NULL){
                        fprintf(newFile,"%s",line);
                        fprintf(newFile,";\n");
                        break;
                    }
                    char *token = strtok(line," ");
                    int index = 0;
                    while(token != NULL){
                            if (isalpha(token[0])){
                                if (!existVariable(token,identifiers,50 )  && index > 0 ){
                                    fprintf(newFile,"0.0");
                                }
                                else{
                                    fprintf(newFile,"%s",token);
                                }
                            }
                            else{
                                fprintf(newFile,"%s",token);
                            }
                        token = strtok(NULL," ");
                        index++;
                    }
                    fprintf(newFile,";\n");
                    break;
                }
            }

        }
    }
    
    //store the details of the files required to be removed
    char rmfilename[50];
    char rmcommand[50];

    fprintf(newFile,"}\n");  
    fclose(file);
    fclose(newFile);
    char command[300];
    char run[300];
    snprintf(command, sizeof(command), "cc -std=c11 %s -o newrunml", newName);
    system(command); 
    snprintf(run,sizeof(run),"./newrunml");
    system(run);
    
    //remove files that are not required
    sprintf(rmfilename, "ml-%d.c", process_id);
    sprintf(rmcommand, "rm %s", rmfilename);
    system("rm newrunml");
    system(rmcommand);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int options = 2;
    // to handle command-line arguments
    while(options < argc){
        arguments[arguments_count++] = atof(argv[options]);
        options++;
    }
    checkSyntax(argv[1]);
    translateCode(argv[1]);
    return EXIT_SUCCESS;
}
