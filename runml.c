#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

// #define num_identifiers 50
// char fileName;
int identifierCount = 0;
// int functionDefined = 0;
// char functions[];,
// int functionCounts =0;
char identifiers[100][100];

char records[50][50];
char records_count[50];
int count = 0;  

typedef struct {
    char name[50];
    int paramCount;  // Number of parameters
} FunctionRecord;

// FunctionRecord funcRecord[50];  // Example records array
// int getExpectedParamCount(const char *functionName, FunctionRecord *funcRecord, int recordCount) {
//     for (int i = 0; i < recordCount; i++) {
//         if (strcmp(funcRecord[i].name, functionName) == 0) {
//             return funcRecord[i].paramCount;  // Return the expected parameter count
//         }
//     }
//     return -1;  // Not found
// }


int getExpectedParamCount(const char *functionName, int recordCount){
    for (int i = 0;i< recordCount;i++){
        if (strcmp(records[i],functionName) == 0){
            return records_count[i];
        }
    }
    return -1;
}






bool existVariable(const char *varName, char identifier[100][100], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(identifier[i], varName) == 0) {
            return true;
        }
    }
    return false;
}



bool existFunction(const char *varName, char record[50][50], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(record[i], varName) == 0) {
            return true;
        }
    }
    return false;
}

char *skip_leading_whitespace(char *str) {
    while (isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

// for identifiers
int isValidIdentifier(char *token) {
    // char *token2;
    int length = strlen(token);
    // printf("%d!!!",length);
    // if (length>50){
    //     return 0;
    // }
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

int isValidNumber(char *token){
    int length = strlen(token);
    // char *token2;
    int dotCount = 0;

    for (int i = 0; i < length; i++) {

        if (token[i] == '.') {
            dotCount++;
            if (dotCount > 1){
                return 0;
            }
        } else if (!isdigit(token[i])) {
            return 0;
        }
    }
    
    return 1;

}



void checkLine(char *line)
{
    line[strlen(line) - 1] = '\0'; // Remove newline
    char *token, *token2,*token3,*token4;
    char *saveptr1, *saveptr2,*saveptr3,*saveptr4;
    char *varName;
    int index=0;
    // char value;

     if(strstr(line,"<-")!=NULL){
        
        token3=strtok_r(line,"<-",&saveptr3);

        if (line[0] !='\t'){

             char *Newtoken3= skip_leading_whitespace(token3);//abc <-10, first token:abc_

        while(token3!=NULL && index==0){//abc <-10
            if(strstr(Newtoken3," ") != NULL ){
                token4=strtok_r(Newtoken3," ",&saveptr4);//fisrt token:abc
                while(token4!=NULL){
                    varName=token4;
                    token4=strtok_r(NULL," ",&saveptr4);
                }
            }
            else{//abc
                varName=Newtoken3;
            }
            token3=strtok_r(NULL,"<-",&saveptr3);
            index++;
        }
          strcpy(identifiers[identifierCount++],varName);
        //   identifiers[identifierCount][strlen(varName)]='\0';
        //   identifierCount++;
        if(identifierCount>50){
            fprintf(stderr, "! Error: the number of identifiers exceed 50\n");
            exit(EXIT_FAILURE);
        }

        }
     }
        
    
    
    
    // Tokenize the line using spaces as delimiters
    token = strtok_r(line, " ", &saveptr1);
    while (token != NULL)
    {
        char *newToken = skip_leading_whitespace(token);

        // Skip tokens like "print", "return", or lines with arithmetic operators
        if (strcmp(newToken, "print") == 0 || strcmp(newToken, "return") == 0 || strpbrk(line, "+-*/") != NULL)
        {
            token = strtok_r(NULL, " ", &saveptr1);
            continue;
        }

        // Tokenize newToken based on the delimiters "(,)"
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

        // Handle remaining newToken
        if (isdigit(newToken[0]))
        {
            if (!isValidNumber(newToken))
            {
                fprintf(stderr, "! Error: Invalid number '%s'\n", newToken);
                exit(EXIT_FAILURE);
            }
        }

        // printf("%s,%lu,||", newToken, strlen(newToken));
        if (isalpha(newToken[0]))
        {
            // printf("aghh: % .s//", newToken);
            if (!isValidIdentifier(newToken))
            {
                fprintf(stderr, "! Error: Invalid identifier '%s'\n", newToken);
                exit(EXIT_FAILURE);
            }
        }

        

        // Get next token from the original line
        token = strtok_r(NULL, " ", &saveptr1);
    }

    if(strstr(line,"<-")!=NULL){
        
        token3=strtok_r(line,"<-",&saveptr3);

        if (line[0] !='\t'){

             char *Newtoken3= skip_leading_whitespace(token3);//abc <-10, first token:abc_

        while(token3!=NULL && index==0){//abc <-10
            if(strstr(Newtoken3," ") != NULL ){
                token4=strtok_r(Newtoken3," ",&saveptr4);//fisrt token:abc
                while(token4!=NULL){
                    varName=token4;
                    token4=strtok_r(NULL," ",&saveptr4);
                }
            }
            else{//abc
                varName=Newtoken3;
            }
            token3=strtok_r(NULL,"<-",&saveptr3);
            index++;
        }
          strcpy(identifiers[identifierCount++],varName);
        //   identifiers[identifierCount][strlen(varName)]='\0';
        //   identifierCount++;

        }
        
       
        if(identifierCount>50){
            fprintf(stderr, "! Error: the number of identifiers exceed 50\n");
            exit(EXIT_FAILURE);
        }
        }

    
    
}

bool record_in_line(const char *line) {
    for (int i = 0; i < count; i++) {
        if (strstr(line, records[i]) != NULL) {
            return true;  
        }
    }
    return false;
}

bool is_exact_print(const char *line) {
    const char *pos = strstr(line, "print");

    // If "print" is found, ensure it's not part of a larger word
    while (pos != NULL) {
        // Check if "print" is followed by a non-alphanumeric character or end of string
        if ((pos[5] == '\0' || !isalnum((unsigned char)pos[5])) &&  // Check what comes after "print"
            (pos == line || !isalnum((unsigned char)pos[-1]))) {    // Check what comes before "print"
            return true;  // Exact "print" found
        }

        // Continue searching in the rest of the line
        pos = strstr(pos + 1, "print");
    }
    return false;  // "print" as a standalone word not found
}


void checkSyntax(const char filename[]) {
    FILE   *file = fopen(filename, "r");

//  CHECK IF ANYTHING WENT WRONG
    if(file == NULL) {
        fprintf(stderr, "Cannot open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[1000];

    while( fgets(line, sizeof line, file) != NULL ) {  
        // for comment or empty
        // line[strlen(line)]='\0';
        // printf("%s",line);
        if (strstr(line,"#")!= NULL || strlen(line)==0){
            // *comment = '\0';
            continue;
        }
        // if(strstr(line,"<-")!= NULL){
            //print 3.5
            // char *token = strtok(line, " ");
            //token = print
            checkLine(line);
            
            // identifierCount++;
        // }
        // if(strstr(line,"print")!= NULL||strstr(line,"return")!= NULL||strstr(line,"function")!= NULL){
            // char *token = strtok(line, " ");
        // else{
            // printf("%s~~",line);
            // checkLine(line);
        // }

        

        }
    fclose(file);
}






bool search_record(char *token) {
    for (int i = 0; i < count; i++) {
        if (strcmp(records[i], token) == 0) {
            return true; 
        }
    }
return false;
}
        

void translateCode(const char *filename){
    FILE *newFile;



    char newName[13]; // 3 (ml-) + 7 (pid) + 2 (.c) + 1 (null character) = 13
    sprintf(newName,"ml-%d.c",getpid());
    newFile = fopen(newName,"w");
fprintf(newFile,"#include <stdlib.h>\n");

fprintf(newFile,"#include <stdio.h>\n");
fprintf(newFile,"#include <math.h>\n");

fprintf(newFile,"\n");
    fprintf(newFile, "void custom_print(double value) {\n");
    fprintf(newFile, "    double int_part, frac_part;\n");
    fprintf(newFile, "    frac_part = modf(value, &int_part);\n");
    fprintf(newFile, "    if (frac_part == 0.0) {\n");
    fprintf(newFile, "        printf(\"%%.0f\\n\", value);\n");
    fprintf(newFile, "    } else {\n");
    fprintf(newFile, "        printf(\"%%.6f\\n\", value);\n");
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
        if(line[0]=='#'){
            continue;
        }
        if (strstr(line,"function") != NULL){

            function_count++;

        }

    }

    rewind(file);

    bool main_added = false;
    int function_number = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if(line[0]=='#'){
            continue;
        }
   

        if (function_number == function_count && !main_added){

            main_added = true;
            fprintf(newFile,"\n int main() { \n");

        }

        line[strcspn(line, "\n")] = 0; 
        if (line[0]=='#' || line[0]=='\t') {
            continue;
        }

        else if (strstr(line,"function") != NULL){
            char parameter_names[100][100]; // Store parameter names
            int param_count = 0;

            char linebuffer[10000];

            char function_identifier[100][100];
            int local_count = 0;

             char *token3;
                char *token4;

                char *saveptr3;
                char *saveptr4;

                char *varName;
                int index = 0;



              snprintf(linebuffer,sizeof(linebuffer)," ");         
            bool hasReturn = false;
            // bool hasPrint = false;
            function_number++;

            char line_copy[1000];

            strcpy(line_copy, line);




             char *token = strtok(line_copy, " ");
             int function_index = 0;

            while (token != NULL) {
 
                if (function_index == 1){

                    strcpy(records[count], token);
                    // strcpy(records_count[count],token);
                    // records_count[count] = 
                    // count++;
                    strcat(linebuffer,token);
                    strcat(linebuffer," (");
                }

                if (function_index == 2){

                    strcat(linebuffer,"double ");
                    strcat(linebuffer,token);

                    //add to function itendifier

                    records_count[count]++;


                    strcpy(function_identifier[local_count++],token);
                    strcpy(parameter_names[param_count++], token);
                }

                if (function_index > 2){

                    strcat(linebuffer,",double ");
                    strcat(linebuffer,token);

                    records_count[count]++;



                    strcpy(function_identifier[local_count++],token);
                    strcpy(parameter_names[param_count++], token);
                }

                function_index++;
                token = strtok(NULL, " "); 
            }

            count++;

        
            strcat(linebuffer,"){\n");

            long place = ftell(file);
             while (fgets(line, sizeof(line), file) != NULL) {

                if(line[0]=='#'){
                    continue;
                }
                if (line[0]!='\t'){
                    fseek(file,place,SEEK_SET);
                    break;
                }
                if (strstr(line,"print") != NULL){
                    
                   

                    char line_copy2[1000];
                    strcpy(line_copy2, line);
                     char *token = strtok(line_copy2, " ");
                    // strcat(linebuffer,"printf(\"%lf\",");
                    strcat(linebuffer,"custom_print(");
                    // int paren_coun /t = 0;

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
                         //abc 

                         else if (isalpha(trimmed_token[0])){

                            //  if (!existVariable(trimmed_token, function_identifier,100) || !existVariable(trimmed_token, identifiers,100))  {
                            //     strcat(linebuffer,"0.0");
                            // }
                            // else{
                            //     strcat(linebuffer,trimmed_token);
                                
                            // }
                            //aghh3:
                            if(existVariable(trimmed_token, function_identifier,100) || existVariable(trimmed_token, identifiers,100)||existVariable(trimmed_token,parameter_names,100))  {
                                strcat(linebuffer,trimmed_token);
                            }
                            else{
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
                else if (strstr(line,"<-") != NULL){

                    char *token = strtok(line, " ");

                    token3=strtok_r(line,"<-",&saveptr3);
                    char *Newtoken3= skip_leading_whitespace(token3);//abc <-10, first token:abc_

                    while(token3!=NULL && index==0){//abc <-10
                    if(strstr(Newtoken3," ") != NULL){
                        token4=strtok_r(Newtoken3," ",&saveptr4);//fisrt token:abc
                        while(token4!=NULL){
                            varName=token4;
                            token4=strtok_r(NULL," ",&saveptr4);
                            strcpy(function_identifier[local_count++],token4);
                        }
                    }
                    else{//abc
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

                        else if (isalpha(trimmed_token[0])){

                            //  if (!existVariable(trimmed_token, function_identifier, 50)) {
                            //     strcat(linebuffer,"0.0");
                            // }
                            // else{
                            //     strcat(linebuffer,trimmed_token);
                            // }
                            //aghh3:
                            if (existVariable(trimmed_token, parameter_names, param_count) ||
                                existVariable(trimmed_token, identifiers, identifierCount)||
                                existVariable(trimmed_token, function_identifier, 50)) {
                                strcat(linebuffer, trimmed_token); // Use 0.0 if variable is not found
                            } else {
                                strcat(linebuffer, "0.0"); // Use the variable
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
            if(!hasReturn){
                fprintf(newFile,"return 0.0;");
            }
            fprintf(newFile,"\n}\n");



        }

        else if (is_exact_print(line)){

              char str[100];

             strcpy(str,line);

             char *token = strtok(line, " ,()");
            fprintf(newFile,"custom_print(");
            

            int count_comma = 0;
    
            char *ptr = str;
            while (*ptr != '\0') {
                // printf("%c", *ptr);  // Print each character
                if (*ptr == ','){
                        count_comma++;
                }
                ptr++;  

            }
            // int mycount = 0;
            int count_identifier = 0;
            int functionDepth = 0;
            int expectedParamCount = 0;
            while (token != NULL) {


                if (strcmp(token,"print") != 0){    
                    if(existFunction(token,records,50)){
                        expectedParamCount = getExpectedParamCount(token,50);


                        printf("%d\n",expectedParamCount);
                                  fprintf(newFile,"%s",token);
                            fprintf(newFile,"(");
                            // mycount++;
                            functionDepth++;
                            count_identifier = 0;
                    }
                    

                    else if  (isdigit(token[0])){

                        if (count_identifier > 0 && count_comma > 0){
                            count_comma--;
                            fprintf(newFile," , ");
                            
                        }

                        fprintf(newFile,"%s",token);
                        count_identifier++;
                      
                    }

                    else if (!isalpha(token[0])){
                         fprintf(newFile,"%s",token);
                    }

                    else if (!existVariable(token,identifiers,50)){

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
                 if (functionDepth > 0 && count_identifier >= expectedParamCount) {
                    printf("%s",token);
                    fprintf(newFile, ")");
                    functionDepth--;
                    count_identifier = 0;  // Reset for the next function
                    expectedParamCount = 0; // Reset expected parameter count
                }
                token = strtok(NULL, " ,()"); 
                
            }
            // modifie this
            // for(int i = 0;i<mycount;i++){
                // fprintf(newFile,")");
            // }

            // aghh3:
            while (functionDepth > 0) {
                fprintf(newFile, ")");
                functionDepth--;
            }
            fprintf(newFile,");\n");
        }
        else if (strstr(line,"<-") != NULL){

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
        else{

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
fprintf(newFile,"}\n");  
fclose(file);
fclose(newFile);
char command[300];
char run[300];
snprintf(command, sizeof(command), "cc -std=c11 %s -o newrunml", newName);
printf("%s\n",newName);
// printf("%s\n",command);
system(command);  // system() only takes one argument (a string)

snprintf(run,sizeof(run),"./newrunml");
system(run);
  
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    checkSyntax(argv[1]);
    // createFile(argv[1]);
    translateCode(argv[1]);
    return EXIT_SUCCESS;
}
