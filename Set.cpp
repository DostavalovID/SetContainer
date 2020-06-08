#include "Set.h"
// ------------------------- начало функций дерева
int Set::compare(void* key1, size_t size1, void* key2, size_t size2) {
	//если первый меньше вернет -1 при равенстве вернет 0 если первый больше вернет 1
	char k1[4], k2[4];
	for (int i = 0; i < 4; i++) {
		k1[i] = ((char*)key1)[3 - i];
		k2[i] = ((char*)key2)[3 - i];
	}
	if (size1 == size2) {
		if (memcmp(k1, k2, size1) == 0)
			return 0;
		else if (memcmp(k1, k2, size1) < 0)
			return -1;
		else
			return 1;
	}
	else if (size1 < size2)
		return -1;
	else
		return 1;
}

bool Set::search(Tree_node* root, void* elem_key, size_t size)
{
	if (root == nullptr) return false;
	int temp = compare(root->key(), root->size(), elem_key, size);
	if (temp == 0)
		return true;
	else if (temp == 1)
		return search(root->left(), elem_key, size);
	else
		return search(root->right(), elem_key, size);
}

Set::tree_pair Set::split(Tree_node* root, void* elem_key, size_t size) {
	tree_pair ans;
	if (root == nullptr) {
		ans.first = nullptr;
		ans.second = nullptr;
		return ans;
	}
	int temp = compare(root->key(), root->size(), elem_key, size);
	if (temp == -1) {
		tree_pair splitted = split(root->right(), elem_key, size);
		root->set_right(splitted.first);
		if(splitted.first != nullptr)
			splitted.first->set_parent(root);
		if(splitted.second != nullptr)
			splitted.second->set_parent(nullptr);
		ans.first = root; ans.second = splitted.second;
	}
	else {
		tree_pair splitted = split(root->left(), elem_key, size);
		root->set_left(splitted.second);
		if(splitted.second != nullptr)
			splitted.second->set_parent(root);
		if(splitted.first != nullptr)
			splitted.first->set_parent(nullptr);
		ans.first = splitted.first; ans.second = root;
	}
	return ans;
}

Set::Tree_node* Set::merge(Tree_node* left, Tree_node* right) {	
	if (left == nullptr || right == nullptr)
		return right == nullptr ? left : right;
	if (left->priority() > right->priority()) {
	
		left->set_right(merge(left->right(), right));
		if (left->right() != nullptr)
			left->right()->set_parent(left);
		return left;
		
	}
	else {
		right->set_left(merge(left, right->left()));
		if (right->left() != nullptr)
			right->left()->set_parent(right);
		return right;
		
	}
}
void Set::Tree_insert(Tree_node*& root, Tree_node *node_for_insert,void* key, size_t size) {
	tree_pair splitted = split(root, key, size);
	root = merge(merge(splitted.first, node_for_insert), splitted.second);
}

void Set::print(Tree_node* root) {
	if (root == nullptr)
		return;
	cout << "I go to the left" << endl;
	print(root->left());
	int* a = static_cast<int*>(root->key());
	cout << "key = " << *a << " priority = " << root->priority() << endl;
	cout << "I go to the right" << endl;
	print(root->right());
}

// ************************ конец функций дерева

// ---------------- начало функций итератора

Set::Tree_node* Set::SetIterator::next(Tree_node* current, bool first) {
	Tree_node* temp_node = current;
	if (temp_node->right() != nullptr && first == true)
		return temp_node->right();
	else if (temp_node->parent() != nullptr) {
		int temp;
		char k1[4], k2[4];
		for (int i = 0; i < 4; i++) {
			k1[i] = ((char*)temp_node->parent()->key())[3 - i];
			k2[i] = ((char*)temp_node->key())[3 - i];
		}
		if (temp_node->parent()->size() == temp_node->size()) {
			if (memcmp(k1, k2, temp_node->size()) == 0)
				temp = 0;
			else if (memcmp(k1, k2, temp_node->size()) < 0)
				temp = -1;
			else
				temp = 1;
		}
		else if (temp_node->parent()->size() < temp_node->size())
			temp = -1;
		else
			temp = 1;
		if (temp == 1)
			return temp_node->parent();
		else {
			return next(temp_node->parent(), false);
		}
	}
	return nullptr;
}

void* Set::SetIterator::getElement(size_t& size) {
	if (_node == nullptr)
		throw Container::Error("Get from empty iterator");
	size = _node->size();
	return _node->key();
}
bool Set::SetIterator::hasNext() {
	if (next(this->_node, true) == nullptr || this->_node == nullptr)
		return false;
	return true;
}
void Set::SetIterator::goToNext() {
	if (this->hasNext() == false) 
		throw Container::Error("Going to not existing element");
	else 
		this->_node = this->next(_node, true);
}
bool Set::SetIterator::equals(Iterator* right) {
	size_t s1, s2; void* k1, * k2;
	k1 = this->getElement(s1);
	k2 = right->getElement(s2);
	return k1 == k2 && s1 == s2;
}
//********************* конец функций итератора

// -------------- начало функций самого множества

int Set::size() {
	return this->c_size;
}
void Set::set_size(int size) {
	this->c_size = size;
}
size_t Set::max_bytes() {
	return this->_memory.maxBytes();
}
int Set::insert(void* elem, size_t size) {
	bool thereis = this->search(this->c_root, elem, size);
	if (thereis) return -1;
	void* temp_elem = this->_memory.allocMem(sizeof(Tree_node));
	Tree_node* node_for_insert = new (temp_elem) Tree_node(elem, size);
	if (this->c_root == nullptr) {
		this->c_root = node_for_insert;
		this->c_size++;
		return 0;
	}
	Tree_insert(this->c_root, node_for_insert, elem, size);
	this->c_size++;
	return 0;
}

Set::Tree_node* Set::forFind(Tree_node* root, void* elem, size_t size)
{
	Tree_node* temp_node = root;
	if (root == nullptr) return nullptr;
	int temp = compare(root->key(), root->size(), elem, size);
	if (temp == 0)
		return temp_node;
	else if (temp == 1) {
		temp_node = temp_node->left();
		return forFind(temp_node, elem, size);
	}	
	else {
		temp_node = temp_node->right();
		return forFind(temp_node, elem, size);
	}
}

Set::SetIterator* Set::newIterator() {
	void* forMem = this->_memory.allocMem(sizeof(SetIterator));
	SetIterator* forReturn = new (forMem) SetIterator;
	return forReturn;
}

Set::SetIterator* Set::find(void* elem, size_t size) {
	Tree_node* temp_node = forFind(this->root(), elem, size);
	SetIterator* Iter = newIterator();
	if (temp_node != nullptr) {
		
		Iter->setNode(temp_node);
		return Iter;
	}
	else {
		throw Container::Error("Haven't found given element");
	}
}

Set::SetIterator* Set::begin() {
	Tree_node* forSearch = this->root();
	while (forSearch->left() != nullptr) {
		forSearch = forSearch->left();
	}
	SetIterator* Iter = newIterator();
	Iter->setNode(forSearch);
	return Iter;
}
Set::SetIterator* Set::end() {
	Tree_node* forSearch = this->root();
	while (forSearch->right() != nullptr) {
		forSearch = forSearch->right();
	}
	SetIterator* Iter = newIterator();
	Iter->setNode(forSearch);
	return Iter;
}

void Set::erase(Tree_node*& root, void* elem, size_t size) {
	int temp = compare(elem, size, root->key(), root->size());
	if (temp < 0) {
		erase(root->c_left, elem, size);
		if(root->c_left != nullptr)
			root->c_left->set_parent(root);
	}
	else if (temp > 0) {
		erase(root->c_right, elem, size);
		if(root->c_right != nullptr)
			root->c_right->set_parent(root);
	}
	else {
		Tree_node* temp_node = root;
		root = merge(root->c_left, root->c_right);
		this->_memory.freeMem((void*)temp_node);
	}
}

void Set::remove(Set::Iterator* iter) {
	//SetIterator* it = (SetIterator*)iter;
	SetIterator* it = dynamic_cast<SetIterator*>(iter);
	if (it == nullptr) {
		throw Container::Error("Trying to remove not existing element");
	}
	try {
		it = find(it->node()->key(), it->node()->size());
	}
	catch (Container::Error exception) {
		if (exception.msg == "Haven't found given element") {
			throw Container::Error("Trying to remove not existing element");
		}
	}
	this->c_size--;
	erase(this->c_root, it->node()->key(), it->node()->size());
	if (it->hasNext()) {
		((SetIterator*)iter)->goToNext();
	}
	else {
		((SetIterator*)iter)->setNode(this->c_begin);
	}
}

void Set::forClear(Tree_node* root) {
	if (root == nullptr) return;
	if(root->left() != nullptr)
		forClear(root->left());
	if(root->right() != nullptr)
		forClear(root->right());
	root->set_parent(nullptr);
	root->set_left(nullptr);
	root->set_right(nullptr);
	root->set_key(nullptr);
	this->_memory.freeMem((void*)root);
	root = nullptr;
}
void Set::clear() {
	forClear(this->root());
	this->~Set();
	return;
}
bool Set::empty() {
	if (this->size() == 0) {
		return true;
	}
	return false;
}
// ************************* конец функций множества