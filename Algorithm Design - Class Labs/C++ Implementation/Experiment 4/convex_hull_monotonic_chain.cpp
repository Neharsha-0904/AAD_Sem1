#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using P=pair<int,int>;
long long cross(P o,P a,P b) {
    return 1LL*(a.first-o.first)*(b.second-o.second)-1LL*(a.second-o.second)*(b.first-o.first);
}
vector<P> hull(vector<P> p) {
    sort(p.begin(),p.end()); p.erase(unique(p.begin(),p.end()),p.end());
    if(p.size()<=1) return p;
    vector<P> lo,up;
    for(P x:p) { while(lo.size()>=2&&cross(lo[lo.size()-2],lo.back(),x)<=0) lo.pop_back(); lo.push_back(x); }
    for(auto it=p.rbegin();it!=p.rend();++it) { P x=*it; while(up.size()>=2&&cross(up[up.size()-2],up.back(),x)<=0) up.pop_back(); up.push_back(x); }
    lo.pop_back(); up.pop_back(); lo.insert(lo.end(),up.begin(),up.end()); return lo;
}
int main() {
    auto h=hull({{0,0},{1,1},{2,0},{1,0}});
    for(auto [x,y]:h) cout<<"("<<x<<","<<y<<") ";
    cout<<"\n";
}
