#ifndef EXPR_PARSERH_H_
#define EXPR_PARSERH_H_

#include "token.h"
#include "variable.h"

extern int tokenCount;
int isVarDeclared(char* id);
int checkInt(char* num);
int checkFloat(char num[4]);
int parseMoreterm(int curToken,char tokens[300][80],token_t postfixTokens[300] );
int parseTerm(int curToken,char tokens[300][80],token_t postfixTokens[300] );
int parseMorefactor(int curToken,char tokens[300][80],token_t postfixTokens[300] );
int parseFactor(int curToken,char tokens[300][80],token_t postfixTokens[300] );

extern char stack[128][32];
void push(char* x);
char* pop();
char* top();
int postfixToInfix(token_t postfixTokens[300]);
int postfixind = 0;
extern var_t variables[50];

#endif