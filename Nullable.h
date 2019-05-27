#pragma once
#include <stdexcept>
#include <type_traits>

using namespace std;

class Null {};

template <typename T> class Nullable {
    bool has_v = false;
    union { T v; };

public:
    Nullable() {}

    Nullable(Null n) {} // 提供一个免去泛型参数的空初始化

    Nullable& operator=(Nullable&& n) noexcept {
        if (&n != this) {
            if (has_v) {
                v.~T();
            }
            has_v = n.has_v;
            if (has_v) {
                new(&v) T(move(n.v));
                n.has_v = false;
            }
        }
        return *this;
    }

    Nullable(Nullable&& n) noexcept {
        *this = n;
    }

    Nullable& operator=(const Nullable& n) {
        if (&n != this) {
            if (has_v) {
                v.~T();
            }
            has_v = n.has_v;
            if (has_v) {
                new(&v) T(n.v);
            }
        }
        return *this;
    }

    Nullable(const Nullable& n) {
        *this = n;
    }

    template<typename ...Args>
    Nullable(Args&& ... args) :
        v(forward<Args>(args)...), has_v(true)
    {
    }

    bool Null() const { return !has_v; }

    const T& Value() const {
        if (!has_v)
            throw logic_error("Null value");
        return v;
    }

    T& Value() {
        if (!has_v)
            throw logic_error("Null value");
        return v;
    }

    const T* operator->() const {
        return &Value();
    }

    T* operator->() {
        return &Value();
    }

    operator bool() const {
        return has_v;
    }

    ~Nullable() {
        if (has_v) {
            has_v = false;
            v.~T();
        }
    }
};
