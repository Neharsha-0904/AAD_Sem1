#include <iostream>
#include <vector>
using namespace std;
bool safe(vector<vector<int>>& b,int r,int c,int n) {
    for(int i=0;i<9;i++) if(b[r][i]==n||b[i][c]==n) return false;
    int sr=r/3*3, sc=c/3*3;
    for(int i=sr;i<sr+3;i++) for(int j=sc;j<sc+3;j++) if(b[i][j]==n) return false;
    return true;
}
bool solve(vector<vector<int>>& b) {
    for(int r=0;r<9;r++) for(int c=0;c<9;c++) if(b[r][c]==0) {
        for(int n=1;n<=9;n++) if(safe(b,r,c,n)) {
            b[r][c]=n; if(solve(b)) return true; b[r][c]=0;
        }
        return false;
    }
    return true;
}
int main() {
    vector<vector<int>> b(9,vector<int>(9,0));
    if(solve(b)) for(auto& row:b) { for(int x:row) cout<<x<<" "; cout<<"\n"; }
    else cout<<"No solution\n";
}
