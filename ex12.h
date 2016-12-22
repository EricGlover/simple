//ex12.h

struct stackNode {
  char data;
  int value;
  struct stackNode *nextPtr;
};

typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;

//char * ex12_main(char *infix);
int ex12_main(char *infix);

void convertToPostfix(char infix[], char postfix[]);
int evaluatePostfixExpression(char *expr);

int isOperator(char c);
int precedence(char operator1, char operator2);

void push(StackNodePtr *topPtr, char value);
void pushNum(StackNodePtr *topPtr, int value);
void pushVar(StackNodePtr *varList, char data, int value);

char pop(StackNodePtr *topPtr);
int popNum(StackNodePtr *topPtr);

char stackTop(StackNodePtr topPtr);
int isEmpty(StackNodePtr topPtr);
void printStack(StackNodePtr topPtr);

//returns 1 if int, 0 if not
//ASCII values 48 - 57
int checkNum(char input);
int checkNums(char *input);
int * checkVar(StackNodePtr varPtr, char var);

int calculate(int op1, int op2, char operator);
int power(int x, int y);
