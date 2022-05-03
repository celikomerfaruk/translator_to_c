#ifndef VARIABLE_H_
#define VARIABLE_H_

typedef enum varType{scalar, onedarray,twodarray} vartypes;
typedef struct var{
    char id[20];
    vartypes type;
    float value[300];
    int col;
    int row; 
}var_t;

#endif