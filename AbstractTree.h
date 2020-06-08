#pragma once
#pragma once
 #include "GroupContainer.h"

 // ����������� �����: ������
 class AbstractTree : public GroupContainer
 {
 public:
          // �����������
          AbstractTree(MemoryManager & mem) : GroupContainer(mem) {}
   
          // ����������
          virtual ~AbstractTree() {}
   
          class Iterator : public Container::Iterator
          {
          public:
                // ������� � ������������ �������. ���������� false ���� ������� ������� - ������ ��� end().
                  virtual bool goToParent() = 0;
       
                  // ������� � �������� ������� � child_index. ���������� false ���� �������� ������� � ����� ������� ���.
                  virtual bool goToChild(int child_index) = 0;
          };
   
        // ���������� �������� ��� �������� ������� � child_index �������, �� �������
        // ��������� ��������. � ������ ��������� ���������� ������� ���������� �������� 0, � ������ ������� 1.
        virtual int insert(Iterator * iter, int child_index, void* elem, size_t size) = 0;
   
        // �������� �������, �� ������� ��������� ��������. ���� leaf_only==1 � ������� �� �������� ������, ���������� false
        // !!! ����������: ����� remove ������� ������� ������ �� ����� �� ���������
        virtual bool remove(Iterator * iter, int leaf_only) = 0;
    };