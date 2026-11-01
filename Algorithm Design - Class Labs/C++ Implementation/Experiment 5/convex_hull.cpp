#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using P=pair<int,int>;
long long cross(P o,P a,P b){return 1LL*(a.first-o.first)*(b.second-o.second)-1LL*(a.second-o.second)*(b.first-o.first);}
vector<P> convexHull(vector<P> p){
    sort(p.begin(),p.end()); p.erase(unique(p.begin(),p.end()),p.end());
    if(p.size()<=1)return p;
    vector<P> l,u;
    for(auto x:p){while(l.size()>=2&&cross(l[l.size()-2],l.back(),x)<=0)l.pop_back();l.push_back(x);}
    for(auto it=p.rbegin();it!=p.rend();++it){auto x=*it;while(u.size()>=2&&cross(u[u.size()-2],u.back(),x)<=0)u.pop_back();u.push_back(x);}
    l.pop_back();u.pop_back();l.insert(l.end(),u.begin(),u.end());return l;
}
int main(){for(auto [x,y]:convexHull({{0,0},{0,1},{1,0},{1,1}}))cout<<x<<","<<y<<" ";}
