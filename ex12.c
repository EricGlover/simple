//ex 12.12 - 12.14
//infix to postfix converter
//Eric Glover
//11/24/16
//TM T-Swift

//convert this : (6 + 2) * 5 - 8 / 4
//to
//this : 6 2 + 5 * 8 4 / -

//STATUS : 12.12 the infix to postix converter & 12.13 (the postfix evaluator)  & 12.14 (allowing ints > 9, single letter variables) are OPERATIONAL
         //fully integrated into the compiler now
//TO-DO :
          //FIX the currently only known bug, see below
          //do the machine code only version of the %  & * operators that don't rely on code Mod and code Exponent


//read expression into char array infix and use algorithm to create postfix stored in char array postfix
/*algorithm
1. push a left parenthesis onto stack
2. append a right parenthesis onto end of infix
3. while stack not empty, read infix from left to right
  if digit copy to postfix
  if left parenthesis push onto stack
  if operator
    pop operators (if there are any) at top of stack while they have equal or higher precedence than the current operator -> insert into postfix
    push current char onto stack
  if right parenthesis
    pop operators from top of stack into postfix until left parenthesis is at top of stack
    pop and discard left parenthesis

operators: + - * / ^ %

algorith for postfix evaluation
1. append the null terminator '\0' to the end of the postfix expression.
2. if not null bit then read expression left to right
  if char is digit
    push it's int value onto the stack
  ohterwise if it's an operator
    pop the two top elements of the stack into var x and y
    calculate y operator x
    push the result of calculation onto the stack
3.When you find the null bit pop the top value of the stack. this is the end result

*-----------------------BUGS----------------------*
//noticed an error in the algorithm I believe
( 2 / 2 + 2 ) * 40 - 10 * 10 + 2 * 11   = 42 program yields = -2

the postfix expression should be 2 2 / 2 + 40 * 10 10 * 2 11 * - +
but it thinks it's               2 2 / 2 + 40 * 10 10 * 2 11 * + -
so when it gets to this step : 120 100 22 - +    <----it does this and not this ----> 120 100 22 - +
aha algorithm for converting #3. if operater .... equal or higher precedence
everything's working in precedence, the problem seems to be when you hit then end you just blindly pop the stack till it's empty


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ex12.h"
#include "simple.h"
#include "simpletron.h"

//should this ptr be in the global namespace??kinda makes sense
StackNodePtr stackPtr = NULL;
StackNodePtr varList = NULL;

//now that this is going to be called from simple.c it would make sense for it to take the string token to be eval'd and converted as an arg
//char * ex12_main(char *infix){
int ex12_main(char *infix){
  //stackPtr will point to top of stack
  int i;
  char c;
  c = i = 0;
  //char infix[50] = {0};
  char postfix[50] = {0};

  TableEntry *table = getTable();         //pass ex12_main the reference to the symbol table
  //printf("ex12_main finds infix to be : %s\n", infix );
  //printf("ex12_main sees that table[0].symbol = %d; .type = %c\n", table[0].symbol, table[0].type );
  printf("ex12_main sees that instructionCounter = %d\n", instructionCounter);


  //pushVar(&varList, 'a', 2);
  //printStack(varList);

  //printf("Please enter infix expression to be converted to postfix: ");
  /*while ( (c = getchar()) != EOF && c != '\n'){
    infix[i] = c;
    i++;
  }*/
  //scanf("%s", infix); does not work
  //infix[i] = '\0';
  printf("This is infix[] : %s\n", infix);
  convertToPostfix(infix, postfix);
  printf("This is infix[] : %s\n", infix);
  printStack(stackPtr);
  printf("The postfix arrangement of \n%s\n is\n %s\n", infix, postfix);

  //printf("The value of %s is : %d\n", postfix, evaluatePostfixExpression(postfix));
  printStack(stackPtr);

  return evaluatePostfixExpression(postfix);

}

//convert infix to postfix
//converting this to deal with ints > 9 and single letter vars
//use checkNums and strtok()
//for now assume everything is spaced out ie ( 2 + 3 ) NOT (2+3)
//actually this still works just make a master copy of the original string
//actually NO assume spaces
//str copy seems succesful


void convertToPostfix(char infix[], char postfix[]){
  char *original = malloc((strlen(infix) + 1) * sizeof(char));
  memset(original, '\0', sizeof(original));
  strcpy(original, infix);
  printf("%s is infix\n%s is original\n", infix, original);
  //1. push a left parenthesis onto stack
  push(&stackPtr, '(');
  //2. append a right parenthesis onto end of infix
  strcat(infix, " )");
/*  3. while stack not empty, read infix from left to right
    if digit copy to postfix
    if left parenthesis push onto stack
    if operator
      pop operators (if there are any) at top of stack while they have equal or higher precedence than the current operator -> insert into postfix
      push current char onto stack
    if right parenthesis
      pop operators from top of stack into postfix until left parenthesis is at top of stack
      pop and discard left parenthesis

  operators: + - * / ^ %
*/
  int i,a,z;
  i = a = z = 0;
  char *token = NULL;
  token = strtok(infix, " ");
  printf("%s is token\n", token);
  while(!isEmpty(stackPtr)){
    //is digit?
    //printf("infix is : %s\n", infix);
    //printf("postfix : %s\n", postfix);
    //printStack(stackPtr);
    /*if((a = atoi(&infix[i])) != 0){
      //strcat(postfix, &infix[i]);
      puts("found an int");
      postfix[strlen(postfix)] = infix[i];
      postfix[strlen(postfix)] = '\0';

    }*/
    //instead of iterating through infix[] using i I'll now use strtok()

    if(checkNums(token)){
      puts("found an int");
      //hmmmm......
      for(z = 0; z < strlen(token); z++){
        postfix[strlen(postfix)] = token[z];
      }
      //reset token ??
      //token -= z * sizeof(char);
      //postfix[strlen(postfix)] = infix[i];
      //postfix[strlen(postfix)] = '\0';
      strcat(postfix, " ");
    }else if ( *token == '(' ){
      //push left parenthesis onto stack
      push(&stackPtr, '(' );
    }else if (isOperator(*token)){
      //if there's an operator on stack of equal or higher value pop it into postfix
      puts("found operator");
      if (isOperator(stackPtr->data)){
        if(precedence(stackPtr->data, *token) >= 0){
          //again the problem of pushing a single char onto a string.....ugh
          //char new = pop(&stackPtr) + '\0';
          //strcat(postfix, new);
          //strcat(postfix, pop(&stackPtr));
          postfix[strlen(postfix)] = pop(&stackPtr);
          //postfix[strlen(postfix)] = "";
          strcat(postfix, " ");
        }
      }
      push(&stackPtr, *token);

    }else if ( *token == ')' ){
      //pop all operators into postfix until you find a (, then throw away the (
      while(stackPtr->data != '('){
        //char *new = pop(&stackPtr) + '\0';
        //strcat(postfix, new);
        //strcat(postfix, pop(&stackPtr));
        //change this so that when the stack holds *-> + -> - you pop * then - then +

        //perhaps use a char pointer to transfer them and then precedence() check them ?


        postfix[strlen(postfix)] = pop(&stackPtr);
        //addnull byte
        strcat(postfix, " ");
      }
      pop(&stackPtr);
    }else if(*token == ' '){
      //do nothing
      //get rid of this later
    }else if(*token <= 122 || *token >= 97){
      postfix[strlen(postfix)] = *token;
      strcat(postfix, " ");
    }else{
      puts("error in evaluating infix expression");
      break;
    }

    printf("postfix : %s\n", postfix);
    printStack(stackPtr);
    token = strtok(NULL, " ");
    printf("%s is token\n", token);
    //i++;
  }
}
/*
algorith for postfix evaluation
1. append the null terminator '\0' to the end of the postfix expression.
2. if not null bit then read expression left to right
  if char is digit
    push it's int value onto the stack
  ohterwise if it's an operator
    pop the two top elements of the stack into var x and y
    calculate y operator x
    push the result of calculation onto the stack
3.When you find the null bit pop the top value of the stack. this is the end result
*/
//remember to use pushNum and popNum
//now must be fed postfix in format num ' ' num ' ' operator '[blankspace]' etc....
//change to accommodate new inputs

//DO HOOK MODIFICATIONS
int evaluatePostfixExpression(char *expr){

  int i,x,y,z;
  i = x = y = z = 0;
  char temp[50];
  char *token;
  TableEntry *table = getTable();
  //FILE *output = getOutput();
  //1. append the null terminator '\0' to the end of the postfix expression.
  printf("expr is %s\n", expr);
  expr[strlen(expr)] = '\0';
  printf("expr is %s\n", expr);
  if( (token = strtok(expr, " ")) != NULL ){

    do {
      printStack(stackPtr);
      i = x = y = z = 0;
      printf("token is %s\n", token);
      if( atoi(token) ){                              //is number
        x = searchTable(table, token, 'C');
        if ( x == -1 ){
          //error constant not found
        }else{
          x = table[x].location;
          pushNum(&stackPtr, x);    //pushh mem address onto stack
        }
      }else if( tolower(*token) <= 122 && tolower(*token) >= 97 ){    //is var, allow for upperCase and lowerCase
        x = searchTable(table, token, 'V');
        if ( x == -1 ){
          //error var not found
          puts("didnt find mem address");
        }else{
          x = table[x].location;
          pushNum(&stackPtr, x);    //push mem address onto stack
          puts("found mem address");
        }
      }else if (isOperator(*token)){                  //is operator
        x = popNum(&stackPtr);
        y = popNum(&stackPtr);
        pushNum(&stackPtr, calculate(x , y, *token) );
        //do something
      }else{
        puts("Error in evaluating the postfix expression\n");
      }
    } while( (token = strtok( NULL, " ")) != NULL );
  }
  return popNum(&stackPtr);

  /*
  while (expr[i] != '\0'){
    printf("%d is x\n", x);
    x = y = 0;
    if(checkNum(expr[i])){
      //convert char to int //change this to allow for ints > 9
      if(expr[i + 1] == '\0'){
        x = expr[i] - '0';
        //if char is single digit push onto the stack now
        pushNum(&stackPtr, x);
        x = 0;
      }else if (expr[i + 1] == ' '){
        x = expr[i] - '0';
        //if char is single digit push onto the stack now
        pushNum(&stackPtr, x);
        x = 0;
      }else if (checkNum(expr[i+1])){
        //keep checking until you find a blankspace or a \0
        for(y = 1; y < strlen(expr) - y; y++){
          if(checkNum(expr[i+y])){
            //keep going
          }else{
            //stop loop
            break;
          }
        }
        //undue last increment not necessary because for will always be broken because expr ends in \0
        //transform char string into int
        for(size_t z = 0; z < y; z++ ){
          //printf("z = %d ; power(10,y - z - 1) = %d\n", z, power(10, y - z - 1));
          //printf("expr[i+z] = %d\n", (expr[i+z] - '0'));
          x += power(10, y - z - 1) * (expr[i + z] - '0');
          //printf("x = %d\n", x);
          temp[z] = expr[i+z];
        }
        temp[i+y] = '\0';
        printf("temp is %s\n", temp);
        //do stuff here, convert x to a string
        //itoa(x, temp, 10);
        printf(" ex12 found %d is a Const at index %d\n", x, searchTable(table, temp, 'C'));
        pushNum(&stackPtr, x);
        i += y;
        x = y = 0;
      }

    }else if(isOperator(expr[i])){
      //remember it's first operator second
      //with hook mod x & y are mem addresses
      x = popNum(&stackPtr);
      y = popNum(&stackPtr);
      pushNum(&stackPtr, calculate(x,y,expr[i]) );
      //printf("%d %c %d = %d\n", x, expr[i], y, calculate(x,y,expr[i]));
      //pushNum(&stackPtr, calculate(x,y,expr[i]) );
      x = y = 0;
    }else if(expr[i] == ' '){
      //puts("postfix contained a blank space");
    }else if(expr[i] <= 122 || expr[i] >= 97){
      //look for lowercase single char vars 97 <==> 122
      //check somewhere for the value ... linked list ?
      if(checkVar(varList, expr[i]) != NULL) {
        x = *checkVar(varList, expr[i]);
        pushNum(&stackPtr, x);
        x = 0;
      }else{
        printf("Sorry I couldn't find a definition for variable %c\n", expr[i]);
      }

    }else if(expr[i] == '\0'){
      //found null terminator what's left on the stack should be the answer
      return popNum(&stackPtr);
    }else{
      puts("postfix evaluation problem");
    }
    printStack(stackPtr);
    i++;
  }
  */
  //found null terminator what's left on the stack should be the answer
  //return popNum(&stackPtr);

}

//determine if c is operator, if yes return 1 ; no return 0
//operators: + - * / ^ %
int isOperator(char c){

  switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '%':
      return 1;
    default:
      return 0;
  }

  return 0;
}

//determine precedence, if 1 < 2 return -1; if 1 == 2 return 0; if 1 > 2 return 1.
//^ > * / % > + -
//ex : ^ , + yields 1 : - , + yields 0; -, * yields -1
// ^ = 94     / = 47    - = 45    + = 43    * = 42    % = 37
int precedence(char operator1, char operator2){
  switch (operator1) {
    case '+':
      operator1 = 1;
      break;
    case '-':
      operator1 = 1;
      break;
    case '*':
      operator1 = 2;
      break;
    case '/':
      operator1 = 2;
      break;
    case '%':
      operator1 = 2;
      break;
    case '^':
      operator1 = 3;
      break;
  }
  switch (operator2) {
    case '+':
      operator2 = 1;
      break;
    case '-':
      operator2 = 1;
      break;
    case '*':
      operator2 = 2;
      break;
    case '/':
      operator2 = 2;
      break;
    case '%':
      operator2 = 2;
      break;
    case '^':
      operator2 = 3;
      break;
  }
  if (operator1 > operator2){
    return 1;
  }else if (operator1 == operator2){
    return 0;
  }else if (operator1 < operator2) {
    return -1;
  }else{
    puts("precedence problem");
  }
  return 9; //erroneous value
}

//push char onto stack
void push(StackNodePtr *topPtr, char data){
  StackNodePtr newPtr = malloc(sizeof(StackNode));

  //insert node to top
  if( newPtr != NULL){
    newPtr->data = data;
    newPtr->nextPtr = *topPtr;
    *topPtr = newPtr;
  }else{
    puts("WE HAVE A PROBLEM. NO MOAR SPACE");
  }
}

//pop char off of stack
char pop(StackNodePtr *topPtr){

  StackNodePtr temp = *topPtr;
  int popVal = (*topPtr)->data;
  *topPtr = (*topPtr)->nextPtr;

  //free space the node that was on top?
  free(temp);

  return popVal;
}
void pushNum(StackNodePtr *topPtr, int value){
  StackNodePtr newPtr = malloc(sizeof(StackNode));

  //insert node to top
  if( newPtr != NULL){
    newPtr->value = value;
    newPtr->nextPtr = *topPtr;
    *topPtr = newPtr;
  }else{
    puts("WE HAVE A PROBLEM. NO MOAR SPACE");
  }
}
void pushVar(StackNodePtr *varList, char data, int value){
  StackNodePtr newPtr = malloc(sizeof(StackNode));

  //insert node to top
  if ( newPtr != NULL){
    newPtr->value = value;
    newPtr->data = data;
    newPtr->nextPtr = *varList;
    *varList = newPtr;
  }else{
    puts("NO MOAR SPACE FOR POINTING");
  }
}

int popNum(StackNodePtr *topPtr){
  StackNodePtr temp = *topPtr;
  int popVal = (*topPtr)->value;
  *topPtr = (*topPtr)->nextPtr;

  //free space
  free(temp);

  return popVal;
}

//is stack empty, yes = 1; no = 0
int isEmpty(StackNodePtr topPtr){
  if (topPtr == NULL){
    return 1;
  }else {
    return 0;
  }
  return 0;
}

//print stack
void printStack(StackNodePtr topPtr){
  //if empty
  if (topPtr == NULL) {
    puts("The stack is empty.");
  }else{
    puts("The stack is :");

    //loop till end of stack
    while(topPtr != NULL){
      printf("%c / %d --> ", topPtr->data, topPtr->value);
      topPtr = topPtr->nextPtr;
    }
    //at end
    puts("NULL");
  }
}
//returns 1 if int, 0 if not
//ASCII values 48 - 57
int checkNum(char input){
  if (input >= 48 && input <= 57){
    return 1;
  }else{
    return 0;
  }
}
//returns 1 if int, 0 if not
//ASCII values 48 - 57
int checkNums(char *input){
  int l, i = 0;
  l = strlen(input);
  for(i = 0; i < l; i++){
    if(*input >= 48 && *input <= 57){
      //remmeber to set the ptr back later,....
      input++;
    }else{
      return 0;
    }
  }
  return 1;
}

int * checkVar(StackNodePtr varPtr, char var){
  int *ptr = NULL;
  //puts("running checkVar");
  while (varPtr != NULL){
    //testing ...
    if(varPtr->data == var){
      ptr = &varPtr->value;
      //printf("checkvar sees that ptr->value = %d\n", varPtr->value);
      //printf("checkvar found that %c = %d\n", var, *ptr);
      return ptr;
    }else{
      //loop through the list
      varPtr = varPtr->nextPtr;
    }
  }
  return ptr;
}

//returns op2 operator op1
//with hook mod this will no longer directly calculate things
//instead it will produce instructions & return a mem address
int calculate(int op1, int op2, char operator){
  TableEntry *table = getTable();
  FILE *output = getOutput();
  int operationCode, operand, instruction = 0;
  int x = 0;
  printf("operator = %c\n", operator);

    switch (operator) {
    case '+' :
      // load op1
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      instruction = 0;

      // add op2
      operationCode = 30;       //ADD = 30
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      instruction = 0;

      //store in temp
      //create a temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;
      operationCode = 21;       //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      //write the final instruction
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      instruction = 0;

      return operand;     //return the temp location
      //return op2 + op1;
    case '-' :
      // load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      // subtract op1
      operationCode = 31;       //SUBTRACT = 31
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //store in temp
      //create a temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;

      operationCode = 21;       //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      return operand;     //return the temp location
      //return op2 - op1;
    case '*' :
      // load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      // multiply op1
      operationCode = 33;       //MULTIPLY = 33
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;
      //store in temp

      //create a temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;

      operationCode = 21;       //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      return operand;     //return the temp location
      //return op2 * op1;
    case '/' :
      // load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      // divide op1
      operationCode = 32;       //DIVIDE = 32
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //store in temp
      //create a temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;

      operationCode = 21;       //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      return operand;     //return the temp location
      //return op2 / op1;
    case '%' :
      //how the fuck do I do this in machine code ????lol
      //return op2 % op1;
      /*the actual hard way, try this later
      //load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      // divide op1
      operationCode = 32;       //DIVIDE = 32
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //multiply by op1
      operationCode = 33;       //MULTIPLY = 33
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //store tempp c
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;

      operationCode = 21;       //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //subtract temp c
      operationCode = 31;       //SUBTRACT = 31
      instruction = MEMSIZE * operationCode;
      instruction += table[x].location;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //store in temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;

      operationCode = 21;       //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;
      */

      /*the easy way, use the simpletron command to cheat */
      //returns op2 operator op1
      //load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //modulo op1
      operationCode = 34;     //MODULO = 34
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //store in temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      printf("operand = %d\n", operand);
      dataCounter--;

      operationCode = 21;     //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      printf("instruction = %d\n", instruction);
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      return operand;

    case '^' :
      //how the fuck do I do this in machine code ????lol
      //return power(op2, op1);

      //load op2
      operationCode = 20;     //LOAD = 20
      instruction = MEMSIZE * operationCode;
      instruction += op2;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //exponent op1
      operationCode = 35;     //EXPONENT = 35
      instruction = MEMSIZE * operationCode;
      instruction += op1;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      //store in temp location
      x = searchTable(table, "last", 'u');
      table[x].location = dataCounter;
      table[x].type = 't';
      operand = dataCounter;
      dataCounter--;

      operationCode = 21;     //STORE = 21
      instruction = MEMSIZE * operationCode;
      instruction += operand;
      fprintf(output, "%d\n", instruction);
      instructionCounter++;

      return operand;

    default :
      printf("%c was sent to calculate, could not evaluate the expression : %d %c %d", operator, op2, operator, op1);
      return 0;
  }
}

//x ^ y
int power(int x, int y){
  int i, old;
  i = old = 0;
  old = x;
  if (y == 0){
    return 1;
  }else if (y > 0){
    for (i = 2; i <= y; i++) {
      x *= old;
    }
  }else if (y < 0){
    puts("pow functionality for negative exponents not yet implemented");
  }
  return x;

}
