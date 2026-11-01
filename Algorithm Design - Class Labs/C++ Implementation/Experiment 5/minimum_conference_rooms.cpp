#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
int minRooms(vector<pair<int,int>> a) {
    sort(a.begin(),a.end());
    priority_queue<int,vector<int>,greater<int>> ends;
    for(auto [s,e]:a) {
        if(!ends.empty()&&ends.top()<=s) ends.pop();
        ends.push(e);
    }
    return ends.size();
}
int main(){cout<<minRooms({{0,30},{5,10},{15,20}})<<"\n";}
