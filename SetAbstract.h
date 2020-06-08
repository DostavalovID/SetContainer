#pragma once
#include "GroupContainer.h"

// ����������� �����: ���������
class AbstractSet : public GroupContainer
{
public:
    // �����������
    AbstractSet(MemoryManager& mem) : GroupContainer(mem) {}

    // ����������
    virtual ~AbstractSet() {}

    // ���������� �������� � ��������� � ��������������� �������, � ������������ � ������� ���������� ���������. � ������ ��������� ���������� ������� ���������� �������� 0, ���� ����� ������� ��� ���� � ���������� - 1, �� ���� ��������� ������� - 2.
    virtual int insert(void* elem, size_t size) = 0;
};
