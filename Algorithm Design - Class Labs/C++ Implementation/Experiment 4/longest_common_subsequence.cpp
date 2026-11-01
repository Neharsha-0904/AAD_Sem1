#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int lcs(string a,string b) {
    vector<vector<int>> d(a.size()+1,vector<int>(b.size()+1));
    for(int i=1;i<=a.size();i++) for(int j=1;j<=b.size();j++)
        d[i][j]=a[i-1]==b[j-1]?1+d[i-1][j-1]:max(d[i-1][j],d[i][j-1]);
    return d.back().back();
}
int main(){cout<<lcs("ABCDGH","AEDFHR")<<"\n";}
