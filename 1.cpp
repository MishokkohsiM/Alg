/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв. Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
 * 1_2. Для разрешения коллизий используйте двойное хеширование.*/




#include <iostream>
#include <cassert>
using namespace std;

template <class T>
struct my_hash {
public:
    my_hash() {}
    size_t operator()(const T &value , int hash) {
        string A = string(value);
        const size_t n = A.length();
        if (!n)
            return 0;
        size_t result = 0;
        size_t i = n - 1;
        while (i > 0) {
            result += (size_t) A[i];
            result *= hash;
            --i;
        }
        result += (size_t) A[0];
        return result;
    }
};

template <class T>
class HashTable {
public:
    HashTable();
    HashTable (const HashTable &);
    HashTable& operator = (const HashTable &);
    bool Add_H(const T &);
    bool Delete_H(const T &);
    bool Has_H(const T &);
    void init();
    void printArray();
    ~HashTable();
private:
    my_hash<T> HASH_FUNC;
    T* array;
    bool* Empty;
    bool* is_delete;
    size_t len_Hash;
    size_t size;
    size_t reSize;
    void ReHash();
};

template <class T>
HashTable<T>::HashTable(): len_Hash(0), size(8), reSize(size/4*3){
    array = new T[size];
    is_delete = new bool[size];
    Empty = new bool[size];
    for (int i = 0; i < size; i++){
        is_delete[i] = false;
        Empty[i] = true;
    }
}

template <class T>
HashTable<T>::HashTable(const HashTable & hash): len_Hash(hash.len_Hash), size(hash.size), reSize(hash.reSize) {
    array = new T[size];
    is_delete = new bool[size];
    Empty = new bool[size];
    for (int i = 0; i < size; i++){
        array[i] = hash.array[i];
        is_delete[i] = hash.is_delete[i];
        Empty[i] = hash.Empty[i];
    }
}

template <class T>
HashTable<T>& HashTable<T>::operator=(const HashTable & other) {
    if (this == &other)
        return *this;
    delete[] array; delete[] is_delete; delete[] Empty;
    len_Hash = other.len_Hash; size = other.size; reSize = other.reSize;
    array = new T [size]; is_delete = new bool[size]; Empty = new bool[size];
    for (int i = 0; i < size; i++){
        array[i] = other.array[i];
        is_delete[i] = other.is_delete[i];
        Empty[i] = other.Empty[i];
    }
    return *this;
}

template <class T>
bool HashTable<T>::Add_H(const T & value) {
    size_t hash_1 = HASH_FUNC(value,5) % size;
    size_t hash_2 = HASH_FUNC(value,7) % size;
    if (hash_2 % 2 == 0)
        hash_2 += 1;
    size_t hash = hash_1;
    size_t step = 1;
    int first_delete = -1;
    while ((!Empty[hash] && !is_delete[hash]) || (Empty[hash] && is_delete[hash])){
        if (array[hash] ==  value && !Empty[hash])
            return false;
        if (step > size){
            break;
        }
        if (is_delete[hash] == true && first_delete == -1)
            first_delete = hash;
        hash = (hash_1 + step * hash_2) % size;
        step++;
    }
    if (first_delete != -1)
        hash = first_delete;
    array[hash] = value;
    Empty[hash] = false;
    is_delete[hash] = false;
    len_Hash++;
    if (len_Hash == reSize)
        ReHash();
    return true;
}

template <class T>
bool HashTable<T>::Has_H(const T & value) {
    if (!len_Hash)
        return false;
    size_t hash_1 = HASH_FUNC(value,5) % size;
    size_t hash_2 = HASH_FUNC(value,7) % size;
    if (hash_2 % 2 == 0)
        hash_2 += 1;
    size_t hash = hash_1;
    size_t step = 1;
    while (array[hash] != value || Empty[hash]){
        if (Empty[hash] && !is_delete[hash])
            return false;
        if (step > size)
            return false;
        hash = (hash_1 + step * hash_2) % size;
        step++;
    }
    return true;
}

template <class T>
bool HashTable<T>::Delete_H(const T & value) {
    if (!len_Hash)
        return false;
    size_t hash_1 = HASH_FUNC(value,5) % size;
    size_t hash_2 = HASH_FUNC(value,7) % size;
    if (hash_2 % 2 == 0)
        hash_2 +=1;
    size_t hash = hash_1;
    size_t step = 1;
    while (array [hash] != value || Empty[hash]){
        if (Empty[hash] && !is_delete[hash])
            return false;
        if (step > size)
            return false;
        hash = (hash_1 + step * hash_2) % size;
        step++;
    }
    len_Hash--;
    Empty[hash] = true;
    is_delete[hash] = true;
    return true;
}

template < class T>
void HashTable<T>::ReHash() {
    size *= 2;
    T* old_array = array;
    bool* old_Empty = Empty;
    Empty = new bool[size];
    array = new T [size];
    delete[] is_delete;
    is_delete = new bool[size];
    for (int i = 0; i < size; i++){
        is_delete[i] = false;
        Empty[i] = true;
    }
    len_Hash = 0; reSize = size/4*3;
    for (int i = 0; i < size / 2; i++){
        if (!old_Empty[i])
            Add_H(old_array[i]);
    }
    delete[] old_Empty;
    delete[] old_array;
}

template <class T>
void HashTable<T>::printArray() {
    for (int i = 0; i < size; i++){
        if (Empty[i])
            cout << "-" << " ";
        else
            cout << array[i] << " ";
    }
    cout << endl;
}

template <class T>
HashTable<T>::~HashTable() {
    delete[] Empty;
    delete[] array;
    delete[] is_delete;
}

int main() {
    HashTable<string> hashtable;
    char command;
    string value;
    while (cin >> command >> value){
        switch (command){
            case '+':
                if (hashtable.Add_H(value))
                    cout << "OK" << endl;
                else
                    cout << "FAIL" << endl;
                break;
            case '-':
                if (hashtable.Delete_H(value))
                    cout << "OK" << endl;
                else
                    cout << "FAIL" << endl;
                break;
            case '?':
                if (hashtable.Has_H(value))
                    cout << "OK" << endl;
                else
                    cout << "FAIL" << endl;
                break;
            default:
                assert(false);
        }
        /*hashtable.printArray();*/
    }
    return 0;
}