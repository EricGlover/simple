//simple.h

struct tableEntry {
  int symbol;
  char type; // 'C', 'L' or 'V' : constant or line number or variable
  int location; // 00 to 99 (to 999 I beleive actually)
};

//do the nickname thing
typedef struct tableEntry TableEntry;
//TableEntry table[TABLESIZE];
//TableEntry *tablePtr = NULL;        //used to pass ex12_main the reference to the symbol table
#define TABLESIZE 100


void firstPass(FILE *filePtr, TableEntry *table, FILE *output);
void secondPass(FILE *filePtr, TableEntry *table, FILE *output, char *outName);
void setConstants(void);
void optimize(char *outFile, TableEntry *table);

//table functions
void printTable(TableEntry table[]);
//int searchTable(TableEntry table[]);    //return index of entry or -1
int searchTable(TableEntry table[], char *token, char type);
TableEntry* getTable(void);
FILE* getOutput(void);

int instructionCounter, dataCounter;
//int flags[MEMSIZE];
