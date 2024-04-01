#include <iostream>
#include <cmath>
#include <algorithm>

bool solvableSudoku(int* grid, int r, int c);
int  cellBox(int* grid, int* row, int* column);
int  possibleValues(int* grid, int* row, int* column, int* okValues, int* boxNumber);
bool areEqual(int* arr1, int* arr2);
bool finalCheck(int* grid);
int  printGrid(int* grid);
int  reset(int* arr);
int  nextIndex(int* r, int* c);
bool legalGrid(int* grid, int* size);

int main() {
    // Matrix that defines the (incomplete) 9*9 matrix grid. The zeros denot the blank cells.
    int grid[81] = {
        0, 3, 0, 0, 5, 8, 0, 0, 0,
        0, 9, 0, 7, 4, 3, 0, 1, 0,
        5, 7, 2, 0, 0, 0, 0, 4, 0,
        0, 0, 0, 0, 9, 0, 0, 0, 5,
        3, 0, 0, 5, 0, 1, 0, 2, 0,
        0, 5, 7, 0, 0, 0, 8, 9, 0,
        0, 0, 5, 0, 0, 0, 0, 0, 9,
        0, 0, 6, 0, 0, 5, 2, 0, 0,
        7, 0, 0, 8, 0, 0, 1, 0, 0
    };

    int size = sizeof(grid)/4;
    if (!legalGrid(grid, &size)){   // check whether it is a legal Sudoku grid regarding dimensions and contents.
        return 0;
    }

    if (solvableSudoku(grid, 0, 0)){
        std::cout << "\nCOMPLETED SUDOKU GRID:\n";
        printGrid(grid);
    }else{
        std::cout << "This Sudoku is not solvable!" << std::endl;
    }

    return 0;
}


// Function that checks whether the grid is a 9*9 grid containing integers from 0 (empty cells) to 9.
bool legalGrid(int* grid, int* size){
    if (*size!=81){
        std::cout << "\nInvalid grid. It has to be a 9*9 grid.\n" << std::endl;
        return false;
    }
    for (int i=0; i<81; i++){
        if (grid[i]<0 || grid[i]>9){
            std::cout << "\nInvalid grid. It has to contain integer values between 0 (for the empty cells) and 9.\n" << std::endl;
            return false;
        }
    }
    return true;
}


// Function that checks whether is a Sudoku grid is solvable. If it is, the grid is completed when the function end.
bool solvableSudoku(int* grid, int r, int c){
    int boxNumber; // value in {0,...,8}
    int arrCell;   // value in {0,...,80}
    int okValues[9] = {1,1,1,1,1,1,1,1,1};
    int allZero[9]  = {0,0,0,0,0,0,0,0,0};
    int old_r;
    int old_c;

    arrCell = (r*9)+c;                // actual index on the "grid" array
    while (grid[arrCell]!=0){         // check if it is an empty cell
        if (nextIndex(&r,&c)==-1){    // check if we got to the end of the sudoku grid
            return finalCheck(grid);  // check if the proposed solution is correct.
        }
        arrCell = (r*9)+c;            // update the index on the grid   
    }
    boxNumber = cellBox(grid, &r, &c);  // determine the 3*3 box in the grid that the current index points to

    possibleValues(grid, &r, &c, okValues, &boxNumber);     // determine the legal values for the current cell in the grid
    if (areEqual(okValues,allZero)){                        // 
        return false;                                       // there are no legal choices for the current cell
    }

    for (int k=0; k<9; k++){        // iteration over all legal values for the current cell
        if (okValues[k]==1){        // check if it is a legal value
            grid[arrCell] = k+1;    // update the cell
            if (r==c==8){           // check if we got to the very end of the grid
                if (finalCheck(grid)){
                    return true;
                }else{
                    continue;
                }
            }
            //printGrid(grid);    // for debugging purposes
            old_r = r;
            old_c = c;
            nextIndex(&r, &c);  // determine the next value for the row and column 
            if (solvableSudoku(grid, r, c)){
                return true;
            }else{
                r=old_r;        // revert to old value of the row
                c=old_c;        // revert to old value of the column
            }
            grid[arrCell] = 0;
            //printGrid(grid);    // for debugging purposes
        }
    }
    return false;
}


// Function that determines the 3*3 box in the grid given the row and the column of the cell. The returned value is between 0 and 8.
int cellBox(int* grid, int* row, int* column){
    int box;
    box = floor(*row/3)*3;
    box = box + floor(*column/3);
    return box;
}


// Function that determines the possible legal values for a cell on the Sudoku grid.
int possibleValues(int* grid, int* row, int* column, int* okValues, int* boxNumber){
    int arrCell;
    int x;
    for (int i=0; i<9; i++){
        arrCell = (i*9)+*column;
        if (grid[arrCell]!=0){
            okValues[std::max(0,grid[arrCell]-1)]=0;
        }
        arrCell = (*row*9)+i;
        if (grid[arrCell]!=0){
            okValues[std::max(0,grid[arrCell]-1)]=0;
        }
    }
    
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            arrCell = (floor(*row/3)*3+i)*9+(floor(*column/3)*3+j);
            if (grid[arrCell]!=0){
                okValues[std::max(0,grid[arrCell]-1)]=0;
            }
        }
    }
    
    return 0;
}


// Function  that checks whether two arrays have the same values.
bool areEqual(int* arr1, int* arr2) {
    for (int i = 0; i < 9; i++)
        if (arr1[i] != arr2[i])
            return false;
    return true;
}


// Function that checks if a sudoku Grid is completed correctly.
bool finalCheck(int* grid){
    int arrCell;
    int vals[9]={0,0,0,0,0,0,0,0,0};
    int allOne[9]={1,1,1,1,1,1,1,1,1};
    for (int row=0; row<9; row++){
        reset(vals);
        for (int column=0; column<9; column++){
            arrCell = (row*9)+column;   // actual index on the grid vector
            vals[grid[arrCell]-1]=1;
        }
        if (!areEqual(vals, allOne)){
            return false;
        }
    }
    //printGrid(grid);
    return true;
}


// Function that prints the Sudoku grid.
int printGrid(int* grid){
    int arrCell;
    for (int row=0; row<9; row++){
        for (int column=0; column<9; column++){
            arrCell = (row*9)+column;   // actual index on the grid vector
            std::cout << grid[arrCell] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    return 0;
}


int reset(int* arr){
    for (int i=0; i<9; i++){
        arr[i]=0;
    }
    return 0;
}


int nextIndex(int* r, int* c){
    *c = *c+1;
    if (*c>8){
        *r = *r+1;
        if (*r>8){
            return -1;
        } 
        *c = 0;
    }
    return 0;
}