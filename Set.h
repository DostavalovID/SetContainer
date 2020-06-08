#pragma once
#include "SetAbstract.h"
#include "AbstractTree.h"
class Set :
	public AbstractSet
{
	class Tree_node
	{
	private:
		Tree_node* c_parent;
		void* c_key; size_t c_size; int c_priority = rand();
	public:
		Tree_node* c_left, * c_right;
		Tree_node(void* new_elem, size_t new_size) {
			this->c_size = new_size;
			this->c_key = new_elem;
			this->c_left = nullptr;
			this->c_right = nullptr;
			this->c_parent = nullptr;
			this->c_priority = rand();
		}
		~Tree_node() {
			this->c_left = nullptr;
			this->c_right = nullptr;
			this->c_parent = nullptr;
			this->c_priority = 0;
			this->c_size = 0;
			this->c_key = nullptr;
		}
		Tree_node* left() { return this->c_left; }
		Tree_node* right() { return this->c_right; }
		Tree_node* parent() { return this->c_parent; }
		void* key() { return this->c_key; }
		size_t size() { return this->c_size; }
		int priority() { return this->c_priority; }
		void set_key(void* key) { this->c_key = key; }
		void set_left(Tree_node* left) { this->c_left = left; }
		void set_right(Tree_node* right) { this->c_right = right; }
		void set_parent(Tree_node* parent) { this->c_parent = parent; }
		void set_elem(void* key, size_t size) { this->c_key = key; this->c_size = size; }
	};
	struct tree_pair { Tree_node* first, * second; };
	
	int compare(void* key1, size_t size1, void* key2, size_t size2);
	bool search(Tree_node* root, void* key, size_t size);
	tree_pair split(Tree_node* root, void* key, size_t size); //
	Tree_node* merge(Tree_node* left, Tree_node* right);
	void Tree_insert(Tree_node* &root, Tree_node* node_for_insert, void* key, size_t size); //сделать функцию с параметрами сета, а потом пересчитывать

	int c_size; Tree_node* c_begin, * c_end;
public:
	Tree_node* c_root;
	Set(MemoryManager& mem) : AbstractSet(mem) {
		this->_memory = mem;
		c_begin = nullptr;
		c_end = nullptr;
		c_root = nullptr;
		c_size = 0;
	}
	~Set() {
		c_begin = nullptr;
		c_end = nullptr;
		c_root = nullptr;
		c_size = 0;
	}
	int size();
	void set_size(int size);
	size_t max_bytes() override;

	class SetIterator : public Container::Iterator 
	{	
		Tree_node* _node;
		Tree_node* next(Tree_node* current, bool first);

	public:
		SetIterator() { this->_node = nullptr; }
		SetIterator(Tree_node* node) { this->_node = node; }
		void setNode(Tree_node* node) { this->_node = node; }
		Tree_node* node() { return this->_node; }
		
		void* getElement(size_t& size) override;
		bool hasNext() override; 
		void goToNext() override; 
		bool equals(Iterator* right) override;
	};
	Tree_node* root() { return this->c_root; }
	void set_root(Tree_node* root) { this->c_root = root; }
	Tree_node* forFind(Tree_node* root, void* elem, size_t size);
	SetIterator* find(void* elem, size_t size) override;
	SetIterator* newIterator() override;
	SetIterator* begin() override;
	SetIterator* end() override;
	void erase(Tree_node* &root, void* elem, size_t size);
	void remove(Iterator* iter) override;
	void forClear(Tree_node* root);
	void clear() override;
	bool empty() override;
	void print(Tree_node* root);
	int insert(void* elem, size_t size) override;



};
/*
*/
