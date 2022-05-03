#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mainh.h"
#define LINELENGTH 80
int iserror = 0;
int errorline = 1;
int main (int argc,char *argv[]) {
    //checking open and closed braces with this var
    loopcount =0 ;
    char line[LINELENGTH];
   
   if (argc != 2) {
    printf("Give filename as command line argument\n") ;
    return(0);
    }
    infile = fopen(argv[1], "r");
    outfile = fopen("file.c","w");
    if(infile == NULL) {
    printf("Cannot open %s\n",argv[1]);
    return(1);
    }
    fprintf(outfile,"%s",headofoutput);
    while( fgets(line,LINELENGTH,infile) != NULL  ) {
        //executing all syntactic checks for current line
        parseLines(line);
        if(iserror!=0){
            printf("%s%d%s\n", "Error (Line ",errorline,")");    
            fclose(infile);
            fclose(outfile);
            return -1;
        }
        errorline++;
    }
    fprintf(outfile,"%s","return 0;}");
    fprintf(outfile,"%s",endOfOutput);

    if(loopcount >0){
        printf("%s %d %s","Error (Line",errorline-1,")");
    }

    
    fclose(infile);
    fclose(outfile);
    return(0);
}

void parseLines(char *text)
{
    char tokens[300][80];
    seperateTokens(text);
    char *token ;
    //splitting tokens with those delimeters
    char delim[] = " \t\r\n\f\v";
    token = strtok(text, delim);
    
    int i = 0;
   while(token != NULL)
	{
		if(strcmp(token,"#")==0){
            return;
        }
        strcpy(tokens[i],token);
        
        i++;
		token = strtok(NULL, delim);    
	}
    tokenCount = i; 
    
    if(tokenCount == 0){
        return;
    }
    //classifying lines
    //appropriate checks are made for current line

    //Scalar Declaration
   if(strcmp(tokens[0],"scalar")==0){
        checkScalar(i,tokens);
    //Vector Declaration
    }else if(strcmp(tokens[0],"vector")==0){
        checkOneDimArr(i,tokens);
    //Matrix Declaration
    }else if(strcmp(tokens[0],"matrix")==0){
       
        checkMatrix(i,tokens);
    //Assignment Statement
    }  else if(strcmp(tokens[1],"=")==0 && tokenCount>1){
        checkAssignment(i,tokens);
    //Printsep Statement
    }else if(strcmp(tokens[0],"printsep")==0){
        if(checkprintsep(tokens) == -1){
            iserror =1;
            return;
        }
    //For Statement
    }else if(strcmp(tokens[0],"for")==0){
        if(checkForLoop(tokens) == -1){
            iserror =1;
            return;
        }
    //Print Statement 
    }else if(strcmp(tokens[0],"print")==0){
        if(checkPrint(tokens) == -1){
            iserror =1;
            return;
        }
    //Closing Bracket Statement
    }else if(strcmp(tokens[0],"}")==0){
        if(loopcount ==0){
            //brace  error
            iserror =1;
            return;
        }
        for (int i = 0; i< loopcount; i++){
            //closing braces
            fprintf(outfile,"%s",tokens[0]);
            fprintf(outfile,"%s","\n");
            
        }
        loopcount = 0;
    }else if(strcmp(tokens[1],"[")==0 ){
        if(checkIndexAssignment(i,tokens) == -1){
            iserror =1;
            return;
        }
    }else{
        iserror =1;
        return;
    }
}

/*seperate tokens by space*/
void seperateTokens(char *text){
    
    char tmpline[strlen(text)+24];
    
    int location = 0;
    for (int i = 0; i < strlen(text); i++)
    {   
        
        if(text[i]=='\n' ){
            tmpline[location] = '\0';
            break;
        }
        //putting spaces before and after of those special characters
        if(i == strlen(text)-1 && text[i] != '\n'){
            if(text[i]=='[' || text[i]==']'|| text[i]=='{'|| text[i]=='}'|| text[i]==','|| text[i]=='='|| text[i]==':'|| text[i]=='('|| text[i]==')'|| text[i]=='<'|| text[i]=='>'|| text[i]=='*'|| text[i]=='-'|| text[i]=='+'||text[i] =='#'){
                tmpline[location] = ' ';
                location++;
            }
            tmpline[location] =   text[i];
            tmpline[location+1] = '\0' ;
            break;
        }
        if (text[i]=='[' || text[i]==']'|| text[i]=='{'|| text[i]=='}'|| text[i]==','|| text[i]=='='|| text[i]==':'|| text[i]=='('|| text[i]==')'|| text[i]=='<'|| text[i]=='>'|| text[i]=='*'|| text[i]=='-'|| text[i]=='+'|| text[i] =='#')
        {
            tmpline[location] = ' ';
            location++;
            tmpline[location] = text[i];
            location++;
            tmpline[location] = ' ';
            location++;
     }else {
        tmpline[location] = text[i];
            location++;
    }}
    strcpy(text,tmpline);
}