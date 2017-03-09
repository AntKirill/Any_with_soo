#ifndef ANYWITHS00_H
#define ANYWITHS00_H

#include <iostream>
#include <typeinfo>

namespace mylib {

    struct bad_any_cast : public std::bad_cast {
        const std::string message;

        bad_any_cast(const std::string &message) : message(message) {}

        virtual const char *what() const noexcept {
            return message.c_str();
        }
    };

    struct any {
        // construct/copy/destruct
        any() {
            ptr = nullptr;
        }

        any(const any &rhs) {
            ptr = rhs.ptr->make_copy();
        }

        any(any &&rhs) noexcept {
            ptr = rhs.ptr;
        }

        template<typename ValueType>
        any(const ValueType &rhs) {
            ptr = new war<ValueType>(rhs);
        }

        template<typename ValueType>
        any(ValueType &&rhs) noexcept {
            ptr = new war<ValueType>(rhs);
        }

        any &operator=(const any &rhs) {
            delete ptr;
            ptr = rhs.ptr->make_copy();
            return *this;
        }

        any &operator=(any &&rhs) noexcept {
            delete ptr;
            ptr = rhs.ptr;
            return *this;
        }

        template<typename ValueType>
        any &operator=(const ValueType &rhs) {
            delete ptr;
            ptr = new war<ValueType>(rhs);
            return *this;
        }

        template<typename ValueType>
        any &operator=(ValueType &&rhs) noexcept {
            delete ptr;
            ptr = new war<ValueType>(rhs);
            return *this;
        }

        ~any() {
            delete ptr;
        }

        // modifiers
        any &swap(any &rhs) noexcept {
            god_of_war *tmp = rhs.ptr;
            rhs.ptr = ptr;
            ptr = tmp;
            return *this;
        }

        // queries
        bool empty() const noexcept {
            return ptr == nullptr;
        }

        const std::type_info &type() const {
            return ptr->get_type_info();
        }

        template<typename T>
        friend T any_cast(any &);

        template<typename T>
        friend T any_cast(any &&);

        template<typename T>
        friend T any_cast(const any &);

        template<typename ValueType>
        friend const ValueType *any_cast(const any *);

        template<typename ValueType>
        friend ValueType *any_cast(any *);

    private:
        struct god_of_war {
            virtual ~god_of_war() {}

            virtual const std::type_info &get_type_info() const noexcept = 0;

            virtual god_of_war *make_copy() const = 0;
        };

        template<typename T>
        struct war : public god_of_war {
            war(T obj) : obj(obj) {}

            const std::type_info &get_type_info() const noexcept {
                return typeid(obj);
            }

            war<T> *make_copy() const {
                return new war<T>(obj);
            }

            T get_obj() noexcept {
                return obj;
            }

            T *get_adr_obj() noexcept {
                return &obj;
            }

        private:
            T obj;
        };

        god_of_war *ptr;

    };

    void swap(any &a, any &b) noexcept {
        a.swap(b);
    }

    template<typename T>
    T any_cast(any &a) {
        any::war<T> *casted = dynamic_cast<any::war<T> *>(a.ptr);
        if (casted == 0) {
            throw mylib::bad_any_cast("bad any cast");
        }
        return casted->get_obj();
    }

    template<typename T>
    T any_cast(any &&a) {
        return any_cast<T>(a);
    }

    template<typename T>
    T any_cast(const any &a) {
        mylib::any tmp;
        tmp = a;
        T ans = any_cast<T>(tmp);
        return ans;
    }

    template<typename T>
    T *any_cast(any *a) {
        any::war<T> *casted = dynamic_cast<any::war<T> *>(a->ptr);
        if (casted == 0) {
            throw mylib::bad_any_cast("bad any cast");
        }
        return casted->get_adr_obj();
    }

    template<typename T>
    const T *any_cast(const any *a) {
        mylib::any *tmp;
        tmp = const_cast<any *>(a);
        T *ans = any_cast<T>(tmp);
        return ans;
    }
}

#endif