#include <iostream>
#include <sstream>
#include <errno.h>

using namespace std;

struct node_t{
int val;
node_t* next;
};



typedef node_t* Node;

void swap(Node* prevSrcNext,Node* prevDstNext){
    Node pointToNextSrc = *prevSrcNext;
    Node pointToNextDst = *prevDstNext;
    Node srcNext = (*prevSrcNext)->next;
    Node dstNext = (*prevDstNext)->next;

    *prevDstNext = pointToNextSrc;
    *prevSrcNext = pointToNextDst;

}

void findSrcDst(Node list,int src, int dst ){
    Node srcPrev = NULL;
    Node dstPrev = NULL;
    Node iter = list;
    while(iter != NULL){

        if(src == iter->val){
            putPrev(srcPrev);
        }else if(dst == iter->val)
            putPrev(dstPrev);

        iter = iter->next;
    }
}

int main() {

    return 0;
}