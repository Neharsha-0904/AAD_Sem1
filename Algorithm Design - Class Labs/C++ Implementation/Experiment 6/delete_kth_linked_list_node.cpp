#include <iostream>
using namespace std;
struct Node { int val; Node* next; Node(int x):val(x),next(nullptr){} };
Node* deleteKth(Node* head,int k) {
    if(k<1) throw invalid_argument("k is 1-based");
    if(!head)return nullptr;
    if(k==1)return head->next;
    Node* cur=head;
    for(int i=0;i<k-2&&cur->next;i++)cur=cur->next;
    if(cur->next)cur->next=cur->next->next;
    return head;
}
int main(){
    Node a(10),b(20),c(30),d(40),e(50),f(60);
    a.next=&b;b.next=&c;c.next=&d;d.next=&e;e.next=&f;
    Node* h=deleteKth(&a,5);
    while(h){cout<<h->val<<(h->next?" -> ":"\n");h=h->next;}
}
