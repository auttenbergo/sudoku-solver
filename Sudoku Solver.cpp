#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Location {
    int x;
    int y;
};

Location createLocation(int x, int y) {
    Location l;
    l.x = x;
    l.y = y;
    return l;
}

const int SIZE = 9;
const int BLOCK_SIZE = 3;
const int MAX_SOLUTIONS = 100;


bool isCharDigit(char c) {
    if (c >= '0' && c <= '9') return true;
    return false;
}


string removeExtraSymbolsFromString(string str) {
    string rv = "";
    for (int i = 0; i < str.length(); i++) {
        if (isCharDigit(str[i]))
            rv += str[i];
    }
    return rv;
}
int** convertToIntegerGrid(string input[]) {
    int** grid = new int*[SIZE];
    for (int i = 0; i < SIZE; i++) {
        grid[i] = new int[SIZE];
        string row = input[i];
        row = removeExtraSymbolsFromString(row);
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = row[j] - '0';
        }
    }
    return grid;
}

void printGrid(int** grid) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << "------------------" << endl;
}
void printGrid(vector<vector<int>>& grid) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << "------------------" << endl;
}

void freeGrid(int** grid) {
    for (int i = 0; i < SIZE; i++) {
        delete[] grid[i];
    }
    delete[]grid;
}
int countEmptySpots(int** grid) {
    int counter = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0)
                counter++;
        }
    }
    return counter;
}

Location* initEmptySpots(int** grid, int emptySpotsSize) {
    Location* emptySpots = new Location[emptySpotsSize];
    int counter = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] != 0) continue;
            emptySpots[counter] = createLocation(i, j);
            counter++;
        }
    }
    return emptySpots;
}

void removeElementFromVector(vector<int>& v, int elem) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == elem) {
            v.erase(v.begin() + i);
            return;
        }
    }
}

vector<vector<int>> convertArrayToVector(int** grid) {
    vector<vector<int>> rv;
    for (int i = 0; i < SIZE; i++) {
        rv.push_back(vector<int>());
        for (int j = 0; j < SIZE; j++) {
            rv[i].push_back(grid[i][j]);
        }
    }
    return rv;
}

vector<int> getSpotPossibilities(int** grid, Location currentSpot) {
    vector<int> possibilities = { 1,2,3,4,5,6,7,8,9 };
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            removeElementFromVector(possibilities, grid[i][currentSpot.y]);
            removeElementFromVector(possibilities, grid[currentSpot.x][i]);

            int blockX = currentSpot.x / BLOCK_SIZE * BLOCK_SIZE + i / BLOCK_SIZE;
            int blockY = currentSpot.y / BLOCK_SIZE * BLOCK_SIZE + i % BLOCK_SIZE;
            removeElementFromVector(possibilities, grid[blockX][blockY]);
        }
    }
    return possibilities;
}

void solve(int** grid, Location* emptySpots,int emptySpotsCount, int currentSpotIndex, vector<vector<vector<int>>>& solutions) {
    if (solutions.size() >= MAX_SOLUTIONS)
        return;
    
    if (currentSpotIndex >= emptySpotsCount) {
        if (solutions.size() < MAX_SOLUTIONS)
            solutions.push_back(convertArrayToVector(grid));
        return;
    }

    Location currentSpot = emptySpots[currentSpotIndex];
    vector<int> possibilities = getSpotPossibilities(grid, currentSpot);
    for (int i = 0; i < possibilities.size(); i++) {
        grid[currentSpot.x][currentSpot.y] = possibilities[i];
        solve(grid, emptySpots, emptySpotsCount, currentSpotIndex + 1, solutions);
        grid[currentSpot.x][currentSpot.y] = 0;
    }
}
/*
    Sudoku solver for 9x9 grid

    For each input row(9 rows), type whatever you want, but it must contain exactly 9 digits.
    Max solutions is set to 100, so program will not "freeze"
    Empty spots are considered to have digit value of 0
*/
int main()
{
    string input[SIZE];
    for (int i = 0; i < SIZE; i++) {
        string row;
        getline(cin, row);
        input[i] = row;
    }
    int** grid = convertToIntegerGrid(input);

    int emptySpotsCount = countEmptySpots(grid);
    Location* emptySpots = initEmptySpots(grid, emptySpotsCount);

    vector<vector<vector<int>>> solutions;
    solve(grid, emptySpots, emptySpotsCount, 0, solutions);

    cout << "Solutions: "<< solutions.size() << endl;
    for (int i = 0; i < solutions.size(); i++) {
        cout << "Solution " << i + 1 << " :" << endl;
        printGrid(solutions[i]);
    }

    freeGrid(grid);
    delete[] emptySpots;

    return 0;
}
