#include <iostream>
//#include <string>       // for using std::getline()
#include <cmath>
#include <algorithm>
//using namespace std;

bool solvableSudoku(int* grid, int r, int c);
int cellBox(int* grid, int row, int column);
int possibleValues(int* grid, int row, int column, int* okValues, int boxNumber);
bool areEqual(int* arr1, int* arr2);
bool finalCheck(int* grid);
int printGrid(int* grid);
int reset(int* arr);
int nextIndex(int* r, int* c);

int main() {
    // this matrix defines the (incomplete) 9*9 matrix grid. The zeros denot the blank cells.
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

    if (solvableSudoku(grid, 0, 0)){
        std::cout << "\nCOMPLETED SUDOKU GRID:\n";
        printGrid(grid);
    }else{
        std::cout << "This Sudoku is not solvable!" << std::endl;
    }

    return 0;
}

bool solvableSudoku(int* grid, int r, int c){
    int boxNumber; // in {0,...,8}
    int arrCell;   // in {0,...,80}
    int okValues[9] = {1,1,1,1,1,1,1,1,1};
    int allZero[9]  = {0,0,0,0,0,0,0,0,0};
    int prevVal;
    int old_r;
    int old_c;

    arrCell = (r*9)+c;   // actual index on the grid vector
    while (grid[arrCell]!=0){
        if (nextIndex(&r,&c)==-1){
            return finalCheck(grid);
        }
        arrCell = (r*9)+c;
    }
    boxNumber = cellBox(grid, r, c);

    possibleValues(grid, r, c, okValues, boxNumber);
    if (areEqual(okValues,allZero)){
        return false;   // no possible choices on the current cell
    }

    // iteration over the possible value choices for the current cell
    for (int k=0; k<9; k++){
        if (okValues[k]==1){    // ok value
            prevVal=grid[arrCell];  // old value
            grid[arrCell] = k+1;
            if (r==c==8){
                if (finalCheck(grid)){
                    return true;
                }else{
                    continue;
                }
            }
            
            //printGrid(grid);    // for debugging purposes
            
            old_r = r;
            old_c = c;
            nextIndex(&r, &c);
            if (solvableSudoku(grid, r, c)){
                return true;
            }else{
                r=old_r;
                c=old_c;
            }
            grid[arrCell] = prevVal;
            //printGrid(grid);    // for debugging purposes
        }
    }
    return false;
}

int cellBox(int* grid, int row, int column){
    int box;
    box = floor(row/3)*3;
    box = box + floor(column/3);
    return box;
}

int possibleValues(int* grid, int row, int column, int* okValues, int boxNumber){
    int arrCell;
    int x;
    for (int i=0; i<9; i++){
        arrCell = (i*9)+column;
        if (grid[arrCell]!=0){
            okValues[std::max(0,grid[arrCell]-1)]=0;
        }
        arrCell = (row*9)+i;
        if (grid[arrCell]!=0){
            okValues[std::max(0,grid[arrCell]-1)]=0;
        }
    }
    
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            arrCell = (floor(row/3)*3+i)*9+(floor(column/3)*3+j);
            if (grid[arrCell]!=0){
                okValues[std::max(0,grid[arrCell]-1)]=0;
            }
        }
    }
    
    return 0;
}

bool areEqual(int* arr1, int* arr2) {
    for (int i = 0; i < 9; i++)
        if (arr1[i] != arr2[i])
            return false;
    return true;
}

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