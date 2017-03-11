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
            if (rhs.empty()) ptr = nullptr;
            ptr = rhs.ptr->make_copy(this->memalloc_ptr);
        }

        any(any &&rhs) noexcept {
            ptr = rhs.ptr;
            rhs.clear();
        }

        template<typename ValueType>
        any(const ValueType &rhs) {
            ptr = memalloc_ptr->allocate(rhs);
        }

        template<typename ValueType, typename = typename std::enable_if<!std::is_same<typename std::decay<ValueType>::type, any>::value>::type >
        any(ValueType &&rhs) noexcept {
            ptr =  memalloc_ptr->allocate(rhs);
        }

        any &operator=(const any &rhs) {
            clear();
            if (rhs.empty()) return *this;
            ptr = rhs.ptr->make_copy(this->memalloc_ptr);
            return *this;
        }

        any &operator=(any &&rhs) noexcept {
            clear();
            *this = rhs;
            rhs.clear();
            return *this;
        }

        template<typename ValueType>
        any &operator=(const ValueType &rhs) {
            clear();
            ptr = memalloc_ptr->allocate( rhs);
            return *this;
        }

        template<typename ValueType, typename = typename std::enable_if<!std::is_same<typename std::decay<ValueType>::type, any>::value>::type>
        any &operator=(ValueType &&rhs) noexcept {
            clear();
            ptr = memalloc_ptr->allocate(rhs);
            return *this;
        }

        ~any() {
            clear();
        }

        // modifiers
        any &swap(any &rhs) noexcept {
//            god_of_war *tmp = rhs.ptr;
//            rhs.ptr = ptr;
//            ptr = tmp;
//
            std::swap(this->ptr, rhs.ptr);
            std::swap(this->memalloc_ptr, rhs.memalloc_ptr);

            return *this;
        }

        // queries
        bool empty() const noexcept {
            return ptr == nullptr;
        }

        const std::type_info &type() const {
            if (empty()) return typeid(void);
            return ptr->get_type_info();
        }

        void clear() noexcept {
            memalloc_ptr->free(ptr);
            ptr = nullptr;
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

        template<typename T>
        using rem_ref_t = typename std::remove_reference<T>::type;

        struct allocator;

        struct god_of_war {
            virtual ~god_of_war() {}

            virtual const std::type_info &get_type_info() const noexcept = 0;

            virtual god_of_war *make_copy(allocator *alloc) const = 0;
        };

        template<typename T>
        struct war : public god_of_war {
            war(T obj) : obj(obj) {}

            war() {}

            const std::type_info &get_type_info() const noexcept {
                return typeid(obj);
            }

            god_of_war *make_copy(allocator *alloc) const {
                return alloc->allocate(obj);
            }

            T get_obj() noexcept {
                return obj;
            }

            T *get_adr_obj() {
                return &obj;
            }

        private:
            T obj;
        };

        class allocator {
            const static int N = 2;
            void * var_for_type;

            using stack_alloc_storage = typename std::aligned_storage<
                    N * sizeof(decltype(var_for_type)), std::alignment_of<decltype(var_for_type)>::value>::type;

            stack_alloc_storage buf[N];

            template <typename T>
            using need_allocation = typename std::integral_constant<bool, !(sizeof(T) <= N * sizeof(stack_alloc_storage) &&
                                                                            std::alignment_of<T>::value <=
                                                                            std::alignment_of<stack_alloc_storage>::value)>;
            enum Tag_t {EMPTY, SMALL, BIG} tag = EMPTY;

        public:
            template<typename T, typename U = typename need_allocation<T>::type>
            god_of_war *allocate(T obj) {
                if (!U::value) {
                    tag = SMALL;
                    return (new(buf) war<rem_ref_t<decltype(obj)>>(obj));
                } else {
                    tag = BIG;
                    return new war<rem_ref_t<decltype(obj)>>(obj);
                }
            }

            void free(god_of_war *ptr) {
                if (tag == SMALL) {
                    ptr->~god_of_war();
                } else if (tag == BIG) delete ptr;
                tag = EMPTY;
            }
        } memalloc;

        god_of_war *ptr;
        allocator *memalloc_ptr = &memalloc;
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
            return nullptr;
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