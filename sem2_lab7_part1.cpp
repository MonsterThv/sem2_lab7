#include <iostream>
#include <utility>

using namespace std;

template<class DataType>
class UniquePtr {
    DataType* rawPtr = nullptr;
public:
    explicit UniquePtr(DataType* ptr) : rawPtr(ptr) {}

    // Удаляем копирование
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Перемещение
    UniquePtr(UniquePtr&& other) noexcept {
        rawPtr = other.rawPtr;
        other.rawPtr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete rawPtr;
            rawPtr = other.rawPtr;
            other.rawPtr = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        delete rawPtr;
    }

    DataType* get() const {
        return rawPtr;
    }

    DataType& operator*() {
        return *rawPtr;
    }

    DataType* operator->() {
        return rawPtr;
    }
};

template<class DataType, class... Args>
UniquePtr<DataType> CreateUnique(Args&&... args) {
    return UniquePtr<DataType>(new DataType(forward<Args>(args)...));
}

class Point {
    int coordX;
    int coordY;
public:
    Point(int x, int y) : coordX(x), coordY(y) {
        cout << "Создан объект: " << coordX << ", " << coordY << "\n";
    }

    ~Point() {
        cout << "Удален объект: " << coordX << ", " << coordY << "\n";
    }

    void display() const {
        cout << "Координаты: (" << coordX << ", " << coordY << ")\n";
    }
};

int main() {
    auto ptr1 = CreateUnique<Point>(30, 40);
    ptr1->display();

    auto ptr2 = move(ptr1);
    if (!ptr1.get()) cout << "Указатель ptr1 теперь пуст\n";
    ptr2->display();

    UniquePtr<Point> ptr3(nullptr);
    ptr3 = move(ptr2);
    if (!ptr2.get()) cout << "Указатель ptr2 теперь пуст\n";
    ptr3->display();

    return 0;
}