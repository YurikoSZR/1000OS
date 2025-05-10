#include <stack.h>
int max(int a,int b){
    return a>b?a:b;
}

typedef struct node{
    int data;
    int height;
    struct node *left, *right;
    node(int d) : data(d), height(1), left(NULL), right(NULL) {}
    int isBlanced(){
        int lh = Lheight();
        int rh = Rheight();
        if(lh - rh > 1 || rh - lh > 1)
            return 0;
        else
            return 1;
    }
    int Lheight(){
        return this->left==NULL?0:this->left->height;
    }
    int Rheight(){
        return this->right==NULL?0:this->right->height;
    } 
}node;

class avltree{
    private:
    node* _insert(node* ptr,node* index){
        if(index == NULL)return index;
        if(ptr->data== index->data)return index;
        if(ptr->data > index->data){
            index->right = _insert(ptr->right,index);
            return keepBalance(index);
        }
        else{
            index->left = _insert(ptr->left,index);
            return keepBalance(index);
        }
    }
    public:
    node *root;node *nil;
    avltree(){
        nil = new node(0x8fffffff);
        root = nil;
    }

    node* LRotate(node * index);
    node* RRotate(node * index);
    node* keepBalance(node * index);

    void insert(int data){
        node* NEW= new node(data);
        root=_insert(NEW,root);
    }
};

node* avltree::LRotate(node * index){
    if(index == root)root = index->right;
    node* right = index->right;
    index->right = right->left;
    index->height = max(index->left->height,index->right->height)+1;
    right->left = index;
    right->height = max(right->left->height,right->right->height)+1;
    return right;
}

node* avltree::RRotate(node * index){
    if(index == root)root = index->left;
    node* left = index->left;
    index->left = left->right;
    index->height = max(index->left->height,index->right->height)+1;
    left->right = index;
    left->height = max(left->left->height,left->right->height)+1;
    return left;
}

node* avltree::keepBalance(node * index){
    if(index->isBlanced())return index;

    if(index->Lheight() > index->Rheight()){
        if(index->left->Rheight() < index->left->Lheight())
            index->left = LRotate(index->left);
        return RRotate(index);
    }
    else{
        if(index->right->Lheight() < index->right->Rheight())
            index->right = RRotate(index->right);
        return LRotate(index);
    }
}

void print(node* root){
    if(root == NULL)return;
    print(root->left);
    print(root->right);
    std::cout<<root->data<<" ";
}


int main(){
    avltree tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    print(tree.root);
}