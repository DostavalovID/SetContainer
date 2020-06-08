#pragma once
#include "MemoryManager.h"

// Простейший менеджер памяти, использует ::new и ::delete
class Mem : public MemoryManager
{
public:
    Mem(size_t sz) : MemoryManager(sz) {}

    void* allocMem(size_t sz) // каждый раз вызывается при инсерте
    {
        return new char[sz]; //побайтово выделяем память
    }

    void freeMem(void* ptr)
    {
        delete[] ptr;
    }
};
