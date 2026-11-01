#include <iostream>
#include <vector>
#include <set>
using namespace std;
bool validSudoku(const vector<vector<char>>& b) {
    for(int i=0;i<9;i++) {
        set<char> row,col;
        for(int j=0;j<9;j++) {
            if(b[i][j]!='.' && !row.insert(b[i][j]).second) return false;
            if(b[j][i]!='.' && !col.insert(b[j][i]).second) return false;
        }
    }
    for(int r=0;r<9;r+=3) for(int c=0;c<9;c+=3) {
        set<char> box;
        for(int i=r;i<r+3;i++) for(int j=c;j<c+3;j++)
            if(b[i][j]!='.' && !box.insert(b[i][j]).second) return false;
    }
    return true;
}
int main() {
    vector<vector<char>> b(9,vector<char>(9,'.'));
    cout<<(validSudoku(b)?"Valid":"Invalid")<<"\n";
}
