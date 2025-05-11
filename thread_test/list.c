#include <stdio.h>
#include <stdlib.h>
struct node{
    int index;
    struct node *next;
};
struct node* create_node(int index){
    struct node *n = malloc(sizeof(struct node));
    n->index = index;
    n->next = NULL;
    return n;
}

struct list{
    struct node *head;
};

void insert_node(struct node* n, struct list* l){
    if(l->head == NULL){
        l->head = n;
    }else{
        struct node *current = l->head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = n;
    }
}
void deleta_node(int index,struct list* L){
    struct node *current = L->head;
    struct node *prev = L->head;
    while(current!= NULL){
        if(current->index == index){
            if(current == L->head){
                L->head = current->next;
                free(current);
                return;
            }
            else{
                prev->next = current->next;
                free(current);
            }
        }
        prev = current;
        current = current->next;
    }
}
void pop(int index, struct list* l_1,struct list* l_2){
    struct node *current = l_1->head;
    struct node* prev = l_1->head;
    while(current!= NULL){
        if(current->index == index){
            if(current == l_1->head){
                l_1->head = current->next;
                current->next=NULL;
                insert_node(current,l_2);
                return;
            }
            prev->next = current->next;
            current->next = NULL;
            insert_node(current,l_2);
            return;
        }
        prev = current;
        current = current->next;
    }
}


int main(){
    struct list *l_1 = malloc(sizeof(struct list));
    struct list *l_2 = malloc(sizeof(struct list));
    l_1->head = NULL;
    l_2->head = NULL;
    insert_node(create_node(1),l_1);
    insert_node(create_node(2),l_1);
    pop(1,l_1,l_2);
    insert_node(create_node(3),l_1);
    printf("l_1:\n");
    for(struct node *current = l_1->head; current != NULL; current = current->next){
        printf("%d\n",current->index);
    }
    printf("l_2:\n");
    for(struct node *current = l_2->head; current != NULL; current = current->next){
        printf("%d\n",current->index);
    }

}