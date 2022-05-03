#ifndef EVALUATE_POSTFIXH_H_
#define EVALUATE_POSTFIXH_H_

#include "token.h"
token_t stack[128];
int stackInd = 0;
int tokenc = 0;
extern int postfixind;
extern int  checkFloat(char* num);
token_t checktr(token_t t1);
token_t checkSquare(token_t t1);
token_t checkPlusMinus(token_t t1, token_t t2, char* operand);
token_t checkmult(token_t t1, token_t t2);
token_t checkonedimarr(token_t t1,token_t ind1);
token_t checktwodimarr(token_t t1,token_t ind1, token_t ind2);
token_t checkchoose(token_t t1,token_t t2,token_t t3, token_t t4);



#endif