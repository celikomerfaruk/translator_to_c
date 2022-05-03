
#include <string.h>
#include <stdio.h>
#include "expr_parserh.h"

//checks whether it complies with the given bnf grammer
//returns -1 if it doesn't
//produces postfix notation of expression

int parseExpr(int curToken,char tokens[300][80],token_t postfixTokens[300]){
    int ind1tok = parseTerm(curToken,tokens,postfixTokens);
    if ( ind1tok == -1){
        return -1;
    }
    int ind2tok =parseMoreterm(ind1tok,tokens,postfixTokens);
    if (ind2tok == -1)
    {
        return -1;
    }
    return ind2tok;
}

int parseMoreterm(int curToken,char tokens[300][80],token_t postfixTokens[300]){
   if(curToken<tokenCount){
       if(strcmp(tokens[curToken],"+")==0){
               int ind1tok = parseTerm(curToken+1,tokens,postfixTokens);
               if (ind1tok== -1)
               {
                   return -1;
               }
               int ind2tok = parseMoreterm(ind1tok,tokens,postfixTokens);
               if (ind2tok== -1)
               {
                   return -1;
               }
              
               strcpy(postfixTokens[postfixind].strcontent , "+");
               postfixTokens[postfixind].type = toperator;
               postfixind++;
               
               return ind2tok;
               
           
       }else if(strcmp(tokens[curToken], "-") ==0 ){
          
               int ind1tok = parseTerm(curToken+1,tokens,postfixTokens);
               if (ind1tok == -1)
               {
                   return -1;
               }
               int ind2tok = parseMoreterm(ind1tok,tokens,postfixTokens);
               if (ind2tok== -1)
               {
                   return -1;
               }
               
               strcpy(postfixTokens[postfixind].strcontent , "-");
               
               postfixTokens[postfixind].type = toperator;
               postfixind++;
               
               
               return ind2tok;
           
       }
   }
       return curToken;
}

int parseTerm(int curToken,char tokens[300][80],token_t postfixTokens[300] ){
   int ind1tok = parseFactor(curToken,tokens,postfixTokens);
    if (ind1tok == -1){
        return -1;
    }
    int ind2tok = parseMorefactor(ind1tok,tokens,postfixTokens);
    if ( ind2tok== -1)
    {
        return -1;
    }
    return ind2tok;
}
int parseMorefactor(int curToken,char tokens[300][80],token_t postfixTokens[300] ){

  if (curToken< tokenCount){
        if(strcmp(tokens[curToken], "*")==0){
            int ind1tok = parseFactor(curToken +1,tokens,postfixTokens);
            if( ind1tok== -1){
                return -1;
            }
            int ind2tok = parseMorefactor(ind1tok,tokens,postfixTokens);
            if(ind2tok == -1){
                return -1;
            }
            strcpy(postfixTokens[postfixind].strcontent , "*");
            postfixTokens[postfixind].type = toperator;
            postfixind++;
            
            
            return ind2tok; }
        }     
        return curToken;
    }

int parseFactor(int curToken,char tokens[300][80],token_t postfixTokens[300] ){
    if (!(curToken < tokenCount))
    {
        return -1;
    }
    int varindex = isVarDeclared(tokens[curToken]);
    if(strcmp( tokens[curToken], "(") == 0 ){
        int ind1tok =parseExpr(curToken+1,tokens,postfixTokens); 
        if ( ind1tok== -1 )
            {return -1;}
        if (!(ind1tok < tokenCount))
            {return -1;}
        if(strcmp(tokens[ind1tok], ")")!=0 ){
            return -1;
        }
        return ind1tok+1;
        return -1;
    }
     
    else if(varindex!= -1){
        if(variables[varindex].type == scalar){
            sprintf(postfixTokens[postfixind].strcontent,"%s",variables[varindex].id);
            postfixTokens[postfixind].type = tscalar;
            postfixTokens[postfixind].trow = 1;
            postfixTokens[postfixind].tcol =1;
            postfixind++;
            
            return curToken+1;
        }
        else if(variables[varindex].type == onedarray){
            if(tokenCount>curToken+1){
                if(strcmp(tokens[curToken+1] ,"[") == 0){
                        
                        int ind1tok = parseExpr(curToken+2,tokens,postfixTokens);
                        if (ind1tok == -1)
                           {
                               return -1;
                           }
                        if(ind1tok<tokenCount){
                       if (strcmp(tokens[ind1tok] ,"]") == 0)
                       {        
                                strcpy(postfixTokens[postfixind].strcontent , variables[varindex].id);
                                postfixTokens[postfixind].type = tonedarray;
                                postfixTokens[postfixind].trow =variables[varindex].row;
                                postfixTokens[postfixind].tcol =variables[varindex].col;
                                postfixind++;
                                
                                strcpy(postfixTokens[postfixind].strcontent , "[");
                                postfixTokens[postfixind].type = toperator;
                                postfixind++;
                                
                              return ind1tok+1;
                       }else{
                           return -1;
                       }
                        }
                        else{
                            return -1;
                        }
                }
            }  
                 
                 strcpy(postfixTokens[postfixind].strcontent , variables[varindex].id);
                 postfixTokens[postfixind].type =  tonedarray;
                 postfixTokens[postfixind].trow =variables[varindex].row;
                 postfixTokens[postfixind].tcol =variables[varindex].col;
                 postfixind++;
                
                return curToken+1;
        }else {
            if(tokenCount>curToken+1){
                if(strcmp(tokens[curToken+1] ,"[") == 0){ 
                                   
                            int token1ind = parseExpr(curToken+2,tokens,postfixTokens);
                            if( token1ind!= -1 &&token1ind < tokenCount ) {
                                if(strcmp(tokens[token1ind],",")==0){
                                    int token2ind = parseExpr(token1ind+1,tokens,postfixTokens);
                                    if( token2ind!= -1 && token2ind < tokenCount ){
                                        if (strcmp(tokens[token2ind],"]")== 0){
                                            
                                            strcpy(postfixTokens[postfixind].strcontent , variables[varindex].id);
                                            postfixTokens[postfixind].type =  ttwodarray;
                                            postfixTokens[postfixind].trow =variables[varindex].row;
                                            postfixTokens[postfixind].tcol =variables[varindex].col;
                                            postfixind++;
                                           
                                            strcpy(postfixTokens[postfixind].strcontent , "[");
                                            postfixTokens[postfixind].type = toperator;
                                            postfixind++;
                                            
                                            return token2ind+1;
                                        }
                                    }}}
                                    
                                return -1;
                }else{
             
                    strcpy(postfixTokens[postfixind].strcontent , variables[varindex].id);
                    postfixTokens[postfixind].type =  ttwodarray;
                    postfixTokens[postfixind].trow =variables[varindex].row;
                    postfixTokens[postfixind].tcol =variables[varindex].col;
                    postfixind++;
                   
                    return curToken+1;
                }
            }else{
               
                strcpy(postfixTokens[postfixind].strcontent , variables[varindex].id);
                postfixTokens[postfixind].type =  ttwodarray;
                postfixTokens[postfixind].trow =variables[varindex].row;
                postfixTokens[postfixind].tcol =variables[varindex].col;
                postfixind++;
                return curToken+1;
            }
            
        }
    }else if(strcmp(tokens[curToken],"sqrt") == 0){

        if(curToken+1 < tokenCount){
        if(strcmp(tokens[curToken+1], "(") == 0){
            int tokind = parseExpr(curToken+2,tokens,postfixTokens);
            if(tokind == -1 ){
                return -1;
            }
            if(tokind<tokenCount){
            if(strcmp(tokens[tokind], ")") == 0){
                strcpy(postfixTokens[postfixind].strcontent , "sqrt");
                postfixTokens[postfixind].type = toperator;
                postfixind++;    
                return tokind+1;
            }else{
                return -1;
            }
            }else{
                return -1;
            }
        }else{
            return -1;
        }
        }
        else{
            return -1;
        }
    }else if(strcmp(tokens[curToken],"choose") == 0){
        if(curToken+1<tokenCount){
            if (strcmp(tokens[curToken+1],"(") == 0)
            {
                     

                int tok1ind = parseExpr(curToken+2,tokens,postfixTokens);
                if(tok1ind != -1 && tok1ind<tokenCount){
                    if (strcmp(tokens[tok1ind],",")== 0)
                    {
                        int tok2ind = parseExpr(tok1ind+1,tokens,postfixTokens);
                        if(tok2ind != -1 && tok2ind<tokenCount){
                         if (strcmp(tokens[tok2ind],",")== 0){
                             
                             int tok3ind = parseExpr(tok2ind+1,tokens,postfixTokens);
                            if(tok3ind != -1 && tok3ind<tokenCount){
                                if (strcmp(tokens[tok3ind],",")== 0){
                                    
                                    int tok4ind = parseExpr(tok3ind+1,tokens,postfixTokens);
                                    if(tok4ind != -1 && tok4ind<tokenCount){
                                        if (strcmp(tokens[tok4ind],")") == 0)
                                        {   
                                            strcpy(postfixTokens[postfixind].strcontent , "choose");
                                            postfixTokens[postfixind].type = toperator;
                                            postfixind++;              
                                            return tok4ind+1;
                                            }
                    }}}}}}}}} 
            return -1;
            }
    else if(strcmp(tokens[curToken],"tr") == 0){
        if(curToken+1<tokenCount){
            if (strcmp(tokens[curToken+1],"(") == 0){
                int tok1ind = parseExpr(curToken+2,tokens,postfixTokens);
                if(tok1ind != -1 && tok1ind<tokenCount){
                    if (strcmp(tokens[tok1ind],")") == 0){
                        strcpy(postfixTokens[postfixind].strcontent , "tr");
                        postfixTokens[postfixind].type = toperator;
                        postfixind++;    
                        return tok1ind +1;
                    }}}}
        return -1;
    }
    else if(checkFloat(tokens[curToken])){
        sprintf(postfixTokens[postfixind].strcontent,"%s%s%s","cnvrt(",tokens[curToken],")");
        postfixTokens[postfixind].type = tscalar;
        postfixTokens[postfixind].trow = 1;
        postfixTokens[postfixind].tcol = 1;
        postfixind++;
        return curToken+1;
    }else{
        
        return -1;
    }
}