#include <stdio.h>
#include <stdlib.h>

int avlCost;            //avlCost and splayCost to count costs
int splayCost;

/*
  node is a structure representing a node in a linked list.
  Used linked list to store values we get from the input file.
*/
typedef struct node {
    int data;
    struct node * next;
}node;

/*
  treeNode is a structure representing a node in a tree.
  It is for both avl and splay trees
*/

typedef struct treeNode {
    int key;
    int balance;
    struct treeNode * left;
    struct treeNode * right;
    struct treeNode * parent;
    int occurrence;

}treeNode;
/*
  splayTree is a structure representing a splay tree.

*/
typedef struct splayTree {
    treeNode * splayNode;
}splayTree;

node * addNode(int data) {
    node * new = (node * ) malloc(sizeof(node));
    new -> data = data;
    new -> next = NULL;
    return new;
}
/*
  addToList is a function that adds a new node with the given data
  to the end of the linked list pointed to by head.
*/
void addToList(node ** head, int data) {
    node * new = addNode(data);

    if (( * head) == NULL) {
        ( * head) = new;
        return;
    }

    node * last = ( * head);
    while (last -> next != NULL) {

        last = last -> next;

    }
    last -> next = new;
}
splayTree * newSplayTree() {
    splayTree * tree = malloc(sizeof(splayTree));
    tree -> splayNode = NULL;
    return tree;
}
/*
  getLength is a function that returns the number of nodes in the
  linked list pointed to by head. To determine the upper limit of the for loop I use in insert operations
*/
int getLength(node * head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head -> next;
    }
    return length;
}


treeNode * addAvlNode(int key) {

    treeNode * newNode = malloc(sizeof(treeNode));
    newNode -> key = key;
    newNode -> balance = 0;
    newNode -> left = NULL;
    newNode -> right = NULL;
    newNode -> occurrence = 1;
    return newNode;
}
                                                                /*addAvlNode and addSplayNode are function that adds
                                                                           nodes to the related tree.*/
treeNode * addSplayNode(int key) {

    treeNode * new = malloc(sizeof(treeNode));
    new -> parent = NULL;
    new -> left = NULL;
    new -> right = NULL;
    new -> key = key;
    return new;
}

/*
getHeight is a recursive function that returns the height of the tree
rooted at root.
*/
int getHeight(treeNode * root) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = 0;
    int rightHeight = 0;
    if (root -> left != NULL) {
        leftHeight = getHeight(root -> left);
    }
    if (root -> right != NULL){
        rightHeight = getHeight(root -> right);
    }
    if (leftHeight > rightHeight){
        return leftHeight + 1;
    }
    else {
        return rightHeight + 1;
    }
}
/*
rightRotate is a function that performs a right rotation on the AVL tree
*/
treeNode * rightRotate(treeNode * root) {
    treeNode * new = root -> left;
    root -> left = new -> right;
    new -> right = root;
    return new;
}
/*
leftRotate is a function that performs a right rotation on the AVL tree
*/
treeNode * leftRotate(treeNode * root) {
    treeNode * new = root -> right;
    root -> right = new -> left;
    new -> left = root;

    return new;
}
/*
rightRotateSplay is a function that performs a right rotation on the splay
tree
*/
void rightRotateSplay(splayTree * tree, treeNode * node) {

    treeNode * temp = node -> left;
    node -> left = temp -> right;
    if (temp -> right != NULL) {
        temp -> right -> parent = node;
    }
    temp -> parent = node -> parent;
    if (node -> parent == NULL) {
        tree -> splayNode = temp;
    } else if (node == node -> parent -> right) {
        node -> parent -> right = temp;
    } else {
        node -> parent -> left = temp;
    }
    temp -> right = node;
    node -> parent = temp;
}
/*
leftRotateSplay is a function that performs a right rotation on the splay
tree
*/
void leftRotateSplay(splayTree * tree, treeNode * node) {
    treeNode * temp = node -> right;
    node -> right = temp -> left;
    if (temp -> left != NULL) {
        temp -> left -> parent = node;
    }
    temp -> parent = node -> parent;
    if (node -> parent == NULL) {
        tree -> splayNode = temp;
    } else if (node == node -> parent -> left) {
        node -> parent -> left = temp;
    } else {
        node -> parent -> right = temp;
    }
    temp -> left = node;
    node -> parent = temp;
}
//splay is a function that performs splaying operations on the splay tree.
void splay(splayTree * tree, treeNode * node) {
    while (node -> parent != NULL) {
        if (node -> parent == tree -> splayNode) {
            if (node == node -> parent -> left) {
                rightRotateSplay(tree, node -> parent);//Zig Rotation
            } else {
                leftRotateSplay(tree, node -> parent);//Zag Rotation
            }
        } else {
            treeNode * parent = node -> parent;
            treeNode * grandparent = parent -> parent;
            if (parent -> parent -> left == parent && node -> parent -> left == node) {
                rightRotateSplay(tree, grandparent);  //Zig-Zig Rotation
                rightRotateSplay(tree, parent);
            } else if (parent -> parent -> right == parent && node -> parent -> right == node) {
                leftRotateSplay(tree, grandparent);     //Zag-Zag Rotation
                leftRotateSplay(tree, parent);
            } else if (parent -> parent -> left == parent && node -> parent -> right == node) {
                leftRotateSplay(tree, parent);      //Zag-Zig Rotation
                rightRotateSplay(tree, grandparent);
            } else if (parent -> parent -> right == parent && node -> parent -> left == node) {
                rightRotateSplay(tree, parent);    //Zig-Zag Rotation
                leftRotateSplay(tree, grandparent);
            }
        }
    }
}

//find_depth Function returns the depth of the node that contains given key
int find_depth(treeNode * root, int key, int depth) {

    if (root == NULL) {
        return -1;
    }

    if (root -> key == key) {
        return depth;
    }

    int depth_left = find_depth(root -> left, key, depth + 1);
    if (depth_left != -1) {
        return depth_left;
    }

    int depth_right = find_depth(root -> right, key, depth + 1);
    if (depth_right != -1) {
        return depth_right;
    }

    return -1;
}
/*
insertSplay is a function that inserts a new node with the given key
into the splay tree
*/
void insertSplay(splayTree * tree, treeNode * node) {
    treeNode * current = NULL;
    treeNode * temp = tree -> splayNode;

    while (temp != NULL) {
        current = temp;
        if (node -> key < temp -> key) {
            splayCost++;
            temp = temp -> left;
        } else if (node -> key > current -> key) {          //traverse until reaching the NULL node, position to insert
            splayCost++;
            temp = temp -> right;
        } else {
            return;
        }
    }
    node -> parent = current;

    if (current == NULL)
        tree -> splayNode = node;
    else if (node -> key < current -> key) {
        current -> left = node;
    } else {
        current -> right = node;
    }

    int firstDepth = find_depth(tree -> splayNode, node -> key, 0);
    splay(tree, node);                                                             //Calculate the number of depth
                                                                                    // levels the nodes have moved through
    int lastDepth = find_depth(tree -> splayNode, node -> key, 0);
    int depthDifference = abs(lastDepth - firstDepth);

    splayCost += depthDifference;                                                   //Increment splayCost by depth change

}
/*
balance is a function that balances the given avl tree if there exist an unbalance case
*/
treeNode * balance(treeNode * root) {
    if (root == NULL) return root;

    int leftHeight = 0;
    if (root -> left != NULL) leftHeight = getHeight(root -> left);
    int rightHeight = 0;                                                        /*Getting heights of left and right subtrees
                                                                                  to check the balance. Then if there is an unbalanced
                                                                                  situation perform required rotation to balance it*/
    if (root -> right != NULL) rightHeight = getHeight(root -> right);
    root -> balance = leftHeight - rightHeight;
    if (root -> balance == 2) {
        int leftBalance = 0;
        if (root -> left != NULL) {
            leftBalance = root -> left -> balance;
        }

        if (leftBalance == -1) {
            root -> left = leftRotate(root -> left);                    //Incrementing cost by 1 for single rot.
                                                                                //by 2 for double rot.
            avlCost++;
        }
        avlCost++;
        root = rightRotate(root);
    } else if (root -> balance == -2) {
        int rightBalance = 0;
        if (root -> right != NULL) {
            rightBalance = root -> right -> balance;
        }
        if (rightBalance == 1) {
            root -> right = rightRotate(root -> right);
            avlCost++;
        }
        avlCost++;
        root = leftRotate(root);

    }
    return root;
}
/*
insert is a function that inserts a new node with the given key
into the avl tree
*/
treeNode * insert(treeNode * root, int key) {
    if (root -> key == key) {
        root -> occurrence++;
        return root;
    }

    if (key < root -> key) {

        avlCost++;
        if (root -> left == NULL) {
            root -> left = addAvlNode(key);

        } else root -> left = insert(root -> left, key);
    } else if (key > root -> key) {
        avlCost++;                                           //traverse until reaching the NULL node, position to insert
                                                             //Increment avlCost by 1 when a comparison performed
        if (root -> right == NULL) {
            root -> right = addAvlNode(key);

        } else root -> right = insert(root -> right, key);
    }

    return balance(root);                                       //pass it to the balance function to check if there exist
                                                                // an unbalance
}
void preorderTraversal(treeNode * root) {
    if (root == NULL) return;
    printf("%d ", root -> key);
    preorderTraversal(root -> left);
    preorderTraversal(root -> right);
}

int main(int argc,char* argv[]) {
    node * head = NULL;
    if(argc==1){
        printf("Error message!");
    }
    if(argc>=2) {
        FILE* fp = fopen(argv[1], "r");

    int number;
    while (fscanf(fp, "%d", & number) == 1) {
        addToList( & head, number);                 //Add the numbers we get from txt file to a linked list
    }
    fclose(fp);
    int length = getLength(head);                              //Length = number count in the txt file
    int i;

    node * current = head;                                      //temporary current pointer to traverse over list
    treeNode * avlTree = addAvlNode(current -> data);       //Creating the trees and adding their first nodes
    splayTree * splayTree = newSplayTree();
    insertSplay(splayTree, addSplayNode(current -> data));
    current = current -> next;

    for (i = 1; i < length; i++) {
        avlTree = (insert(avlTree, current -> data));

        insertSplay(splayTree, addSplayNode(current -> data));
        current = current -> next;
    }

    printf("avlTree Tree : \n");
    preorderTraversal(avlTree);
    printf("\n Avl Total Cost : %d\n", avlCost);
                                                                //Printing preOrderTraversals and costs
    printf("\nSplay Tree : \n");
    preorderTraversal(splayTree -> splayNode);
    printf("\nSplay Total  Cost : %d", splayCost);

    return 0;
}
}