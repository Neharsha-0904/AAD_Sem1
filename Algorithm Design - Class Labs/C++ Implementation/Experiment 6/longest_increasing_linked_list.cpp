#include <iostream>
#include <string>
using namespace std;
struct Node { char ch; Node* next; Node(char c):ch(c),next(nullptr){} };
string longestIncreasing(Node* h) {
    string best,run;
    while(h) {
        if(run.empty()||h->ch>run.back()) run+=h->ch;
        else run=string(1,h->ch);
        if(run.size()>best.size()) best=run;
        h=h->next;
    }
    return best;
}
int main(){
    Node a('a'),b('b'),c('d'),d('c'),e('e');
    a.next=&b;b.next=&c;c.next=&d;d.next=&e;
    cout<<longestIncreasing(&a)<<"\n";
}
