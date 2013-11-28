//
//  main.cpp
//  CSL201Ass3
//
//  Created by Robin Malhotra on 10/09/13.
//  Copyright (c) 2013 Robin's code kitchen. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string.h>


using namespace std;

struct emp//employee contains name,level, no of children, and a vector of all children
//A vector is used as a substitute for a queue
{
    char name[100];
    int level;
    
    
    int no_of_child;
    vector<emp*> child;
    emp *parent;
    
};



struct AVLnode//pointer to node in AVL tree that contains a pointer to an employee node, and reqd left, right and parent pointers
{
    emp *point;
    int height=0;
    AVLnode *left;
    AVLnode *right;
    AVLnode *parent;
};

class AVLtree
{
public:
    AVLnode *root;
    
    int countnodes(AVLnode *temp)//counts no of nodes in left and right subtree in the node temp
    {
        int c=1;
        if (temp->left!=NULL)
        {
            c=c+countnodes(temp->left);
        }
        if (temp->right!=NULL)
        {
            c=c+countnodes(temp->right);
        }
        
        return c;
    }
    
    void updateheight(AVLnode *temp)//update heights
    {
        if (temp->left==NULL && temp->right==NULL)
        {
            temp->height=1;
        }
        
        else
        {
            if (temp->left==NULL)
                temp->height=temp->right->height+1;
            else if (temp->right==NULL)
                temp->height=temp->left->height+1;
            else
                temp->height=max(temp->left->height, temp->right->height)+1;
        }
    }
    
    void Insertheightincrease(AVLnode *t2)//increasees heights of ancestors by 1 after a node is inserted
    {
        while (t2!=NULL)
        {
            t2=t2->parent;
            if (t2!=NULL)
            {
                updateheight(t2);
            }
            
        }
    }
    
    void Deleteheightdecrease(AVLnode *t2)//devreases height of ancestors
    {
        while (t2!=NULL)
        {
            updateheight(t2);
            t2=t2->parent;
        }
    }
    
    
    void rotation(AVLnode *a,AVLnode *b)//rotation in case of an unbalanced AVL tree
    {
        a->parent=b->parent;
        b->parent=a;
        
        if (a==b->right)
        {
            b->right=a->left;
            a->left=b;
        }
        
        else if (a==b->left)
        {
            b->left=a->right;
            a->right=b;
        }
        
        if (b==root)
        {
            root=a;
        }
        
        updateheight(b);
        updateheight(a);
        
    }
    
    void Insert(emp *temp)//inserts node in AVL tree
    {
        AVLnode *p=new AVLnode;
        AVLnode *t=new AVLnode;
        AVLnode *t2=new AVLnode;
        if (root==NULL)
        {
            root=new AVLnode;
            root->point=temp;
            root->height=1;
            root->left=NULL;
            root->right=NULL;
            root->parent=NULL;
            root->point=temp;
            return;
        }
        
        
        else
        {
            p=root;
            
            while (true)
            {
                if (strcmp(temp->name,p->point->name)<0)
                {
                    if (p->left==NULL)
                    {
                        t->point=temp;
                        t->parent=p;
                        t->height=1;
                        t->left=NULL;
                        t->right=NULL;
                        p->left=t;
                        break;
                    }
                    
                    else
                    {
                        p=p->left;
                    }
                }
                
                
                else if (strcmp(temp->name,p->point->name)>0)
                {
                    if (p->right==NULL)
                    {
                        t->point=temp;
                        t->parent=p;
                        t->height=1;
                        t->left=NULL;
                        t->right=NULL;
                        p->right=t;
                        break;
                    }
                    
                    else
                    {
                        p=p->right;
                    }
                }
            }
        }
        
        
        t2=t;
        Insertheightincrease(t2);
        
        AVLnode *counter=t;
        
        AVLnode *x=new AVLnode;
        AVLnode *y=new AVLnode;
        AVLnode *z=new AVLnode;
        
        
        
        while (counter!=NULL)
        {
            if (counter->parent!=NULL)
            {
                counter=counter->parent;
            }
            else
            {
                break;
            }
            
            
            if (counter->left==NULL && counter->right==NULL)
            {
                break;
            }
            
            else if (counter->left==NULL && counter->right->height>1)//special case
            {
                z=counter;
                y=counter->right;
                rotation(y, z);
                
                return;
            }
            
            else if (counter->right==NULL && counter->left->height>1)//special case
            {
                z=counter;
                y=counter->left;
                rotation(y, z);
                
                return;
            }
            
            if ((counter->right==NULL ||counter->left==NULL) && counter==root)//comes after above 2 special cases, so not executed in case there's only 2 nodes
            {
                break;
            }
            
            if(counter->right!=NULL)
            {
                
                if (abs(counter->left->height-counter->right->height)>1)
                {
                    z=counter;
                    y=(counter->left->point->name>counter->right->point->name)?(counter->left):(counter->right);
                    x=(counter->left->point->name<counter->right->point->name)?(counter->left):(counter->right);
                }
                
                
                if ((y==z->left && x==y->left) || (y==z->right && x==y->right))
                {
                    rotation(y, z);
                    return;
                }
                
                else if (((y==z->left && x==y->right) || (y==z->right && x==y->left)))
                {
                    rotation(x, y);
                    rotation(x, z);
                    return;
                }
            }
        }
    }
    
    
    AVLnode * AVLsearch(char s1[],AVLnode *temp)//searches within AVL tree
    {
        if (temp==NULL)
        {
            return NULL;
        }
        if (strcmp(s1, temp->point->name)==0)
        {
            return temp;
        }
        
        else if (strcmp(s1, temp->point->name)<0)
        {
            return AVLsearch(s1, temp->left);
        }
        else if (strcmp(s1, temp->point->name)>0)
        {
            return AVLsearch(s1, temp->right);
        }
        
        else
        {
            return NULL;
        }
    }
    
    void Deletion(char s1[])//Deletes node in AVL tree
    {
        AVLnode *temp=AVLsearch(s1, root);
        if (temp==NULL)
        {
            return;
        }
        
        AVLnode *p=temp->parent;
        if (temp->left==NULL && temp->right==NULL)
        {
            temp->parent=NULL;
            if (temp==p->left)
            {
                p->left=NULL;
            }
            else if (temp==p->right)
            {
                p->right=NULL;
            }
            
            //fixx the parent and child pointer
            delete temp;
            
        }
        
        else if ((temp->left==NULL && temp->right!=NULL)||(temp->right==NULL && temp->left!=NULL))
        {
            if (temp->left==NULL)
            {
                std::swap(temp->point, temp->right->point);
                delete temp->right;
                temp->right=NULL;
            }
            else
            {
                std::swap(temp->point, temp->left->point);
                delete temp->left;
                temp->left=NULL;
            }
        }
        
        else
        {
            AVLnode *swapnode=temp->left;
            
            while (swapnode!=NULL)
            {
                swapnode=swapnode->right;
            }
            
            std::swap(swapnode->point, temp->point);
            
            if (swapnode->left==NULL)
            {
                p=swapnode->parent;
                swapnode->parent=NULL;
                p->right=NULL;
            }
            else
            {
                
                std::swap(swapnode->point, swapnode->left->point);
                swapnode->left->parent=NULL;
                swapnode->left=NULL;
                
            }
        }
        
        
        AVLnode *counter=p;//check if this backup is effective
        
        Deleteheightdecrease(p);//misnomer, decrease not necessary
        
        
        //rebalance entire thing
        
        
        
        AVLnode *x=new AVLnode;
        AVLnode *y=new AVLnode;
        AVLnode *z=new AVLnode;
        
        
        while (counter!=NULL)
        {
            counter=counter->parent;
            
            if (counter==NULL)
            {
                break;
            }
            if (abs(counter->left->height-counter->right->height)>1)
            {
                z=counter;
                y=(counter->left->point->name>counter->right->point->name)?(counter->left):(counter->right);
                x=(counter->left->point->name<counter->right->point->name)?(counter->left):(counter->right);
            }
            
            
            if ((y==z->left && x==y->left) || (y==z->right && x==y->right))
            {
                rotation(y, z);
                updateheight(z);
                updateheight(y);
                
            }
            
            else if (((y==z->left && x==y->right) || (y==z->right && x==y->left)))
            {
                rotation(x, y);
                rotation(x, z);
                updateheight(z);
                updateheight(y);
                
            }
        }
        
    }
};
AVLtree searchtreeAVL;




class tree//contains root
{
    
public:
    emp *root;
    
    
    
    tree()//constructor
    {
        root=NULL;
        
    }
    
    
    
    
    
    emp *searchandreturn(char s1[],emp *node)//searches and returns a node in the tree o(n)
    {
        if (strcmp(s1, node->name)==0)
        {
            return node;
        }
        
        
        emp *result=NULL;
        
        if (node->no_of_child)
        {
            for (int i=0; i<node->no_of_child; i++)
            {
                result=searchandreturn(s1,node->child.at(i));
                if (result!=NULL)
                {
                    return result;
                }
            }
        }
        
        
        
        return result;
        
    }
    
    
    void printsubtree( emp *head)//prints a subtree starting from "head",O(n)
    {
        queue<emp *> q1;
        
        
        if (root==NULL)
        {
            cout<<"Tree doesn't exist,nothing to display";
            return;
        }
        
        else
        {
            q1.push(head);
            
            while (!q1.empty())
            {
                
                
                int check=head->level;
                cout<<check;
                
                
                while (!q1.empty() )
                {
                    
                    check=q1.front()->level;
                    
                    
                    
                    
                    
                    cout<<q1.front()->name<<" ";
                    
                    if (q1.front()->child.size()>0)
                    {
                        for (int i=0; i<q1.front()->child.size(); i++)
                        {
                            q1.push(q1.front()->child.at(i));
                        }
                    }
                    q1.pop();
                    
                    if (q1.empty())
                    {
                        return;
                    }
                    if (q1.front()->level!=check)
                    {
                        cout<<endl;
                        cout<<q1.front()->level;
                    }
                    
                    
                }
                
                
            }
            
        }
        
        cout<<"\n"<<"\n";
        cout<<std::endl;
        
        
    }
    
    
    void levelandsubordinate(int l)
    {
        queue<emp *> q1;
        
        
        if (root==NULL)
        {
            cout<<"Tree doesn't exist,nothing to display";
            return;
        }
        
        else
        { 
            cout<<"the employees with level "<<l<<" and their respective children are"<<endl;
            emp *temp=root;
            
            q1.push(temp);
            
            do {
                if (l==q1.front()->level)
                {
                    
                    cout<<q1.front()->name<<":";
                    
                    for (int i=0; i<q1.front()->no_of_child; i++)
                    {
                        cout<<q1.front()->child.at(i)->name;
                        if (i<(q1.front()->no_of_child-1))
                        {
                            cout<<",";
                        }
                    }
                    
                    cout<<endl;
                }
                
                
                else if (q1.front()->no_of_child>0 && q1.front()->level<l)
                {
                    for (int i=0; i<q1.front()->no_of_child; i++)
                    {
                        q1.push(q1.front()->child.at(i));
                    }
                }
                
                else if (q1.empty())
                {
                    return;
                }
                q1.pop();
            } while (!q1.empty());
            
        }
    }

    
    emp *Lowestcommonancestor(char s1[],char s2[])//Searches for the lowest common O(n)
    {
        emp *temp1=searchtreeAVL.AVLsearch(s1, searchtreeAVL.root)->point;
        emp *temp2=searchtreeAVL.AVLsearch(s2, searchtreeAVL.root)->point;
        
        if (temp1==NULL ||temp2==NULL)
        {
            cout<<" one of the nodes doesn't exist,";
            return NULL;
        }
        
        int clevel=(temp1->level>temp2->level)?(temp2->level):(temp1->level);
        emp *lower=(temp1->level>temp2->level)?(temp1):(temp2);
        emp *higher=(temp1->level>temp2->level)?(temp2):(temp1);
        
        
        
        while (lower->level!=clevel)
        {
            lower=lower->parent;
        }
        
        
        while (lower!=higher)
        {
            lower=lower->parent;
            higher=higher->parent;
        }
        
        return lower;
    }
    
    
    void removeandrearrange(char s1[],char s2[])//removes node with name s1 and assigns its children to node s2
    {
        emp *temp=searchtreeAVL.AVLsearch(s1, searchtreeAVL.root)->point;
        emp *newparent=searchtreeAVL.AVLsearch(s1, searchtreeAVL.root)->point;
        int oldsize=newparent->no_of_child;
        
        searchtreeAVL.Deletion(s1);
        newparent->child.resize(oldsize+temp->no_of_child);
        newparent->no_of_child=oldsize+temp->no_of_child;
        
        for (int i=0; i<temp->no_of_child; i++)
        {
            newparent->child.at(oldsize+i)=temp->child.at(i);
            newparent->child.at(oldsize+i)->level=newparent->level+1;
            newparent->child.at(oldsize+i)->parent=newparent;
        }
        
        
        for (int i=0; i<temp->no_of_child; i++)
        {
            temp->child.erase(temp->child.begin()+i);
        }
        
        temp->no_of_child=0;
        temp->child.clear();
        
        char x[80];
        
        
        vector<emp*> v2;
        v2=temp->parent->child;
        
        temp->parent->no_of_child--;
        strcpy(x, temp->name);
        temp->parent->child.resize(temp->parent->no_of_child);
        
        for (int i=0; i<v2.size(); i++)
        {int j=0;
            if (strcmp(x, v2.at(i)->name)!=0)
            {
                temp->parent->child.at(j)=v2.at(i);
                j++;
                
            }
            else if (v2.size()>1000)
            {
                try {
                    throw 1;
                } catch (int e) {
                    cout<<"Size of vector>1000,errors may result";
                }
                
                
            }
        }
        
        
        
        
        
        
        
    }
    
    
};


std::fstream f1;
tree t1;
char word[80];





void credits()//displays name and entry number O(1)
{
    std::cout<<"This code was made by Robin Malhotra, entry no:2012TT10951"<<endl;
    f1.getline(word, 80, ' ');
}








void lineofdashes()//displays line of dashes
{
    std::cout<<"----------------------------------------------------------------------------";
    f1.getline(word, 80, ' ');
}




void addpair(char s2[80],char s1[80])//adds node with name s2 as a child of node with name s1
{
    emp *temp=new emp;
    
    if (s1[0]=='\0')
    {
        return;
    }
    temp=t1.searchandreturn(s1,t1.root);
    if (temp==NULL)
    {
        cout<<"element not present";
        return;
    }
    
    temp->no_of_child++;
    temp->child.resize(temp->child.size()+1);
    temp->child.at(temp->child.size()-1)=new emp;
    strcpy(temp->child.at(temp->child.size()-1)->name,s2);
    temp->child.at(temp->child.size()-1)->level=temp->level+1;
    temp->child.at(temp->child.size()-1)->parent=temp;
    searchtreeAVL.Insert(temp->child.at(temp->child.size()-1));
    
}

void createroot()//creates a node O(1)
{
    f1.getline(word, 80, ' ');
    
    t1.root=new emp;
    strcpy(t1.root->name,word);
    t1.root->parent=NULL;
    t1.root->level=1;
    
    cout<<"the C.E.O. is "<<t1.root->name<<"\n";
    
    searchtreeAVL.Insert(t1.root);
    
}


void ancestors(char s1[])//finds ancestors by traversing up a tree
{
    emp *temp=new emp;
    temp=searchtreeAVL.AVLsearch(s1, searchtreeAVL.root)->point;
    cout<<"{";
    
    while (temp!=NULL)
    {
        puts(temp->name);
        temp=temp->parent;
    }
}












int main()
{
    f1.open("/Users/robinmalhotra2/Desktop/csl201ass4.txt",std::ios::in|std::ios::out);
    f1.getline(word, 80, ' ');
    
    
    
    while (f1.good() || !f1.eof())
    {
        
        
        
        if ((strlen(word)==1) && ((word[0])<'9' && (word[0])>'0') )
        {
            switch (word[0])//switch case statement for various options
            {
                case '1':
                {
                    createroot();
                    break;
                }
                case '2':
                {
                    
                    do {
                        
                        
                        char s1[80],s2[80];
                        f1.getline(word, 80, ' ');
                        strcpy(s1, word);
                        
                        if (isdigit(s1[0])||s1[0]=='\0')
                        {
                            break;
                        }
                        f1.getline(word, 80, ' ');
                        strcpy(s2, word);
                        
                        
                        addpair(s1,s2);
                        
                        
                        
                    } while (!((word[0])<'9' && (word[0])>'0') && !f1.eof() && f1.good());
                    
                    
                    
                    break;
                }
                    
                case '3':
                    
                {
                    f1.getline(word, 80, ' ');
                    
                    t1.levelandsubordinate(int(word[0]-'0'));
                    
                    break;
                    
                }
                case '4':
                    
                {
                    f1.getline(word, 80, ' ');
                    
                    ancestors(word);
                    
                    lineofdashes();
                    
                    
                    
                    break;
                    
                }
                case '5':
                {
                    char s1[80],s2[80];
                    f1.getline(word, 80, ' ');
                    strcpy(s1, word);
                    f1.getline(word, 80, ' ');
                    strcpy(s2, word);
                    
                    t1.removeandrearrange(s1,s2);
                    
                    searchtreeAVL.AVLsearch(s1, searchtreeAVL.root);
                    
                    t1.printsubtree(t1.root);
                    
                    break;
                }
                    
                case '6':
                {
                    char s1[80],s2[80];
                    f1.getline(word, 80, ' ');
                    strcpy(s1, word);
                    f1.getline(word, 80, ' ');
                    strcpy(s2, word);
                    cout<<"{";
                    puts(s1);
                    cout<<" ";
                    puts(s2);
                    cout<<"}";
                    
                    
                    emp *result=t1.Lowestcommonancestor(s1,s2);
                    if(result==NULL)
                    {
                        cout<<"No such employee found";
                    }
                    
                    if (result==NULL)
                    {
                        break;
                    }
                    puts(result->name);
                    break;
                }
                    
                    
                case '7':
                {
                    lineofdashes();
                }
                    
                    break;
                    
                case '8':
                {
                    credits();
                    
                }
                    
                    break;
                    
                    
                case '9':
                {
                    cout<<"AVL\n";
                    if (searchtreeAVL.root->left)
                    {
                        cout<<"Left:"<<searchtreeAVL.countnodes(searchtreeAVL.root->left)<<endl;
                    }
                    else
                    {
                        cout<<"Left:0\n";
                    }
                    
                    if (searchtreeAVL.root->right)
                    {
                        cout<<"Right:"<<searchtreeAVL.countnodes(searchtreeAVL.root->right)<<endl;
                    }
                    else
                    {
                        cout<<"Right:0\n";
                    }
                    
                }
                    
                    break;
                    
                
                case '0':
                    
                {
                    f1.getline(word, 80, ' ');
                    if (strcmp(word, "top")==0)
                    {
                        t1.printsubtree(t1.root);
                    }
                    
                    
                    else
                    {
                        t1.printsubtree(t1.searchandreturn(word, t1.root));
                    }
                    
                    cout<<"\n";
                    
                    break;
                    
                }
                default:
                    break;
            }
        }
        
        
        else
        {
            f1.getline(word, 80, ' ');
        }
    }
    
    
    
    if ((strlen(word)==1) && ((word[0])<='9' && (word[0])>'0') )//put here in case of left out option
    {
        switch (word[0])//switch case statement for various options
        {
            case '1':
            {
                createroot();
                break;
            }
            case '2':
            {
                
                do {
                    
                    
                    char s1[80],s2[80];
                    f1.getline(word, 80, ' ');
                    strcpy(s1, word);
                    
                    if (isdigit(s1[0])||s1[0]=='\0')
                    {
                        break;
                    }
                    f1.getline(word, 80, ' ');
                    strcpy(s2, word);
                    
                    
                    addpair(s1,s2);
                    
                    
                    
                } while (!((word[0])<'9' && (word[0])>'0') && !f1.eof() && f1.good());
                
                
                
                break;
            }
                
            case '3':
                
            {
                f1.getline(word, 80, ' ');
                
                t1.levelandsubordinate(int(word[0]-'0'));
                
                break;
                
            }
            case '4':
                
            {
                f1.getline(word, 80, ' ');
                
                ancestors(word);
                
                lineofdashes();
                
                
                
                break;
                
            }
            case '5':
            {
                char s1[80],s2[80];
                f1.getline(word, 80, ' ');
                strcpy(s1, word);
                f1.getline(word, 80, ' ');
                strcpy(s2, word);
                
                t1.removeandrearrange(s1,s2);
                
                searchtreeAVL.AVLsearch(s1, searchtreeAVL.root);
                
                t1.printsubtree(t1.root);
                
                break;
            }
                
            case '6':
            {
                char s1[80],s2[80];
                f1.getline(word, 80, ' ');
                strcpy(s1, word);
                f1.getline(word, 80, ' ');
                strcpy(s2, word);
                cout<<"{";
                puts(s1);
                cout<<" ";
                puts(s2);
                cout<<"}";
                
                
                emp *result=t1.Lowestcommonancestor(s1,s2);
                if(result==NULL)
                {
                    cout<<"No such employee found";
                }
                
                if (result==NULL)
                {
                    break;
                }
                puts(result->name);
                break;
            }
                
                
            case '7':
            {
                lineofdashes();
            }
                
                break;
                
            case '8':
            {
                credits();
                
            }
                
                break;
                
                
            case '9':
            {
                cout<<"AVL\n";
                if (searchtreeAVL.root->left)
                {
                    cout<<"Left:"<<searchtreeAVL.countnodes(searchtreeAVL.root->left)<<endl;
                }
                else
                {
                    cout<<"Left:0\n";
                }
                
                if (searchtreeAVL.root->right)
                {
                    cout<<"Right:"<<searchtreeAVL.countnodes(searchtreeAVL.root->right)<<endl;
                }
                else
                {
                    cout<<"Right:0\n";
                }
                
            }
                
                break;
                
                
            case '0':
                
            {
                f1.getline(word, 80, ' ');
                if (strcmp(word, "top")==0)
                {
                    t1.printsubtree(t1.root);
                }
                
                
                else
                {
                    t1.printsubtree(t1.searchandreturn(word, t1.root));
                }
                
                cout<<"\n";
                
                break;
                
            }
            default:
                break;
        }
    }
    
    
    else
    {
        f1.getline(word, 80, ' ');
    }
    
}

