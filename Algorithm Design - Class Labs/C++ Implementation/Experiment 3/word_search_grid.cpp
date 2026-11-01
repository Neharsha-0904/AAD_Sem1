#include <iostream>
#include <vector>
#include <string>
using namespace std;
bool dfs(vector<vector<char>>& b,const string& w,int r,int c,int i) {
    if(i==(int)w.size()) return true;
    if(r<0||r>=(int)b.size()||c<0||c>=(int)b[0].size()||b[r][c]!=w[i]) return false;
    char ch=b[r][c]; b[r][c]='#';
    bool ok=dfs(b,w,r+1,c,i+1)||dfs(b,w,r-1,c,i+1)||
            dfs(b,w,r,c+1,i+1)||dfs(b,w,r,c-1,i+1);
    b[r][c]=ch; return ok;
}
int main() {
    vector<vector<char>> b={{'A','B'},{'C','D'}}; string w="ABD";
    bool found=false;
    for(int r=0;r<2;r++) for(int c=0;c<2;c++) found|=dfs(b,w,r,c,0);
    cout<<(found?"True":"False")<<"\n";
}
