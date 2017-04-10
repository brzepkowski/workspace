//android l6 zad 1 parser


#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


struct node
{
    int key;
    struct node *left;
    struct node *right;
    int height;
};

void findNode(struct node *nodes, int szukane);
void inOrder(struct node *nodes);
struct node * max(struct node* node);
struct node * min(struct node* node);
int maxOfTwo(int a, int b);
int getHeight(struct node *nodes);
int getBalance(struct node *nodes);
struct node * maxValueNode(struct node* node);
struct node * minValueNode(struct node* node);
struct node* insertNew(struct node* node, int ins);
struct node* deleteNode(struct node* nodes, int key);

int main(int argc, const char * argv[])
{
    
    typedef FILE* plik;
    int n, num;
    char word[9];
    struct node *nodes = NULL;
    /*
    //nodes=insertNew(nodes,4);
    //maxValueNode(nodes);
    //nodes=deleteNode(nodes,4);
    //nodes=insertNew(nodes,2);
    //nodes=insertNew(nodes, 5);
    //findNode(nodes,2);
    //minValueNode(nodes);
    //maxValueNode(nodes);
    //inOrder(nodes);
    
    //printf("%d", nodes);
    */
    
    plik fp;
    
    fp = fopen(argv[1], "r");
    if( fp == NULL )
    {
        perror("błąd otwarcia pliku");
        exit(-10);
    }
    else{
        fscanf(fp, "%d", &n);
        printf("%d\n", n);
        for(int i=0; i<n; i++){
            fscanf(fp, "%8s", word);
            if(strcmp(word, "inorder")==0){
                inOrder(nodes);
            }else if(strcmp(word, "insert")==0){
                fscanf(fp, "%d", &num);
                nodes=insertNew(nodes, num);
            }else if(strcmp(word, "delete")==0){
                fscanf(fp, "%d", &num);
                nodes=deleteNode(nodes, num);
            }else if(strcmp(word, "find")==0){
                fscanf(fp, "%d", &num);
                findNode(nodes, num);
            }else if(strcmp(word, "max")==0){
                maxValueNode(nodes);
            }else if(strcmp(word, "min")==0){
                minValueNode(nodes);
            }
            i++;
        }
    }
    
    return 0;
}


int getHeight(struct node *nodes)
{
    if (nodes == NULL)
        return 0;
    
    //printf("height %d \n", nodes->height);
    return nodes->height;
}


int maxOfTwo(int a, int b)
{
    int max=0;
    
    if(a<b)
        max=b;
    if(a>=b)
        max=a;
    
    return max;
}


struct node* newNode(int key)
{
    struct node* nodes = (struct node*)
    malloc(sizeof(struct node));
    nodes->key   = key;
    nodes->left   = NULL;
    nodes->right  = NULL;
    nodes->height = 1;
    
    return(nodes);
}


int getBalance(struct node *nodes)
{
    if (nodes == NULL)
        return 0;
    return getHeight(nodes->left) - getHeight(nodes->right);
}


struct node * minValueNode(struct node* node)
{
    struct node* current = node;
    
    while (current->left != NULL){
        current = current->left;}
    printf("min %d \n", current->key);
    
    return current;
}


struct node * maxValueNode(struct node* node)
{
    struct node* current = node;
    
    while (current->right != NULL){
        current = current->right;}
    
    printf("max %d \n", current->key);
    return current;
}


void findNode(struct node *nodes, int szukane){
    int a=0;
    
    if(nodes==NULL)
        printf("brak find\n");
    
    struct node* current = nodes;
    
    while(current!=NULL){
        if(current->key==szukane)
            a=1;
        else if(szukane<current->key)
            current=current->left;
        else
            current=current->right;
    }
    
    if(a==1)
        printf("found 1 \n ");
    else
        printf("not found 0 \n");
}


void inOrder(struct node *nodes){
    if(!(nodes==NULL)){
    inOrder(nodes->left);
    printf("%d \n",nodes->key);
    inOrder(nodes->right);
    }
    return;
}


struct node *newRightRotate(struct node *nodes)
{
    struct node *newRoot = nodes->right;
    nodes->right=newRoot->left;
    newRoot->left =nodes;
    
    nodes->height = maxOfTwo(getHeight(nodes->left), getHeight(nodes->right))+1;
    newRoot->height = maxOfTwo(getHeight(newRoot->right), nodes->height)+1;
    
    return newRoot;
}


struct node *newLeftRotate(struct node *nodes)
{
    struct node *newRoot = NULL;
    
    newRoot=nodes->left;
    nodes->left=newRoot->right;
    newRoot->right=nodes;
    
    nodes->height = maxOfTwo(getHeight(nodes->left), getHeight(newRoot->right))+1;
    newRoot->height = maxOfTwo(getHeight(newRoot->left), nodes->height)+1;
    
    return newRoot;
}


struct node *insertNew(struct node *nodes, int ins)
{
    if(!nodes)
    {
        nodes=(struct node*)malloc(sizeof(struct node));
        nodes->key = ins;
        nodes->left = NULL;
        nodes->right = NULL;
        nodes->height = 1;
        return(nodes);
    }
    
    if(nodes->key > ins){
        nodes->left = insertNew(nodes->left,ins);
        if( getHeight( nodes->left ) - getHeight( nodes->right ) == 2 ){
            if( ins < nodes->left->key )
                nodes = newLeftRotate(nodes);
            else{
                nodes->left = newRightRotate(nodes->left);
                return newLeftRotate(nodes);}
        }
    }
    else if( ins > nodes->key )
    {
        nodes->right = insertNew(nodes->right, ins );
        if( getHeight( nodes->right ) - getHeight( nodes->left ) == 2 ){
            if( ins > nodes->right->key)
                nodes = newRightRotate(nodes);
            else{
                nodes->right = newLeftRotate(nodes->right);
                return newRightRotate(nodes);}
        }
    }
    
     nodes->height = maxOfTwo( getHeight( nodes->left ), getHeight( nodes->right ) ) + 1;
    
    return(nodes);
}


struct node* deleteNode(struct node* nodes, int key)
{
    if (nodes == NULL)
        return nodes;
    
    if ( key < nodes->key )
        nodes->left = deleteNode(nodes->left, key);
    else if( key > nodes->key )
        nodes->right = deleteNode(nodes->right, key);
    else
    {
        if( (nodes->left == NULL) || (nodes->right == NULL) )
        {
            struct node *temp = nodes->left ? nodes->left : nodes->right;
            
            if(temp == NULL)
            {
                temp = nodes;
                nodes = NULL;
            }
            else
                *nodes = *temp;
        }
        else
        {
            struct node* temp = minValueNode(nodes->right);
            
            nodes->key = temp->key;
            
            nodes->right = deleteNode(nodes->right, temp->key);
        }
    }
    
    if (nodes == NULL)
        return nodes;
    
    nodes->height = maxOfTwo(getHeight(nodes->left), getHeight(nodes->right)) + 1;
    
    int balance = getBalance(nodes);
    
    if (balance > 1 && getBalance(nodes->left) >= 0)
        return newRightRotate(nodes);
    if (balance > 1 && getBalance(nodes->left) < 0){
        nodes->left = newLeftRotate(nodes->left);
        return newRightRotate(nodes);}
    
    if (balance < -1 && getBalance(nodes->right) <= 0)
        return newLeftRotate(nodes);
    
    if (balance < -1 && getBalance(nodes->right) > 0){
        nodes->right = newRightRotate(nodes->right);
        return newLeftRotate(nodes);}
    
    return nodes;
}
