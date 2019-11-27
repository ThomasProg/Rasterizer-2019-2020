#ifndef _DYNAMIC_AS_STATIC_ARRAY_H_
#define _DYNAMIC_AS_STATIC_ARRAY_H_

#include <cassert>

template<typename T>
class DynamicAsStaticArray
{
private:
    unsigned int _size = 0;
    T* _elements = nullptr;
public:
    DynamicAsStaticArray()
    {

    }

    DynamicAsStaticArray(unsigned int size)
        : _size(size), _elements(new T[size])
    {

    }

    ~DynamicAsStaticArray()
    {
        if (_elements != nullptr)
        {
            delete[] _elements;
        }
    }

    void setNewSize(unsigned int size)
    {
        _elements = new T[size];
        _size = size;
    }

    unsigned int size() const
    {
        return _size;
    }

    T& operator[](unsigned int id)
    {
        assert(id >= 0 && id < _size);
        return _elements[id];
    }
    const T& operator[](unsigned int id) const
    {
        assert(id >= 0 && id < _size);
        return _elements[id];
    }
};


#endif