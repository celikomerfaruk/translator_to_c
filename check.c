#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "checkh.h"
extern FILE *outfile;

//check identifier is valid
int checkIdentifier(char* id){
    if ((isalpha(id[0]) || id[0] == '_')){
        if(strlen(id) > 1){
        for (int i = 1; i < strlen(id); i++) {
            if(!(isalnum(id[i]) || id[i] == '_')){
                return -1;
            }}}
        return 1;

    }else{
        return -1;
    }  
}

void checkScalar(int tokencount,char tokens[300][80]){
    
    if(tokencount != 2 || checkIdentifier(tokens[1]) == -1 ){
        //syntax error print it and terminate the program
        iserror =1;
        return;
    }else{
        //filling fields of the variable
        strcpy(variables[varindex].id,tokens[1]);
        variables[varindex].type = scalar;
        variables[varindex].col = 1;
        variables[varindex].row = 1;
        varindex++;
        printDeclaration(tokens[1],"1","1");
    }
}

void checkOneDimArr(int tokencount,char tokens[300][80]){
    if(strcmp(tokens[2],"[")  == 0  && strcmp(tokens[4],"]")  == 0 && tokencount ==5 && checkIdentifier(tokens[1])!=-1){
        //filling fields of the variable
        strcpy(variables[varindex].id,tokens[1]);
        variables[varindex].type = onedarray;
        variables[varindex].row = atoi(tokens[3]);
        variables[varindex].col = 1;
        varindex++;
        printDeclaration(tokens[1],tokens[3],"1");

    }else{
        //syntax error print it and terminate the program
        iserror =1;
        return;
    }

}

void checkMatrix(int tokencount,char tokens[300][80]){
    if( strcmp(tokens[4],",")  == 0 && strcmp(tokens[2],"[")  == 0  && strcmp(tokens[6],"]")  == 0 && tokencount ==7 &&checkIdentifier(tokens[1])!=-1){
        //filling fields of the variable
        strcpy(variables[varindex].id,tokens[1]);
        variables[varindex].type = twodarray;
        variables[varindex].row = atoi(tokens[3]);
        variables[varindex].col = atoi(tokens[5]);
        varindex++;
        printDeclaration(tokens[1],tokens[3],tokens[5]);
    }else{
        //syntax error print it and terminate the program
        iserror =1;
        return;
    }
}

void printDeclaration(char id[] , char* row, char* col){    
    //declaring and defining variables in output file
    fprintf(outfile,"%s%s%s%s%s%s%s%s%s%s%s","matrix_t ",id,";\n",id,"="," define(",row,",",col,")",";\n");
    
}

int checkInt(char* num){
    int len = strlen(num);
    for (int i = 0; i < len; i++)
    {
        if(!isdigit(num[i])){
            return 0; 
        }
    }
    return 1;
}

//assigning specific index
int checkIndexAssignment(int tokencount,char tokens[300][80]){
    int index = isVarDeclared(tokens[0]);
    if(index != -1){
        if(variables[index].type == onedarray){
            
            if(checkInt(tokens[2]) ==1 && strcmp(tokens[3],"]") == 0 && strcmp(tokens[4],"=") == 0){
            
            token_t pTokens[300];
            //go to expression control
            if(parseExpr(5,tokens,pTokens) == -1){
                //expression error print it and terminate the program
                return -1;
            }
            if(postfixToInfix(pTokens) ==-1){
                return -1;
            }
            //semantic analyse
            if(top().tcol ==  1 && top().trow == 1  ){
                fprintf(outfile,"%s%s%d%s%s%s",tokens[0],".ind[",atoi(tokens[2])-1,"][0] = ",pop().strcontent,".ind[0][0] ;\n");
                return 1;
            }else{
                //variable and expression doesn't match error print it and terminate the program
                return -1;
            }

            }else{
                return -1;
            }
        }else if(variables[index].type == twodarray){

             if(checkInt(tokens[2]) ==1&& strcmp(tokens[3],",") == 0 && checkInt(tokens[4]) ==1 && strcmp(tokens[5],"]") == 0 && strcmp(tokens[6],"=") == 0){
                 
            token_t pTokens[300];
            //go to expression control
            if(parseExpr(7,tokens,pTokens) == -1){
                //expression error print it and terminate the program
                return -1;
            }
            if(postfixToInfix(pTokens) ==-1){
                return -1;
            }
            //semantic analyse
            if(top().tcol ==  1 && top().trow == 1  ){
                fprintf(outfile,"%s%s%d%s%d%s%s%s",tokens[0],".ind[",atoi(tokens[2])-1,"][",atoi(tokens[4])-1,"] = ",pop().strcontent,".ind[0][0];\n");
                return 1;
            }else{
                //variable and expression doesn't match error print it and terminate the program
                return -1;
            }
             }else{
                 return -1;
             }

        }else{
            // var should be either matrix or vector
            return -1;
        }
    }else{
        //var not declared
        return -1;
    }
}   

void checkAssignment(int tokencount,char tokens[300][80]){
    int index = isVarDeclared(tokens[0]);

    if (index != -1)
    {
    //check vector initialization
     if(strcmp(tokens[2] ,"{") == 0){
        if(strcmp(tokens[tokencount-1],"}")==0 &&(variables[index].type == onedarray || variables[index].type == twodarray ) && (tokencount-4) == (variables[index].col*variables[index].row) ){          
            int a = 0;
            
                for (int i = 0; i < variables[index].row; i++)
                {
                    for (int k = 0; k < variables[index].col; k++)
                    { 

                        fprintf(outfile,"%s%s%i%s%i%s%s%s",tokens[0],".ind[",i,"][",k,"] = ",tokens[a+3],";\n");
                        
                        variables[index].value[a] = atoi(tokens[a+3]);    
                        a++;

                    }  
                }     
        }else{
             //syntax error print it and terminate the program
            iserror =1;
            return;
        }
    }else{

        fprintf(outfile,"%s","assign(&");
        fprintf(outfile,"%s",tokens[0]); 
        fprintf(outfile,"%s",",");
        
        if(variables[index].type == scalar ||(variables[index].type == onedarray)|| (variables[index].type == twodarray)){
            token_t pTokens[300];
            //go to expression control
            if(parseExpr(2,tokens,pTokens) == -1){
                //expression error print it and terminate the program
                iserror =1;
                return;
            }
            if(postfixToInfix(pTokens) ==-1){
                iserror =1;
                return;
            }
            
            //semantic analyse
            if(top().tcol ==  variables[index].col && top().trow == variables[index].row  ){
                fprintf(outfile,"%s",pop().strcontent);
            }else{
                //variable and expression doesn't match error print it and terminate the program
                iserror =1;
                return;
            }
        }
       
         fprintf(outfile,"%s",");\n");
    }}else{
        //variable not declared print error and terminate the program
        iserror =1;
        return;
    }
    
}


int checkFloat(char* num){
    int len = strlen(num);
    for (int i = 0; i < len; i++)
    {
        if(!isdigit(num[i])){
            if((num[i] == '.')  && (i!= 0) && (i !=len-1)){
                return 1;
            }else{
            return 0;}
        }
    }
    return 1;
    
}
int isVarDeclared(char* id){
    for (int i = 0; i < varindex; i++){  
        if (strcmp(id,variables[i].id)==0){
            return i; }     
    }
    return -1;
}

int checkForLoop(char tokens[300][80]){
    if(strcmp(tokens[1],"(") == 0 ){
        if(strcmp(tokens[3] , ",") == 0 && strcmp(tokens[5] , "in") == 0 ){
            //nested loop
            int varind1 = isVarDeclared(tokens[2]);
            int varind2 = isVarDeclared(tokens[4]);
            if(varind1 != -1 && varind2 != -1){
                if(variables[varind1].type == scalar && variables[varind2].type == scalar){
                    char* expr1;
                    char* expr2;
                    char* expr3;
                    char* expr4;
                    char* expr5;
                    char* expr6;
                    token_t pTokens[300];
                    int tmp1 = parseExpr(6,tokens,pTokens);
                    if(tmp1 == -1){
                        //expression is not valid
                        return -1;
                    }
                    if(postfixToInfix(pTokens) ==-1){
                            return -1;
                        }
                    if(top().type == tscalar){
                        expr1 = pop().strcontent;
                    }else{
                        return -1; //expression should be scalar
                    }
                    if(strcmp(tokens[tmp1],":")== 0){
                        
                        token_t pTokens[300];
                        int tmp2 = parseExpr(tmp1+1,tokens,pTokens);
                        if(tmp2 == -1){
                            //expression is not valid
                            return -1;
                        }
                        if(postfixToInfix(pTokens) ==-1){
                            return -1;
                        }
                        if(top().type == tscalar){
                            expr2 = pop().strcontent;
                            
                        }else{
                            return -1; //expression should be scalar
                        }
                        if(strcmp(tokens[tmp2],":")== 0){
                            token_t pTokens[300];
                            int tmp3 = parseExpr(tmp2+1,tokens,pTokens);
                            if(tmp3 == -1){
                                //expression is not valid
                                return -1;
                            }
                            if(postfixToInfix(pTokens) ==-1){
                            return -1;
                        }
                            if(top().type == tscalar){
                                expr3 = pop().strcontent;
                            }else{
                                return -1; //type error
                            }
                            if(strcmp(tokens[tmp3],",")== 0){
                                token_t pTokens[300];
                                int tmp4 = parseExpr(tmp3+1,tokens,pTokens);
                                if(tmp4 == -1){
                                    //expression is not valid
                                    return -1;
                                }
                                if(postfixToInfix(pTokens) ==-1){
                            return -1;
                        }
                                if(top().type == tscalar){
                                    expr4 = pop().strcontent;
                                    
                                }else{
                                    return -1; //type error
                                }
                                if(strcmp(tokens[tmp4],":")== 0){
                                    token_t pTokens[300];
                                    int tmp5 = parseExpr(tmp4+1,tokens,pTokens);
                                    if(tmp5 == -1){
                                        //expression is not valid
                                        return -1;
                                    }
                                    if(postfixToInfix(pTokens) ==-1){
                            return -1;
                        }
                                    if(top().type == tscalar){
                                        expr5 = pop().strcontent;
                                    }else{
                                        return -1; //type error
                                    }
                                    if(strcmp(tokens[tmp2],":")== 0){
                                        token_t pTokens[300];
                                        int tmp6 = parseExpr(tmp5+1,tokens,pTokens);
                                        if(tmp6 == -1){
                                            //expression is not valid
                                            return -1;
                                        }
                                        if(postfixToInfix(pTokens) ==-1){
                                            return -1;
                                        }
                                        if(top().type == tscalar){
                                            expr6 = pop().strcontent;
                                        }else{
                                            return -1; //type error
                                        }
                                        if(strcmp(tokens[tmp6],")")==0 && strcmp(tokens[tmp6+1],"{")== 0){
                                            
                                            fprintf(outfile,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s","for(",variables[varind1].id,".ind[0][0] = ",expr1,".ind[0][0] ; ",variables[varind1].id,".ind[0][0] <= ",expr2,".ind[0][0] ; ",variables[varind1].id ,".ind[0][0] +=",expr3,".ind[0][0]){\n","for(" ,variables[varind2].id,".ind[0][0] = " ,expr4,".ind[0][0]; " ,variables[varind2].id,".ind[0][0] <= ",expr5 ,".ind[0][0];",variables[varind2].id,".ind[0][0]+= "  ,expr6,".ind[0][0]){\n");
                                    
                                            loopcount +=2;
                                            return 1;
                            }}}}}}
            
                //syntax error
                return -1;     
                }else{
                    //var should be scalar
                    return -1;
                }
            }else{
                //var didnt declared
                return -1;
            }
        }else if(strcmp(tokens[3] , "in") == 0 ){
                
            int varind = isVarDeclared(tokens[2]);
            if(varind != -1 ){
                if (variables[varind].type == scalar)
                {
                    char* expr1;
                    char* expr2;
                    char* expr3;
                    token_t pTokens[300];
                    int tmp1 = parseExpr(4,tokens,pTokens);
                    if(tmp1 == -1){
                        //expression is not valid
                        return -1;
                    }
                    if(postfixToInfix(pTokens) ==-1){
                        return -1;
                    }
                    if(top().type == tscalar){
                        expr1 = pop().strcontent;
                        
                    }else{
                        return -1; //type error
                    }
                      
                    if(strcmp(tokens[tmp1],":")== 0){
                        token_t pTokens[300];
                        int tmp2 = parseExpr(tmp1+1,tokens,pTokens);
                        if(tmp2 == -1){
                            //expression is not valid
                            return -1;
                        }
                        if(postfixToInfix(pTokens) ==-1){
                                            return -1;
                                        }
                        if(top().type == tscalar){
                            expr2 = pop().strcontent;
                        }else{
                            return -1; //type error
                        }
                        if(strcmp(tokens[tmp2],":")== 0){
                            token_t pTokens[300];
                            int tmp3 = parseExpr(tmp2+1,tokens,pTokens);
                            if(tmp3 == -1){
                                //expression is not valid
                                return -1;
                            }
                            if(postfixToInfix(pTokens) ==-1){
                                            return -1;
                                        }
                            if(top().type == tscalar){
                                expr3 = pop().strcontent;
                                
                            }else{
                                return -1; //type error
                            }
                            if(strcmp(tokens[tmp3],")")==0 && strcmp(tokens[tmp3+1],"{")== 0){
                            
                                fprintf(outfile,"%s%s%s%s%s%s%s%s%s%s%s%s%s","for(",variables[varind].id,".ind[0][0] = ",expr1,".ind[0][0] ; ",variables[varind].id,".ind[0][0] <= " ,expr2,".ind[0][0] ; "  ,variables[varind].id,".ind[0][0] +=",expr3 ,".ind[0][0]){\n"  );
                                loopcount++;
                                return 1;
                            }}}
                    //syntax error
                    return -1;
                 }
                else{
                    //var should be scalar
                    return -1;
                }
                }else{
                    //var didnt declared
                    return -1;
                }
                
        }else{
            return -1; // error
        }
    }else{
        return -1; // error
    }
}

//check syntax of print statement and generate output
int checkPrint(char tokens[300][80]){
    fprintf(outfile,"printmtrx(");
    if(tokenCount<4){
        return -1;
    }
    int varindex = isVarDeclared(tokens[2]);
    if(strcmp(tokens[1],"(") == 0){
        if(varindex != -1){
            if(tokenCount==4){
                if(strcmp(tokens[3],")")== 0 ){
                    fprintf(outfile,"%s",tokens[2]);
                }else{
                    return -1;
                }
            }else{
                if(variables[varindex].type == onedarray){
                    if(tokenCount ==7){
                        if(strcmp(tokens[3],"[")== 0 && checkInt(tokens[4]) ==1 &&strcmp(tokens[5],"]")== 0 && strcmp(tokens[6],")")== 0){
                            fprintf(outfile,"%s%s%s%s%s","getInd(",tokens[2],",cnvrt(",tokens[4],"),cnvrt(1))");
                          
                        }else{
                            return-1;
                        }
                    }else{
                        return -1;
                    }    
                } else if (variables[varindex].type == twodarray){
                    if(tokenCount ==9){
                        if(strcmp(tokens[3],"[")== 0 && checkInt(tokens[4]) ==1 &&strcmp(tokens[5],",")== 0  && checkInt(tokens[6]) ==1 && strcmp(tokens[7],"]")== 0 && strcmp(tokens[8],")")== 0){
                        
                            fprintf(outfile,"%s%s%s%s%s%s%s","getInd(",tokens[2],",cnvrt(",tokens[4],"),cnvrt(",tokens[6],"))");

                         }else{
                            return -1;
                        }
                    }else{
                        return -1;
                    }
                } else{
                    return -1;
                }
            }
        }else{
            return -1 ;// error
        }
    }else{
        return -1; //error
    }
        

        fprintf(outfile,"%s",");\n");
        return 1;
    }
    
int checkprintsep(char tokens[300][80]){
    if(tokenCount == 3){
        if(strcmp(tokens[1] ,"(" )== 0 && strcmp(tokens[2] ,")" )== 0){
            for (int i = 0; i < 3; i++)
        {
        
               fprintf(outfile,"%s",tokens[i]);
        }
      
        fprintf(outfile,"%s",";\n");
        return 1;
        }
    }
    return -1;
}
    



