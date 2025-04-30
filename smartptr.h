#ifndef SMARTPTR_H
#define SMARTPTR_H

template <typename T>
class SmartPtr
{
private:
    T* obj;
public:
    explicit SmartPtr(T* obj = nullptr) : obj(obj) {}
    ~SmartPtr() {delete obj;}

    T* operator->() {return obj;}
    T& operator* () {return *obj;}
    T* get() {return obj;}
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
    SmartPtr(SmartPtr&& other) : obj(other.obj) {other.obj = nullptr;}

    SmartPtr& operator=(SmartPtr&& other)
    {
        if (this != &other)
        {
            delete obj;

            obj = other.obj;
            other.obj = nullptr;
        }

        return *this;
    }
};

#endif
