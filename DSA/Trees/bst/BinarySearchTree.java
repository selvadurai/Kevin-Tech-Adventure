package com.tree.bst;



class Node {
    int key;
    Node left, right;
 
    public Node(int item) {
        key = item;
        left = right = null;
    }
}
 
class BinarySearchTree {
    Node root;
 
    BinarySearchTree() {
        root = null;
    }
 
    // insert a key into the tree
    void insert(int key) {
        root = insertRec(root, key);
    }
 
    // recursively insert a key into a subtree
    Node insertRec(Node root, int key) {
        if (root == null) {
            root = new Node(key);
            return root;
        }
 
        if (key < root.key)
            root.left = insertRec(root.left, key);
        else if (key > root.key)
            root.right = insertRec(root.right, key);
 
        return root;
    }
 
    // delete a key from the tree
    void delete(int key) {
        root = deleteRec(root, key);
    }
 
    // recursively delete a key from a subtree
    Node deleteRec(Node root, int key) {
        if (root == null)
            return root;
 
        if (key < root.key)
            root.left = deleteRec(root.left, key);
        else if (key > root.key)
            root.right = deleteRec(root.right, key);
        else {
        	
        	//Case 1  and Case 2
            // node with only one child or no child
            if (root.left == null)
                return root.right;
            else if (root.right == null)
                return root.left;
            
            //Case 3
 
            // node with two children: get the inorder successor (smallest
            // in the right subtree)
            root.key = minValue(root.right);
 
            // delete the inorder successor
         
            root.right = deleteRec(root.right, root.key);
        }
 
        return root;
    }
 
    // recursively find the minimum value in a subtree
    int minValue(Node root) {
        int minv = root.key;
        while (root.left != null) {
            minv = root.left.key;
            root = root.left;
        }
        return minv;
    }
 
    // print the tree in sorted order
    void inorder() {
        inorderRec(root);
    }
 
    // recursively print a subtree in sorted order
    void inorderRec(Node root) {
        if (root != null) {
            inorderRec(root.left);
            System.out.print(root.key + " ");
            inorderRec(root.right);
        }
    }

 

    public static void main(String[] args) {
        BinarySearchTree tree = new BinarySearchTree();
 
        // insert some keys into the tree
        tree.insert(50);
        tree.insert(30);
        tree.insert(20);
        tree.insert(40);
        tree.insert(70);
        tree.insert(60);
        tree.insert(80);
 
        // print the sorted values of the tree
        System.out.println("Inorder traversal of the binary search tree:");
        tree.inorder();
 
        // delete a key from the tree
        tree.delete(20);
 
        // print the sorted values of the tree after deletion
        System.out.println("\nInorder traversal of the binary search tree after deleting 20:");
        tree.inorder();
    }
}
