#include <stack.h>

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
    node(int d){
        data = d;
        left = right = NULL;
    }

    ~node(){
        if(left) delete left;
        if(right) delete right;
    }
} node;


class tree{
    public:
    node* root;
    tree(){
        root = NULL;
    }
    void insert(int d){
        node* new_node = new node(d);
        if(root == NULL){
            root = new_node;
            return;
        }
        node* temp = root;
        while(temp){
            if(temp->data == d){
                return;
            }
            if(temp->data > d){
                if(temp->left == NULL){
                    temp->left = new_node;
                    return;
                }
                temp = temp->left;
            }
            else{
                if(temp->right == NULL){
                    temp->right = new_node;
                    return;
                }
                temp = temp->right;
            }
        }
    }

    void after_order(node* root){
        if(root == NULL) return;
        after_order(root->right);
        after_order(root->left);
        std::cout<<root->data<<" ";
    }
};

int main(){

    tree t;
    t.insert(10);
    t.insert(5);
    t.insert(15);
    t.insert(3);
    t.insert(7);
    t.after_order(t.root);

    std::cout<<std::endl;
}


