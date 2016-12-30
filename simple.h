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

void toString(int num, char *string);

int instructionCounter, dataCounter;
//int flags[MEMSIZE];

/*    PRIMER ON USING searchTable
printf("searchTable(table, '123', 'C') returns %d\n", searchTable(table, "123", 'C'));  //-1
table[0].symbol = 1;
table[0].location = 11;
table[0].type = 'C';

table[1].symbol = atoi("11");
table[1].location = 10;
table[1].type = 'L';

table[99].symbol = 'x';
table[99].location = 9;
table[99].type = 'V';

table[98].symbol = atoi("1");
table[98].location = 8;
table[98].type = 'C';


printf("searchTable(table, '1', 'L') returns %d\n", searchTable(table, "1", 'L'));    //0
printf("searchTable(table, '11', 'L') returns %d\n", searchTable(table, "11", 'L'));  //1
printf("searchTable(table, 'x', 'V') returns %d\n", searchTable(table, "x", 'V'));    //99
printf("searchTable(table, '1', 'C') returns %d\n", searchTable(table, "1", 'C'));    //98
printf("first empty index = %d\n", searchTable(table,"first", 'u') );
printf("last empty index = %d\n", searchTable(table,"last", 'u') );

printTable(table);
puts("Thus ends the searchTable Test");

*/
