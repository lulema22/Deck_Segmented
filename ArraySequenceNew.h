#ifndef ARRAY_SEQUENCE_NEW_H
#define ARRAY_SEQUENCE_NEW_H
#include "SequenceNew.h"
#include "DynamicArrayNew.h"
#include <utility>
#include <algorithm>
// Предварительное объявление класса MutableArraySequence
//template <class T>
//class MutableArraySequence;


template <class T>

class ArraySequence : public Sequence<T>{
    private:
        DynamicArray<T>* array;

    public:
        ArraySequence(T* item, int count){
            array = new DynamicArray<T>(item, count);
        }
        ArraySequence(){
            array = new DynamicArray<T>(0);
        }
        ArraySequence(const DynamicArray<T>& other){
            array = new DynamicArray<T>(other);
        }
        ArraySequence(const ArraySequence<T>& other){
            array = new DynamicArray<T>(*other.array);
        }

        ~ArraySequence(){
            delete array;
        }
////////////////////////
        T GetFirst() const override {
            if (array->GetSize() == 0)
                throw std::out_of_range("Sequence is empty");
            return array->Get(0);
        }
        T GetLast() const override {
            if (array->GetSize() == 0)
                throw std::out_of_range("Sequence is empty");
            return array->Get(array->GetSize() - 1);
        }
        T Get(int index) const override {
            if (index < 0 || index >= array->GetSize())
                throw std::out_of_range("Index out of range");
            return array->Get(index);
        }
        void Set(int index, const T &value) {
            if (index < 0 || index >= array->GetSize())
                throw std::out_of_range("Index out of range");
            array->Set(index, value);
        }
        int GetLength() const override {
            return array->GetSize();
        }

//////////////////////
        virtual Sequence<T>* Append(T item) override {
            return Instance()->AppendInternal(item);

        }
        virtual Sequence<T>* Prepend(T item) override{
            return Instance()->PrependInternal(item);
        }



        Sequence<T>* InsertAt(T item, int index) override {
            if (index < 0 || index > array->GetSize())
                throw std::out_of_range("Index out of range");
            return Instance()->InsertAtInternal(item, index);
        }

        
//////////
        virtual ArraySequence<T>* Instance()= 0;
        virtual ArraySequence<T>* Clone()const = 0;
//////////

        Sequence<T>* Map(T (*func)(T)) override {
            return Instance()->MapInternal(func);
        }
        T Reduce (T (*func)(T, T), T init) override {
            T result = init;
            for (int i = 0; i < array->GetSize(); i++) {
                result = func(result, array->Get(i));
            }
            return result;
        }
        Sequence<T>* Where(bool (*predicate)(T)) override {
            return Instance()->Where(predicate);
        }


    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex)override{
            if (startIndex<0||startIndex>=array->GetSize()||endIndex<0||endIndex>=array->GetSize()||endIndex<startIndex) {
                throw std::out_of_range("index out of range");
            }
            return Instance()->GetSubsequenceInternal(startIndex,endIndex);
        }
    ArraySequence <T>* Concat(Sequence <T> *list) override {return Instance()->ConcatInternal(list);}


    protected:
        ArraySequence<T>* AppendInternal(T item) {
            
            array -> Resize(array->GetSize() + 1);
            array->Set(array->GetSize() - 1, item);
            return this;
        }
        ArraySequence<T>* PrependInternal(T item) {
            array->Resize(array->GetSize() + 1);
            for (int i = array->GetSize() - 1; i > 0; i --){
                array->Set(i, array->Get(i- 1));
            }
            array->Set(0, item);
            return this;
            
        }
        ArraySequence<T>*InsertAtInternal(T item, int index) {
            array->Resize(array->GetSize() + 1);
            for (int i = array->GetSize() - 1; i> index; i--){
                array->Set(i, array->Get(i-1));
            }
            array->Set(index, item);
            return this;  
        }

    ArraySequence<T>* MapInternal(T (*func)(T)) {
            for (int i=0;i<array->GetSize();i++){
                array->Set(i,func(array->Get(i)));
            }
            return this;
        }
    ArraySequence<T>* WhereInternal(bool (*predicate)(T)) {
            DynamicArray<T>* d_array= new DynamicArray<T>(0);
            for (int i=0; i<array->GetSize();i++){
                if (h(array->Get(i))){
                    d_array->Resize(d_array->GetSize()+1);
                    d_array->Set(d_array->GetSize()-1, array->Get(i));
                }
            }
            delete array;
            array=d_array;
            return this;

        }



    ArraySequence<T>* GetSubsequenceInternal(int startIndex, int endIndex) {
            if (startIndex < 0 || endIndex >= array->GetSize() || startIndex > endIndex) {
                throw std::out_of_range("index out of range");
            }
            DynamicArray<T>* d_array = new DynamicArray<T>(endIndex-startIndex+1);
            for (int i=0;i<d_array->GetSize();i++){
                d_array->Set(i,array->Get(i+startIndex));
            }
            delete array;
            array=new DynamicArray<T>(*d_array);
            return this;
        }
    ArraySequence<T>* ConcatInternal(Sequence <T> *sequence){ //разные мб?

            DynamicArray<T>* d_array = new DynamicArray<T>(array->GetSize()+sequence->GetLength());
            for (int i=0;i<array->GetSize();i++){
                d_array->Set(i,array->Get(i));
            }
            for (int i=array->GetSize();i<d_array->GetSize();i++){
                d_array->Set(i,sequence->Get(i-array->GetSize()));
            }
            this->array->Resize(d_array->GetSize());
            for (int i = 0; i < array->GetSize(); i++) {
                this->array->Set(i, d_array->Get(i));
            }
            delete d_array;
            return this;
        }

};
#endif // ARRAY_SEQUENCE_H


