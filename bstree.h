#ifndef BSTREE_H
#define BSTREE_H
#include <iostream>
#include <queue>

template <class K, class V> class bstree;
template <class K, class V> struct node {
	K key;
	V value;
	node<K, V>* l_ptr; 
	node<K, V>* r_ptr; 

	/** Constructor.
	 * 
	 * @param key: Key of the pair.
	 * @param value: Value of the pair.
	 * @param l_ptr: .
	 * @param r_ptr: .
	 */
	node(const K& key = K(), const V& value = V(), node<K, V>* l_ptr = nullptr, node<K, V>* r_ptr = nullptr) {
		this->key = key;
		this->value = value;
		this->l_ptr = l_ptr;
		this->r_ptr = r_ptr;
	}
};

template <class K, class V> class bstree {
private:
	node<K, V>* root_node_ptr;
	size_t tree_size; 

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

/** Constructor. */
template <class K, class V> bstree<K, V>::bstree() {
	root_node_ptr = nullptr;
	tree_size = 0;
}

/** Destructor. */
template <class K, class V> bstree<K, V>::~bstree() {
	clear();
}

/** Copy constructor.
 *
 * @param x: Applied to the data members.
 */
template <class K, class V> bstree<K, V>::bstree(const bstree<K, V>& x) {
	tree_size = x.tree_size;

	// Copy the nodes of the other bstree object
	root_node_ptr = clone(x.root_node_ptr);
}

/** Overloaded assignment operator.
 *
 * @param x: Applied to the data members.
 * @return: Pointer to this object.
 */
template <class K, class V> bstree<K, V>& bstree<K, V>::operator=(const bstree<K, V>& x) {
	if (this != &x) {
		clear();
		tree_size = x.tree_size; 
		root_node_ptr = clone(x.root_node_ptr); 
	}

	return *this;
}

/** Set tree to default state. */
template <class K, class V> void bstree<K, V>::clear() {
	destroy(root_node_ptr);
	root_node_ptr = nullptr;

	tree_size = 0;
}

/** Accessor methods. */
template <class K, class V> size_t bstree<K, V>::size() const {
	return tree_size;
}

template <class K, class V> size_t bstree<K, V>::height() const {
	return height(root_node_ptr);
}

/** Get the tree's height.
 * 
 * @return: The tree's height. 
 */
template <class K, class V> size_t bstree<K, V>::height(node<K, V>* p) const {
	size_t tree_height = 0;
	// p: Pointer to a tree node.
    // l_height: Computed height of node's left subtree.
    // r_height: Computed height of node's right subtree.
	
	// Pointer does not point to a node.
	if (p == nullptr)
		return tree_height;
	
	size_t l_height = height(p->l_ptr);
	size_t r_height = height(p->r_ptr);
	
    if (l_height > r_height)
        return l_height + 1;
    else 
        return r_height + 1;
}

/** Check if the tree is empty. 
 * 
 * @return: True if the size of the tree is 0. 
 */
template <class K, class V> bool bstree<K, V>::empty() const {
	return (tree_size == 0);
}

/** Get the minimum key in the tree.
 * 
 * @return: The minimum key in the tree. 
 */
template <class K, class V> const K& bstree<K, V>::min() const {
	node <K, V>* p = root_node_ptr;
	
	while (p->l_ptr != nullptr)
		p = p->l_ptr;

	return p->key;
}

/** Get the maximum key in the tree. 
 * 
 * @return: The maximum key in the tree. 
 */
template <class K, class V> const K& bstree<K, V>::max() const {	
	node <K, V>* p = root_node_ptr;
	
	while (p->r_ptr != nullptr)
		p = p->r_ptr;

	return p->key;
}

/** Check for duplicate keys or null links.
 *
 * @param key: The key assigned to a node.
 * @param value: The value assigned to a node.
 * @return: Return false if the key exists in the tree.
 */
template <class K, class V> bool bstree<K, V>::insert(const K& key, const V& value) {
	node<K, V>* p; 
	node<K, V>* parent; 
	std::queue<node<K, V>*> q;

	// Start at the root of the tree.
	p = root_node_ptr;
	parent = nullptr;

	// Search the tree for a null link or a duplicate key.
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

	// Create a tree node and insert it as a new leaf node.
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

/** Remove specified key from binary search tree.
 *
 * @param key: Specified key to be removed.
 * @return: False if the key is not in the tree.
 */
template <class K, class V> bool bstree<K, V>::remove(const K& key) {
	node<K, V>* p; 
	node<K, V>* parent;
	node<K, V>* replace; 
	node<K, V>* replace_parent; 

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
		replace = p->r_ptr;
	
	else if (p->r_ptr == nullptr)
		replace = p->l_ptr;
	
	else {
		replace_parent = p;
		replace = p->l_ptr;

		while (replace->r_ptr != nullptr) {
			replace_parent = replace;
			replace = replace->r_ptr;
		}

		if (replace_parent != p) {
			replace_parent->r_ptr = replace->l_ptr;
			replace->l_ptr = p->l_ptr;
		}

		replace->r_ptr = p->r_ptr;
	}

	// Connect replacement node to the parent node of p.    
	if (parent == nullptr)
		root_node_ptr = replace;
	
	else {
		if (p->key < parent->key)
			parent->l_ptr = replace;
		else
			parent->r_ptr = replace;
	}

	delete p;
	tree_size = tree_size - 1;

	return true;
}

/** Find the specified key in binary search tree.
 *
 * @param key: Specified key to be found.
 * @return: If the key is not within the tree, the function returns nullptr.
 */
template <class K, class V> const node<K, V>* bstree<K, V>::find(const K& key) const {
	node<K, V>* p = root_node_ptr; 

	// Search the tree for a null link or a matching key.
	while (p != nullptr && key != p->key) {
		if (key < p->key)
			p = p->l_ptr;
		else
			p = p->r_ptr;
	}

	return p; 
}

/** Accessor methods. */
template <class K, class V> void bstree<K, V>::preorder() const {
	preorder(root_node_ptr);
}

template <class K, class V> void bstree<K, V>::inorder() const {
	inorder(root_node_ptr);
}

template <class K, class V> void bstree<K, V>::postorder() const {
	postorder(root_node_ptr);
}

/** Perform preorder traversal of the tree from left to right.
 *
 * @param p: Pointer to a tree node.
 */
template <class K, class V> void bstree<K, V>::preorder(node<K, V>* p) const {
	if (p != nullptr) {
		std::cout << p->key << ": " << p->value << ' ' << "\n";
		preorder(p->l_ptr);
		preorder(p->r_ptr);
	}
}

/** Perform inorder traversal of the tree from left to right.
 *
 * @param p: Pointer to a tree node.
 */
template <class K, class V> void bstree<K, V>::inorder(node<K, V>* p) const {
	if (p != nullptr) {
		inorder(p->l_ptr);
		std::cout << p->key << ": " << p->value << ' ' << "\n";
		inorder(p->r_ptr);
	}
}

/** Perform postorder traversal of the tree from left to right.
 *
 * @param p: Pointer to a tree node.
 */
template <class K, class V> void bstree<K, V>::postorder(node<K, V>* p) const {
	if (p != nullptr) {
		postorder(p->l_ptr);
		postorder(p->r_ptr);
		std::cout << p->key << ": " << p->value << ' ' << "\n"; 
	}
}

/** Perform level order traversal of tree from left to right. */
template <class K, class V> void bstree<K, V>::level_order() const {
	node<K, V>* p; 
	std::queue<node<K, V>*> q; 

	if (root_node_ptr == nullptr)
		return;

	q.push(root_node_ptr);

	while (!q.empty()) {
		p = q.front();
		q.pop();
		std::cout << p->key << ": " << p->value << ' ' << "\n";

		if (p->l_ptr != nullptr)
			q.push(p->l_ptr);

		if (p->r_ptr != nullptr)
			q.push(p->r_ptr);
	}
}

/** Recursively deletes the nodes of a bstree object.
 *	
 * @param p Pointer to a tree node.
 */
template <class K, class V> void bstree<K, V>::destroy(node<K, V>* p) const {
	if (p != nullptr) {
		destroy(p->l_ptr);
		destroy(p->r_ptr);
	}

	delete p;
}

/** Recursively copies the nodes of a bstree object. This is a modified version of a
 *  preorder traversal of the tree.
 *
 * @param p: Pointer to a tree node.
 * @return: The new node after reaching nullptr.
 */
template <class K, class V> node<K, V>* bstree<K, V>::clone(node<K, V>* p) const {
	if (p != nullptr) {
		node<K, V>* new_node = new node<K, V>(); 
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
