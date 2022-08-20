//
//  Cell.h
//  HW1
//
//  Created by Yunus Kerestecioğlu on 28.03.2022.
//

#ifndef Cell_h
#define Cell_h
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class Cell
{
public:
    Cell(int,int);
    Cell(int,int,int,int,int,int);
    string ToStr();
    void BreakTop();
    void BreakBottom();
    void BreakLeft();
    void BreakRight();
    int GetX();
    int GetY();
    void CheckWall(int M, int N, bool & wallCheck, char & w, const vector<vector<bool>> & visit);
    void CheckCellToGo(bool & cellCheck, char & w, const vector<vector<bool>> & visit);
private:
    int x;
    int y;
    int up;
    int bottom;
    int left;
    int right;
};


#endif /* Cell_h */
