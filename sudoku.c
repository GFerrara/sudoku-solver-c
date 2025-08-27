#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"


//  --- Implementations ---

short initSudokuFromMultipleStrings(Sudoku *s, const char *rows[]) {
    short blankCount = 0;
    short i, j;

    // Fill grid and count blanks
    for (i = 0; i < SIZE; i++) {
        const char *row = (rows[i] != NULL) ? rows[i] : "";
        for (j = 0; j < SIZE; j++) {
            char c = row[j];
            if (c == '\0') {
                for (; j < SIZE; j++) {
                    s->grid[i][j] = 0;
                    blankCount++;
                }
            } else if (c >= '1' && c <= '9') {
                s->grid[i][j] = c - '0';
            } else {
                s->grid[i][j] = 0;
                blankCount++;
            }
        }
    }

    return blankCount;
}

short initSudokuFromSingleString(Sudoku *s, const char *schemaAsString) {
    short blankCount = 0;
    short i, j;
    
    // Fill grid and count blanks
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            char c = schemaAsString[i * SIZE + j];
            if (c == '\0') {
                for (; j < SIZE; j++) {
                    s->grid[i][j] = 0;
                    blankCount++;
                }
                i++;
                for (; i < SIZE; i++) {
                    for (j = 0; j < SIZE; j++) {
                        s->grid[i][j] = 0;
                        blankCount++;
                    }
                } 
            } else if (c >= '1' && c <= '9') {
                s->grid[i][j] = c - '0';
            } else {
                s->grid[i][j] = 0;
                blankCount++;
            }
        }
    }

    return blankCount;
}

BlankCell *initBlankCells(Sudoku *s, short blankCount) {
    short i, j;
    
    // No blanks
    if (blankCount == 0) 
        return NULL;
    
    // Allocate blanks + sentinel
    BlankCell *blanks = (BlankCell *)malloc((blankCount + 1) * sizeof(BlankCell));
    if (!blanks) 
        return NULL;

    // Fill BlankCell structures
    short blankIndex = 0;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (s->grid[i][j] == 0) {
                fillBlankCell(s, blanks, blankIndex++, i, j);
            }
        }
    }
    blanks[blankIndex].cell = NULL; // sentinel
    return blanks;
}

void fillBlankCell(Sudoku *s, BlankCell *blanks, short index, short row, short column) {
    BlankCell *b = &blanks[index];
    b->row = row;
    b->col = column;
    b->cell = &s->grid[row][column];
    short k;
    for (k = 0; k < SIZE; k++) {
        s->grid[row][column] = k + 1;
        b->possibleValues[k] = isCellValueAdmissible(s, row, column);
    }
    s->grid[row][column] = 0;
}

bool isValidCombination(Sudoku *s, short rowMin, short rowMax, short colMin, short colMax) {
    bool isUsed[SIZE] = {0};
    short i, j;
    short value;
    for (i = rowMin; i < rowMax; i++) {
        for (j = colMin; j < colMax; j++) {
            if ((value = s->grid[i][j] - 1) >= 0) {
                if (isUsed[value]) return FALSE;
                isUsed[value] = TRUE;
            }
        }
    }
    return TRUE;
}

bool isCellValueAdmissible(Sudoku *s, short row, short column) {
    short x = (column / ONE_THIRD_SIZE) * ONE_THIRD_SIZE;
    short y = (row / ONE_THIRD_SIZE) * ONE_THIRD_SIZE;
    return isValidCombination(s, row, row + 1, 0, SIZE) &&
           isValidCombination(s, 0, SIZE, column, column + 1) &&
           isValidCombination(s, y, y + ONE_THIRD_SIZE, x, x + ONE_THIRD_SIZE);
}

bool solve(Sudoku *s, BlankCell *blanks) {
    if (!blanks || !blanks[0].cell) return TRUE;

    short blankIndex = 0;
    while (blanks[blankIndex].cell) {
        short row = blanks[blankIndex].row;
        short col = blanks[blankIndex].col;
nextTrial:        
        short currentValue = *blanks[blankIndex].cell;
        short nextValue = 0;
        short j;

        // Find next admissible value
        for (j = currentValue; j < SIZE; j++) {
            if (blanks[blankIndex].possibleValues[j]) {
                nextValue = j + 1;
                break;
            }
        }
        s->grid[row][col] = nextValue;

        if (nextValue == 0) {
            if (blankIndex == 0) return FALSE;
            blankIndex--;
            continue;
        }

        if (isCellValueAdmissible(s, row, col)) {
            blankIndex++;
            continue;
        }
        // try with another value
        goto nextTrial;
    }
    return TRUE;
}


// --- Printing functions ---

void printSudoku(const Sudoku *s) {
    short i, j;
    for (i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) printf("---------------------\n");
        for (j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) printf("| ");
            printf("%hd ", s->grid[i][j]);
        }
        printf("\n");
    }
}

void printBlankCells(BlankCell *blanks, short n) {
    short i, k;
    for (i = 0; i < n; i++) {
        BlankCell *b = &blanks[i];
        printf("BlankCell[%hd]: (%hd,%hd)", i, b->row, b->col);
        for (k = 0; k < SIZE; k++) printf(",%d", b->possibleValues[k] ? 1 : 0);
        printf("\n");
    }
}


// --- Main ---

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <sudoku_string>\n", argv[0]);
        printf("The string should contain up to 81 characters (1-9 or blanks).\n");
        return 1;
    }

    const char *input = argv[1];
    Sudoku s;
    short nBlanks;
        
    nBlanks = initSudokuFromSingleString(&s, input);
    BlankCell *blanks = initBlankCells(&s, nBlanks);

    printf("Initial puzzle:\n");
    printSudoku(&s);
    
    printf("\n");

    if (solve(&s, blanks)) {
        printf("Solved puzzle:\n");
        printSudoku(&s);
    } else {
        printf("No solution found\n");
    }

    if (blanks) free(blanks);
    return 0;
}