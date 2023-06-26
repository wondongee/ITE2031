/* Assembler code fragment for LC-2K */
/* To do
 * 1. Error Checking
 *      undefined labels, duplicate labels, offsetFields that don't fit in 16bits, and unrecognized opcodes
 * 2. Test Cases
 *      5 test cases for full credit (test the ability of an assembler to check for the errors)
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int negativeNum(char *);
int negativeNum2(int);
void transform(int, char[]);
void transform16(int, char[]);


int main(int argc, char *argv[]) {

    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
        arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
        exit(1); 
    }

    inFileString = argv[1];
    outFileString = argv[2];
    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    /* first pass : caclulate the address for every symbolic label */
    char str[MAXLINELENGTH][7];
    int addr = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        if(strcmp(label, "")) {
            for(int i=0; i<addr; i++) {
                if(!strcmp(label, str[i])) {
                    printf("duplicated labels!\n");
                    exit(1);
                }
            }
        }
        strcpy(str[addr], label);
        addr = addr + 1;
    }
    rewind(inFilePtr);

    /* second pass : generate a machine-language instruction (in decimal) 
     * for each line of assembly language
     */
    
    int pc = 0;
    char bin[MAXLINELENGTH][32] = {0, };

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        /* instructoin : 31-25 bit are always 0 */
        
        if(!strcmp(arg0, "0")) {
    
        } else if (!strcmp(arg0, "1")) {
            bin[pc][19] = 1;
        } else if (!strcmp(arg0, "2")) {
            bin[pc][20] = 1;
        } else if (!strcmp(arg0, "3")) {
            bin[pc][19] = 1;
            bin[pc][20] = 1;
        } else if (!strcmp(arg0, "4")) {
            bin[pc][21] = 1;
        } else if (!strcmp(arg0, "5")) {
            bin[pc][19] = 1;
            bin[pc][21] = 1;
        } else if (!strcmp(arg0, "6")) {
            bin[pc][20] = 1;
            bin[pc][21] = 1;
        } else if (!strcmp(arg0, "7")) {
            bin[pc][19] = 1;
            bin[pc][20] = 1;
            bin[pc][21] = 1;
        } else {

        }

        if(!strcmp(arg1, "0")) {
    
        } else if (!strcmp(arg1, "1")) {
            bin[pc][16] = 1;
        } else if (!strcmp(arg1, "2")) {
            bin[pc][17] = 1;
        } else if (!strcmp(arg1, "3")) {
            bin[pc][16] = 1;
            bin[pc][17] = 1;
        } else if (!strcmp(arg1, "4")) {
            bin[pc][18] = 1;
        } else if (!strcmp(arg1, "5")) {
            bin[pc][16] = 1;
            bin[pc][18] = 1;
        } else if (!strcmp(arg1, "6")) {
            bin[pc][17] = 1;
            bin[pc][18] = 1;
        } else if (!strcmp(arg1, "7")) {
            bin[pc][16] = 1;
            bin[pc][17] = 1;
            bin[pc][18] = 1;
        } else {

        }

        if(!strcmp(opcode, "add")) {            // 000
            int arg = atoi(arg2);
            char offset[3] = {0, };
            transform(arg, &offset[0]);
            for(int i=0; i<3; i++) {   
                bin[pc][i] = offset[i];
            }
        } else if (!strcmp(opcode, "nor")) {    // 001
            int arg = atoi(arg2);
            char offset[3] = {0, };
            transform(arg, &offset[0]);
            for(int i=0; i<3; i++) {   
                bin[pc][i] = offset[i];
            }
            bin[pc][22] = 1;
        } else if (!strcmp(opcode, "lw")) {     // 010
            char offset[16] = {0, };
            if(isNumber(arg2)) {
                int arg = atoi(arg2);
                if(-32768>arg || arg>32767) {
                    printf("offsetFields that don't fit in 16 bits!\n");
                    exit(1);
                }                
                transform16(arg, &offset[0]);
                for(int i=0; i<16; i++) {
                    bin[pc][i] = offset[i];
                }
            } else {
                int i;
                for(i=0; i<addr+1; i++) {
                    if(!strcmp(str[i], arg2)) {
                        transform16(i, &offset[0]);
                        for(int j=0; j<16; j++) {
                            bin[pc][j] = offset[j];
                        }
                        break;
                    }
                }
                if(-32768>i || i>32767) {
                    printf("offsetFields that don't fit in 16 bits!\n");
                    exit(1);
                } 
                if(i == addr+1) {
                    printf("undefined labels");
                    exit(1);
                }
            } 
            bin[pc][23] = 1;
        } else if (!strcmp(opcode, "sw")) {     // 011
            char offset[16] = {0, };
            if(isNumber(arg2)) {
                int arg = atoi(arg2);
                if(-32768>arg || arg>32767) {
                    printf("offsetFields that don't fit in 16 bits!\n");
                    exit(1);
                } 
                transform16(arg, &offset[0]);
                for(int i=0; i<16; i++) {
                    bin[pc][i] = offset[i];
                }
            } else {
                int i;
                for(i=0; i<addr+1; i++) {
                    if(!strcmp(str[i], arg2)) {
                        transform16(i, &offset[0]);
                        for(int j=0; j<16; j++) {
                            bin[pc][j] = offset[j];
                        }
                        break;
                    }
                }
                if(-32768>i || i>32767) {
                    printf("offsetFields that don't fit in 16 bits!\n");
                    exit(1);
                } 
                if(i == addr+1) {
                    printf("undefined labels");
                    exit(1);
                }
            }           
            bin[pc][22] = 1;
            bin[pc][23] = 1;
        } else if (!strcmp(opcode, "beq")) {    // 100
            char offset[16] = {0, };
            if(isNumber(arg2)) {
                int arg = atoi(arg2);
                if(-32768>arg || arg>32767) {
                    printf("offsetFields that don't fit in 16 bits!\n");
                    exit(1);
                } 
                transform16(arg, &offset[0]);
                for(int i=0; i<16; i++) {
                    bin[pc][i] = offset[i];
                }
            } else {
                int i;
                for(i=0; i<addr+1; i++) {
                    if(!strcmp(str[i], arg2)) {
                        break;
                    }
                }
                if(i == addr+1) {
                    printf("undefined labels");
                    exit(1);
                }
                int offs = i - 1 - pc;
                transform16(offs, &offset[0]);
                for(int i=0; i<16; i++) {
                    bin[pc][i] = offset[i];
                }
                if(-32768>offs || offs>32767) {
                    printf("offsetFields that don't fit in 16 bits!\n");
                    exit(1);
                } 
            }
            bin[pc][24] = 1;
        } else if (!strcmp(opcode, "jalr")) {   // 101
            bin[pc][22] = 1;
            bin[pc][24] = 1;
        } else if (!strcmp(opcode, "halt")) {   // 110
            bin[pc][23] = 1;
            bin[pc][24] = 1;
        } else if (!strcmp(opcode, "noop")) {   // 111
            bin[pc][22] = 1;
            bin[pc][23] = 1;
            bin[pc][24] = 1;
        } else if (!strcmp(opcode, ".fill")) {
            char offset[32] = {0, };
            if(isNumber(arg0)) {
                int arg = atoi(arg0);
                transform(arg, &offset[0]);
                for(int i=0; i<32; i++) {
                    bin[pc][i] = offset[i];
                }            
            } else {
                int i;
                for(i=0; i<addr+1; i++) {
                    if(!strcmp(str[i], arg0)) {
                        transform(i, &offset[0]);
                        for(int j=0; j<32; j++) {
                            bin[pc][j] = offset[j];
                        }
                        break;
                    }
                }
                if(i == addr+1) {
                    printf("undefined labels");
                    exit(1);
                }
            }
        } else {
            printf("error : unrecognized opcode\n");
            printf("%s\n", opcode);
            exit(1);
        }

        int mc = 0;
        for(int i=0; i<=30; i++) {
            if(bin[pc][i] == 1) {
                mc = mc + pow(2, i);
            }
        }
        if(bin[pc][31] == 1) {
            mc = mc - pow(2,31);
        }
        pc = pc + 1;

        printf("%d\n", mc);
        char Code[12];
        sprintf(Code, "%d", mc);
        fwrite(Code, strlen(Code), 1, outFilePtr);
        fputs("\n", outFilePtr);

    }
    fclose(outFilePtr);
    exit(0);


    /* this is how to rewind the file ptr so that you start reading from the 
    beginning of the file */
    rewind(inFilePtr);
    
}


/*
 * Read and parse a line of the assembly-language file. Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 * 
 * Return values:
 *      0 if reached end of file
 *      1 if all went well
 * 
 *  exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {
    char line[MAXLINELENGTH];
    char *ptr = line;
    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }
    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n"); 
        exit(1);
    }
    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }
    /*
     * Parse the rest of the line. Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}
int isNumber(char *string) { 
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
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

void transform16(int decimal, char binary[]) {
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
        for(int i=16; i>=0; i--) {
            if(binary[i]==1) binary[i]=0;
            else binary[i]=1;
        }
        
        for(int i=0; i<=16; i++) {
            if(binary[i]==1) binary[i]=0;
            else {
                binary[i]=1;
                break;
            }
        }
    }   
}

