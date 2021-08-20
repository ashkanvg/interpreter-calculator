// include
// camel case
//----------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//----------------------------------------------------
#define NONE   -1
#define NUM    256
#define DONE   260
//----------------------------------------------------
float emit(float a, float b, char p); /*  generates output  */
/* print compile error for errors! */
//----------------------------------------------------
int isOperator(char p); /* lexical analyzer --> isOperator(char) */
char isDigit(char p); /* lexical analyzer --> isDigit(char) */
int isNumber(char * p); /* lexical analyzer --> isNumber(str) */
int isString(char * p); /* lexical analyzer --> isString(str) */
//-----------------------------------------------------
int factorOne(char *p); /* factor -> * and / */
int isLeftParenthesis(char p); /* factor -> parenthesis */
int isRightParenthesis(char p); /* factor -> parenthesis */
//-----------------------------------------------------
int getLeftOperand(char * p, char * l); /* more terms -> L */
int getOperator(char * p, int index, char * op); /* more terms -> O */
int getRightOperand(char * p, char * r); /* more terms -> R */
//-----------------------------------------------------
float parse(char * p); /*  parses and translates expression list  */
//-----------------------------------------------------
int length(char * p);
int isEmpty(char * p);
char *removeWhiteSpaces(char *str);

//----------------------------------------------------
int main()
{
    char exp[NUM];
    printf("Enter Expression:\n");
    fgets(exp, NUM, stdin);

    // remove white space
    removeWhiteSpaces(exp);
    // parse
    float result = parse(exp);
    printf("%f", result);

}
//----------------------------------------------------

// check if operator
int isOperator(char p) {
    if (p == '+' || p == '-' || p == '*' || p == '/' || p == '%' || p == '^' ) return p;
    else return 0;
}
// chech if p is digit (one char)
char isDigit(char p) {
    if (p >= '0' && p <= '9') return 1;
    else {
        return 0;
    }
}

// use for expression (string)
int isNumber(char *p) {
    while(*p) {
        if (!isDigit(*p)){
          if(isOperator(*p) || isLeftParenthesis(*p) || isRightParenthesis(*p)){
              return 0;
          }else{
              printf("compile error!");
              exit(0);
          }
        }
        p++;
    }
    return 1;
}
int isString(char *p){
    while(*p) {
        if (isDigit(*p)) return 0;
        p++;
    }
    return 1;
}

// find length
int length(char * p)
{
    return (int) strlen(p);
}

int factorOne(char *p)
{
    for(; *p; p++)
    {
        if (!isDigit(*p) && *p != '/' && *p != '*' && *p != '^' && *p != '%') return 0;
    }
    return 1;
}

// check if left parenthesis
int isLeftParenthesis(char p) {
    if (p == '(') return 1;
    else return 0;
}
// check if right parenthesis
int isRightParenthesis(char p) {
    if (p == ')') return 1;
    else return 0;
}

// LOR
int getLeftOperand(char * p, char * l) {

    // l --> left operand
    int i = 0;

    // factorTwo
    if (factorOne(p)) {
        while(1) {
            if (*p == '*' || *p == '/' || *p == '^' || *p == '%') break;
            l[i++] = *p++;
        }
        return i;
    }

    // (parenthesis)
    if(isLeftParenthesis(*p)) {
        int countLeftParenthesis = 1; // count of left parenthesis
        int countRightParenthesis= 0; // count of right parenthesis
        p++;
        while(1) {
            if (isLeftParenthesis(*p))  countLeftParenthesis++;
            if (isRightParenthesis(*p)) countRightParenthesis++;

            if (isRightParenthesis(*p) && countLeftParenthesis == countRightParenthesis)
                break;
            l[i++] = *p++;
        }
        l[i] = '\0';

        return i+2;
    }

    // Operand is a number
    while (1) {
        if (!isDigit(*p)) break;
        l[i++] = *p++;
    }
    l[i] = '\0';
    return i;
}
int getOperator(char * p, int index, char * op) {
    *op = p[index];
    return index + 1;
}
int getRightOperand(char * p, char * r) {
    // r --> right operator
    while(*p) {
        if(isDigit(*p) || isOperator(*p) || isLeftParenthesis(*p) || isRightParenthesis(*p)){
            *r = *p;
            *p++;
            *r++;
        }else{
            if(*p=='\0' || *p=='\n'){
                break;
            }
            printf("compile error");
            exit(0);
        }
    }
    *r = '\0';
    return 0;
}


int isEmpty(char * p) {
    // Check if string/char is empty
    if (length(p) == 0) return 1;
    else return 0;
}

//  parses and translates expression list
float parse(char * p) { //with stack (recursive)
    // if all of expression was number return expression and finished
    if (isNumber(p)) return atoi(p);
    if (isString(p)) {
        printf("compile error!");
        exit(0);
    }
    // Get Left, Right and Op from expression.
    char leftOperand[NUM] = ""; char rightOperand[NUM]= "";
    char op;

    // LOR --> use for stack
    int leftOpIndex   = getLeftOperand(p, leftOperand);
    int operatorIndex = getOperator(p, leftOpIndex, &op);
    int rightOpIndex  = getRightOperand(p+operatorIndex, rightOperand);

    // if there is no right operand parse leftOperand
    // but i exit program and we never see line 181 because of compiler nature
    if (isEmpty(rightOperand)) return parse(leftOperand);

    // emit function use for calculate
    return emit(parse(leftOperand),parse(rightOperand),op);
}

// emit function use for calculate
float emit(float a, float b, char p) {
    switch(p) {
        case '+': return a+b; // +
        case '-': return a-b; // -
        case '*': return a*b; // *
        case '^': return pow(a,b); // ^
        case '/': // /
            if (b == 0) {
                printf("Compile Error");
                exit(1);
            }
            float d = ((float)a)/((float)b);
            //printf("%f",d);
            return d;
        case '%':
            if (b == 0) {
                printf("Compile Error"); exit(1);
            }
            return (int)a%(int)b;
        default:
            puts("Compile Error.\n"); // if input undefined bad operator
            exit(0);
            //break;
    }
    return 0;
}

char *removeWhiteSpaces(char *str)
{
    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] != ' ')
        str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    return str;
}