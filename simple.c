//Simple compiler
//12/3/16
//Eric Glover
//TM T - Swift

//STATUS : ex 12.26 & ex 12.27 done, it seems to be working overvall
    //if..goto may be done, seems to work fine
    //secondPass() seems functional
    //simpletron seems functional
    //currently working on ex 12.28 optimization of code, seems operational
    //working on ex 12.29, subroutines

//TO-DO :
//consider changing the filename of the sml instructions to [whatever the input file name was sans .simple.txt].sml.txt
//ex 12.29, modifications : syntax and error checking, subroutines
  //subroutines, modifying searchTable to allow for type 'R'

//simple language, a complex explanation
/*
command   example statement           description

rem       50 rem this is a remark     text following rem is ignored by compiler
input     30 input x                  prompt user to enter an int, store it in x
let       80 let u = 4 * (j-56))      assign u the value of RH expression (arbtrarily complex)
print     10 print w                  desplay value of w
goto      70 goto 45                  transfer program control to line 45
if...goto 35 if i == z goto 80        compare i & z for equality and transfer control
                                      to line 80 if true, if false continue to next line
end       99 end                      terminate program execution

7 commands total
only lowercase letters can be used in the file
no descriptive variable names, variables are a single letter
only int variables
no variable declaration, merely mentioning a variable cause it to be
  declared and initialized to 0
no string manipulation (strings are a syntax error)
compiler assumes the program is entered correctly
every simple statement has a line number (they must be in ascending order)
  and a simple instruction
all commands can be used repeatedly except for end which must occur only once

equality operators allowed in goto & if goto statements (<, >, <=, >=, ==, !=)

example program
1   10 rem    determine and print the sum of two intergers
2   15 rem
3   20 rem    input the two ints
4   30 input a
5   40 input b
6   45 rem
7   50 rem    add ints and store result in c
8   60 let c = a + b
9   65 rem
10  70 rem    print result
11  80 print c
12  90 rem    terminate program
13  99 end

example a) input 3 ints, determine their average and print
1   1 rem   input 3 ints, determine their average and print
2   2 input a
3   3 input b
4   4 input c
5   5 let z = (a+b+c) / 3
6   6 print z
7   7 end

example b)
1   1 rem     use a sentinel-controlled loop to input 10 ints and compute and print their sum
2   2 input a
3   3 if a == -9999 goto 6
4   4 let z = z + a
5   5 goto 2
6   6 print z
7   7 end

example c)
1   1 rem     use counter-controlled loop to input seven ints (some pos, some neg)
2   2 rem     and compute and print their average
3   3 input a
4   4 let z = z + a
5   5 let c = c + 1
6   6 if c < 7 goto input a
7   7 let c = c / 7
8   8 print c
9   9 end

example d)
1   1  rem     input a series of ints, determine and print largest
2   2  rem     first input indicates how many numbers will be processed
3   3  input s
4   4  input a
5   5  let c = c + 1
6   6  if a <= z goto 8
7   7  let z = a
8   8  if c < s goto 4
9   9  print z
10  10 end
//the above functions like an if statement inside a while loop oddly enough

example e)
1   1   rem      input 10 ints and print the smallest
2   2   rem      s is the total number of ints to try
3   3   let s = 10
4   4   input a
5   5   rem      z = current smallest #
6   6   let z = a
7   7   rem      c = counter var (init to zero)
8   8   let c = c + 1
9   9   rem      thus begins the while loop with the condition at if c < s
10  10  input a
11  11  rem      the if statement
12  12  if a >= z goto 14
13  13  let z = a
14  14  if c < s goto 10
15  15  print z
16  16  end

example f)
1   1   rem       calculate and print the sum of the even intergers from 2 - 30
2   2   rem       so set total = t, int to add = a, stop value = s = 30
3   3   let a = 2
4   4   let s = 30
5   5   let t = t + a
6   6   if a == s goto 9
7   7   let a = a + 2
8   8   goto 5
9   9   print t
10  10  end

1   1   rem       calculate and print the sum of the even intergers from 2 - 30
2   2   rem       so set total = t, int to add = a, stop value = s = 30
3   3   let s = 30
4   4   let a = a + 2
5   5   let t = t + a
6   6   if a < s goto 4
7   7   print t
8   8   end

example g)
1   1   rem       calculate and print the product of odd ints from 1 to 9
2   2   rem       total = t; int to * = a = 1; stop value = 9
3   3   let s = 9
4   4   let a = 3
5   5   let t = 1
6   6   let t = t * a
7   7   let a = a + 2
8   8   if a <= s goto 6
9   9   print t
10  10  end
*/
/*
subroutines
	10 gosub <letter>	(calling a sub routine)
	90 sub a		(subroutine definition)
		let x = y + 1
	 	print x
		return
	95 end
no unique namespaces, no line #’s in subroutines due to worries over branching into subroutines (what’s it do when it hits a return command) (how to keep these lines in a protected space?), 10 branch line(suba), line 90 compile code for subroutine pretty much like normal here,
return…how the fuck should this work?
does the subroutine code need to be placed every time the name appears ?
maybe the return is just a series of possible branches to all the locations after gosuba is called ???
maybe you can actually change operands on the fly?
gosuba
store your line number into var A*
	load A*
	0 out A*
	set command code to branch
	add your line number
	store in A*
actually it’s
	load constant 0
	add constant command code thats needed here
	add your line number(well your memory address) (how do?) (stored in a constant)
	store in A*
	branch to line sub a
		code for sub a executes
		return code = A*
	when you encounter A* flag it with a sentineL? or different array of flags ?
	in secondPass sort out the A* flags first then do the rest of the line numbers , it’s treated as a line number that is a variable? or just a variable, that happens to represent a line number


how to link subA with it’s line # for the secondPass()?
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simpletron.h"
#include "ex12.h"

//use strtok to read lines ??
//if the token is a line #, var, or constant then put it in the symbol table
//symbol table is an array of tableEntrys of size 100 for the moment
//location data type : for line #'s it's element in simpletron's memory where that line begins
    //for vars and constants it's the address where those things are stored
//vars and consts are allocated at the end of simpletron's memory, going backward to front
//ie 99, 98, 97 etc

//note to track memory usage during compilation and print an error if exceeded
  //how to do this is in page 7
//instruction counter : keeps track of the location for the next SML instruction to be placed into simpletron's memory
//data counter : keep track of where the location for the next var / constant to be placed into simpletron's memory
    //if instruction counter > data counter the program is full b/c instructions go front -> back and vars / constant storage is back -> front

struct tableEntry {
  int symbol;
  char type; // 'C', 'L', 'V', 'u', 'R' : constant or line number or variable or unnassigned or routine (function line number)
  int location; // 00 to 999
};

//do the nickname thing
typedef struct tableEntry TableEntry;
//TableEntry table[TABLESIZE];
TableEntry *tablePtr = NULL;        //used to pass ex12_main the reference to the symbol table
FILE *output = NULL;
#define TABLESIZE 100


void firstPass(FILE *filePtr, TableEntry *table, FILE *output);
void secondPass(FILE *filePtr, TableEntry *table, FILE *output, char *outName);
void setConstants(void);
void optimize(char *outFile, TableEntry *table);

//table functions
void printTable(TableEntry table[]);
//return index of entry or -1
int searchTable(TableEntry table[], char *token, char type);  //searchTable(table, "last", 'u') gives index of first unnassigned var symbol index
TableEntry* getTable(void);
FILE* getOutput(void);
void toString(int num, char *string);


int instructionCounter, dataCounter;
int flags[MEMSIZE];
int subFlags[MEMSIZE];




//  ./a.out simple.input.txt output.txt <option>
//option "o" = optimize
int main(int argc, char *argv[]){
  int z = 0;
  char *simpleCommands[3] = {argv[0], argv[2], "simpletron.dump.txt"};
  printf("simpleCommands[0] = %s\n", simpleCommands[0]);
  printf("simpleCommands[1] = %s\n", simpleCommands[1]);
  printf("simpleCommands[2] = %s\n", simpleCommands[2]);


  //create the symbol table
  TableEntry table[TABLESIZE];
  for( z = 0; z < TABLESIZE; z++){
    table[z].symbol = -1;     //indicates undeclared/unused
    table[z].location = -1;   //indicates undeclared/unused
    table[z].type = 'u';    //indicates undeclared/unused
  }
  tablePtr = table;
  //alternate style of doing things .... StackNodePtr newPtr = malloc(sizeof(StackNode));

  //int instructionCounter, dataCounter;
  instructionCounter = 0;
  //memsize of simpletron is currently 1000 i believe
  dataCounter = MEMSIZE - 1;

  //setup flags array
  //int flags[MEMSIZE] = {-1}; // this is declared in global namespace already
  for(size_t q = 0; q < MEMSIZE; q++){
    flags[q] = -1;
  }

  //read txt file formatted in simple and use it as the program
  //must input ./a.out <input.txt file name here> <output.txt file name here>
  if(argc < 3){
    printf("You provided %d arguments, please provide 3 or 4\n", argc);
    return 0;
  }
  FILE *filePtr;
  FILE *outPtr;
  if((filePtr = fopen(argv[1], "r")) == NULL){
    printf("Couldn't open %s\n", argv[1]);
    return 0;
  }else if( (outPtr = fopen(argv[2], "w")) == NULL){
    printf("Counldn't write to %s\n", argv[2]);
    return 0;
  }
  output = outPtr;

  firstPass(filePtr, table, outPtr);
  /*fclose(outPtr);
  if( (outPtr = fopen(argv[2], "rb+")) == NULL){
    printf("Couldn't write to %s\n", argv[2]);
    return 0;
  }*/
  fclose(outPtr);

  if(argc > 3){   //if given an option
    if (! strcmp(argv[3], "o") ){
      puts("running optimize");
      optimize(argv[2], table);
    }
  }

  if( (outPtr = fopen(argv[2], "r+")) == NULL){
    printf("Counldn't write to %s\n", argv[2]);
    return 0;
  }

  secondPass(filePtr, table, outPtr, argv[2]);
  fclose(filePtr);
  fclose(outPtr);



  simpletron_main(3, simpleCommands);

  //simpletron then reads compilation output file
  //simpletron()
  //simpletron() writes output to screen and to an output.txt file


  //try and run simpletron from here
  //simpletron_main(argc, argv);
  //ex12_main(infix);

  return 0;
}

        /*SIMPLE FUNCTIONS */
//still need to do the flags for the secondPass in goto / if ...goto
void firstPass(FILE *filePtr, TableEntry *table, FILE *output){
  //current iteration breaks files into 500 char lines

  char d[] = "  \n"; //if you don't delimit escape sequences they'll appear in your token string
  char *token = NULL;
  char line[500];
  int i, z, cNum, operand, operationCode, instruction, lineNum = 0; //sml instruction = operationCode[2 digits] + operand[3 digits]
  int needFlag = 0;
  char *command[] = {"rem", "input", "let", "print", "goto", "if", "end", "gosub", "sub", "return"};
  char *eqOp[] = {"<", ">", "<=", ">=", "==", "!="};   //allowed operators (<, >, <=, >=, ==, !=)
  int command_length = 9; //# of commands allowed in the language
  //let
  char var[10] = {0};  //list of vars to be assigned
  int varMem[10] = {-1}; //list of corresponding locations of vars , keep size same as var
  int tempM = 0;
  //print
  char type = 0;
  //char infix[100];
  char *infix = malloc(sizeof(char) * 500);
  char *postfix;
  char *temp;
  //if goto vars
  //char op1[10];
  //char op2[10];
  int op1, op2, branchLine = -1;
  //char branchLine[10];

  //gosub
  char string[100];

  i = 0;
  //read the file line by line , i guess that's an fgets thing ......
  while ( fgets(line, sizeof(line) , filePtr) != NULL){
    instruction = cNum = operand = operationCode = z = i = lineNum = 0;
    needFlag = 0;
    printf("line is %s\n", line);

    for (z = 0; z < 10; z++ ){ //just in case reset the arrays used in let
      var[z] = 0;
      varMem[z] = -1;  //was 0
    }
    //at the begining of every line reset infix to /0
    //printf("infix is now %s\n", infix);
    memset(infix, '\0', sizeof(infix));
    //printf("infix is now %s\n", infix);

    //begin a do while loop for this line
    do {
      if(i == 0){
        token = strtok( line, d);
      }
      printf("%s is token\n", token);

      //look for symbol in library, if not found insert into table
      if(i == 0 && checkNum(*token)){
        //LINE #        //this should find a line # or it's a syntax error
        if ( -1 == (searchTable(table, token, 'L')) ){    //if not in table then add it
          z = searchTable(table, "first", 'u');    //find first empty index for a line #
          table[z].symbol = atoi(token);
          table[z].location = instructionCounter;
          table[z].type = 'L';      // 'L' for Line #
          printf("table[%d].symbol = %d\n", z, table[z].symbol);
          printf("table[%d].location = %d\n", z, table[z].location);
          z = 0;  //reset z
        }

      }else if (i == 1){
        //COMMAND       //this should find a command or it's a syntax error

        //match the token with a command, find it's index in the command string array: index = cNum
        while(cNum < command_length && (strcmp(token, command[cNum])) ){    //fails when a command is found at index cNum
          cNum++;
        }
        /*this works too, actually, no I don't believe it does
        while(cNum < 6 ? strcmp(token, command[cNum]) : 0 ){
          cNum++;
        }
        */
        if(cNum > command_length - 1){ //is command_length -1 right?
          puts("Error command not found");
        }/*else{
          //puts("cNum > 5 = false?");
          //printf("cNum = %d\n", cNum);
          //printf("strcmp(token, command[cNum]) = %d\n", strcmp(token, command[cNum]) );
        }*/
        switch (cNum) {
          case 0 :
            //REM,  ignore everything after rem in this line
            //don't move instructionCounter, all sequential rem lines refer to the same line of sml code (the next actual instruction)
            while( (token = strtok( NULL, d)) != NULL ){
              //cycle through the rest of the tokens until the end of the line
            }
            break;
          case 1 :
            // INPUT , check the next token for a var, see if that's in the symbol table, if not put it in, convert to sml code , #define READ 10
            //10 input x 00 +1099 read x into location 99
            //how to talk to simpletron from here ?...don't just write this in a file for simpletron to use
            //if not found
            puts("input is running");
            operationCode = READ;
            //find var, should be next token ---consider error checking this
            for( size_t i = 0; (token = strtok(NULL, d)) != NULL; i++){     //search through line for vars, do the same code for each one you see
                z = searchTable(table, token, 'V');
                if ( z == - 1){    //symbol not found
                  operand = dataCounter--;
                  z = searchTable(table, "last", 'u');    //find last empty index
                  table[z].symbol = *token;               //token is char
                  table[z].type = 'V';                    // 'V' for variable
                  table[z].location = operand;
                }else {                                   //symbol was found
                  operand = table[z].location;
                }
                //write this sml instruction to file and increment the instructionCounter
                //convert operationCode + operand into one #
                instruction = MEMSIZE * operationCode;
                instruction += operand;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;
            }

            break;
          case 2 :
            // LET
            //find all symbols , allow multiple variable assignment? fuck it why not
            //consider passing through the line[] more than once

            //line is index, workaround
            //printf("strlen(line) = %lu\n", strlen(line));
            /*
            for(size_t i = 0; i < 500 ; i++){
              if ( line[i] == '='){
                infix = line + i;
                break;
              }
            }*/
            //copy line into infix token by token ?


            //infix = strchr(line, '='); //find remainder of line from = onwards and assign to infix
            //memcpy(infix, strchr(line, '='), sizeof(strchr(line, '='))+1);
            //printf("line is : %s\n section after '=' is : %s\n", line, infix);
            //simple.c:410:58: warning: result of comparison against a string literal is unspecified (use strncmp instead) [-Wstring-compare]
            //for( size_t i = 0; (token = strtok(NULL, d)) != "="; i++){
            //for( size_t i = 0; (token = strtok(NULL, d)) != "="; i++){
            for( size_t i = 0; strcmp((token = strtok(NULL, d)), "="); i++){     //search through line for vars, stop when token = "=" //non-zero is true
                //*token = tolower(*token);  //convert to lowercase
                var[i] = *token; //questionable string char problems
                //unoptimized search here
                z = searchTable(table, token, 'V');
                if (z == - 1){
                  z = searchTable(table, "last", 'u');
                  table[z].symbol = *token;
                  table[z].location = dataCounter--;
                  table[z].type = 'V';
                  varMem[i] = table[z].location;
                }else {
                  varMem[i] = table[z].location;
                }
            }
            for ( size_t i = 0; i < 10; i++){
              printf("var[i] is %c, varMem[i] is %d\n", var[i], varMem[i]);
            }

            //printf("before the while loop infix is %s\n", infix);
            //make sure everything on the right hand side is in the symbol table && put the expression into infix
            while ( (token = strtok(NULL, d)) != NULL ){
              //*token = tolower(*token);  //convert to lowercase
              strcat(infix, token);
              strcat(infix, " ");
              //is token var or constant ?

              //search for symbol in table , if not found insert it
              //fuck me I have to deal with multidigit numbers again ......... ohhh, I can atoi it?
              //printf("INSIDE WHILE LOOP: token is %s\n", token);
              //printf("INSIDE WHILE LOOP: infix is %s\n", infix);
              //what about operators???
              if( atoi(token) ){                            //if it's a number, possibly multidigit
                z = searchTable(table, token, 'C');
                if (z == -1 ){
                  z = searchTable(table, "last", 'u');
                  table[z].symbol = (atoi(token));
                  table[z].type = 'C';
                  table[z].location = dataCounter--;
                  printf("didn't find %s now is at table[%d]\n", token, z);
                }else {
                  //do nothing if found
                  printf("found %s at table[%d]\n", token, z);
                }
              }else{                                          //if it's a var
                z = searchTable(table, token, 'V');
                if (z == - 1 && (*token >= 97 && *token <= 122 )){
                  z = searchTable(table, "last", 'u');
                  table[z].symbol = *token;
                  table[z].location = dataCounter--;
                  table[z].type = 'V';
                  printf("didn't find %s now is at table[%d]\n", token, z);
                }else {
                  //do nothing if found, or if it's a non-number / non-letter
                  printf("found %s at table[%d]\n", token, z);
                }
              }

            }

            printTable(table);

            printf("line is : %s\n section after '=' is : %s\n", line, infix);
            /*for( size_t i = 0; i < 10; i++){
              printf("This is var[%zu] : %c\n", i, var[i]);
            }*/

              //convert to postfix
              //convertToPostfix(infix, postfix);

              printf("infix is now %s\nThe converted postfix of this is : %s\n", infix, postfix);
              //evaluate postfix kinda
              //evaluatePostfixExpression(postfix)
            //create appropriate code
            //load      ex12_main(infix);
            //store in vars
            tempM = ex12_main(infix); //returns the temp mem address
            printf("the temporary address is %d\n", tempM);
            z = 0;

            while( varMem[z] != -1 ){
              printf("This is varMem[%d] : %d\n", z, varMem[z]);
              //load
              operationCode = 20;     //LOAD = 20
              instruction = MEMSIZE * operationCode;
              instruction += tempM;
              fprintf(output, "%d\n", instruction);
              instructionCounter++;

              //store
              operationCode = 21;     //STORE = 21
              instruction = MEMSIZE * operationCode;
              instruction += varMem[z];
              fprintf(output, "%d\n", instruction);
              instructionCounter++;
              z++;
            }

            break;
          case 3 :
            //PRINT
            //Note : merely mentioning a variable name in a program causes the variable to be declared and initialized to zero automatically.
            //if symbol is found or if it's not found the sml instruction is the same (address may be different obvi)
            operationCode = WRITE;
            //find var, should be next token ---consider error checking this
            for( size_t i = 0; (token = strtok(NULL, d)) != NULL; i++){
              //token = strtok(NULL, d);
              type = atoi(token) ? 'C' : 'V';  //allow for printing constants cause why not
              printf("token being printed is %s, type is thought to be %c, requires further investigation\n", token, type);
              z = searchTable(table, token, type);
              if ( z == -1){    //symbol not found
                operand = dataCounter--;
                z = searchTable(table, "last", 'u');    //find last empty index
                table[z].symbol = *token;               //token is char
                table[z].symbol = atoi(token) ? atoi(token) : *token;         //if token is int, or if token is char
                table[z].type = type;                    // 'V' for variable                table[z].location = operand;
                table[z].location = operand;
              }else {                                   //symbol was foud
                operand = table[z].location;
              }
              //write this sml instruction to file and increment the instructionCounter
              //convert operationCode + operand into one #
              instruction = MEMSIZE * operationCode;
              instruction += operand;
              fprintf(output, "%d\n", instruction);
              instructionCounter++;
            }
            break;
          case 4 :
            //GOTO
            operationCode = BRANCH;     //BRANCH == 40
            token = strtok(NULL, d);
            printf("token of the goto line is : %s\n", token);
            z = searchTable(table, token, 'L');
            if (z == -1){                                 //not found in table
              operand = 0;
              flags[instructionCounter] = atoi(token);
              break;  //skip to next token
            }else {                                       //found in table
              operand = table[z].location;
            }
            //write this sml instruction to file and increment the instructionCounter
            //convert operationCode + operand into one #
            instruction = MEMSIZE * operationCode;
            instruction += operand;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;
            break;
          case 5 :
            //IF ... GOTO
            //syntax rules? if expression goto line #
            //what can an expression consist of ?, currently it will consist of
            //<one operand> <eqaulity operator> <one operand>
            //<one operand> : int or single letter var
            //expression EQUALITY OPERATOR expression , is presumably acceptable
            //char *eqOp[] = {"<", ">", "<=", ">=", "==", "!="};   //allowed operators (<, >, <=, >=, ==, !=)
            //if goto vars
            //char *op1;
            //char *op2;
            //char *branchLine;

            token = strtok(NULL, d);  //find first operand
            //searchTable for op1
            printf("%s is token will be op1\n", token);
            printf("searchTable(table, token, 'V')] = %d\n", searchTable(table, token, 'V'));
            printf("table[searchTable(table, token, 'V')].location = %d\n", table[searchTable(table, token, 'V')].location);
            if ( atoi( token ) ){
                op1 = searchTable(table, token, 'C');
                if(op1 == -1 ){
                  op1 = searchTable(table, "last", 'u');
                  table[op1].location = dataCounter;
                  table[op1].symbol = atoi(token);
                  table[op1].type = 'C';
                  op1 = dataCounter;    //op1 needs to be the memory address and not the index in the symbol table
                  dataCounter--;
                }else{
                  op1 = table[op1].location;
                }
            }else{
                op1 = searchTable(table, token, 'V');
                printf("this is op1 : %d\n", op1);
                if(op1 == -1 ){
                  op1 = searchTable(table, "last", 'u');
                  printf("this is op1 : %d\n", op1);
                  table[op1].location = dataCounter;
                  table[op1].symbol = *token;
                  table[op1].type = 'V';
                  op1 = dataCounter;    //op1 needs to be the memory address and not the index in the symbol table
                  printf("this is op1 : %d\n", op1);
                  printf("dataCounter = %d\n", dataCounter);
                  dataCounter--;
                }else{
                  op1 = table[op1].location;
                }
            }

            printf("this is op1 : %d\n", op1);

            token = strtok(NULL, d);  //find eqaulity operator
            //printf("%s is token is eq operator\n", token);
            for(z = 0; z < 6; z++){
              if (! strcmp(eqOp[z], token) ){   //found the correct operator
                printf("token is %s, eqOp[z] is %s, they're equal. z is %d\n", token, eqOp[z], z);
                break;
              }
            }

            token = strtok(NULL, d);  //find op2
            printf("%s is token will be op2\n", token);
            //searchTable for op2
            if ( atoi( token ) ){
                op2 = searchTable(table, token, 'C');
                if(op2 == -1 ){
                  op2 = searchTable(table, "last", 'u');
                  table[op2].location = dataCounter;
                  table[op2].symbol = atoi(token);
                  table[op2].type = 'C';
                  op2 = dataCounter;    //op1 needs to be the memory address and not the index in the symbol table
                  dataCounter--;
                }else{
                  op2 = table[op2].location;
                }
            }else{
                op2 = searchTable(table, token, 'V');
                printf("this is op2 : %d\n", op2);
                if(op2 == -1 ){
                  op2 = searchTable(table, "last", 'u');
                  printf("this is op2 : %d\n", op2);
                  table[op2].location = dataCounter;
                  table[op2].symbol = *token;
                  table[op2].type = 'V';
                  op2 = dataCounter;    //op1 needs to be the memory address and not the index in the symbol table
                  printf("this is op2 : %d\n", op2);
                  printf("dataCounter = %d\n", dataCounter);
                  dataCounter--;
                }else{
                  op2 = table[op2].location;
                }
            }
            printf("this is op2 : %d\n", op2);

            token = strtok(NULL, d);  //find goto     //check later
            token = strtok(NULL, d);  //find line
            //strcpy(branchLine, token);

            if( (branchLine = searchTable(table, token, 'L')) == -1 ){
              //flag it later at the correct instructionCounter
              //flags[instructionCounter] = atoi(token);
              branchLine = atoi(token);
              needFlag = 1;//can't be fucking up my instructions with wrong brancline codes
            }else{
              //if found give the memory address to branchLine
              branchLine = table[branchLine].location;
            }
            //search branchLine
            printf("this is branchLine : %d, needFlag = %d\n", branchLine, needFlag);
            switch (z) {
              case  0:
                //<
                //load op1
                operationCode = 20;     //LOAD = 20
                instruction = MEMSIZE * operationCode;
                instruction += op1;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //subtract op2
                operationCode = 31;     //SUBTRACT = 31
                instruction = MEMSIZE * operationCode;
                instruction += op2;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branchneg to the line after goto
                operationCode = 41;     //BRANCHNEG = 41
                instruction = MEMSIZE * operationCode;
                //needFlag ? flags[instructionCounter] = branchLine : instruction += branchLine;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                //instruction += branchLine;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                break;
              case 1:
                //>
                //load op2
                operationCode = 20;     //LOAD = 20
                instruction = MEMSIZE * operationCode;
                instruction += op2;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //subtract op1
                operationCode = 31;     //SUBTRACT = 31
                instruction = MEMSIZE * operationCode;
                instruction += op1;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branchneg to the line after goto
                operationCode = 41;     //BRANCHNEG = 41
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                break;
              case 2:
                // <=
                //load op1
                operationCode = 20;     //LOAD = 20
                instruction = MEMSIZE * operationCode;
                instruction += op1;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                instruction = 0;

                //subtract op2
                operationCode = 31;     //SUBTRACT = 31
                instruction = MEMSIZE * operationCode;
                instruction += op2;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branchneg to the line after goto
                operationCode = 41;     //BRANCHNEG = 41
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branch zero to the line after goto
                operationCode = 42;     //BRANCHZERO = 42
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;
                break;
              case 3:
                //>=
                //load op2
                operationCode = 20;     //LOAD = 20
                instruction = MEMSIZE * operationCode;
                instruction += op2;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //subtract op1
                operationCode = 31;     //SUBTRACT = 31
                instruction = MEMSIZE * operationCode;
                instruction += op1;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branchneg to the line after goto
                operationCode = 41;     //BRANCHNEG = 41
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branch zero to the line after goto
                operationCode = 42;     //BRANCHZERO  = 42;
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                break;
              case 4:
                //==
                //load op1
                operationCode = 20;     //LOAD = 20
                instruction = MEMSIZE * operationCode;
                instruction += op1;
                printf("instruction is : %d\n", instruction);
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //subtract op2
                operationCode = 31;     //SUBTRACT = 31
                instruction = MEMSIZE * operationCode;
                instruction += op2;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branch zero to the line after goto
                operationCode = 42;     //BRANCHZERO  = 42;
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                  puts("noflag needed bby");
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                break;
              case 5:
                //!=          //more difficult, just do the code for == to branch away from if the condition is true,
                //load op1
                //subtract op2
                //branchZero to the line after branch , flag this
                //load op1
                operationCode = 20;     //LOAD = 20
                instruction = MEMSIZE * operationCode;
                instruction += op1;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //subtract op2
                operationCode = 31;     //SUBTRACT = 31
                instruction = MEMSIZE * operationCode;
                instruction += op2;
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branch zero to the line after the next branch statement
                operationCode = 42;     //BRANCHZERO  = 42;
                instruction = MEMSIZE * operationCode;
                instruction += instructionCounter + 2;   //does this work ?? //currently don't know where this'll end up at
                //no flag neded now?? //flags[instructionCounter] = atoi(branchLine);    //so flag it
                fprintf(output, "%d\n", instruction);
                instructionCounter++;

                //branch to the line after goto
                operationCode = 40;     //BRANCH = 40
                instruction = MEMSIZE * operationCode;
                if(needFlag){
                  flags[instructionCounter] = branchLine;
                }else{
                  instruction += branchLine;
                }
                fprintf(output, "%d\n", instruction);
                instructionCounter++;
                break;
              default :
                puts("Error is Equality operator switch");
            }
            z = 0;
            break;
          case 6 :
            //END       //End == 43
            operationCode = 43;
            //write this sml instruction to file and increment the instructionCounter
            //convert operationCode + operand into one #
            instruction = MEMSIZE * operationCode;
            //no operand assignment necessary, always = 000
            instruction += operand;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;
            break;
          case 7 :
            //GOSUB
            //10 gosub <letter>
            //tok over to the letter
            token = strtok(NULL, d);
            //first set up A*, A* is used to store the correct branch instruction that will later be used in return
            //    ie 10 gosub a : A* would be branch 11
              //Instrunction 1: load 40000; 2 : add instructionCounter + 3; 3 : store in A*
            //load 40000 + instructionCounter + 3
            // = 40000 + instructionCounter + 3;   //check the three later

            //transform int into string
            printf("in gosub\n");
            printf("instructionCounter = %d\n", instructionCounter);
            toString((40000+instructionCounter+2), string);
            puts("ran toString");
            printf("string is : %s\n", string);
            z = searchTable(table, string, 'C');
            puts("ran searchTable");
            if (z == -1 ){
              z = searchTable(table, string, 'C');
              table[z].symbol = atoi(string);
              table[z].type = 'C';
              table[z].location = dataCounter;
              dataCounter--;
            }else{

            }
            operationCode = 20;     //LOAD = 21
            instruction = MEMSIZE * operationCode;
            operand = table[z].location;
            instruction += operand;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;
            puts("ran first instruction");
            /*
            //add instructionCounter + 3
            operationCode = 30;     //ADD = 30
            instruction = MEMSIZE * operationCode;
            operand +=
            */

            //Store this instruction in A*
            z = searchTable(table, "a", 'R');
            puts("ran searchTable");
            if (z == - 1){
              z = searchTable(table, "last", 'u');
              table[z].symbol = 'a';
              table[z].location = dataCounter;
              table[z].type = 'R';
              dataCounter--;
              operand = table[z].location;
            }else {
              operand = table[z].location;
            }
            operationCode = 21;     //STORE = 21
            instruction = MEMSIZE * operationCode;
            instruction += operand;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;
            puts("ran second instruction");

            //second setup the branch statement that sends simpletron execution over to where the function's instructioncode is
                //<letter> will be stored in table under the type 'R'
            //Instruction 1: branch to <letter> 'F''s location
            z = searchTable(table, token, 'F');
            if (z == -1){
              /*z = searchTable(table, "first", 'u');
              table[z].symbol = *token;
              //table[z].location = UNKNON;
              table[z].*/
              //subFlag it
              subFlags[instructionCounter] = *token;
              operand = 0;
            }else{
              operand = table[z].location;
            }
            operationCode = 40;     //BRANCH = 40
            instruction = MEMSIZE * operationCode;
            instruction += operand;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;
            puts("exiting gosub");

            break;
          case 8 :
            //SUB
            //90 sub a		(subroutine definition)
		          //91 let x = y + 1
	 	           //92 print x
		           //93 return
            // 90 and a are both line #'s
            //run the next lines through the firstPass like normal,consider using a sub sentinel if you don't want consequctive line numbers to be required in here
            //sub = 1;
            token = strtok(NULL, d);
            //put a into symbol table     //as in now, set the memory address to be a load operation of this instruction #, similar to how constants are set

            z = searchTable(table, token, 'F');
            if (z == - 1){  //function not in symbol key
                //z = searchTable(table, "last", 'u');
                z = searchTable(table, "first", 'u');
                table[z].location = instructionCounter;
                table[z].symbol = *token;
                table[z].type = 'F';
            }else {         //function in symbol key
              //do nothing
            }

            break;
          case 9 :
            //RETURN
            //turn off sub sentinel
            //sub = 0;

            //load a*
            z = searchTable(table, "a", 'R');
            if (z == -1){
              //error, a* must be in the symbol table by now
              puts("error in subroutine, couldn't find the line that called me");
              puts("perhaps a subroutine was written but not used in this simple file, unfortunately such behavior isn't allowed currently");
            }else{
              operand = table[z].location;
            }
            operationCode = 20;     //LOAD = 20
            instruction = MEMSIZE * operationCode;
            instruction += operand;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;

            //store a* in the next instruction
            operationCode = 21;     //STORE = 21
            instruction = MEMSIZE * operationCode;
            instruction += instructionCounter + 1;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;

            //blank branch instruction that will be overwritten at runtime
            operationCode = 40;     //BRANCH = 40
            instruction = MEMSIZE * operationCode;
            instruction += 0;
            fprintf(output, "%d\n", instruction);
            instructionCounter++;

            break;
          default :
            puts("Error in cNum switch");
        }
      }else if (i > 2){
        //EXPRESSION    //this should find an expression or it's a syntax error
        //remember to handle if..goto differently
      }
      //firstPass();
      i++;
    } while((token = strtok( NULL, d)) != NULL);
    /*end of the line do while loop */
    printf("run %d of while\n", i);
  }
}

void printTable(TableEntry table[]){
  int z = 0;
  for( z = 0; z < TABLESIZE; z++){
    printf("Table Entry %d: symbol = %d;\t location = %d;\t type = %c\n", z, table[z].symbol, table[z].location, table[z].type );
  }
}

//return index of entry or -1
//optimized to only look in the area where the token should be so if entries get into the wrong areas it wont see them
// when searching for the first empty index call searchTable(table, "first", 'u')
// when searching for the last empty index call searchTable(table, "last", 'u')
int searchTable(TableEntry table[], char *token, char type){
  int z = 0;
  switch (type) {
    case 'L':
      for ( z = 0; z < TABLESIZE; z++ ){
        if (table[z].symbol == atoi(token) && table[z].type == 'L'){
          return z;
        }else if (table[z].type == 'u'){    //unassigned you've searched too far
          return -1;
        }else{
          //keep looking
        }
      }
      return -1;        //couldn't find it
    case 'V':
      //allow for upperCase and lowerCase letters
      *token = toupper(*token);
      for ( z = TABLESIZE - 1; z >= 0; z-- ){
        if ( ( (table[z].symbol == *token) || (table[z].symbol == tolower(*token)) ) && table[z].type == 'V'){    //if z.symbol matches upper or lower) and is type v
          return z;
        }else if (table[z].type == 'u'){    //unassigned you've searched too far
          return -1;
        }else{
          //keep looking
        }
      }
      return -1;        //couldn't find it
    case 'C':
      for ( z = TABLESIZE - 1; z >= 0; z-- ){
        if (table[z].symbol == atoi(token) && table[z].type == 'C'){
          return z;
        }else if (table[z].type == 'u'){    //unassigned you've searched too far
          return -1;
        }else{
          //keep looking
        }
      }
      return -1;        //couldn't find it
    case 'u':     //searching for the first empty spot
      if(!strcmp(token, "first")){
        for ( z = 0 ; z < TABLESIZE; z++ ){
          if (table[z].type == 'u'){
            return z;
          }else{
            //keep looking
          }
        }
        return -1;    //huston we have a problem
      }else if(!strcmp(token, "last")){
        for ( z = TABLESIZE - 1; z >= 0; z-- ){
          if (table[z].type == 'u'){
            return z;
          }
        }
        return -1;    //huston we have a problem
      }
      //if not return by now then print default error ?
    case 'R':
      //allow for upperCase and lowerCase letters
      *token = toupper(*token);
      for ( z = TABLESIZE - 1; z >= 0; z-- ){
        if ( ( (table[z].symbol == *token) || (table[z].symbol == tolower(*token)) ) && table[z].type == 'R'){    //if z.symbol matches upper or lower) and is type R
          return z;
        }else if (table[z].type == 'u'){    //unassigned you've searched too far
          return -1;
        }else{
          //keep looking
        }
      }
      return -1;        //couldn't find it
      break;
    case 'F':
      //allow for upperCase and lowerCase letters
      *token = toupper(*token);
      for ( z = TABLESIZE - 1; z >= 0; z-- ){
        if ( ( (table[z].symbol == *token) || (table[z].symbol == tolower(*token)) ) && table[z].type == 'F'){    //if z.symbol matches upper or lower) and is type R
          return z;
        }else if (table[z].type == 'u'){    //unassigned you've searched too far
          return -1;
        }else{
          //keep looking
        }
      }
      return -1;        //couldn't find it
      break;
    default :
      printf("Error searching the table for type %c\n", type);

  }
  return -1;
}


void secondPass(FILE *filePtr, TableEntry *table, FILE *output, char *outName){
  //go through flags && link things
  int x, z, index, operand;
  x = z = index = operand = 0;
  //char temp[10];
  //int c[10] = {0};
  int c = 0;
  char type = 'u';
  FILE *temp;
  for(size_t i = 0; i < MEMSIZE; i++){
      if(flags[i] != -1){
        //found flag[i] with line# of incomplete line
        //switch flag[i] the int to a string
        //set type
        //type
        printf("found flag at index %zu, flags[%zu] = %d\n", i, i, flags[i]);
        for ( z = 0; z < TABLESIZE; z++ ){                                      //search the table manually for the line number
          if (table[z].symbol == flags[i] && table[z].type == 'L'){
            //index of line # found at z
            break;  //???does this work
          }else if (table[z].type == 'u'){    //unassigned you've searched too far
            printf("Unable to find line # %d so instruction %zu is incomplete\n", flags[i], i);
            puts("error in second pass");
            z = -1;
          }else{
            //keep looking
          }
        }
        //
        if (z != -1){
            //change instruction to have the correct memory address
            //reset file pointer
            //puts("secondPass found z != -1");
            //fprintf(output, "%d\n", 99999 );
            rewind(output);
            //fprintf(output, "%d\n", 99999 );

            //read all the current contents into a temp file
            if ( (temp = fopen("temp.txt", "w")) == NULL) {
              puts("error in making temp file");
            }
            //token = strtok( temp, " ");
            //fprintf()
            /*while ( (c = fgetc(output)) != feof ){
              fputc(c, temp);
            }*/
            instructionCounter--; //undo last increment
            //create a clone of output in temp

            /*for (size_t q = 0; q < instructionCounter; q ++){
              fscanf(output, "%d\n", &c);
              printf("secondPass sees c as %d\n", c);
              //printf("secondPass sees *c as %d\n", *c);
              //fprintf(temp, "%d\n", c);
            }*/
            fscanf(output, "%d\n", &c);
            printf("c is now %d\n", c);
            while(!feof(output)){
              fprintf(temp, "%d\n", c);
              printf("c is now %d\n", c);
              fscanf(output, "%d\n", &c);
            }
            fprintf(temp, "%d\n", c);
            printf("c is now %d\n", c);
            //setup temp for write priveledges
            fclose(temp);
            if ( (temp = fopen("temp.txt", "r+")) == NULL) {
              puts("error in making temp file");
            }
            //close out ouput and open a new one
            fclose(output);
            if ( (output = fopen( outName, "w")) == NULL) {
              puts("error in rewriting the output file");
            }

            //fputc(temp, fgetc(output));
            //read the temp file into the old file while making the update
            // z is symbol table index , x is line indicator / instruction indicator, i is instruction number of the flagged machine code
            x = 0;
            fscanf(temp, "%d\n", &c);
            printf("c is now %d\n", c);
            while(!feof(temp)){
              printf(" second pass sees i as %zu, and x as %d\n", i, x);
              if ( i == x ){        //change value
                operand = table[z].location;
                c += operand;
              }
              fprintf(output, "%d\n", c);
              printf("c is now %d\n", c);
              fscanf(temp, "%d\n", &c);
              x++;
            }
            fprintf(output, "%d\n", c);
            printf("c is now %d\n", c);
            fprintf(output, "%d\n", -999999);   //this is simpletrons EOF signal value
            //delete the temp file
            fclose(temp);


            //string tok through to the correct instruction
            //fseek(output, i * 5, SEEK_SET );
            //overwrite it ?
            //fprintf(output, "%d\n", 99999 );
        }else{
          //there was an error
        }
      }
  }
  for(size_t i = 0; i < MEMSIZE; i++){          //do the same thing for the subFlags array
      if(subFlags[i] != -1){
        //search the table manually for the line number
        for ( z = 0; z < TABLESIZE; z++ ){
          if (table[z].symbol == subFlags[i] && table[z].type == 'F'){
            //index of line # found at z
            break;
          }else if (table[z].type == 'u'){    //unassigned you've searched too far
            z = -1;
          }else{
            //keep looking
          }
        }
        //
        if (z != -1){       //change instruction to have the correct memory address
            //reset file pointer
            rewind(output);

            //read all the current contents into a temp file
            if ( (temp = fopen("temp.txt", "w")) == NULL) {
              puts("error in making temp file");
            }
            instructionCounter--; //undo last increment

            //create a clone of output in temp
            fscanf(output, "%d\n", &c);
            printf("c is now %d\n", c);
            while(!feof(output)){
              fprintf(temp, "%d\n", c);
              printf("c is now %d\n", c);
              fscanf(output, "%d\n", &c);
            }
            fprintf(temp, "%d\n", c);
            printf("c is now %d\n", c);

            //setup temp for write priveledges
            fclose(temp);
            if ( (temp = fopen("temp.txt", "r+")) == NULL) {
              puts("error in making temp file");
            }

            //close out ouput and open a new one
            fclose(output);
            if ( (output = fopen( outName, "w")) == NULL) {
              puts("error in rewriting the output file");
            }

            //read the temp file into the old file while making the update
            // z is symbol table index , x is line indicator / instruction indicator, i is instruction number of the flagged machine code
            x = 0;
            fscanf(temp, "%d\n", &c);
            printf("c is now %d\n", c);
            while(!feof(temp)){
              printf(" second pass sees i as %zu, and x as %d\n", i, x);
              if ( i == x ){        //change value
                operand = table[z].location;
                c += operand;
              }
              fprintf(output, "%d\n", c);
              printf("c is now %d\n", c);
              fscanf(temp, "%d\n", &c);
              x++;
            }
            fprintf(output, "%d\n", c);
            printf("c is now %d\n", c);
            fprintf(output, "%d\n", -999999);   //this is simpletrons EOF signal value
            //delete the temp file
            fclose(temp);
        }else{
          //there was an error
        }
      }
  }
}

TableEntry* getTable(void){
  return tablePtr;
}

FILE* getOutput(void){
  return output;
}

//insert all the constant values into simpletron's memory at the correct locations
void setConstants(void){
  int z;
  z = 0;
  printTable(tablePtr);
  for (z = TABLESIZE - 1; z >= 0; z-- ){
    if (tablePtr[z].type == 'C'){
      printf("memory[table[z].location] is %d\n", memory[tablePtr[z].location]);
      memory[tablePtr[z].location] = tablePtr[z].symbol;
      printf("memory[table[z].location] is %d, table[z].location is %d, z is %d\n", memory[tablePtr[z].location], tablePtr[z].location, z);
    }else if (tablePtr[z].type == 'u'){    //unassigned you've searched too far
      //break;
    }else{
      //keep looking
    }
  }
}
//optimize code by eliminating redundant code
void optimize(char *outFile, TableEntry *table){
  //find code that stores a value from the accumulator into a temp address then immediately loads that temp address into the accumulator
  int operand1, operand2, operationCode1, operationCode2, operationCode3, instruction1, instruction2, instruction3, instructionCounter;
  instructionCounter = 0;
  FILE *temp, *output;
  //create a temp file
  if ( (temp = fopen("temp.txt", "w")) == NULL ){
    printf("Error in opening temp file in optimize\n");
  }
  if ( (output = fopen(outFile, "r")) == NULL ){
    printf("Error in opening outFile in optimize\n");
  }
  //read output into temp
  fscanf(output, "%d\n", &instruction1);
  while(!feof(output)){
    //printf("Stuff in while loop of optimize\n");
    operationCode1 = instruction1 / 1000;
    if (operationCode1 == 21){              //store operation (store = 21)
      fscanf(output, "%d\n", &instruction2);
      operationCode2 = instruction2 / 1000;
      if (operationCode2 == 20){            //if next operation is load (load = 20)
        operand1 = instruction1 - 21000;
        operand2 = instruction2 - 20000;
        //printf("op1 op2 : %d %d\n", operand1, operand2);
        if(operand1 == operand2){           //if they share a memory address then ignore them
          fscanf(output, "%d\n", &instruction3);
          //printf("instruction3 is %d\n", instruction3);
          operationCode3 = instruction3 / 1000;
          //printf("operationCode3 is %d\n", operationCode3);
          if (operationCode3 == 21){
            //ignore instructions 1 & 2
            fprintf(temp, "%d\n", instruction3);
            //shift the line numbers appropriately down
            //for line numbers > instructionCounter .location -= 2
            //printTable(table);
            for(size_t i = 0; i < TABLESIZE; i++){
              if (table[i].type == 'L' && table[i].location > instructionCounter){
                table[i].location -= 2;
              }else if (table[i].type == 'u'){        //type unassigned you've searched too far
                i = TABLESIZE;  //break;
              }
            }
            //printTable(table);
            instructionCounter++;

          }else{
            fprintf(temp, "%d\n", instruction1);
            fprintf(temp, "%d\n", instruction2);
            fprintf(temp, "%d\n", instruction3);
            instructionCounter += 3;
          }
        }else{
          fprintf(temp, "%d\n", instruction1);
          fprintf(temp, "%d\n", instruction2);
          instructionCounter += 2;
        }
      }else{
        fprintf(temp, "%d\n", instruction1);
        fprintf(temp, "%d\n", instruction2);
        instructionCounter += 2;
      }
    }else{
      fprintf(temp, "%d\n", instruction1);
      instructionCounter++;
    }
    fscanf(output, "%d\n", &instruction1);
  }
  fprintf(temp, "%d\n", instruction1);
  //search output file for store commands, note operand
    //if instruction is store
    //set next instruction
      //if next is load && operand == operand
      //then don't fprintf
  //if next instruction is a load command of the same operand then ignore last instruction and this one

  //read temp into output
  fclose(output);
  fclose(temp);
  if ( (temp = fopen("temp.txt", "r")) == NULL ){
    printf("Error in opening temp file in optimize\n");
  }
  if ( (output = fopen(outFile, "w")) == NULL ){
    printf("Error in opening outFile in optimize\n");
  }

  fscanf(temp, "%d\n", &instruction1);
  while(!feof(temp)){
    fprintf(output, "%d\n", instruction1);
    fscanf(temp, "%d\n", &instruction1);
  }
  fprintf(output, "%d\n", instruction1);

  //close all files
  fclose(output);
  fclose(temp);

}

void toString(int num, char *string){
  int i, rem, len, n;
  //char *string[100];
  n = num;
  len = 0;
  while(n != 0){
    len++;
    n /= 10;
  }
  for(i = 0; i < len; i++){
    rem = num %10;
    num /= 10;
    string[len - (i + 1)] = rem + '0';
  }
  string[len] = '\0';
  //return string;
}
