#include <stdio.h>
#include "stdbool.h"
#include "stdlib.h"
#define MAX 5
#define NULL ((void *) 0)
struct data{
    unsigned int id;
    char name[50];
    char email[50];
    char password[50];
    unsigned long long int phno;
    char address[100];
    unsigned int amount;
    char transferrecord[];
};

struct BTreeNode{
    struct data user[MAX];
    int count;
    bool leaf;
    struct BTreeNode* child[MAX+1];
};
int checkingpos(struct BTreeNode* prevnode,int flag,struct BTreeNode* node,struct data newdata);
void clone_char_arr(char arr1[100],char arr2[100]);
struct BTreeNode* root=NULL;
void copystruct(struct data old,struct data new){
    old.phno=new.phno;
    old.id=new.id;
    old.amount=new.amount;
    clone_char_arr(new.address,old.address);
    clone_char_arr(new.name,old.name);
    clone_char_arr(new.transferrecord,old.transferrecord);
    clone_char_arr(new.password,old.password);
    clone_char_arr(new.email,old.email);
}
void cnull(struct BTreeNode* node){
    for(int i=0;i<MAX+1;i++){
        node->child[i]=NULL;
    }
}
void nullarr(char arr[100]){
    int i=0;
    while(arr[i]!='\0'){
        arr[i]=NULL;
        i++;
    }
}
void usernull(struct BTreeNode* node,int pos){
    node->user[pos].phno=NULL;
    nullarr(node->user[pos].password);
    nullarr(node->user[pos].name);
    nullarr(node->user[pos].email);
    nullarr(node->user[pos].address);

}
void shifttoleft(int pos,struct BTreeNode* node){
    for(int j=pos;j<node->count;j++){
        node->user[j]=node->user[j+1];
        node->child[j+1]=node->child[j+2];
    }
}
void shifttoright(int pos,struct BTreeNode* shiftnode){
    int index=shiftnode->count-1;
    for(int i=index;i>=pos;i--){
        shiftnode->user[i+1]=shiftnode->user[i];
        shiftnode->child[i+2]=shiftnode->child[i+1];
        if(i<=0){
            break;
        }
    }
}
int insertnonfull(int pos, struct BTreeNode* node, struct data newdata){
//    for(int i=0;i<node->count;i++){
//        if(node->keys[i]==newkey){
//
//        } else{
    shifttoright(pos,node);
    node->user[pos]=newdata;
    node->count++;
//        }
//    }

}
void clone_char_arr(char arr1[100],char arr2[100]){
    int i=0;
    while(arr2[i]!='\0'){
        arr1[i]=arr2[i];
        i++;
    }
}
int sp(struct BTreeNode* prevnode,struct BTreeNode* splitNode,struct data newdata){
    struct BTreeNode* rightchild= malloc(sizeof (struct BTreeNode));
    if(splitNode->leaf==true){
        cnull(splitNode);
        cnull(rightchild);
    } else{
        int c=0;
        for(int i=((MAX-1)/2)-1;i<=MAX;i++){
            rightchild->child[c]=splitNode->child[i];
            splitNode->child[i]=NULL;
            c++;
        }//this condition for child
    }//this condition for child

    rightchild->count=0;
    for(int i=((MAX-1)/2)+1;i<=MAX;i++){
        rightchild->user[rightchild->count]=splitNode->user[i];
        usernull(splitNode,i);
        rightchild->count++;
    }

    struct BTreeNode* troot;
    //= malloc(sizeof (struct BTreeNode));
    troot->user[0]=splitNode->user[((MAX-1)/2)+1];
    //copystruct(splitNode->user[((MAX-1)/2)+1],troot->user[0]);
    troot->count=1;//copy key to troot
    troot->leaf=false;
    splitNode->count=(MAX-1)/2;//clean split node

    usernull(splitNode,((MAX-1)/2)+1);
//    splitNode->keys[MAX-2]=NULL;
//    splitNode->keys[MAX-3]=NULL;


    if(prevnode==NULL){//root
        //level++;
        if(splitNode->child[0]==NULL){
            rightchild->leaf=true;
        } else{
            rightchild->leaf=false;
        }
        troot->child[0]=splitNode;
        troot->child[1]=rightchild;
        root=troot;
        checkingpos(prevnode,0,root,newdata);
    } else{//internal
        if(rightchild->child[0]==NULL){
            rightchild->leaf=true;
        } else{
            rightchild->leaf=false;
        }
        int pos=checkingpos(prevnode,1,prevnode,newdata);
        prevnode->child[pos]=rightchild;
        prevnode=NULL;
        checkingpos(prevnode,0,root,newdata);
    }
}
int checkingpos(struct BTreeNode* prevnode,int flag,struct BTreeNode* node,struct data newdata){
    int f=0;
    if(node->count>=MAX){//full
//            if(node->child[0]==NULL){f
        sp(prevnode,node,newdata);
        return 0;
//            }
//            for(int i=0;i<6;i++){
//                if(node->child[i]->count<4){
//                    f=1;
//                }
//            }
//            if(prevnode==NULL && f==1){
//                check_num_of_key(root);
//            } else{
//                sp(prevnode,node,newkey);
//                return pos;
//           }
    }
    for(int pos=0;pos<node->count;pos++){
        if(newdata.phno>node->user[node->count-1].phno ){
            if(flag==0){
                if(node->leaf==false) {
                    prevnode=node;
                    checkingpos(prevnode,0,node->child[node->count],newdata);
                    return pos;
                }else{
                    node->user[node->count]=newdata;
                    node->count++;
                    return pos;
                }
            } else{
                node->user[node->count]=newdata;
                node->count++;
                return node->count;
            }
        }

        if(newdata.phno<node->user[pos].phno){
            if(flag==0){
                if(node->leaf==false){
                    prevnode=node;
                    checkingpos(prevnode,0,node->child[pos],newdata);
                    return pos;
                } else{
                    insertnonfull(pos,node,newdata);
                    return pos;
                }
            } else{
                insertnonfull(pos,node,newdata);
                return pos+1;
            }
        }
    }
}
void insert(struct data user){
    if(root==NULL){
        struct BTreeNode* newnode=malloc(sizeof (struct BTreeNode));
        newnode->user[0].phno=user.phno;
        newnode->user[0].id=user.id;
        newnode->user[0].amount=user.amount;
        clone_char_arr(newnode->user[0].name,user.name);
        clone_char_arr(newnode->user[0].password,user.password);
        clone_char_arr(newnode->user[0].email,user.email);
        clone_char_arr(newnode->user[0].address,user.address);
        clone_char_arr(newnode->user[0].transferrecord,user.transferrecord);

        newnode->count=1;
        newnode->leaf=true;
        cnull(newnode);
        root=newnode;
    } else {
        struct BTreeNode* prevnode=NULL;
        checkingpos(prevnode, 0, root,user);
    }
}

int main(){
    struct data newdata;
    struct data newdata1;
    struct data newdata2;
    struct data newdata3;
    struct data newdata4;
    struct data newdata5;
    newdata.phno=6;
    newdata1.phno=1;
    newdata2.phno=2;
    newdata3.phno=3;
    newdata4.phno=4;
    newdata5.phno=5;
//    newdata.amount=10000;
//    newdata.id=1234;
//    newdata.name[0]="H";
//    newdata.address[0]="M";
//    newdata.password[0]="Y";
//    newdata.transferrecord[0]="N";
//    newdata.email[0]="@";
    insert(newdata);
    insert(newdata1);
    insert(newdata2);
    insert(newdata3);
    insert(newdata4);
    insert(newdata5);
}
//char name[MAX][30];
//struct data name[100];



//#include <stdio.h>
//struct myStructure {
//    int myNum;
//    char *myString;
//};
//struct node{
//    struct myStructure arr[3];
//};
//int main() {
//    struct node n;
//    n.arr[0].myNum = 13;
//    n.arr[0].myString =  "hi";
//    n.arr[1].myNum= 12;
//    n.arr[1].myString= "Hi";
//    n.arr[0]=n.arr[1];
//    printf("%d %c\n", n.arr[0]);
//    printf("%d %c", n.arr[1]);
//    return 0;
//}
