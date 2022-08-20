//
//  Cell.cpp
//  HW1
//
//  Created by Yunus Kerestecioğlu on 28.03.2022.
//

#include "Cell.h"
#include <string>
#include <iostream>
#include "randgen.h"

using namespace std;

/*
 Constructor with 2 parameters
*/
Cell::Cell(int x_c, int y_c)
: x(x_c), y(y_c)
{
    up = 1;
    bottom = 1;
    left = 1;
    right = 1;
}

/*
 Constructor with 6 parameters
*/
Cell::Cell(int x_c, int y_c, int u, int b, int l, int r)
: x(x_c), y(y_c), up(u), bottom(b), left(l), right(r){}

string Cell::ToStr()
{
    string s = "";
    
    s+= "x=" + to_string(x) + " y=" + to_string(y) + " l=" + to_string(left) + " r=" + to_string(right) + " u=" + to_string(up) + " d=" + to_string(bottom);
//    x=0 y=0 l=1 r=1 u=0 d=1
    return s;
}

/*
 used for breaking upper wall
 make up = 0
*/
void Cell::BreakTop(){
    up = 0;
}
/*
 used for breaking bottom wall
 make bottom = 0
*/
void Cell::BreakBottom(){
    bottom = 0;
}
/*
 used for breaking left wall
 make left = 0
*/
void Cell::BreakLeft(){
    left = 0;
}
/*
 used for breaking right wall
 make right = 0
*/
void Cell::BreakRight(){
    right = 0;
}

/*
 returns x coordinate of cell
*/
int Cell::GetX()
{
    return x;
}
/*
 returns y coordinate of cell
*/
int Cell::GetY()
{
    return y;
}

/*
 returns random wall
 checks if there is a wall to break
*/
void Cell::CheckWall(int M, int N, bool & wallCheck, char & w, const vector<vector<bool>> & visit)
{
    vector<char> walls;
    RandGen r;
    wallCheck = false;
    if (x >= 0 && x < N && y >= 0 && y < N){ // check if cells in the maze
        
        //check for left cell
        if (x!= 0 && !visit[y][x-1]){
            walls.push_back('L');
            wallCheck = true;
        }
        
        // check for bottom cell
        if (y!= 0 && !visit[y-1][x]){
            walls.push_back('D');
            wallCheck = true;
        }
        
        // check for right cell
        if (x!= N-1 && !visit[y][x+1]){
            walls.push_back('R');
            wallCheck = true;
        }
        
        // check for upper cell
        if (y!= M-1 && !visit[y+1][x]){
            walls.push_back('U');
            wallCheck = true;
        }
    }
    
    if (wallCheck)
        w = walls[r.RandInt(walls.size())];
}

/*
 returns random wall
 checks if there is cell near to go
*/
void Cell::CheckCellToGo(bool & cellCheck, char & w, const vector<vector<bool>> & visit)
{
    vector<char> walls;
    RandGen r;
    cellCheck = false;
    // up bottom left right
    
    // check for left cell
    if (left == 0 && !visit[y][x-1]){
        walls.push_back('L');
        cellCheck = true;
    }
    
    // check for bottom cell
    if (bottom == 0 && !visit[y-1][x]){
        walls.push_back('D');
        cellCheck = true;
    }
    
    // check for right cell
    if (right == 0 && !visit[y][x+1]){
        walls.push_back('R');
        cellCheck = true;
    }
    
    // check for upper cell
    if (up == 0 && !visit[y+1][x]){
        walls.push_back('U');
        cellCheck = true;
    }
    
    if (cellCheck)
        w = walls[r.RandInt(walls.size())];
}
