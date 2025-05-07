#ifndef DYNAMIC_ARRAY_NEW_H
#define DYNAMIC_ARRAY_NEW_H
#include <iostream>

template <class T> // универсальный шаблон под разные типы данных

class DynamicArray{
    private:
    T* data; //указатель на динамически выделенный массив 
    int size;
    public:
    DynamicArray( T* elem, int size) : size(size) {
        data = new T[size]();
        if (elem != nullptr) {
            for (int i = 0; i < size; i++){
                data[i] = elem[i];
            }
        }
    }
    DynamicArray(int size) : size(size) {
        if (size < 0)
            throw std::invalid_argument("Size cannot be negative");
        data = new T[size]();
    }
    
    DynamicArray(const DynamicArray<T> &array) : size(array.size) {
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = array.data[i];
        }
    }

    ~DynamicArray(){
        delete[] data;
    }

    T Get(int index) const {
        if (index < 0 || index >= size){
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    int GetSize() const {
        return size;
    }
   
    void Set(int index, T elem){
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        data[index] = elem;
    }
    void Resize(int newSize) {
        if (newSize < 0)
            throw std::invalid_argument("Size cannot be negative");

        T* newData = new T[newSize]();
        int elementsToCopy = (newSize < size) ? newSize : size;
        
        for (int i = 0; i < elementsToCopy; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }   
};


// int main(){
//     int initialData[] = {1, 2, 3, 4 ,5};
//     DynamicArray<int> arr1(initialData, 5);

//     std::cout << "массив " << std::endl;
//     for (int i = 0; i < arr1.GetSize(); i++){
//         std::cout << arr1.Get(i) << " ";
//     }
// }

#endif // DYNAMIC_ARRAY_H