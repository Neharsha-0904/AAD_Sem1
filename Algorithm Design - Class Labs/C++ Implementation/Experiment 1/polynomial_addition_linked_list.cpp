#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Node { int coeff, exp; Node* next; Node(int c,int e):coeff(c),exp(e),next(nullptr){} };
Node* build(vector<pair<int,int>> terms) {
    sort(terms.begin(), terms.end(), [](auto a, auto b){ return a.second>b.second; });
    Node dummy(0,0), *tail=&dummy;
    for (auto [c,e] : terms) { tail->next=new Node(c,e); tail=tail->next; }
    return dummy.next;
}
Node* addPoly(Node* a, Node* b) {
    Node dummy(0,0), *tail=&dummy;
    while(a || b) {
        int c,e;
        if(!b || (a && a->exp>b->exp)) c=a->coeff,e=a->exp,a=a->next;
        else if(!a || b->exp>a->exp) c=b->coeff,e=b->exp,b=b->next;
        else c=a->coeff+b->coeff,e=a->exp,a=a->next,b=b->next;
        if(c) { tail->next=new Node(c,e); tail=tail->next; }
    }
    return dummy.next;
}
void display(Node* p) {
    if(!p) { cout<<"0\n"; return; }
    while(p) { cout<<p->coeff<<"x^"<<p->exp; if(p->next) cout<<" + "; p=p->next; }
    cout<<"\n";
}
int main() {
    Node* a=build({{3,2},{2,1},{5,0}});
    Node* b=build({{4,3},{3,1},{2,0}});
    display(addPoly(a,b));
}
