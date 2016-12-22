//simpletron.h


#define READ 10
#define WRITE 11

#define LOAD 20
#define STORE 21

#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define MODULO 34
#define EXPONENT 35

#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

#define MEMSIZE 1000

int memory[MEMSIZE];


int simpletron_main(int argc, char** argv);

void load(int *memory);
int load_file(char *file, int *memory);

void execution(int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *memory);

void dump(int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *memory);
void file_dump(char *out, int *accumulator, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *memory);
