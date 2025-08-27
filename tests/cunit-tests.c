#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <CUnit/Basic.h>
#include "../sudoku.h"


// ---- Prototypes ----

void serializeSudoku(const Sudoku *s, char out[SIZE*SIZE + 1]);
int countChars(const char *str, char c);
void splitSudokuString(const char *str, char (*rows)[SIZE + 1]);


// ---- Tests ----

void blankCellsCount() {
    // Given
    Sudoku s;
    const char *schema = "53  7    6  195    98    6 8   6   34  8 3  17   2   6 6    28    419  5    8  79";
    const char *rowsPtr[SIZE];

    char rows2D[SIZE][SIZE+1];
    splitSudokuString(schema, rows2D); // fill it
    for (int i = 0; i < SIZE; i++) {
        rowsPtr[i] = rows2D[i];
    }
    
    // When
    short nBlanks = initSudokuFromSingleString(&s, schema);
    short mBlanks = initSudokuFromMultipleStrings(&s, rowsPtr);
    
    // Then
    CU_ASSERT(nBlanks == countChars(schema, ' '));
    CU_ASSERT(nBlanks == mBlanks);
}

void sudokuWithSolution() {
    // Given
    Sudoku s;
    const char *rowsPtr[] = {
        "     4  3",
        " 71 9 4",
        "3  7  9 6",
        "  517  6",
        "1 64 3 9",
        "  96 2 35",
        "        7",
        "",
        "6      4",
        NULL
    };
    
    // When
    short nBlanks = initSudokuFromMultipleStrings(&s, rowsPtr);
    BlankCell* blankCells = initBlankCells(&s, nBlanks);
    bool hasSolution = solve(&s, blankCells);
    
    // Then
    CU_ASSERT_TRUE(hasSolution);
    
    char solvedSudoku[82];
    serializeSudoku(&s, solvedSudoku);
    CU_ASSERT_STRING_EQUAL(solvedSudoku, "962814573571396482384725916835179264126453798749682135213948657498567321657231849");
}

void sudokuWithoutSolution() {
    // Given
    Sudoku s;
    const char *rowsPtr[] = {
        "  6 71 3",
        "    4  7",
        "    567",
        " 1   4 5",
        " 8  3",
        "7  5  6",
        " 68 2",
        " 429    7",
        " 97  35",
        NULL
    };
    
    // When
    short nBlanks = initSudokuFromMultipleStrings(&s, rowsPtr);
    BlankCell* blankCells = initBlankCells(&s, nBlanks);
    bool hasSolution = solve(&s, blankCells);
    
    // Then
    CU_ASSERT_FALSE(hasSolution);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("cunit-tests", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "Count of blank cells", blankCellsCount)) ||
        (NULL == CU_add_test(pSuite, "Find sudoku solution", sudokuWithSolution)) ||
        (NULL == CU_add_test(pSuite, "Sudoku without solution", sudokuWithoutSolution))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}


// ---- Implementations ----

void serializeSudoku(const Sudoku *s, char out[SIZE*SIZE + 1]) {
    short i, j, pos = 0;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            short v = s->grid[i][j];
            out[pos++] = (v >= 1 && v <= 9) ? (char)('0' + v) : ' ';
        }
    }
    out[pos] = '\0';
}

int countChars(const char *str, char c) {
    int count = 0;
    for (const char *p = str; *p != '\0'; p++) {
        if (*p == c) count++;
    }
    return count;
}

void splitSudokuString(const char *str, char (*rows)[SIZE + 1]) {
    size_t len = strlen(str);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            size_t index = i * SIZE + j;
            if (index < len) {
                rows[i][j] = str[index];
            } else {
                rows[i][j] = '0'; // fill with blanks (or ' ')
            }
        }
        rows[i][SIZE] = '\0'; // null terminate
    }
}