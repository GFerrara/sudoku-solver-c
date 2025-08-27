/**
 * @file sudoku.h
 * @brief Sudoku solver data structures and function declarations.
 *
 * Provides structures and functions to initialize, manipulate, and solve
 * Sudoku puzzles using a backtracking algorithm.
 * 
 * @author Giorgio Ferrara
 * @date 2025-08-27
 */

#ifndef SUDOKU_H
#define SUDOKU_H

/** @brief Sudoku grid size (9x9). */
#define SIZE 9

/** @brief Size of a sub-grid (3x3 in a 9x9 Sudoku). */
#define ONE_THIRD_SIZE (SIZE / 3)

/** @brief Boolean type (short integer used as 0/1). */
typedef short bool;

/** @brief Boolean value representing true. */
#define TRUE 1
/** @brief Boolean value representing false. */
#define FALSE 0

/**
 * @struct Sudoku
 * @brief Represents a Sudoku puzzle as a 9x9 grid.
 */
typedef struct {
    short grid[SIZE][SIZE]; /**< 2D array representing Sudoku cells (0 = empty). */
} Sudoku;

/**
 * @struct BlankCell
 * @brief Represents an empty cell in the Sudoku puzzle.
 */
typedef struct {
    short row;                      /**< Row index of the blank cell. */
    short col;                      /**< Column index of the blank cell. */
    bool possibleValues[SIZE];      /**< Array of possible values (true if allowed). */
    short *cell;                    /**< Pointer to the actual grid cell. */
} BlankCell;

/**
 * @brief Checks if a subgrid contains a valid combination of values.
 *
 * @param s Pointer to the Sudoku structure.
 * @param rowMin Starting row index (inclusive).
 * @param rowMax Ending row index (exclusive).
 * @param colMin Starting column index (inclusive).
 * @param colMax Ending column index (exclusive).
 * @return TRUE if valid, FALSE otherwise.
 */
bool isValidCombination(Sudoku *s, short rowMin, short rowMax, short colMin, short colMax);

/**
 * @brief Checks if the current value of a cell is admissible.
 *
 * Ensures that the cell value does not violate Sudoku rules
 * (row, column, and subgrid uniqueness).
 *
 * @param s Pointer to the Sudoku structure.
 * @param row Row index of the cell.
 * @param column Column index of the cell.
 * @return TRUE if admissible, FALSE otherwise.
 */
bool isCellValueAdmissible(Sudoku *s, short row, short column);

/**
 * @brief Initializes a blank cell entry.
 *
 * Sets up the @ref BlankCell structure with position and possible values.
 *
 * @param s Pointer to the Sudoku structure.
 * @param blanks Array of blank cells.
 * @param index Index of the blank cell to fill.
 * @param row Row index of the cell.
 * @param column Column index of the cell.
 */
void fillBlankCell(Sudoku *s, BlankCell *blanks, short index, short row, short column);

/**
 * @brief Initializes a Sudoku grid from multiple row strings.
 *
 * Each string represents a row in the Sudoku. Digits '1'-'9' are values,
 * and all other characters are treated as blanks.
 *
 * @param s Pointer to the Sudoku structure.
 * @param rows Array of strings, each representing one row (NULL-terminated).
 * @return Number of blank cells found.
 */
short initSudokuFromMultipleStrings(Sudoku *s, const char *rows[]);

/**
 * @brief Initializes a Sudoku grid from a single 81-character string.
 *
 * Digits '1'-'9' are values, all other characters are treated as blanks.
 *
 * @param s Pointer to the Sudoku structure.
 * @param schemaAsString String containing Sudoku layout (≤81 chars).
 * @return Number of blank cells found.
 */
short initSudokuFromSingleString(Sudoku *s, const char *schemaAsString);

/**
 * @brief Initializes an array of blank cells.
 *
 * @param s Pointer to the Sudoku structure.
 * @param blankCount Number of blanks in the grid.
 * @return Pointer to dynamically allocated array of BlankCell.
 */
BlankCell *initBlankCells(Sudoku *s, short blankCount);

/**
 * @brief Attempts to solve the Sudoku puzzle using backtracking.
 *
 * @param s Pointer to the Sudoku structure.
 * @param blanks Array of blank cells.
 * @return TRUE if solved, FALSE otherwise.
 */
bool solve(Sudoku *s, BlankCell *blanks);

/**
 * @brief Prints the Sudoku grid to stdout.
 *
 * Empty cells are displayed as blanks (0).
 *
 * @param s Pointer to the Sudoku structure.
 */
void printSudoku(const Sudoku *s);

/**
 * @brief Prints the list of blank cells and their possible values.
 *
 * @param blanks Array of blank cells.
 * @param n Number of blank cells.
 */
void printBlankCells(BlankCell *blanks, short n);

#endif // SUDOKU_H
