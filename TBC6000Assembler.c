#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char* decToBin(int num, int bits);
int binToDec(const char* binStr);
int* assemble(const char* code);

int main() {
    const char* code = "inc 0\nadd 1 2\nld a 15\nst b 7\njmp 14";
    
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
    const int numOperands[] = {1, 1, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, -1};
    
    // Mapping registers a -> 00, b -> 01, c -> 10, d -> 11
    const char* registers[] = {"a", "b", "c", "d"};

    int* newCode = (int*)malloc(sizeof(int) * 16);
    for (int i = 0; i < 16; i++) newCode[i] = -1;

    char* lines = strdup(code);
    char* line = strtok(lines, "\n");
    int i = 0;

    while (line != NULL && i < 16) {
        char* tokens[3] = {NULL, NULL, NULL};
        int tokenCount = 0;

        // Tokenize the line into words
        char* token = strtok(line, " ");
        while (token != NULL && tokenCount < 3) {
            tokens[tokenCount++] = token;
            token = strtok(NULL, " ");
        }

        // Find the mnemonic index
        int mnemonic = 0;
        while (mnemonic < 16 && strcmp(tokens[0], mnemonics[mnemonic]) != 0) {
            mnemonic++;
        }

        int numOperand = numOperands[mnemonic];
        char newLine[17] = "";

        // Append the binary of the mnemonic to the instruction
        strcat(newLine, decToBin(mnemonic, 4));

        // Handle operands
        if (numOperand == 0) {
            strcat(newLine, "0000"); // No operands, just zeros
        } else if (numOperand == 1) {
            // Single operand: 2 bits for the operand and 2 bits of padding
            strcat(newLine, decToBin(atoi(tokens[1]), 2));
            strcat(newLine, "00");
        } else if (numOperand == 2) {
            if (strcmp(tokens[0], "lda") == 0 || strcmp(tokens[0], "ldb") == 0 ||
                strcmp(tokens[0], "ldc") == 0 || strcmp(tokens[0], "ldd") == 0 ||
                strcmp(tokens[0], "sta") == 0 || strcmp(tokens[0], "stb") == 0 ||
                strcmp(tokens[0], "stc") == 0 || strcmp(tokens[0], "std") == 0) {
                // For ld and st, the first operand is a register (a, b, c, d), second is the value
                int reg = -1;
                for (int j = 0; j < 4; j++) {
                    if (strcmp(tokens[1], registers[j]) == 0) {
                        reg = j;
                        break;
                    }
                }

                if (reg == -1) {
                    fprintf(stderr, "Error: Invalid register %s\n", tokens[1]);
                    free(newCode);
                    free(lines);
                    return NULL;
                }

                // Register (2 bits) + operand (2 bits)
                strcat(newLine, decToBin(reg, 2));
                strcat(newLine, decToBin(atoi(tokens[2]), 2));
            } else {
                // For other instructions like add, sub, etc., use the standard method
                strcat(newLine, decToBin(atoi(tokens[1]), 2));
                strcat(newLine, decToBin(atoi(tokens[2]), 2));
            }
        } else if (numOperand == -1) {
            strcat(newLine, "0000"); // Default for no operands
        }

        // Convert binary string to integer and store it
        newCode[i++] = binToDec(newLine);

        // Move to the next line
        line = strtok(NULL, "\n");
    }

    free(lines);
    return newCode;
}

