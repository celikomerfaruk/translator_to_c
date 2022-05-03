#ifndef TOKEN_H_
#define TOKEN_H_


typedef enum tokenType{tscalar, tonedarray,ttwodarray,toperator,error} tokenType;
typedef struct token{
    char strcontent[1000];
    tokenType type;
    int tcol;
    int trow; 
}token_t;

#endif