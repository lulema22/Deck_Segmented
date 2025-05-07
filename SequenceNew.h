#ifndef SEQUENCE_NEW_H
#define SEQUENCE_NEW_H
#include <utility>  // для std::pair
#include <memory>   // для std::unique_ptr (если будете использовать умные указатели)
#include <algorithm> // для std::min
template <class T>
class Sequence{
    public:
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;



    virtual Sequence<T>* Append(T item) = 0;
    //{
        //return Instance()->AppendInternal(item);
    //}

    virtual Sequence<T>* Prepend(T item) = 0;
    //{
        //return Instance()->PrependInternal(item);
    //}
    virtual Sequence<T>* InsertAt(T item, int index) = 0;
    //{
        //return Instance()->InsertInternal(item, index);
    //}
    virtual Sequence<T>* Concat(Sequence<T> * list)=0;
    //{
        //return Instance()->ContactInternal(list);
    //}

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) = 0;
    virtual ~Sequence() = default;

    virtual Sequence<T>* Map(T (*func)(T)) = 0;
    virtual T Reduce(T (*func)(T, T), T init) = 0;
    virtual Sequence<T>* Where(bool (*predicate)(T)) = 0;


    // virtual Sequence<T>* Instance() = 0;
    // virtual Sequence<T>* Clone() const = 0;
    



    // protected:
    // virtual Sequence<T>* AppendInternal(T item) = 0;
    // virtual Sequence<T>* PrependInternal(T item) = 0;
    // virtual Sequence<T>* InsertAtInternal(T item, int index) = 0;
    // virtual Sequence<T>* ContactInternal(Sequence<T> * list) = 0;
};

#endif // SEQUENCE_H