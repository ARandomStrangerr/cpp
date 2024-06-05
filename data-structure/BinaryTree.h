#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <stdexcept>

// Node structure for the AVL Tree
template <typename T>
class AVLNode {
public:
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const T& value) : data(value), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree class
template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    // Utility function to get the height of the tree
    int height(AVLNode<T>* node) {
        return node == nullptr ? 0 : node->height;
    }

    // Utility function to get the balance factor of the node
    int getBalance(AVLNode<T>* node) {
        return node == nullptr ? 0 : height(node->left) - height(node->right);
    }

    // Right rotate subtree rooted with y
    AVLNode<T>* rightRotate(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    // Left rotate subtree rooted with x
    AVLNode<T>* leftRotate(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    // Recursive function to insert a value in the subtree rooted with node and returns the new root of the subtree
    AVLNode<T>* insert(AVLNode<T>* node, const T& value) {
        // Perform the normal BST insertion
        if (node == nullptr) {
            return new AVLNode<T>(value);
        }

        if (value < node->data) {
            node->left = insert(node->left, value);
        } else if (value > node->data) {
            node->right = insert(node->right, value);
        } else {
            // Duplicate keys not allowed
            return node;
        }

        // Update height of this ancestor node
        node->height = 1 + std::max(height(node->left), height(node->right));

        // Get the balance factor of this ancestor node to check whether this node became unbalanced
        int balance = getBalance(node);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && value < node->left->data) {
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && value > node->right->data) {
            return leftRotate(node);
        }

        // Left Right Case
        if (balance > 1 && value > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && value < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the (unchanged) node pointer
        return node;
    }

    // Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
    AVLNode<T>* deleteNode(AVLNode<T>* root, const T& key) {
        // Perform standard BST delete
        if (root == nullptr) {
            return root;
        }

        if (key < root->data) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->data) {
            root->right = deleteNode(root->right, key);
        } else {
            // Node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode<T>* temp = root->left ? root->left : root->right;

                // No child case
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else { // One child case
                    *root = *temp;
                }
                delete temp;
            } else {
                // Node with two children: Get the inorder successor (smallest in the right subtree)
                AVLNode<T>* temp = minValueNode(root->right);

                // Copy the inorder successor's data to this node
                root->data = temp->data;

                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->data);
            }
        }

        // If the tree had only one node then return
        if (root == nullptr) {
            return root;
        }

        // Update height of the current node
        root->height = 1 + std::max(height(root->left), height(root->right));

        // Get the balance factor of this node (to check whether this node became unbalanced)
        int balance = getBalance(root);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Utility function to find the node with the minimum value
    AVLNode<T>* minValueNode(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        // Loop down to find the leftmost leaf
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Utility function to print the tree (in-order traversal)
    void inOrder(AVLNode<T>* root) const {
        if (root != nullptr) {
            inOrder(root->left);
            std::cout << root->data << " ";
            inOrder(root->right);
        }
    }

public:
	AVLTree() : root(nullptr) {}
	void insert(const T& value) {
		root = insert(root, value);
  }

  void remove(const T& key) {
		root = deleteNode(root, key);
	}

	void print() const {
		inOrder(root);
		std::cout << std::endl;
  }
};

#endif // AVL_TREE_H

