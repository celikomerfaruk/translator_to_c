#ifndef MAINH_H_
#define MAINH_H_
void parseLines(char *text);
void seperateTokens(char *text);
void checkScalar(int tokencount,char tokens[300][80]);
void checkOneDimArr(int tokencount,char tokens[300][80]);
void checkMatrix(int tokencount,char tokens[300][80]);
void checkAssignment(int tokencount,char tokens[300][80]);
int checkPrint(char tokens[300][80]);
int checkIndexAssignment(int tokencount,char tokens[300][80]);
FILE *infile;
FILE *outfile;
int loopcount;
int curLine;
int tokenCount;
int checkprintsep(char tokens[300][80]);
int checkForLoop(char tokens[300][80]);
char headofoutput[660] = "#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>\ntypedef struct matrix{\nint col;\nint row; \nfloat** ind;\n}matrix_t;\nmatrix_t define(int row,int col);\nmatrix_t cnvrt(float x);\nmatrix_t choose(matrix_t expr1,matrix_t expr2, matrix_t expr3,matrix_t expr4);\nmatrix_t getInd(matrix_t r,matrix_t p1,matrix_t p2);\nvoid printmtrx(matrix_t m);\nmatrix_t addsub(matrix_t v1,matrix_t v2,char oprtr);\nvoid assign(matrix_t *left,matrix_t right);\nmatrix_t msqrt(matrix_t x);\nmatrix_t tr(matrix_t x);\nmatrix_t mult(matrix_t o1,matrix_t o2);\nvoid printsep();\nint main(){\n";
char endOfOutput[2400] = "\nvoid printsep(){\n printf(\"------------\\n\"); }\nvoid assign(matrix_t* left,matrix_t right){\nfor(int i = 0; i< (*left).row ; i++){\nfor(int k = 0 ; k< (*left).col ; k++){\n(*left).ind[i][k] = right.ind[i][k]; } } }\n void printmtrx(matrix_t m){ for (int i = 0; i < m.row; i++) { for (int k = 0; k < m.col; k++) { if(ceilf(m.ind[i][k]) == m.ind[i][k]){ printf(\"%i \",(int)(m.ind[i][k])); } else{ printf(\"%f \",m.ind[i][k]);} } printf(\"\\n\"); } }\n matrix_t mult(matrix_t o1,matrix_t o2){\nmatrix_t res; res = define(o1.row,o2.col); if(o1.row == 1 && o1.col == 1){\nfor(int i=0;i<o2.row;i++){\n for(int j=0;j<o2.col;j++){\n res.ind[i][j]= o2.ind[i][j]*o1.ind[0][0]; } } }else if(o2.row==1&&o2.col==1){ for(int i=0;i<o1.row;i++){ for(int j=0;j<o1.col;j++){\n res.ind[i][j]= o1.ind[i][j]*o2.ind[0][0]; } } }else{\n for(int i=0;i<o1.row; i++){ for(int j=0;j<o2.col; j++){ res.ind[i][j] = 0; for(int k=0;k<o1.col; k++){\n res.ind[i][j]+=o1.ind[i][k]*o2.ind[k][j]; } } } }\n return res; }\n matrix_t msqrt(matrix_t x){\n matrix_t tmp; tmp=define(1,1); tmp.ind[0][0] = sqrt(x.ind[0][0]);\n return tmp; }\n matrix_t tr(matrix_t x){\n matrix_t tmp; tmp = define(x.col,x.row); for(int i = 0;i<tmp.row;i++){ for(int j=0;j<tmp.col;j++){ tmp.ind[i][j]=x.ind[j][i]; } }\n return tmp; }\n matrix_t define(int row,int col){ matrix_t tmp; tmp.col = col; tmp.row = row; tmp.ind = (float**)calloc(row , sizeof(float*)); for (int i = 0; i < row; i++) tmp.ind[i] = (float*)calloc(col , sizeof(float*));\n return tmp; }\n matrix_t addsub(matrix_t v1,matrix_t v2,char oprtr){ matrix_t tmp; tmp = define(v1.row,v1.col); for (int i = 0; i < tmp.row; i++) { for (int k = 0; k < tmp.col; k++) { if(oprtr == '+'){ tmp.ind[i][k] = v1.ind[i][k] + v2.ind[i][k]; }else if(oprtr == '-'){ tmp.ind[i][k] = v1.ind[i][k] - v2.ind[i][k]; } }}\n return tmp; }\n matrix_t getInd(matrix_t r,matrix_t p1,matrix_t p2){\n matrix_t tmp; tmp = define(1,1); int ind1 = (int)(p1.ind[0][0]); int ind2 = (int)(p2.ind[0][0]); tmp.ind[0][0] = r.ind[ind1-1][ind2-1];\n return tmp; }\n matrix_t cnvrt(float x){ matrix_t res; res = define(1,1); res.ind[0][0] = x;\n return res; }\n matrix_t choose(matrix_t expr1,matrix_t expr2, matrix_t expr3,matrix_t expr4){\n matrix_t tmp ; tmp =define(1,1); if(expr1.ind[0][0] == 0){\n tmp.ind[0][0]= expr2.ind[0][0]; } else if(expr1.ind[0][0]> 0 ){\n tmp.ind[0][0]= expr3.ind[0][0]; }else {\n tmp.ind[0][0]= expr4.ind[0][0]; }\n return tmp; }";
#endif