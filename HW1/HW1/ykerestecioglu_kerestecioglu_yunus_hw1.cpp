// Yunus Kerestecioglu

#include <iostream>
#include <string>
#include "Stack.h"
#include "Stack.cpp"
#include "Cell.h"
#include <vector>
#include <fstream>

using namespace std;

//returns true if all cells visited in the maze
bool allCellsVisited(const vector<vector<bool>> & maze){
    for (int i = 0; i < maze.size(); i++){
        for (int j = 0; j < maze[0].size(); j++){
            if (!maze[i][j])
                return false;
        }
    }
    return true;
}

/*
 writes maze into file
 used in main
 maze_mazeID.txt
*/
void writeMaze(string filename,const vector<vector<Cell>> & maze)
{
    ofstream ofile(filename);
    ofile << maze.size() << " " << maze[0].size() << endl;
    for (int i = 0; i < maze.size(); i++){
        for (int j = 0; j < maze[0].size(); j++){
            Cell c = maze[i][j];
            ofile << c.ToStr() << endl;
        }
    }
    ofile.close();
}

/*
 writes path into file
 maze_mazeID_path_enterX_enterY_exitX_exitY.txt
*/
void writePath(string filename,Stack<Cell> stck)
{
    ofstream ofile(filename);
    // stck contains cells in the path in reverse order
    Stack<Cell> path;
    while (!stck.isEmpty()) {
        Cell c = stck.topAndPop();
        path.push(c);
    }
    while (!path.isEmpty()) { // re-reversing
        Cell c = path.topAndPop();
        ofile << c.GetX() << " " << c.GetY() << endl;
    }
    ofile.close();
}


/*
 Constructs maze with cells that have broken walls.
*/
vector<vector<Cell>> constructMaze(int M, int N){
    Stack<Cell> stack;
    // constructing initial maze with walls
    vector<vector<Cell>> maze;
    for (int i = 0; i < M; i++){
        vector<Cell> row;
        for(int j = 0; j < N; j++){
            Cell c = Cell(j,i);
            row.push_back(c);
        }
        maze.push_back(row);
    }
    // matrix for tracking visited cells
    vector<vector<bool>> visited(M,vector<bool>(N,false));
    
    // pushing first element to stack
    stack.push(maze[0][0]);
    visited[0][0] = true;
    
    bool wallCheck;
    char W; // random wall
    int x,y; // coordinates of cell
    while(!allCellsVisited(visited)){ // continue till every cell in the maze visited
        Cell top = stack.top();
        top.CheckWall(M, N, wallCheck, W, visited);

        if (wallCheck){ // check for a suitable wall to knock down
            x = top.GetX(); // x coordinate
            y = top.GetY(); // y coordinate

            if (W == 'L'){ // break left wall of cell
                maze[y][x].BreakLeft();
                maze[y][x-1].BreakRight(); // break right of neighbor
                visited[y][x-1] = true;
                stack.push(maze[y][x-1]);
            }
            else if (W == 'D'){ // break bottom wall of cell
                maze[y][x].BreakBottom();
                maze[y-1][x].BreakTop(); // break top of neighbor
                visited[y-1][x] = true;
                stack.push(maze[y-1][x]);
            }
            else if (W == 'R'){ // break right wall of cell
                maze[y][x].BreakRight();
                maze[y][x+1].BreakLeft(); // break left of neighbor
                visited[y][x+1] = true;
                stack.push(maze[y][x+1]);
            }
            else { // W == 'U' break top wall of cell
                maze[y][x].BreakTop();
                maze[y+1][x].BreakBottom(); // break bottom of neighbor
                visited[y+1][x] = true;
                stack.push(maze[y+1][x]);
            }
        }
        else
            stack.pop();
    }
    return maze;
}

/*
 Finds path between two cells
*/
Stack<Cell> constructPath(int M, int N, int entryX,int entryY,int exitX, int exitY, vector<vector<Cell>> maze){
    Stack<Cell> stack;
    
    int x=entryX,y=entryY; // to track the coordinates of current cell
    char W;
    bool cellCheck;
    
    // matrix for tracking visited cells
    vector<vector<bool>> visited(M,vector<bool>(N,false));
    
    // pushing first element to stack
    stack.push(maze[entryY][entryX]);
    visited[entryY][entryX] = true;
    
//    x!= exitX || y!= exitY
    bool check = true; // while check
    while ( check ){
        Cell top = stack.top();
        x = top.GetX();
        y = top.GetY();
        maze[y][x].CheckCellToGo(cellCheck, W, visited);
        if (x == exitX && y == exitY )
            check = false; // exit cell is reached
        else {
            if (cellCheck) {
                if (W == 'L'){
                    stack.push(maze[y][x-1]);
                    visited[y][x-1] = true;
                }
                else if (W == 'D'){
                    stack.push(maze[y-1][x]);
                    visited[y-1][x] = true;
                }
                else if (W == 'R'){
                    stack.push(maze[y][x+1]);
                    visited[y][x+1] = true;
                }
                else{
                    stack.push(maze[y+1][x]);
                    visited[y+1][x] = true;
                }
            }
            else{
                stack.pop();
            }
        }
    }
    return stack;
}

int main() {
    vector<vector<vector<Cell>>> mazeContainer;
    int K,M,N;
    int mazeID;
    int entryX,entryY,exitX,exitY;
    string filename;

    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    
    for (int i = 0; i < K; i++){
        vector<vector<Cell>> maze = constructMaze(M,N);
        mazeContainer.push_back(maze);
        filename = "maze_" + to_string(i+1) + ".txt";
        writeMaze(filename,maze);
    }
    cout << "All mazes are generated." << endl << endl;

    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
    cin >> mazeID;
    vector<vector<Cell>> mainMaze = mazeContainer[mazeID-1];

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryX >> entryY;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;

    Stack<Cell> path = constructPath(M, N, entryX, entryY, exitX, exitY, mainMaze);
    
    filename = "maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
    
    writePath(filename, path);
    
    return 0;
}
