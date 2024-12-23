#include <stdio.h>
#include <stdlib.h>

typedef struct tree{
	int *chds;
	int *keys;
	struct tree **chds;
	int keyc;
	int chdc;
	struct tree *parn;
	int isLeaf;
}tree;

int order;

void xchg(int *a, int *b){
	*a=*a+*b;
	*b=*a-*b;
	*a=*a-*b;
}

void addToAry(int *keys, int *keyc, int val){
	for(int i=0;i<*keyc;i++){//Keys
		if(keys[i]>val){
			xchg(&keys[i],&val);
		}
	}
	keys[keyc++];
}


void search(tree *head){
	
}

BTree* add(tree *head, int val){
	if(head==NULL){
		head=malloc(sizeof(tree *));
		head->isLeaf=0;
	}else{
		head->isLeaf=1;
	}
	if(head->keyc<order-1){
		addToAry(head->keys, head->&keyc, val);
	}
	head->chdc=0;
	return head;
}

void delete(tree *head, int val){
	
}

void traverse(tree *head){
	
}

void split(tree *head){
	
}

void merge(tree *root1, tree *tree2){
	
}

int isBTree(tree *root){
	if(root==NULL){
		return 1;
	}
	if(root->isRoot){
		if(root->chdc==0||root->chdc>1){
			if(root->keyc>0)
		}
	}
	
	if(root->keyc<order)
}

int main(){
	order=4;
	int val[]={ 11, 55, 99, 22, 45, 78, 56, 87, 78, 4, 47, 8, 98, 35, 12 };
	tree *head=malloc(NULL);
	for(int i=0;i<sizeof(val)/sizeof(val[0]);i++){
		add(head,val[i]);
	}
	
	
	return 0;
}
