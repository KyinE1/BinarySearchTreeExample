/*
	bstree.h
	Date Written: 4/15
	Created by: Kyin Edwards
*/

#ifndef BSTREE_H
#define BSTREE_H

#include <iostream>
#include <queue>

template <class K, class V> class bstree; // Forward declaration

template <class K, class V> struct node {
	K key;
	V value;
	node<K, V>* l_ptr; // Points to node's left subtree
	node<K, V>* r_ptr; // Points to node's right subtree

	node(const K& key = K(), const V& value = V(), node<K, V>* l_ptr = nullptr, node<K, V>* r_ptr = nullptr) {
		this->key = key;
		this->value = value;
		this->l_ptr = l_ptr;
		this->r_ptr = r_ptr;
	}
};

template <class K, class V> class bstree {
private:
	node<K, V>* root_node_ptr; // Points to root node of tree or be nullptr if empty
	size_t tree_size; // Store tree size, n elements or values currently stored in the binary search tree

	// Recursive functions (example of function overloading for the order functions).
	void preorder(node<K, V>*) const;
	void inorder(node<K, V>*) const;
	void postorder(node<K, V>*) const;
	void destroy(node<K, V>*) const;
	node<K, V>* clone(node<K, V>*) const;
	size_t height(node<K, V>*) const;

public:
	bstree();
	~bstree();
	bstree(const bstree<K, V>&);
	bstree<K, V>& operator=(const bstree<K, V>&);
	void clear();
	size_t size() const;
	size_t height() const;
	bool empty() const;
	const K& min() const;
	const K& max() const;
	bool insert(const K&, const V&);
	bool remove(const K&);
	const node<K, V>* find(const K&) const;
	void preorder() const;
	void inorder() const;
	void postorder() const;
	void level_order() const;
};

/* Default constructor. Sets the root pointer data member of the tree to nullptr and tree size to 0. */
template <class K, class V> bstree<K, V>::bstree() {
	root_node_ptr = nullptr;
	tree_size = 0;
}

/* Destructor. Call the clear() method. */
template <class K, class V> bstree<K, V>::~bstree() {
	clear();
}

/*
	Copy constructor. Make a copy of the tree nodes by performing a modified preorder traversal.

	@param x Object of the class.
*/
template <class K, class V> bstree<K, V>::bstree(const bstree<K, V>& x) {
	// Copy the other bstree object's size
	tree_size = x.tree_size;

	// Copy the nodes of the other bstree object
	root_node_ptr = clone(x.root_node_ptr);
	
	/*
	if (x != nullptr) {
		std::cout << x->value << ' '; // Visit node pointed to by x (using recursion)
		bstree(x->l_ptr);
		bstree(x->r_ptr);
	}
	*/
}

/*
	Overloaded assignment operator.

	@param x Object of the class.
*/
template <class K, class V> bstree<K, V>& bstree<K, V>::operator=(const bstree<K, V>& x) {
	if (this != &x) {
		clear(); // Make left bstree object empty
		tree_size = x.tree_size; // Copy the other bstree object's size
		root_node_ptr = clone(x.root_node_ptr); // Copies nodes of other bstree object
	}

	return *this;
}

/* Set the tree back to empty state, deleting all nodes of the tree and setting size to 0. */
template <class K, class V> void bstree<K, V>::clear() {
	destroy(root_node_ptr);
	root_node_ptr = nullptr;

	tree_size = 0;
}

/* @return the tree size. */
template <class K, class V> size_t bstree<K, V>::size() const {
	return tree_size;
}

/* Call private member height(). */
template <class K, class V> size_t bstree<K, V>::height() const {
	return height(root_node_ptr);
}

/* @return the tree height. */
template <class K, class V> size_t bstree<K, V>::height(node<K, V>* p) const {
	size_t tree_height = 0;
	// p : pointer to a tree node
    // l_height : computed height of node's left subtree
    // r_height : computed height of node's right subtree
	
	if (p == nullptr)
		return tree_height;
	
	size_t l_height = height(p->l_ptr);
	size_t r_height = height(p->r_ptr);
	
    if (l_height > r_height)
        return l_height + 1;
    else 
        return r_height + 1;
}

/* @return true if the tree size is 0. */
template <class K, class V> bool bstree<K, V>::empty() const {
	return (tree_size == 0);
}

/* @return the minimum key in the tree. */
template <class K, class V> const K& bstree<K, V>::min() const {
	node <K, V>* p = root_node_ptr;
	// Continue down the tree.
		while (p->l_ptr != nullptr)
			p = p->l_ptr;

	return p->key;
}

/* @return the maximum key in the tree. */
template <class K, class V> const K& bstree<K, V>::max() const {	
	node <K, V>* p = root_node_ptr;
	// Continue up the tree.
		while (p->r_ptr != nullptr)
			p = p->r_ptr;

	return p->key;
}

/*
	Insert a key and value into binary search tree.

	@param key pass into binary tree search if it does not exist.

	@param value pass into binary tree search.

	@return If the key already exists in the tree, return false, else a new tree node containing the key and value will be inserted in the correct spot to maintain the ordered property of the binary search tree, size incremented and return true.
*/
template <class K, class V> bool bstree<K, V>::insert(const K& key, const V& value) {
	node<K, V>* p; // p: pointer to a tree node
	node<K, V>* parent; // pointer to the parent node of the node to delete from the tree (or nullptr if deleting the root node)
	// node<K, V>* new_node = new node<K, V>(p); // Allocate a new tree node, new_node. new_node: pointer used to create a new tree node
	std::queue<node<K, V>*> q; // q: a queue of pointers to tree nodes(nullptr if p points to the root node)

	// Start at the root of the tree.
	p = root_node_ptr;
	parent = nullptr;

	// Search the tree for a null link or a duplicate key (if duplicates are disallowed).
	while (p != nullptr && key != p->key) {
		parent = p;
		if (key < p->key)
			p = p->l_ptr;
		else
			p = p->r_ptr;
	}

	// If duplicates are disallowed, signal that insertion has failed.
	if (p != nullptr)
		return false;

	// Otherwise, create a tree node and insert it as a new leaf node (create a new tree node new_node to contain key and value).
	node<K, V>* new_node = new node<K, V>(key, value);

	if (parent == nullptr)
		root_node_ptr = new_node;
	else {
		if (new_node->key < parent->key)
			parent->l_ptr = new_node;
		else
			parent->r_ptr = new_node;

	}

	tree_size = tree_size + 1;

	// If duplicates are disallowed, signal that insertion has succeeded. 
	return true;
}

/*
	Remove specified key from binary search tree.

	@param key Specified key to be removed.

	@return False if the key is not in the tree.
*/
template <class K, class V> bool bstree<K, V>::remove(const K& key) {
	node<K, V>* p; //pointer to the node to delete from the tree
	node<K, V>* parent; // pointer to the parent node of the node to delete from the tree (or nullptr if deleting the root node)
	node<K, V>* replace; // pointer to node that will replace the deleted node
	node<K, V>* replace_parent; // pointer to parent of node that will replace the deleted node

	// Start at the root of the tree and search for the key to delete.
	p = root_node_ptr;
	parent = nullptr;
	while (p != nullptr && key != p->key) {
		(parent = p);
		if (key < p->key)
			p = p->l_ptr;
		else
			p = p->r_ptr;
	}

	// If the node to delete was not found, signal failure.
	if (p == nullptr)
		return false;

	if (p->l_ptr == nullptr)
		// Case 1a: p has no children. Replace p with its right child (which is nullptr).
		//   - or -
		// Case 1b: p has no left child but has a right child. Replace p with its right child.
		replace = p->r_ptr;
	else if (p->r_ptr == nullptr)
		// Case 2: p has a left child but no right child. Replace p with its left child.
		replace = p->l_ptr;
	else {
		// Case 3: p has two children. Replace p with its inorder predecessor.

		// Go left...
		replace_parent = p;
		replace = p->l_ptr;

		// ...then all the way to the right.
		while (replace->r_ptr != nullptr) {
			replace_parent = replace;
			replace = replace->r_ptr;
		}

		// If we were able to go to the right, make the replacement node's left child the right child of its parent. Then make the left child of p the replacement's left child.
		if (replace_parent != p) {
			replace_parent->r_ptr = replace->l_ptr;
			replace->l_ptr = p->l_ptr;
		}
		// Make the right child of p the replacement's right child.
		replace->r_ptr = p->r_ptr;
	}

	// Connect replacement node to the parent node of p (or the root if p has no parent).    
	if (parent == nullptr)
		root_node_ptr = replace;
	else {
		if (p->key < parent->key)
			parent->l_ptr = replace;
		else
			parent->r_ptr = replace;
	}

	// Delete the node, decrement the tree size, and signal success.
	// destroy(p->key); // Delete the node pointed to by p
	delete p;
	// or delete[] p->value; // delete node pointed to by p?
	tree_size = tree_size - 1;

	return true;
}

/*
	Attempt to find the specified key in binary search tree.

	@param key Specified key to be found.

	@return if the key is not within the tree, the function returns nullptr, else the address of the node that contained the key.
*/
template <class K, class V> const node<K, V>* bstree<K, V>::find(const K& key) const {
	node<K, V>* p = root_node_ptr; // Pointer to tree node, start at the root of the tree.

	// Search the tree for a null link or a matching key.
	while (p != nullptr && key != p->key) {
		if (key < p->key)
			p = p->l_ptr;
		else
			p = p->r_ptr;
	}

	// p either points to the node with a matching key or is nullptr if the key is not in the tree.
	return p; // Or you can return true if the key is found
}

/* Public member, call to private preorder function. */
template <class K, class V> void bstree<K, V>::preorder() const {
	preorder(root_node_ptr);
}

/* Public member, call to private inorder function. */
template <class K, class V> void bstree<K, V>::inorder() const {
	inorder(root_node_ptr);
}

/* Public member, call to private postorder function. */
template <class K, class V> void bstree<K, V>::postorder() const {
	postorder(root_node_ptr);
}

/*
	Perform preorder traversal of the tree from left to right.

	@param p Pointer to a tree node.
*/
template <class K, class V> void bstree<K, V>::preorder(node<K, V>* p) const {
	if (p != nullptr) {
		std::cout << p->key << ": " << p->value << ' ' << "\n";
		preorder(p->l_ptr);
		preorder(p->r_ptr);
	}
}

/*
	Perform inorder traversal of the tree from left to right.

	@param p Pointer to a tree node.
*/
template <class K, class V> void bstree<K, V>::inorder(node<K, V>* p) const {
	if (p != nullptr) {
		inorder(p->l_ptr);
		std::cout << p->key << ": " << p->value << ' ' << "\n"; // Visit the node pointed to by p
		inorder(p->r_ptr);
	}
}

/*
	Perform postorder traversal of the tree from left to right.

	@param p Pointer to a tree node.
*/
template <class K, class V> void bstree<K, V>::postorder(node<K, V>* p) const {
	if (p != nullptr) {
		postorder(p->l_ptr);
		postorder(p->r_ptr);
		std::cout << p->key << ": " << p->value << ' ' << "\n"; // Visit the node pointed to by p
	}
}

/* Perform level order traversal of tree from left to right. */
template <class K, class V> void bstree<K, V>::level_order() const {
	node<K, V>* p; // pointer to a tree node
	std::queue<node<K, V>*> q; // a queue of pointers to tree nodes

	// If tree is empty, return.
	if (root_node_ptr == nullptr)
		return;

	q.push(root_node_ptr);

	while (!q.empty()) {
		// Remove front item in the queue and visit it.
		p = q.front();
		q.pop();
		// Visit the node pointed to by p
		std::cout << p->key << ": " << p->value << ' ' << "\n";

		// Insert left child of p into queue.
		if (p->l_ptr != nullptr)
			q.push(p->l_ptr);

		// Insert right child of p into queue.
		if (p->r_ptr != nullptr)
			q.push(p->r_ptr);
	}
}

/*
	Recursively deletes the nodes of a bstree object (modified postorder traversal of the tree).

	@param p Pointer to a tree node.
*/
template <class K, class V> void bstree<K, V>::destroy(node<K, V>* p) const {
	if (p != nullptr) {
		destroy(p->l_ptr);
		destroy(p->r_ptr);
	}

	// delete[] p->value; // delete node pointed to by p
	delete p;
}

/*
	Recursively copies the nodes of a bstree object. This is a modified version of a preorder traversal of the tree.

	@param p Pointer to a tree node.

	@return The new_node after reaching nullptr, else nullptr.
*/
template <class K, class V> node<K, V>* bstree<K, V>::clone(node<K, V>* p) const {
	if (p != nullptr) {
		// Make a copy of the node pointed to by p.
		node<K, V>* new_node = new node<K, V>(); // Allocate a new tree node, new_node
		new_node->key = p->key;
		new_node->value = p->value;

		// Recursively copy the left and right subtrees of the node pointed to by p.
		new_node->l_ptr = clone(p->l_ptr);
		new_node->r_ptr = clone(p->r_ptr);

		return new_node;
	}
	else
		return nullptr;

}

#endif