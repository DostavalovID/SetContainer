#pragma once
#include "TreeAbstract.h"
#include <iostream>

class Tree : public AbstractTree {

private:
    int _size;

    class Vertex {

    private:
        void* _element;
        size_t _element_size;
        Vertex* _parent;
        Vertex* _left_brother, * _right_brother;
        Vertex* _first_child;

    public:
        Vertex(void* _element, size_t _size_element, Vertex* _parent) {
            this->_element = _element;
            this->_element_size = _size_element;
            this->_parent = _parent;
            this->_left_brother = this->_right_brother = NULL;
            this->_first_child = NULL;
        }

        void* element() { return this->_element; }
        size_t element_size() { return this->_element_size; }

        Vertex* parent() { return _parent; }

        Vertex* first_child() { return _first_child; }
        void set_first_child(Vertex* _first_child) { this->_first_child = _first_child; }

        Vertex* left_brother() { return _left_brother; }
        Vertex* right_brother() { return _right_brother; }
        void set_left_brother(Vertex* _left_brother) { this->_left_brother = _left_brother; }
        void set_right_brother(Vertex* _right_brother) { this->_right_brother = _right_brother; }

    };

    Vertex* _root;

    void removeDFS(Vertex* v, bool remove_v) {
        Vertex* u = v->first_child();
        while (u != NULL) {
            Vertex* nu = u->right_brother();
            removeDFS(u, 1);
            u = nu;
        }
        v->set_first_child(NULL);
        if (remove_v) {
            change_size(-1);
            free_mem(v);
        }
    }

public:
    Tree(MemoryManager& mem) : AbstractTree(mem) {
        this->_memory = mem;
        _root = new (alloc_mem(sizeof(Vertex))) Vertex(NULL, 0, NULL);
        _size = 0;
    }

    ~Tree() {
        Tree::clear();
    }

    void* alloc_mem(size_t size) {
        return _memory.allocMem(size);
    }

    void free_mem(void* ptr) {
        _memory.freeMem(ptr);
    }

    class Iterator : public AbstractTree::Iterator {

    private:
        Vertex* _vertex;

        Vertex* goNext(Vertex* s) {
            Vertex* v = s;
            if (v->first_child() != NULL) {
                return v->first_child();
            }
            while (v != NULL) {
                if (v->right_brother() != NULL) {
                    return v->right_brother();
                }
                v = v->parent();
            }
            return NULL;
        }

    public:
        Iterator() { _vertex = NULL; }

        Iterator(Vertex* vertex) { this->_vertex = vertex; }

        Vertex* vertex() { return _vertex; }
        void set_vertex(Vertex* _vertex) { this->_vertex = _vertex; }

        void* getElement(size_t& size) {
            if (_vertex == NULL) {
                throw Container::Error("Called getElement from empty Iterator");
            }
            size = _vertex->element_size();
            return _vertex->element();
        }

        bool hasNext() {
            if (_vertex == NULL)
                return false;
            if (goNext(_vertex) == NULL)
                return false;
            return true;
        }

        void goToNext() {
            if (!hasNext()) {
                throw Container::Error("Wanted to go next, but it doesn't exist");
            }
            _vertex = goNext(_vertex);
        }

        bool equals(Container::Iterator* rgt) {
            Tree::Iterator* right = dynamic_cast<Tree::Iterator*>(rgt);
            if (vertex() == NULL || right->vertex() == NULL)
                return false;
            return vertex() == right->vertex();
            //            size_t sz1, sz2;
            //            void* v1 = getElement(sz1);
            //            void* v2 = right->getElement(sz2);
            //            return v1 == v2 && sz1 == sz2;
        }

        bool goToParent() {
            if (_vertex == NULL)
                return false;
            if (_vertex->parent() == NULL)
                return false;
            _vertex = _vertex->parent();
            return true;
        }

        bool goToChild(int child_index) {
            Vertex* child = _vertex->first_child();
            for (int i = 0; i < child_index; i++) {
                if (child == NULL)
                    return false;
                child = child->right_brother();
            }
            if (child == NULL)
                return false;
            _vertex = child;
            return true;
        }
    };

    Vertex* root() { return _root; }

    int size() { return _size; }

    void change_size(int delta) { _size += delta; }

    size_t max_bytes() { return this->_memory.maxBytes(); } // TODO TESTING

    Tree::Iterator* find(void* elem, size_t size) {
        Tree::Iterator* iterator = new (alloc_mem(sizeof(Iterator))) Iterator(_root);
        while (iterator->hasNext()) {
            iterator->goToNext();
            size_t nsize;
            void* nelem = iterator->getElement(nsize);
            if (elem == nelem && size == nsize) {
                return iterator;
            }
        }
        return newIterator();
    }

    Tree::Iterator* newIterator() {
        return new (alloc_mem(sizeof(Iterator))) Iterator();
    }

    Tree::Iterator* begin() {
        return new (alloc_mem(sizeof(Iterator))) Iterator(_root);
    }

    Tree::Iterator* end() {
        return newIterator();
    }

    void remove(Container::Iterator* it) {
        Tree::Iterator* iter = dynamic_cast<Tree::Iterator*>(it);
        remove(iter, 1);
    }

    void clear() {
    Tree:Iterator* iterator = new Iterator(_root);
        remove(iterator, 0);
        _root = new (alloc_mem(sizeof(Vertex))) Vertex(NULL, 0, NULL);
        _size = 0;
    }

    bool empty() {
        return this->size() ? false : true;
    }

    int insert(AbstractTree::Iterator* it, int child_index, void* elem, size_t size) {
        Tree::Iterator* iter = dynamic_cast<Tree::Iterator*>(it);
        if (iter->vertex() == NULL || child_index < 0) {
            return 1;
        }
        Vertex* vertex = iter->vertex();
        Vertex* nvertex = new (alloc_mem(sizeof(Vertex))) Vertex(elem, size, vertex);
        Vertex* first_child = vertex->first_child();
        if (child_index == 0) {
            vertex->set_first_child(nvertex);
            nvertex->set_right_brother(first_child);
            if (first_child != NULL)
                first_child->set_left_brother(nvertex);
        }
        else {
            for (int i = 0; i < child_index - 1; i++) {
                first_child = first_child->right_brother();
                if (first_child == NULL) {
                    return 1;
                }
            }
            Vertex* right_brother = first_child->right_brother();
            first_child->set_right_brother(nvertex);
            nvertex->set_left_brother(first_child);
            nvertex->set_right_brother(right_brother);
            if (right_brother != NULL)
                right_brother->set_left_brother(nvertex);
        }
        change_size(1);
        return 0;
    }

    bool remove(AbstractTree::Iterator* it, int leaf_only) {
        Tree::Iterator* iter = dynamic_cast<Tree::Iterator*>(it);
        Vertex* v = iter->vertex();
        if (leaf_only == 1 && v->first_child() != NULL) {
            return false;
        }
        if (v->left_brother() != NULL) {
            v->left_brother()->set_right_brother(v->right_brother());
        }
        if (v->right_brother() != NULL) {
            v->right_brother()->set_left_brother(v->left_brother());
        }
        Vertex* p = v->parent();
        if (p != NULL && p->first_child() == v) {
            p->set_first_child(v->right_brother());
        }
        removeDFS(v, 0);
        if (iter->hasNext())
            iter->goToNext();
        else
            iter->set_vertex(NULL);
        removeDFS(v, 1);
        return true;
    }

};