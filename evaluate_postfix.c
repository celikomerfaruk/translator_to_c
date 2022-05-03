#include  <string.h>
#include <stdio.h>
#include "evaluate_postfixh.h"

//stack functions for executing postfix notation

void push(token_t x){
    if(stackInd == 128){
        return;
    }
    stack[stackInd] =x;
    
    stackInd++;
    
}

token_t pop(){
    token_t tmp;
    tmp.type = error;
    if(stackInd==0){
        return tmp;
    }
    token_t popItem = stack[stackInd-1];
    stackInd--;
    return popItem; 
}

token_t top(){
    token_t popItem = stack[stackInd-1];
    return popItem; 
}

//checking type compatibility
//semantic and syntactic analysis
//executing postfix input and generating appropriate output that c code can execute
int postfixToInfix(token_t postfixTokens[300]){
   
    for(int i= 0; i<postfixind ; i++){
  
        if(postfixTokens[i].type == toperator){
            if(strcmp(postfixTokens[i].strcontent, "+")==0){
                // left association is provided
                if((i+1)<postfixind){
                    if(strcmp(postfixTokens[i+1].strcontent ,"-")==0 ||strcmp(postfixTokens[i+1].strcontent ,"+")==0 ){
                        
                        token_t p1 = pop();
                        token_t p2 = pop();
                        token_t p3 = pop();
                        
                        token_t tresult1 = checkPlusMinus(p3,p2,postfixTokens[i+1].strcontent);
                        if(tresult1.type != error){
                            token_t tresult2 = checkPlusMinus(tresult1,p1,postfixTokens[i].strcontent);
                            if(tresult2.type != error){
                                push(tresult2);
                            }else{
                   
                                return -1; //error
                            }
                        }else{
                   
                            return -1; //error
                        }
                        i++;
                        continue;
                    }}
               
                token_t t1 = pop();
                token_t t2 = pop();
                token_t tresult = checkPlusMinus(t2,t1,postfixTokens[i].strcontent);
                if(tresult.type != error){
                    push(tresult);
                }else{
                   
                   return -1; //error
                }
                
          } else if(strcmp(postfixTokens[i].strcontent, "tr")==0){
                token_t t1 = pop();
                token_t tresult = checktr(t1);
                if(tresult.type != error){
                    push(tresult);
                }else{
                    
                    return -1 ; //error
                }
            } 
              else if(strcmp(postfixTokens[i].strcontent, "choose")==0){
                token_t t1 = pop();
                token_t t2 = pop();
                token_t t3 = pop();
                token_t t4 = pop();
                token_t tresult = checkchoose(t4,t3,t2,t1);
                 
                if(tresult.type != error){
                   
                
                    push(tresult);
                }else{
                    return -1 ; //error
                }
            }
            else if(strcmp(postfixTokens[i].strcontent, "-")==0){
                token_t t1 = pop();
                token_t t2 = pop();
                token_t tresult = checkPlusMinus(t2,t1,"-");
                if(tresult.type != error){
                    push(tresult);
                }else{
                    return -1; //error
                }
            }
            else  if(strcmp(postfixTokens[i].strcontent, "[")==0){
                token_t tmp1 = pop();
                token_t ind1 = pop();
                if(tmp1.type ==tonedarray){
                    token_t tresult =checkonedimarr(tmp1,ind1);
                    if(tresult.type != error){
                        push(tresult);
                    }else{
                        return -1 ;//error
                    }
                }else if(tmp1.type == ttwodarray){
                    token_t ind2 = pop();
                    token_t tresult =checktwodimarr(tmp1,ind1,ind2);
                    if(tresult.type != error){
                        push(tresult);
                    }else{
                        return -1; //error
                    }
                }
            }
            else  if (strcmp(postfixTokens[i].strcontent, "sqrt")==0){
            
                token_t t1 = pop();
                token_t tresult = checkSquare(t1);
                if(tresult.type != error){
              
                    push(tresult);
                }else{
                    return -1; //error
                }
            }  else if (strcmp(postfixTokens[i].strcontent, "*")==0){
                token_t t1 = pop();
                token_t t2 = pop();
                token_t tresult = checkmult(t2,t1);
                if(tresult.type != error){
                    push(tresult);
                }else{
                    return -1; //error 
                } 
            }  
        }if(postfixTokens[i].type == tscalar){
            push(postfixTokens[i]);
            
        }if(postfixTokens[i].type == tonedarray){
            push(postfixTokens[i]);
        }if(postfixTokens[i].type == ttwodarray){
            push(postfixTokens[i]);
        } 

    }
    return 1;
    postfixind = 0;
    
}
// check square root
token_t checkSquare(token_t t1){
    token_t tmp = t1;

    if(t1.type == tscalar){
        sprintf(tmp.strcontent,"%s%s%s","msqrt(",t1.strcontent,")");
        
    }else{
        tmp.type = error;
    }
    return tmp;
}
//check transpose
token_t checktr(token_t t1){
    token_t tmp = t1;
    

    if(t1.type != error){
    sprintf(tmp.strcontent,"%s%s%s","tr(",t1.strcontent,")");

    tmp.trow = t1.tcol ;
    tmp.tcol = t1.trow ;
    }
    if(tmp.type != error){
        if(tmp.trow ==1 && tmp.tcol ==1){
        tmp.type = tscalar;
    }else if(tmp.tcol == 1){
        tmp.type = tonedarray;
    }else{
        tmp.type = ttwodarray;
    }}
    return tmp;

}
// check addition subraction
token_t checkPlusMinus(token_t t1, token_t t2, char* operand){
    
    token_t tmp = t1;
    

    if(t1.type == tscalar || t1.type == tonedarray || t1.type == ttwodarray){
        if((t2.type== tscalar || t2.type==tonedarray ||t2.type==ttwodarray)  && t2.trow == t1.trow && t2.tcol ==t1.tcol ){
            
            sprintf(tmp.strcontent,"%s%s%s%s%s%s%s","addsub(",t1.strcontent,",",t2.strcontent,",'",operand,"')");
        }else{
            tmp.type = error;
        }
    }else{
        tmp.type = error;
    }

    return tmp;
}

//check multiplication
token_t checkmult(token_t t1, token_t t2){
    token_t tmp = t1;
    
    if(t1.type == tscalar  ){
            
            sprintf(tmp.strcontent,"%s%s%s%s%s","mult(",t1.strcontent,",",t2.strcontent,")");
            tmp.trow = t2.trow;
            tmp.tcol = t2.tcol;
    }
    else if(t1.type == tonedarray ||t1.type == ttwodarray ){
        
        if((t2.type==tonedarray || t2.type == ttwodarray) && (t1.tcol == t2.trow) ){
            
          
            sprintf(tmp.strcontent,"%s%s%s%s%s","mult(",t1.strcontent,",",t2.strcontent,")");
            tmp.tcol = t2.tcol;
        }else if (t2.type == tscalar){
            
            sprintf(tmp.strcontent,"%s%s%s%s%s","mult(",t1.strcontent,",",t2.strcontent,")");
        }else{
            tmp.type = error;
        }
    }else{
        tmp.type = error;
    }
    if(tmp.type != error){
        if(tmp.trow ==1 && tmp.tcol ==1){
        tmp.type = tscalar;
    }else if(tmp.tcol == 1){
        tmp.type = tonedarray;
    }else{
        tmp.type = ttwodarray;
    }}
    
    return tmp;
}
//check vector notation
token_t checkonedimarr(token_t t1,token_t ind1){
    token_t tmp = t1;

    if(ind1.type == tscalar){
            
            sprintf(tmp.strcontent,"%s%s%s%s%s","getInd(",t1.strcontent,",",ind1.strcontent,",1)");
            tmp.tcol =1;
            tmp.trow = 1;
            tmp.type = tscalar;
    }else{
        tmp.type = error;
    }
    return tmp;
}
//check matrix notation
token_t checktwodimarr(token_t t1,token_t ind1, token_t ind2){
    token_t tmp = t1;
    if(ind1.type == tscalar && ind2.type == tscalar){
            
            sprintf(tmp.strcontent,"%s%s%s%s%s%s%s","getInd(",t1.strcontent,",",ind1.strcontent,",",ind2.strcontent,")");
            tmp.tcol =1;
            tmp.trow = 1;
            tmp.type = tscalar;
    }else{
        tmp.type = error;
    }
    return tmp;
}
// check choose function
token_t checkchoose(token_t t1,token_t t2,token_t t3, token_t t4){
    token_t tmp = t1;
    
    if(t1.type == tscalar && t2.type == tscalar&& t3.type == tscalar&& t4.type == tscalar ){
            
            sprintf(tmp.strcontent,"%s%s%s%s%s%s%s%s%s","choose(",t1.strcontent,",",t2.strcontent,",",t3.strcontent,",",t4.strcontent,")");
            
    }else{
        tmp.type = error;
    }
    return tmp;
}