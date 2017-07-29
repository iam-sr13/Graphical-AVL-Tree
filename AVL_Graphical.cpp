/*
* Author: Shriraj Sawant
* Description: This program implements a complete AVL tree and shows a coloured visual representation using my own C-Pos API.
*
*/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <cmath>
#include <conio.h>
#include "c_pos.hpp"
#define CLR (system("cls"))

using namespace std;

//basic element of a binary tree.. u can say its root

struct btnode
{
	int data;
	btnode *left,*right;
	
	btnode(const int& element=0, btnode* leftChild=NULL, btnode* rightChild=NULL)
	{
		data=element;
		left=leftChild;
		right=rightChild;		
	}
};

//This is a class defn of binary search tree

class BSTree 
{
	private:
		btnode* root;
		int treeSize;
		
		static void (*visit)(btnode*);  // a function ptr, basically a task to do while traversing a tree
		static void dispose(btnode* t){delete t;}		//a visit fn
		static void pre_Order(btnode*);
		static void in_Order(btnode*);
		static void post_Order(btnode*);
		static void level_Order(btnode*, bool);
		
		static btnode* insert(btnode*, int);  //both recursive
		static btnode* del(btnode*, int);	
		
		//AVL methods
		static int height(btnode*);		
		static int getBalance(btnode*);
		static btnode *minValueNode(btnode*);
		static btnode *maxValueNode(btnode*);		
		static btnode *rightRotate(btnode*);
		static btnode *leftRotate(btnode*);
		
		//special		
		COORD showTree(btnode*,COORD cd);
		COORD treeEdges(btnode*,COORD cd);
		int level(btnode* t) {if(t==this->root) return 1; else return level(parent(t))+1;} //recursive level fn
		int leaves(btnode* t) {int h=height(t); return pow(2,h-1);}  //leaves for resp node e.g a leaf node will return 1, a height 2 tree returns 2;
		int fnodes(btnode* t) {int h=height(t); return pow(2,h)-1;}  //no. of nodes of full bin tree for resp height;
		int noofedge(btnode*t) {int h=height(t); if(h<=1) return 0; else if(h==3) return 2; else return 2*noofedge(h-1)+1;} //wrt node
		int noofedge(int h) {if(h<=1) return 0; else if(h==3) return 2; else return 2*noofedge(h-1)+1;}		//wrt height
		bool isLeftChild(btnode*t) {if(parent(t)->left == t) return true; else return false;}
		static btnode* parentX(btnode* t, btnode* x){if( (t->left == x) || (t->right == x) ) return t; else return NULL;}
		btnode* parent(btnode* x) //a BFS algorithm to find parent of a node for a general binary tree
		{
			btnode *p=NULL, *t=root;
			queue<btnode*> q;
			while(t!=NULL && p==NULL)
			{		
				p=parentX(t,x);
				
				if(t->left != NULL) q.push(t->left);
				if(t->right != NULL) q.push(t->right);		
				if(q.size()==0){return p;}						
				t=q.front();
				q.pop();		
			}			
		    return p;	
		}			
					
	public:
		BSTree() {root=NULL; treeSize=0;}
		~BSTree() {erase();}
		
		//ADT methods
		bool empty() const {return treeSize==0;}
		int size() const {return treeSize;}		
		
		void preOrder(void (*dvisit)(btnode*))
		{BSTree::visit=dvisit; pre_Order(root);}
		
		void inOrder(void (*dvisit)(btnode*))
		{BSTree::visit=dvisit; in_Order(root);}
		
		void postOrder(void (*dvisit)(btnode*))
		{BSTree::visit=dvisit; post_Order(root);}
		
		void postOrder(void (*dvisit)(btnode*), btnode* t)        //overloaded for disposal of various nodes
		{BSTree::visit=dvisit; post_Order(t);}
		
		void levelOrder(void (*dvisit) (btnode*), bool lvl_on = false)	
		{BSTree::visit=dvisit; level_Order(root, lvl_on);}			
		
		//bst methods
		bool search(int n, btnode* &parent, btnode* &x) const;		
		void insert(int);	
		void del(int);		
		
		//xtras
		static void showData(btnode* t) {cout<<t->data<<' ';}  //a visit fn
		static void bf(btnode* t) {cout<<setcurs(cX()+21,cY())<<"|"<<setw(2)<<t->data<<" : "<<setw(2)<<getBalance(t)<<"|"<<endl;}  //a visit fn
					
		void traverse() {inOrder(showData);}		
		void erase() { postOrder(dispose); root=NULL; treeSize=0;}
		int height() {return height(root);}		
		
		//special
		void showTree(){cout<<endl<<endl;showTree(root,wherexy());}						
};
//initialise func ptr visit:
void (*BSTree::visit)(btnode*) = BSTree::showData;

//function defn:
void BSTree::insert(int n)
{	
	this->root = insert(this->root, n);
	treeSize++;	
}

void BSTree::del(int n)
{
	btnode *p,*x;
	bool found = search(n,p,x); 
	
	if(found) {
		this->root = del(this->root, n);
		treeSize--;
	}
	else {
		cout<<"\nNode to be deleted is not found!!\n"; getch();
	}
}

btnode* BSTree::insert(btnode* node, int key)
{
    if (node == NULL)  return(new btnode(key)); 
    if (key < node->data) node->left  = insert(node->left, key);
    else node->right = insert(node->right, key); 
    
    int balance = getBalance(node);
  
    // Left Left Case
    if (balance > 1 && key < node->left->data)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key >= node->right->data)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key >= node->left->data)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && key < node->right->data)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    } 
    
    return node;
} 
 
btnode* BSTree::del(btnode* node, int key)
{ 
    if (node == NULL)  return node;    
    if ( key < node->data )  node->left = del(node->left, key);   
    else if( key > node->data )  node->right = del(node->right, key); 
    
    else
    {
        // node with only one child or no child
        if( (node->left == NULL) || (node->right == NULL) )
        {
            btnode *temp = node->left ? node->left : node->right; 
            
            if(temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else 
             *node = *temp;
 
           delete temp;
        }
        else
        {
            // node with two children
            btnode* temp = minValueNode(node->right);            
            node->data = temp->data;            
            node->right = del(node->right, temp->data);
        }        
    } 
    
    if (node == NULL)
      return node; 
    
    int balance = getBalance(node);    
 
    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);
 
    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);
 
    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
 
    return node;
}

bool BSTree::search(int n, btnode* &parent, btnode* &x) const
{
	btnode* q;			
	parent = NULL;			
	if(BSTree::root==NULL) return false;
				
	q=BSTree::root;
	while(q!=NULL)
	{
		if(q->data==n)
		{					
			x=q;
			return true;
		}
		if(n<q->data)
		{
			parent=q;
			q=q->left;
		}
		else{
			parent=q;
			q=q->right;
		}
	}			
	return false;
}

void BSTree::pre_Order(btnode* t)
{
	if(t!=NULL)
	{
		BSTree::visit(t);
		pre_Order(t->left);
		pre_Order(t->right);
	}
}

void BSTree::in_Order(btnode* t)
{
	if(t!=NULL)
	{
		in_Order(t->left);
		BSTree::visit(t);		
		in_Order(t->right);
	}
}

void BSTree::post_Order(btnode* t)
{
	if(t!=NULL)
	{		
		post_Order(t->left);
		post_Order(t->right);
		BSTree::visit(t);
	}
}

void BSTree::level_Order(btnode* t, bool lvl_on)
{
	//if(lvl_on) cout<<endl;	
	queue<btnode*> q;
	while(t!=NULL)
	{		
		BSTree::visit(t);
		
		if(t->left != NULL) q.push(t->left);
		if(t->right != NULL) q.push(t->right);		
		if(q.size()==0){return;}
		
		//if(level(t) < level(q.front()) && lvl_on) cout<<endl;
				
		t=q.front();
		q.pop();		
	}
	
}

int BSTree::height(btnode* t)
{
	if(t==NULL) return 0;
	int hl=height(t->left);
	int hr=height(t->right);
	if(hl>hr) return ++hl;
	else return ++hr;
}

//special fn defn:
COORD BSTree::showTree(btnode* t,COORD cd)
{		
	if(t!=NULL)
	{
		COORD parentPos=treeEdges(t,cd); --parentPos.X;		
		showTree(t->left,parentPos);
		showTree(t->right,parentPos);		
	}	
	return cd;
}

COORD BSTree::treeEdges(btnode* t,COORD cd)
{
	if(t==root) {
		int end = (leaves(t) + fnodes(t))/2;
		cout<<setcurs(cd.X+end,cd.Y)<<setcolor(WF|ID|BB)<<t->data<<resetcolor;
		return wherexy();
	}
	else if(isLeftChild(t)) {
		cout<<setcurs(cd);
		int end = noofedge(parent(t));
		for(int i=1; i<=end; i++) {
			cout<<setcurs(cd.X-i,cd.Y+i)<<setcolor(RF|IF)<<"/"<<resetcolor;             //<<setcurs(cX()+pow(2,i) + 1, cY())<<"\\";
		}
		cout<<setcurs(cX()-2,cY()+1)<<setcolor(WF|ID|BB)<<t->data<<resetcolor;
		return wherexy();
	}
	else {
		cout<<setcurs(cd);
		int end = noofedge(parent(t));
		for(int i=1; i<=end; i++) {
			cout<<setcurs(cd.X+i,cd.Y+i)<<setcolor(RF|IF)<<"\\"<<resetcolor;
		}
		cout<<setcurs(cX(),cY()+1)<<setcolor(WF|ID|BB)<<t->data<<resetcolor;
		return wherexy();
	}		
	
}

//AVL balancing:
btnode *BSTree::rightRotate(btnode *y)
{
    btnode *x = y->left;
    btnode *T2 = x->right;    
    x->right = y;
    y->left = T2;    
    return x;
} 

btnode *BSTree::leftRotate(btnode *x)
{
    btnode *y = x->right;
    btnode *T2 = y->left;    
    y->left = x;
    x->right = T2;   
    return y;
}
 
//balfactor
int BSTree::getBalance(btnode *t)
{
    int l_height = height (t->left);
    int r_height = height (t->right);
    int b_factor= l_height - r_height;
    return b_factor;
}

//min and max val
btnode * BSTree::minValueNode(btnode* node)
{
    btnode* current = node;    
    while (current->left != NULL)
        current = current->left;
 
    return current;
}
btnode * BSTree::maxValueNode(btnode* node)
{
    btnode* current = node;    
    while (current->right != NULL)
        current = current->right;
 
    return current;
}

int main()
{
	BSTree tree;	
	int ch,x;
	do {
		CLR;
		cout<<"\nWelcome to AVL Binary search tree Program....\n\n  MAIN MENU\n ___________\n\n1. Insert\n2. Delete\n3. Size\n4. Height\n5. PreOrder\n6. InOrder\n7. PostOrder\n8. BalanceFactor\n9. Search\n10. Exit\n";
		COORD choicePos=wherexy();
		cout<<"\n\n\n\n\n\n\n\nYour Tree: ";
		tree.showTree();				
		cout<<setcurs(choicePos)<<"\nYour choice: ";
		cin>>ch;
		switch(ch) {
			case 1:{
				cout<<"\nInsert value: ";cin>>x;
				tree.insert(x);
				break;
			}			
			case 2:{				
				if(tree.empty()) {cout<<"\nTree is Empty!!";}
				else {					
					cout<<"\nEnter value to delete: "; cin>>x;
					tree.del(x);
				}								
				break;
			}
			case 3:{
				cout<<"\nSize of your BST is: "<<tree.size();
				getch();
				break;
			}
			case 4:{
				cout<<"\nHeight of your BST is: "<<tree.height(); 									
				getch();
				break;
			}
			case 5:{
				cout<<"\nPreOrder Traversal: ";
				tree.preOrder(BSTree::showData);				
				getch();
				break;
			}				
			case 6:{
				cout<<"\nInOrder Traversal: ";
				tree.inOrder(BSTree::showData);				
				getch();
				break;
			}
			case 7:{
				cout<<"\nPostOrder Traversal: ";
				tree.postOrder(BSTree::showData);				
				getch();
				break;
			}
			case 8:{
				cout<<"\nBalFactors:\n";
				cout<<setcurs(cX()+21,cY())<<" _______\n";				
				tree.inOrder(BSTree::bf);
				cout<<setcurs(cX()+21,cY())<<" _______\n";
				getch();	 
				break;
			}
			case 9:{
				btnode *p,*t;
				cout<<"\nEnter value to be searched: "; cin>>x;
				bool found=tree.search(x,p,t);
				if(found) cout<<"\nElement exist in the tree!!";
				else cout<<"\nElement don't exist in tree!!";
				getch();
				break;
			}
			case 10: break;	
			default: cerr<<"\nInvalid Choice!!!\n";getch();
		}
	}while(ch!=10);
	
	return 0;
}

