#ifndef DECK_H
#define DECK_H

#include "MutableArray.h"
#include "SequenceNew.h"
#include <stdexcept>
#include <algorithm>
#include <vector>


#ifndef SEGMENT_SIZE
#define SEGMENT_SIZE 5
#endif


template <class T>
class SegmentDek {
    protected:
    int segmentCapacity; //мкес колич элем в одн сегменте
    int elementCount; // общее число элементов в деке
    MutableArraySequence<MutableArraySequence<T>*> segmentArray;
    int headSegInd; //сегмент содерж первый элемент дек
    int tailSegInd; //сегмент содерж посл элемент дека
    int headIndSeg; // позиция первого элем в сегменте headSegInd
    int tailIndSeg; // позиция последнего элемента в сегменте tailIndInd

    SegmentDek() :
            segmentCapacity(SEGMENT_SIZE), elementCount(0), segmentArray(), headSegInd(0),
        tailSegInd(0), headIndSeg(0), tailIndSeg(-1) {
        segmentArray.Append(creatNewElem());
    }

    SegmentDek(const SegmentDek<T>& other) : segmentCapacity(other.segmentCapacity),elementCount(other.elementCount),segmentArray(),
          headSegInd(other.headSegInd),tailSegInd(other.tailSegInd),headIndSeg(other.headIndSeg),tailIndSeg(other.tailIndSeg)
    {
        for (int i = 0; i < other.segmentArray.GetLength(); ++i) {
            segmentArray.Append(new MutableArraySequence<T>(*other.segmentArray.Get(i)));
        }
    }

    // virtual ~SegmentDek() {
    //     for (int i = 0; i < segmentArray.GetLength(); i++) {
    //         delete segmentArray.Get(i);
    //     }
    // }
// добавл нового сегмента
    MutableArraySequence<T>* creatNewElem() {
                // создаём буфер из segmentCapacity элементов (инициализируются нулями)
                T* items = new T[segmentCapacity]();
                // конструируем сегмент с заданным массивом и размером segmentCapacity
                auto seg = new MutableArraySequence<T>(items, segmentCapacity);
                delete[] items;
                return seg;
            }

    void addSegmentFront() {
        MutableArraySequence<T>* newElem = creatNewElem();
        segmentArray.Prepend(newElem);
        tailSegInd++;
    }
    void addSegmentBack() {
        MutableArraySequence<T>* newElem = creatNewElem();
        segmentArray.Append(newElem);
    }

    void clearInternal() {
        for (int i = 0; i < segmentArray.GetLength(); ++i)
            delete segmentArray.Get(i);
        segmentArray = MutableArraySequence< MutableArraySequence<T>* >();
        elementCount = 0;
        headSegInd = tailSegInd = 0;
        headIndSeg = 0;
        tailIndSeg = -1;
        segmentArray.Append(creatNewElem());
    }

    virtual SegmentDek<T>* Instance() = 0;
    virtual SegmentDek<T>* Clone() const = 0;


    SegmentDek<T>* pushBackInternal(const T &value) {
        if (empty()) {
            headSegInd = tailSegInd = 0;
            headIndSeg = tailIndSeg = 0;
            segmentArray.Get(0)->Set(0, value);
            elementCount = 1;
            return this;
        }
        if (tailIndSeg == segmentCapacity - 1) {
            if (tailSegInd == segmentArray.GetLength() - 1) {
                addSegmentBack();
            }
            tailSegInd++;
            tailIndSeg = 0;
        } else {
            tailIndSeg++;
        }
        segmentArray.Get(tailSegInd)->Set(tailIndSeg, value);
        elementCount++;
        return this;
    }


    SegmentDek<T>* pushFrontInternal(const T &value) {
        if (empty()) {headSegInd = tailSegInd = 0;headIndSeg = tailIndSeg = 0;segmentArray.Get(0)->InsertAt(value, 0);elementCount = 1;
            return this;
        }
        if (headIndSeg == 0) {
            if (headSegInd == 0) {addSegmentFront();headIndSeg = segmentCapacity - 1;
            } else {headSegInd--;headIndSeg = segmentCapacity - 1;
            }
        } else {
            headIndSeg--;
        }
        segmentArray.Get(headSegInd)->InsertAt(value, headIndSeg);
        elementCount++;
        return this;
    }
    SegmentDek<T>* popBackInternal() {
        if (empty()) {
            throw std::out_of_range("popBack: deque is empty");
        }
        elementCount--;
        if (tailIndSeg == 0) {
            if (tailSegInd > headSegInd) {
                tailSegInd--;
                tailIndSeg = segmentCapacity - 1;
            } else {
                tailIndSeg = -1;
            }
        } else {
            tailIndSeg--;
        }
        if (elementCount == 0) {
            headSegInd = tailSegInd = 0;
            headIndSeg = 0;
            tailIndSeg = -1;
        }
        return this;
    }

    SegmentDek<T>* popFrontInternal() {
        if (empty()) {
            throw std::out_of_range("popFront: deck is empty");
        }
        elementCount--;
        if (headIndSeg == segmentCapacity - 1) {
            headSegInd++;
            headIndSeg = 0;

        } else {
            headIndSeg++;
        }
        if (elementCount == 0) {
            headSegInd = tailSegInd = 0;
            headIndSeg = 0;
            tailIndSeg = -1;
        }
        return this;
    }

    SegmentDek<T>* setInternal(int ind, const T &value) {
        if (ind < 0 || ind >= elementCount) {
            throw std::out_of_range("Index out of range");
        }
        int segIndex = headSegInd;
        int pos = headIndSeg;
        int count = 0;
        while (count < ind) {
            pos++;
            if (pos == segmentCapacity) {
                segIndex++;
                pos = 0;
            }
            count++;
        }
        segmentArray.Get(segIndex)->Set(pos, value);
        return this;
    }
    T getInternal(int ind) const {
        if (ind < 0 || ind >= elementCount) {
            throw std::out_of_range("Index out of range");
        }
        int segIndex = headSegInd;
        int pos = headIndSeg;
        int count = 0;
        while (count < ind) {
            pos++;
            if (pos == segmentCapacity) {
                segIndex++;
                pos = 0;
            }
            count++;
        }
        return segmentArray.Get(segIndex)->Get(pos);
    }

    // template <class Compare>
    // SegmentDek<T>* sortInternal (Compare comp) {
    //     std::vector<T> elem;
    //     elem.reserve(elementCount);
    //     for (int i = 0; i < elementCount; i++) {
    //         elem.push_back(getInternal(i));
    //     }
    //     std::sort(elem.begin(), elem.end(), comp);
    //     for (int i = 0; i < elementCount; i++) {
    //         setInternal(i, elem[i]);
    //     }
    //     return this;
    // }
    SegmentDek<T>* sortInternal() {

        std::vector<T> elem;
        elem.reserve(elementCount);
        for (int i = 0; i < elementCount; i++) {
            elem.push_back(getInternal(i));
        }
        std::sort(elem.begin(), elem.end());
        for (int i = 0; i < elementCount; i++) {
            setInternal(i, elem[i]);
        }
        return this;
    }
    SegmentDek<T>* mapInternal(T (*func)(T)) {
        for (int i = 0; i < elementCount; ++i)
            setInternal(i, func(getInternal(i)));
        return this;
    }
    // SegmentDek<T>* whereInternal(bool (*pred)(T)) {
    //     std::vector<T> buf;
    //     std::cout<<"-1";
    //     for (int i = 0; i < elementCount; ++i) {
    //         T v = getInternal(i);
    //         std::cout<<"0";
    //         if (pred(v)) buf.push_back(v);
    //     }
    //     std::cout<<"1";
    //     clearInternal();
    //     std::cout<<"2";
    //     for (auto &v : buf) pushBackInternal(v);
    //     std::cout<<"3";
    //     return this;
    // }

    SegmentDek<T>* concatInternal(SegmentDek<T>* other) {
        for (int i = 0; i < other->elementCount; ++i)
            pushBackInternal(other->getInternal(i));
        return this;
    }

    SegmentDek<T>* getSubsequenceInternal(int start, int end) {
        if (start < 0 || end >= elementCount || start > end)
            throw std::out_of_range("Index out of range");
        std::vector<T> buf;
        for (int i = start; i <= end; ++i)
            buf.push_back(getInternal(i));
        clearInternal();
        for (auto &v : buf) pushBackInternal(v);
        return this;
    }

    // вставка элемента по индексу
    SegmentDek<T>* mergeInternal(const SegmentDek<T>& other) {
        std::vector<T> buf;
        int i = 0, j = 0;
        while (i < elementCount && j < other.elementCount) {
            if (getInternal(i) < other.getInternal(j))
                buf.push_back(getInternal(i++));
            else
                buf.push_back(other.getInternal(j++));
        }
        while (i < elementCount) buf.push_back(getInternal(i++));
        while (j < other.elementCount) buf.push_back(other.getInternal(j++));
        clearInternal();
        for (auto &v : buf) pushBackInternal(v);
        return this;
    }

    public:
    virtual ~SegmentDek() {
        for (int i = 0; i < segmentArray.GetLength(); i++) {
            delete segmentArray.Get(i);
        }
    }


    // проверка на пустой дек
    bool empty() const {
        return elementCount == 0;
    }
    // общ число сегм в деке
    int size() const {
        return elementCount;
    }
    // T reduce(T (*func)(T), T first) const {
    //     T result = first;
    //     for (int i = 0; i < elementCount; i++) {
    //         result += func(get(i));
    //     }
    //     return result;
    // }
    //
    // int findSubSequence(const SegmentDek<T>& other) const {
    //     int len = other.size();
    //     if (len == 0) {
    //         return 0;
    //     }
    //     for (int i = 0; i <= elementCount - len; i++) {
    //         bool flag = true;
    //         for (int j = 0; j < len; j++) {
    //             if (get(i + j) != other.get(j)) {
    //                 flag = false;
    //                 break;
    //             }
    //         }
    //         if (flag)
    //             return i;
    //     }
    //     return -1;
    // }
    // вставка элемента по индексу без полного пересоздания
    SegmentDek<T>* InsertAt(T item, int index) {
        if (index < 0 || index > elementCount)
            throw std::out_of_range("Индекс вне диапазона");

        // простые случаи
        if (empty() || index == elementCount) return pushBackInternal(item);
        if (index == 0)                  return pushFrontInternal(item);

        // запомним старую длину и значение последнего элемента
        int oldCount = elementCount;
        T lastVal = getInternal(oldCount - 1);

        // 1) дублируем последний элемент в хвосте — расширяем дек на 1
        pushBackInternal(lastVal);

        // 2) сдвигаем все элементы от старого конца до позиции index
        for (int i = oldCount - 2; i >= index; --i) {
            // элемент i → i+1
            setInternal(i + 1, getInternal(i));
        }

        // 3) в позицию index ставим наш новый элемент
        setInternal(index, item);

        return this;
    }




    T Reduce(T (*func)(T, T), T init) {
        T result = init;
        for (int i = 0; i < elementCount; ++i) {
            result = func(result, getInternal(i));
        }
        return result;
    }
    T GetFirst() const { return getInternal(0); }
    T GetLast()  const  { return getInternal(elementCount - 1); }
    T Get(int idx) const { return getInternal(idx); }
    void Set(int idx, const T& v) { Instance()->setInternal(idx, v); }
    int GetLength() const  { return elementCount; }

    SegmentDek<T>* Append(T item)   {return Instance()->pushBackInternal(item);}
    SegmentDek<T>* Prepend(T item)  {return Instance()->pushFrontInternal(item);}
    SegmentDek<T>* Map(T (*f)(T))    {return Instance()->mapInternal(f);}
    // T  Reduce(T (*f)(T,T), T init) const override {
    //     T acc = init;
    //     for (int i = 0; i < elementCount; ++i)
    //         acc = f(acc, getInternal(i));
    //     return acc;
    // }
    // SegmentDek<T>* Where(bool (*p)(T))  {
    //     return Instance()->whereInternal(p);
    // }
    SegmentDek<T>* Concat(SegmentDek<T>* seq) {
        return Instance()->concatInternal(seq);
    }
    SegmentDek<T>* GetSubsequence(int s, int e) {
        return Instance()->getSubsequenceInternal(s, e);
    }
    SegmentDek<T>* PopBack() { return Instance()->popBackInternal(); }
    SegmentDek<T>* PopFront(){ return Instance()->popFrontInternal(); }

    // template <class Compare>
    // SegmentDek<T>* Sort(Compare comp) {
    //     return Instance()->sortInternal(comp);
    // }

    SegmentDek<T>* Sort() {
        return Instance()->sortInternal();
    }

    SegmentDek<T>* Merge(const SegmentDek<T>& other) {
        return Clone()->mergeInternal(other);
    }
};

#endif //DECK_H
