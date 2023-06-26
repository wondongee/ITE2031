/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000



typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
void transform(int, char[]);
int decimal(char, char, char);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]); perror("fopen");
        exit(1);
    }


    for(int i=0; i<NUMREGS; i++) {
        state.reg[i] = 0;
    }
    char bin[MAXLINELENGTH][32] = {0, };  
    state.pc = 0;
    


    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }

        transform(state.mem[state.numMemory], &bin[state.numMemory][0]);
        /*
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
        for(int i=31; i>=0; i--) {
            printf("%d", bin[state.numMemory][i]);
        }        
        printf("\n");
        */
    }

    int count = 1;
    while(1) {
        printState(&state);

        int opcode=0;
        int regA, regB, destReg, offset = 0;
        opcode = decimal(bin[state.pc][22], bin[state.pc][23], bin[state.pc][24]);
        regA = decimal(bin[state.pc][19], bin[state.pc][20], bin[state.pc][21]);
        regB = decimal(bin[state.pc][16], bin[state.pc][17], bin[state.pc][18]);
        destReg = decimal(bin[state.pc][0], bin[state.pc][1], bin[state.pc][2]);
        for(int i=0; i<=14; i++) {
            if(bin[state.pc][i] == 1) {
                offset = offset + pow(2, i);
            }
        }
        if(bin[state.pc][15] == 1) {
            offset = offset - pow(2, 15);
        }
        if(opcode == 0) {                   // add   
            state.reg[destReg] = state.reg[regA] + state.reg[regB];
            state.pc = state.pc + 1;
        } else if (opcode == 1) {           // nor
            int a = state.reg[regA];
            int b = state.reg[regB];
            int c, d;
            c = a|b;
            d = ~c;
            state.reg[destReg] = d;
            state.pc = state.pc + 1;
        } else if (opcode == 2) {           // lw
            state.reg[regB] = state.mem[state.reg[regA] + offset];
            state.pc = state.pc + 1;
        } else if (opcode == 3) {           // sw
            state.mem[state.reg[regA] + offset] = state.reg[regB];
            state.pc = state.pc + 1;
        } else if (opcode == 4) {           // beq
            if(state.reg[regA] == state.reg[regB]) {
                state.pc = state.pc + 1 + offset;
            } else {
                state.pc = state.pc + 1;
            }
        } else if (opcode == 5) {           // jalr
            state.pc = state.pc + 1;
            state.reg[regB] = state.pc;
            state.pc = state.reg[regA];
        } else if (opcode == 6) {           // halt
            state.pc = state.pc + 1;
            printf("machine halted\n");
            printf("total of %d instructions executed\n", count);
            printf("final state of machine : \n");
            printState(&state);
            break;
        } else if (opcode == 7) {           // noop
            state.pc = state.pc + 1;
        } else {
            printf("opcode error\n");
            //exit(1);
        }
        count = count + 1;
    }
    exit(0);
    return(0);
}

void printState(stateType *statePtr) {
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc); printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int decimal(char a, char b, char c) {
    return a + 2*b + 4*c;
}
/* 10진수 -> 2진수 변환 */
void transform(int decimal, char binary[]) {
    int position = 0;
    if(decimal >= 0) {
        while (1) {
            binary[position] = decimal % 2;
            decimal = decimal / 2;
            position++;
            if (decimal == 0) {
                break;
            }
        }
    } else {
        decimal = decimal * -1;
        while (1) {
            binary[position] = decimal % 2;
            decimal = decimal / 2;
            position++;
            if (decimal == 0) {
                break;
            }
        }
        for(int i=31; i>=0; i--) {
            if(binary[i]==1) binary[i]=0;
            else binary[i]=1;
        }
        
        for(int i=0; i<=31; i++) {
            if(binary[i]==1) binary[i]=0;
            else {
                binary[i]=1;
                break;
            }
        }
    }   
}