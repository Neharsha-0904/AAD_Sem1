#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
using P=pair<double,double>;
double cross(P o,P a,P b){return (a.first-o.first)*(b.second-o.second)-(a.second-o.second)*(b.first-o.first);}
vector<P> hull(vector<P> p){
    sort(p.begin(),p.end());p.erase(unique(p.begin(),p.end()),p.end());if(p.size()<=1)return p;
    vector<P> l,u;
    for(auto x:p){while(l.size()>=2&&cross(l[l.size()-2],l.back(),x)<=0)l.pop_back();l.push_back(x);}
    for(auto it=p.rbegin();it!=p.rend();++it){auto x=*it;while(u.size()>=2&&cross(u[u.size()-2],u.back(),x)<=0)u.pop_back();u.push_back(x);}
    l.pop_back();u.pop_back();l.insert(l.end(),u.begin(),u.end());return l;
}
double perimeter(const vector<P>& h){
    if(h.size()<2)return 0;double sum=0;
    for(int i=0;i<h.size();i++){auto a=h[i],b=h[(i+1)%h.size()];sum+=hypot(a.first-b.first,a.second-b.second);}
    return sum;
}
int main(){
    auto h=hull({{0,0},{0,1},{1,0},{1,1}});
    cout<<"Hull: ";for(auto p:h)cout<<"("<<p.first<<","<<p.second<<") ";
    cout<<"\nPerimeter: "<<perimeter(h)<<"\n";
}
