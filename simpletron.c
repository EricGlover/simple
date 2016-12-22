//Simpletron Chp Pointers ex 27 & 28
//Eric Glover
//11/23/16
//TM T - Swift

//Operation Status : simulator is fully working, prog 1 works, prog 2 doesn't, prog 3 unwritten
    //modifications success : now has option of writting to and from files //rather handy
  //Currently :

//limitations : no saves...sad eric

//Improvements : add functionality ? Maybe skip this and do the compiler exercise
    //Improve the readibility of mem dumps DONE
    //considering dynamically allocating memory (maybe via linked lists?)
    //ADD a SAVE PROGRAM option , or a program editing capacity or something....
      //writing data to a programs.txt file / having the options of reading saved programs from programs.txt would be pretty dope
      //renaming output program to simpletron.out or something would also be dope
    //do the mods on pg 33: handling strings, handing floats, using hex



//example program 1 [working, sentinel value = 0 to continue ; anything but 0 to exit and get the sum]
//use a sentinel-controlled loop to read positive integers and compute and print their sum
/*
Location
00          +10008     Read A
01          +30008     Add A
02          +21010     Store C
03          +10009     Read B
04          +20009     Load B
05          +42011     Branch zero to later
06          +11010     Write C
07          +43000     Halt
08          +00000     A
09          +00000     B
10          +00000     C
11          +20010     Load C
12          +40000     Branch 00
13          -99999     HALT code for reading in the file


//example program 2: use a counter controlled loop to read seven numbers, some positive some negative, and compute and print their average

Location    number    Instructions
00          +1019     Read C (counter)
01          +2019     Load C
02          +2120     Store N
03          +1017     Read A
04          +2018     Load B (total)
05          +3017     Add A
06          +2118     Store B (total)
07          +2119     Load C
08          +3121     Subtract D (D == 1)
09          +4212     Branch Zero to Compute and Print
10          +2119     Store C
11          +4003     Branch Beginning To Read A
12          +2018     Load B
13          +3220     Divide N
14          +2118     Store B
15          +1118     Write B
16          +4300     Halt
17          +0000     A (input)
18          +0000     B (total)
19          +0000     C (count)
20          +0000     N (total # of args)
21          +0001     D (increment value = 1)

----doesn't seem to terminate ......... works fine for count == 1 hrmmmmm....

//example program 3: read a series of numbers and determine and print the largest. The first number indicates how many numbers should be processed

Location    number    Instructions
00          +10A      Read A
01          +10



------Simpletron Design Notes -------------
Running simpletron
use a one-dimensional array size 100 for the memory
each value in the array will be a word of the program to be executed
var accumulator will represent the register
var operationCode will indicate the current operation being performed
var operand will indicate the memory location on which the current instruction operates
don't execute instructions directly from memory instead transfer the next instruction to be performed from memory to
  var instructionRegister then take the left two digits and place them in var operationCode and the right two
    digits and place them in var operand
    //if mem size is 1000 then operation code is two digits but the operand needs to be 3 so every word has to be 5 digits now
    //hence words are now 5 digits long

init the vars as follows
  accumulator         +0000
  instructionCounter     00
  instructionRegister +0000
  operationCode          00
  operand                00

the instruction execution cycle
  fetch : instructionRegister = memory[instructionCounter];
  extract : operationCode = instructionRegister / 100;
            operand = instructionRegister % 100;

  Operate : use a switch to determine which of the twelve things we should do
    read : scanf("%d", &memory[operand]);
    load : accumulator = memory[operand];
    add :  accumulator += memory[operand];
    branches :
    halt : printf("%s", "*** Simpletron execution terminated ***"); then print name and contents of each register and contents of memory [computer dump]

  increment instructionCounter
  repeat

Error checking
  check in the loading phase
    memory range must be -9999 to +9999
  Checking in the execution phase
    check for divide by 0
    check for invalid operation codes
    check for accumulator overflows (adding numbers equaling above +9999 or below -9999)
    execution phase errors print fatal error messages "blah happened" "Simpletron abnormally terminated" then full dump

    functions:
    main : contains all memory and registers
    load: load a word from a specific location into the accumulator
    execute :
    dump :

    ---------------------------------------------END NOTES ------------------------

    */


#include <stdio.h>
#include "simpletron.h"
#include "simple.h"

/*Input / Output operations */
#define READ 10       //read a word from terminal into a specific location in memory
#define WRITE 11      //write a word from a memory location to the terminal

/*Load/Store operations*/
#define LOAD 20       //load a word from a memory location into the accumulator
#define STORE 21      //store a word from the accumulator into a memory location

/*Arithemtic operations*/
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define MODULO 34     //remainder
#define EXPONENT 35

/*transfer-of-control operations */
#define BRANCH 40     //branch to a memory location
#define BRANCHNEG 41  //branch to a memory location if the accumulator is negative
#define BRANCHZERO 42 //branch to a memory location if the accumulator is 0
#define HALT 43       //end the program

#define MEMSIZE 1000  //memory address size of simpletron

int memory[MEMSIZE];


int simpletron_main(int argc, char** argv) {
  int instructionCounter, operationCode, operand;
  signed int accumulator, instructionRegister;
  accumulator = instructionRegister = +00000;
  operationCode = 00;
  instructionCounter = operand = 000;

  int *instructionCounterPtr, *operationCodePtr, *operandPtr;
  signed int *accumulatorPtr, *instructionRegisterPtr;
  instructionCounterPtr = &instructionCounter;
  operationCodePtr = &operationCode;
  operandPtr = &operand;
  accumulatorPtr = &accumulator;
  instructionRegisterPtr = &instructionRegister;
  /*
  accumulator         +0000
  instructionCounter     00
  instructionRegister +0000
  operationCode          00
  operand                00
  */
  //int memory[MEMSIZE] = {0};
  for ( size_t i = 0; i < MEMSIZE; i++) {
    memory[i] = 0;
  }
  //set my memory with the needed constants
  setConstants();

  //print welcome banner
  printf("%s","*** Welcome to Simpletron! ***\n*** Please enter your program one instruction ***\n*** (or data word) at a time. I will type the ***\n");
  printf("%s","*** location number and a question mark (?).  ***\n*** You then type the word for that location. ***\n*** Type the sentinel -9999 to stop entering  ***\n");
  printf("%s", "*** your program ***\n");

  //load program : from file if specified otherwise use manaul entry
  if (argc > 1){
    if(load_file(argv[1], memory) != 0){
      printf("Error loading your file ending program now.\n");
      return 0;
    }else{
      //nothing
    }
  }else{
    load(memory);
  }


  //execute program
  execution( accumulatorPtr, instructionCounterPtr, instructionRegisterPtr, operationCodePtr, operandPtr, memory);

  //terminate and dump : from file if specified otherwise just to the screen
  if(argc > 2) {
      file_dump( argv[2], accumulatorPtr, instructionCounterPtr, instructionRegisterPtr, operationCodePtr, operandPtr, memory);
  }
  dump( accumulatorPtr, instructionCounterPtr, instructionRegisterPtr, operationCodePtr, operandPtr, memory);


  return 0;

}

//sentinel value for load complete is -999,999 "-999999"
void load (int *memory ){
  //load program increment instructionCounter then reset later
  //this flow control is soooo shity
  int word, instructionCounter;
  word = instructionCounter = 0;

  while ( word != -999999 ) {
    //user inputs a instruction code into word
    printf("%02d ? ", instructionCounter);
    scanf("%d", &word);

    //error check word
    while (word < 0 || word > 99999){
      if (word == -999999) {
        //print load complete
        printf("%s", "*** Program loading completed ***\n*** Program execution begins ***\n");
        break; //out of the error checking while loop
      }else{
        printf("%s", "Input needs to be between 0 - 99999; or enter -999999 to finish loading\n");
      }
      printf("%02d ? ", instructionCounter);
      scanf("%d", &word);
    }
    //dont run when given sentinel value
    if (word != -999999 ) {
      memory[instructionCounter] = word;
      instructionCounter++;
    }
  }
  //unneccessary code now? since this instructionCounter is local to load (?)
  instructionCounter = 0;
}

//return 1 on file error 0 for completed
//file format must be all memory values with -999999 as the EOF value
  /*example
+10008
+30008
+21010
+10009
+20009
+42011
+11010
+43000
+00000
+00000
+00000
+20010
+40000
-999999
*/
int load_file(char *file, int *memory){
  int word, instructionCounter;
  word = instructionCounter = 0;
  FILE *filePtr;
  if((filePtr = fopen(file, "r")) != NULL){
    fscanf(filePtr,"%d", &word);
    printf("I AM THE WORD = %d\n", word);
    while(word != 43000){
      memory[instructionCounter] = word;
      instructionCounter++;
      fscanf(filePtr,"%d", &word);
      printf("I AM THE WORD = %d\n", word);
    }
    memory[instructionCounter] = word;
    instructionCounter++;
  }else{
    return 1;
  }
  fclose(filePtr);
  return 0;
}

//execute
/*the instruction execution cycle
  fetch : instructionRegister = memory[instructionCounter];
  extract : operationCode = instructionRegister / 100;
            operand = instructionRegister % 100;

  Operate : use a switch to determine which of the twelve things we should do
    read : scanf("%d", &memory[operand]);
    load : accumulator = memory[operand];
    add :  accumulator += memory[operand];
    branches :
    halt : printf("%s", "*** Simpletron execution terminated ***"); then print name and contents of each register and contents of memory [computer dump]

  increment instructionCounter
  repeat*/
void execution(int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *memory){
  int halt, forceQuit;
  halt = forceQuit = 0;

  while (halt == 0){

    //fetch
    /*printf("instructionCounter = %d\n", *instructionCounter);*/
    *instructionRegister = memory[*instructionCounter];

    //extract
    *operationCode = *instructionRegister / 1000;
    *operand = *instructionRegister % 1000;

    /*printf("operationCode = %02d\n", *operationCode);
    printf("operand = %02d\n", *operand);*/

    //operate
    switch (*operationCode) {
      case READ:
        puts("Please enter a number ");
        scanf("%d", &memory[*operand]);
        break;
      case WRITE:
        printf("%d\n", memory[*operand]);
        break;
      case LOAD:
        puts("loading");
        *accumulator = memory[*operand];
        break;
      case STORE:
        puts("storing");
        memory[*operand] = *accumulator;
        break;
      case ADD:
        puts("adding");
        *accumulator += memory[*operand];
        break;
      case SUBTRACT:
        puts("subtracting");
        *accumulator -= memory[*operand];
        break;
      case DIVIDE:
        puts("dividing");
        if (memory[*operand] != 0){
            *accumulator /= memory[*operand];
        }else{
          printf("Execution error, currently Simpletron doesn't allow for you to divide by 0, terminating now\n");
          halt = 1;
        }
        break;
      case MULTIPLY:
        puts("multiplying");
        *accumulator *= memory[*operand];
        break;
      case MODULO:
        puts("Modulo-tizing");
        *accumulator %= memory[*operand];
        break;
      case EXPONENT:
        puts("Exponentating");
        //(don't want to deal with doubles) so I'll write my own pow *accumulator = pow(*accumulator, memory[*operand]);
        int p, initValue;
        p = 0;
        initValue = *accumulator;
        //x^0 = 1; x^3 runs X*initValue 2 times, x^7 runs X*initValue 6 times, etc..; x^-3 fuck that for now cause I'm using ints
        if(memory[*operand] == 0){
          *accumulator = 1;
        }else if (memory[*operand] < 0) {
          printf("%s","Execution error, currently Simpletron doesn't allow for taking negative exponents - terminating now\n");
        }else {
          for (p = 1; p < memory[*operand]; p++){
            *accumulator *= initValue;
          }
        }
        break;
      case BRANCH:
        puts("branching");
        *instructionCounter = *operand - 1;
        break;
      case BRANCHNEG:
        puts("branchNeg");
        if (*accumulator < 0){
          *instructionCounter = *operand - 1;
        }
        break;
      case BRANCHZERO:
        puts("BranchZero");
        if (*accumulator == 0){
          *instructionCounter = *operand - 1;
        }
        break;
      case HALT:
        halt = 1;
        printf("%s", "*** Simpletron execution terminated ***\n");
        break;
      default:
        printf("Runtime Error: the operation code (%d) in memory address %d isn't allowed.\n", *operationCode, *operand);
        printf("Would you like to terminate execution now? 1 = yes, 0 = no\n");
        scanf("%d", &forceQuit);
        if (forceQuit == 1){
          halt = 1;
          printf("%s", "*** Simpletron execution terminated ***\n");
        }
    }
    //end operation phase
    *instructionCounter += 1;

  }
  //end of while loop
}

void dump(int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *memory){
  int i, j, z;
  i = j = z = 0;
  puts("REGISTERS:");
  printf("accumulator \t\t%+06d\n", *accumulator);
  printf("instructionCounter \t%03d\n", *instructionCounter);
  printf("instructionRegister \t%+06d\n", *instructionRegister);
  printf("operationCode \t\t%03d\n", *operationCode);
  printf("operand    \t\t%03d\n", *operand);
  puts("");
  puts("MEMORY");
  //11 rows & 11 columns
  //poop out the memory in blocks of 100 with a space inbetween and headers per 100
  for(z = 0; z < 10; z++){
      printf("  ");
      for (i = 0; i < 11; i++){
        for (j = 0; j < 11; j++){
            //column headers then row headers (row head is j == 0)
            if( i == 0){
              if( j < 10){
                printf("\t%d", j);
              }
              //row headers?
            }else if(j == 0){
              printf("%3d", (i-1)*10 + (z * 100) );
              //mem blocks
            }else{
              printf("\t%+06d", memory[ ((i-1)*10)+ j-1 + (z * 100)]);
            }
        }
        puts("");
      }
      puts("");
  }
}

//mem-dumps to an ouptut file
void file_dump(char *out, int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *memory){
  int i, j, z;
  i = j = z = 0;
  FILE *filePtr;
  if((filePtr = fopen(out, "w")) != NULL){
    fputs("REGISTERS:\n", filePtr);
    fprintf(filePtr, "accumulator \t\t%+06d\n", *accumulator);
    fprintf(filePtr, "instructionCounter \t%03d\n", *instructionCounter);
    fprintf(filePtr, "instructionRegister \t%+06d\n", *instructionRegister);
    fprintf(filePtr, "operationCode \t\t%03d\n", *operationCode);
    fprintf(filePtr, "operand    \t\t%03d\n", *operand);
    fputs("\n", filePtr);
    fputs("MEMORY\n", filePtr);
    //11 rows & 11 columns
    //poop out the memory in blocks of 100 with a space inbetween and headers per 100
    for(z = 0; z < 10; z++){
        fprintf(filePtr, "  ");
        for (i = 0; i < 11; i++){
          for (j = 0; j < 11; j++){
              //column headers then row headers (row head is j == 0)
              if( i == 0){
                if( j < 10){
                  fprintf(filePtr, "\t%d", j);
                }
                //row headers?
              }else if(j == 0){
                fprintf(filePtr, "%3d", (i-1)*10 + (z * 100) );
                //mem blocks
              }else{
                fprintf(filePtr, "\t%+06d", memory[ ((i-1)*10)+ j-1 + (z * 100)]);
              }
          }
          fputs("\n", filePtr);
        }
        fputs("\n", filePtr);
    }
    fclose(filePtr);
  }else{
    puts("Dump issues, no file to dump in");
  }

}
