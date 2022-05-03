#ifndef CHECK_H_
#define CHECK_H_

#include "token.h"
#include "variable.h"

extern int iserror;
void printDeclaration(char* id , char* row, char* col);
extern int loopcount;
int parseExpr(int curToken,char tokens[300][80],token_t postfixTokens[300]);
int postfixToInfix(token_t postfixTokens[300]);
extern token_t stack[128];
extern int tokenCount;
token_t pop();
token_t top();
var_t variables[50];
int varindex = 0;
int isVarDeclared(char* id);

#endif