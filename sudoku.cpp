#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <set>

using namespace std;

stack<int> filledIndices; // Stack to store filled cell indices (two-digit: tens for row, units for column)
int sudoku[9][9] = {0}; // Sudoku matrix
typedef long long ll;

// Global variables for deciding the order of filling cells
vector<int> rowOrder; // Order of rows to be filled
vector<int> colOrder; // Order of columns to be filled
map<int, int> rowFillCount; // Maps rows to the number of filled cells in each row
map<int, int> colFillCount; // Maps columns to the number of filled cells in each column
set<pair<int, int>, greater<pair<int, int>>> orderedRows; // Rows ordered by number of filled cells
set<pair<int, int>, greater<pair<int, int>>> orderedCols; // Columns ordered by number of filled cells

// Function to determine the order of rows and columns based on the number of filled cells
void determineOrder() {
    for (const auto& row : rowFillCount) {
        orderedRows.insert({row.second, row.first});
    }
    for (const auto& col : colFillCount) {
        orderedCols.insert({col.second, col.first});
    }
    for (const auto& row : orderedRows) {
        rowOrder.push_back(row.second);
    }
    for (const auto& col : orderedCols) {
        colOrder.push_back(col.second);
    }
}

// Function to check if a value can be placed in a given cell (row i, column j)
bool isValid(int i, int j, int val) {
    // Check row
    for (int k = 0; k < 9; ++k) if (sudoku[i][k] == val) return false;
    // Check column
    for (int k = 0; k < 9; ++k) if (sudoku[k][j] == val) return false;
    // Check 3x3 subgrid
    int startRow = (i / 3) * 3;
    int startCol = (j / 3) * 3;
    for (int row = startRow; row < startRow + 3; ++row) {
        for (int col = startCol; col < startCol + 3; ++col) {
            if (sudoku[row][col] == val) return false;
        }
    }
    return true;
}

// Function to generate the next possible value for a given cell (row i, column j)
int generateValue(int i, int j) {
    int val = sudoku[i][j] + 1;
    while (val <= 9 && !isValid(i, j, val)) {
        ++val;
    }
    return val <= 9 ? val : -1;
}

// Function to get the next cell to be filled, encoded as a two-digit number
int getNextCell() {
    for (int row : rowOrder) {
        for (int col : colOrder) {
            if (sudoku[row][col] == 0) {
                return row * 10 + col;
            }
        }
    }
    return -1;
}

// Function to fill the Sudoku matrix and count pre-filled cells in each row and column
void fillMatrix() {
    for (int i = 0; i < 9; ++i) {
        int filledCount = 0;
        for (int j = 0; j < 9; ++j) {
            cin >> sudoku[i][j];
            if (sudoku[i][j] > 0) {
                ++filledCount;
            }
        }
        rowFillCount[i] = filledCount;
    }
    for (int j = 0; j < 9; ++j) {
        int filledCount = 0;
        for (int i = 0; i < 9; ++i) {
            if (sudoku[i][j] > 0) {
                ++filledCount;
            }
        }
        colFillCount[j] = filledCount;
    }
}

// Function to print the Sudoku matrix
void printMatrix() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cout << sudoku[i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

// Recursive function to fill the Sudoku cells
void fillCell(int i, int j) {
    int newValue = generateValue(i, j);
    if (newValue > 0) {
        sudoku[i][j] = newValue;
        filledIndices.push(i * 10 + j);
        int nextCell = getNextCell();
        if (nextCell == -1) {
            printMatrix();
        } else {
            fillCell(nextCell / 10, nextCell % 10);
        }
    } else {
        sudoku[i][j] = 0;
        if (!filledIndices.empty()) {
            int prevCell = filledIndices.top();
            filledIndices.pop();
            fillCell(prevCell / 10, prevCell % 10);
        }
    }
}

int main() {
    fillMatrix();
    determineOrder();
    int firstCell = getNextCell();
    if (firstCell != -1) {
        fillCell(firstCell / 10, firstCell % 10);
    }
}
