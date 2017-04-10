#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//http://www.geeksforgeeks.org
//-------------------------------
struct node {
    int key;
    struct node *left;
    struct node *right;
    int height;
};

struct node* newNode(int key) {
    struct node* node = (struct node*)
                        malloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return node;
}

int heightOfSubtree(struct node *node) {
    if (node == NULL) {
        return 0;
    }
    else {
    	return node->height;
    }
}

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

int balanceOfSubtree(struct node *node) {
    if (node == NULL) {
        return 0;
    }
    return heightOfSubtree(node->left) - heightOfSubtree(node->right);
}
//-----------------------------
struct node *rotateFromLeft (struct node *y) {
		struct node *x = y->left;

		y->left = x->right;
	    x->right = y;

	    y->height = max(heightOfSubtree(y->left), heightOfSubtree(y->right))+1;
	    x->height = max(heightOfSubtree(x->left), heightOfSubtree(x->right))+1;

	    return x;
}

struct node *rotateFromRight(struct node *x) {
		struct node *y = x->right;

		x->right = y->left;
		y->left = x;

	    x->height = max(heightOfSubtree(x->left), heightOfSubtree(x->right))+1;
	    y->height = max(heightOfSubtree(y->left), heightOfSubtree(y->right))+1;

	    return y;
}

struct node* doubleRotateFromRight(struct node* x) {
	x->right = rotateFromLeft(x->right);
	return rotateFromRight(x);
}

struct node* doubleRotateFromLeft(struct node* x) {
	x->left = rotateFromRight(x->left);
	return rotateFromLeft(x);
}


struct node* insert(struct node* node, int key) {

	//Normal BST tree insertion
	if (node == NULL) {
		return newNode(key);
	}
	if (key < node->key) {
		node->left = insert(node->left, key);
	}
	else if(key == node->key) {
		//printf("Klucz %d dodano juz wczesniej\n", key);
		return node;
	}
	else {
		node->right = insert(node->right, key);
	}

	node->height = max(heightOfSubtree(node->left), heightOfSubtree(node->right)) + 1;

	int balance = balanceOfSubtree(node);

		// Left Left
	    if (balance > 1 && key < node->left->key) {
	        return rotateFromLeft(node);
	    }
	    // Right Right
	    if (balance < -1 && key > node->right->key) {
	        return rotateFromRight(node);
	    }
	    // Left Right
	    if (balance > 1 && key > node->left->key) {
	    	return doubleRotateFromLeft(node);
	    }
	    // Right Left
	    if (balance < -1 && key < node->right->key) {
	    	return doubleRotateFromRight(node);
	    }

	return node;
}

struct node* delete(struct node* node, int key) {

    if (node == NULL) {
        return node;
    }

    if ( key < node->key ) {
    	node->left = delete(node->left, key);
    }
    else if( key > node->key ) {
    	node->right = delete(node->right, key);
    }
    else {
        if( (node->left == NULL) || (node->right == NULL) ) {
            struct node *temp;
            if (node->left != NULL) {
            	temp = node->left;
            }
            else {
            	temp = node->right;
            }

            if(temp == NULL) {
                temp = node;
                node = NULL;
            }
            else {
             *node = *temp;
            }
            free(temp);
        }
        else {
            struct node* temp = node->right;
            while (temp->left != NULL) {
                    temp = temp->left;
            }

            node->key = temp->key;

            node->right = delete(node->right, temp->key);
        }
    }

    if (node == NULL)
      return node;

    node->height = max(heightOfSubtree(node->left), heightOfSubtree(node->right)) + 1;

    int balance = balanceOfSubtree(node);
    int leftBalance = balanceOfSubtree(node->left);
    int rightBalance = balanceOfSubtree(node->right);

    // Left Left
    if (balance > 1 && leftBalance >= 0) {
        return rotateFromLeft(node);
    }
    // Left Right
    if (balance > 1 && leftBalance < 0) {
    	return doubleRotateFromLeft(node);
    }
    // Right Right
    if (balance < -1 && rightBalance <= 0) {
        return rotateFromRight(node);
    }
    // Right Left
    if (balance < -1 && rightBalance > 0) {
    	return doubleRotateFromRight(node);
    }

    return node;
}

struct node* find(struct node *root, int key) {
	if(root == NULL) {
		//printf("Drzewo puste --> nie znaleziono klucza %d\n", key);
		printf("0\n");
		return NULL;
	}
	else {
		if (key == root->key) {
			//printf("Znaleziono klucz %d\n", key);
			printf("1\n");
			return root;
		}
		else if (key < root->key) {
			if (root->left == NULL) {
				//printf("Nie znaleziono klucza %d\n", key);
				printf("0\n");
				return NULL;
			}
			else {
				find(root->left, key);
			}
		} else if (key > root->key) {
			if (root->right == NULL){
				//printf("Nie znaleziono klucza %d\n", key);
				printf("0\n");
				return NULL;
			}
			else{
				find(root->right, key);
			}
		}
	}
	return root;
}

void minNode(struct node* root) {
	if (root == NULL) {
		//printf("Drzewo puste --> nie ma wartości minimalnej\n");
		printf("\n");
	}
	else {
		if (root->left != NULL) {
			minNode(root->left);
		}
		else {
			//printf("Min = %d\n", root->key);
			printf("%d\n", root->key);
		}
	}
}

void maxNode(struct node *root) {
	if (root == NULL) {
		//printf("Drzewo puste --> nie ma wartości maksymalnej\n");
		printf("\n");
	}
	else {
		if (root->right != NULL) {
			maxNode(root->right);
		}
		else {
			//printf("Max = %d\n", root->key);
			printf("%d\n", root->key);
		}
	}
}

void inorder(struct node* root) {
	if (root != NULL) {
		inorder(root->left);
		printf("%d ", root->key);
		inorder(root->right);
	}
}


int main(int argc, char *argv[]) {
		FILE *file;
		struct node *root = NULL;

		int n = 0; //n - Number of different operations

		if ((file=fopen(argv[1], "r"))==NULL) {
			printf ("I can't open file %s to read!\n", argv[1]);
			exit(1);
		}

		else {
			fscanf(file, "%d", &n);
		}
		    //Checking the conditions of exercise
		if(n <= 0 || n > 2000){
			printf("You gave wrong n. Program will exit. \n");
		    return 0;
		}
		else{
			//printf("n = %d\n", n);
		}

		char* operation = malloc(1000 * sizeof(char));
		int number;
		int j;

		for(j = 0; j < n; j++){
			fscanf(file, "%s ",operation);
		    if(operation == ""){
		    	//printf("%d: ", j+1);
		    	printf("Wrong data. Program will exit. \n");
		        exit(0);
		    }
		    else if (strcmp(operation, "insert") == 0){
		    	//printf("%d: ", j+1);
		    	fscanf(file, "%d ",&number);
		    	//printf("Insert %d\n", numberber);
		    	root = insert(root, number);
		    }
		    else if (strcmp(operation, "delete") == 0) {
		    	//printf("%d: ", j+1);
		    	fscanf(file, "%d ",&number);
		    	//printf("Delete %d\n", number);
		    	root = delete(root, number);
		    }
		    else if (strcmp(operation, "min") == 0) {
		    	//printf("%d: ", j+1);
		    	minNode(root);
		    }
		    else if (strcmp(operation, "max") == 0) {
		    	//printf("%d: ", j+1);
		    	maxNode(root);
		    }
		    else if (strcmp(operation, "find") == 0) {
		    	//printf("%d: ", j+1);
		    	fscanf(file, "%d ",&number);
		    	find(root, number);
		    }
		    else if (strcmp(operation, "inorder") == 0) {
		    	//printf("%d: Inorder: ", j+1);
		    	inorder(root);
		    	printf("\n");
		    }
		    else {
		    	return 0;
		    }
		}

		return 0;
}
