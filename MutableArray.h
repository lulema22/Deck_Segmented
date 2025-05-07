#ifndef MUTABLE_ARRAY_H
#define MUTABLE_ARRAY_H

#include "ArraySequenceNew.h"

template <class T>
class MutableArraySequence : public ArraySequence<T>{

public:
    using ArraySequence<T>::ArraySequence;
    // MutableArraySequence(T* item, int count) : ArraySequence<T>(item, count) {}
    // MutableArraySequence() : ArraySequence<T>() {}
    // MutableArraySequence(const ArraySequence<T>& other) : ArraySequence<T>(other) {}
    // MutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}

    ArraySequence<T>* Instance() override{
        return this;
    }

    // Переопределяем Clone() – создаём копию текущего объекта.
    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }

};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {

public:
    using ArraySequence<T>::ArraySequence;
    // ImmutableArraySequence(T* item, int count) : ArraySequence<T>(item, count) {}
    // ImmutableArraySequence() : ArraySequence<T>() {}
    // ImmutableArraySequence(const ArraySequence<T>& other) : ArraySequence<T>(other) {}
    // ImmutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}

    ArraySequence<T>* Instance() override {
        return this->Clone();
    }

    ArraySequence<T>* Clone() const  override {
        return new ImmutableArraySequence<T>(*this);
    }

};

#endif // MUTABLE_ARRAY_H

