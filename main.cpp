#include <iostream>

using std::cout;
using std::endl;

/**
 * Fibonacci compile time magic
 */
template <int count>
class Fib {
  public:
    constexpr static int value = Fib<count-1>::value + Fib<count-2>::value;
};

template <>
class Fib<0> {
  public:
    constexpr static int value = 1;
};

template <>
class Fib<1> {
  public:
    constexpr static int value = 1;
};

// trait to put values in local array instead of allocating
template <typename T>
class is_local_array {
    enum {
        result = false

    };
};
// enable for ints
template <>
class is_local_array<int> {
    enum {
        result = true
    };
};

template <class T>
class Ptr {
    T* val_;
public:
    Ptr(T* val) {
        val_ = val;
        cout<<"Create Ptr"<<endl;
    }
    Ptr(Ptr<T>&& other) {
        val_ = other.val_;
        other.val_ = nullptr;
        cout<<"Move to Ptr"<<endl;
    }
    Ptr(Ptr<T>& other) = delete;
    ~Ptr() {
        if (val_) {
            cout<<"Destroy Ptr with value"<<endl;
            delete val_;
        } else {
            cout<<"Destroy Ptr without value"<<endl;
        }
    }
    T* operator->() {
        return val_;
    }
    const T& operator*() {
        return *val_;
    }
};

template <int v>
class AllocationSelector {
    enum {
        value = v
    };
};

const int ALLOC_BUF_SIZE = 8192;
uint8_t data[ALLOC_BUF_SIZE];
int freePos = 0;

template <typename T>
class Allocator {

    template <class ... Types>
    static T* allocate(Types ... args) {
        // should make choice of allocator
        return allocate(
            std::forward<Types>(args)..., 
            AllocationSelector<is_local_array<T>::value>());
    }

    template <class ... Types>
    static T* allocateObj(Types ... args, AllocationSelector<false>) {
        return new T(std::forward<Types>(args)...);
    }
    template <class ... Types>
    static T* allocateObj(Types ... args, AllocationSelector<true>) {
        if 
        // should make different allocation
        return Ptr<T>(new T(std::forward<Types>(args)...));
    }
};

template <class T, class ... Types>
Ptr<T> makePtr(Types ... args) {
    return Ptr<T>(new T(std::forward<Types>(args)...));
}

struct Value {
    int x;
    Value(int val) : x(val) {}
};

Ptr<Value> work();

Ptr<Value> work() {
    Ptr<Value> element = makePtr<Value>(1);
    cout<<"New object contains "<<element->x<<endl;
    return std::move(element);
}

int main(int /*argc*/, char ** /*argv*/) {
    cout << "Value is " << Fib<40>::value << endl;
    Ptr<Value> v = work();
    auto s = makePtr<std::string>("Hello");
    cout<<"Value = " << *s << endl;
    return 0;
}
