#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* decToBin(int num, int bits);
int binToDec(const char* binStr);
int* assemble(const char* code);

int main() {
    const char* code = "inc 3\n"
                       "add 1 2\n"
                       "lda 4\n"
                       "hlt";
    
    int* machineCode = assemble(code);
    
    printf("Assembled Machine Code:\n");
    for (int i = 0; i < 16; i++) {
        if (machineCode[i] == -1) break; // End of input
        printf("Instruction %d: %d\n", i, machineCode[i]);
    }
    
    free(machineCode);
    return 0;
}

char* decToBin(int num, int bits) {
    static char binStr[17];
    binStr[bits] = '\0';
    for (int i = bits - 1; i >= 0; i--) {
        binStr[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }
    return binStr;
}

int binToDec(const char* binStr) {
    int result = 0;
    for (int i = 0; binStr[i] != '\0'; i++) {
        result = (result << 1) + (binStr[i] - '0');
    }
    return result;
}

int* assemble(const char* code) {
    const char* mnemonics[] = {
        "inc", "dec", "add", "sub", "lda", "ldb", "ldc", "ldd",
        "sta", "stb", "stc", "std", "jmp", "jz", "jc", "hlt"
    };
    const int numOperands[] = {1, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};

    int* newCode = (int*)malloc(sizeof(int) * 16);
    for (int i = 0; i < 16; i++) newCode[i] = -1;

    char* lines = strdup(code);
    char* line = strtok(lines, "\n");
    int i = 0;

    while (line != NULL && i < 16) {
        char* tokens[3] = {NULL, NULL, NULL};
        int tokenCount = 0;

        char* token = strtok(line, " ");
        while (token != NULL && tokenCount < 3) {
            tokens[tokenCount++] = token;
            token = strtok(NULL, " ");
        }

        int mnemonic = 0;
        while (mnemonic < 16 && strcmp(tokens[0], mnemonics[mnemonic]) != 0) {
            mnemonic++;
        }

        int numOperand = numOperands[mnemonic];
        char newLine[17] = "";

        strcat(newLine, decToBin(mnemonic, 4));

        if (numOperand == 0) {
            strcat(newLine, decToBin(atoi(tokens[1]), 4));
        } else if (numOperand == 1) {
            strcat(newLine, decToBin(atoi(tokens[1]), 2));
            strcat(newLine, "00");
        } else if (numOperand == 2) {
            strcat(newLine, decToBin(atoi(tokens[1]), 2));
            strcat(newLine, decToBin(atoi(tokens[2]), 2));
        } else if (numOperand == -1) {
            strcat(newLine, "0000");
        }

        newCode[i++] = binToDec(newLine);

        line = strtok(NULL, "\n");
    }

    free(lines);
    return newCode;
}
