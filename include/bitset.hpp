#pragma once

#include <cstdint>

namespace detail {

/// TODO: make with constexpr if
template <typename T>
inline constexpr std::uint32_t get_element_count(const std::uint32_t size)
{
    if ((size / (sizeof(T) * 8)) % (sizeof(T) * 8) != 0)
        return size / (sizeof(T) * 8) + 1;
    else
        return size / (sizeof(T) * 8);
}

/// Bitset interface
///
template <typename T = std::size_t>
class bitset {
public:
    // Constructors
    bitset() = delete;
    bitset(const std::uint32_t size);
    bitset(const bitset<T>& copy);
    bitset(bitset<T>&& copy);

    // Destructor
    ~bitset();

    // Operators
    bool operator[](const std::uint32_t number);

    bitset<T>& operator=(const bitset<T>& copy);
    bitset<T>& operator=(bitset<T>&& copy);

    bool operator==(const bitset<T>& set);
    bool operator!=(const bitset<T>& set);

    bitset<T> operator>>(const std::uint32_t r_shift);
    bitset<T> operator<<(const std::uint32_t l_shift);
    bitset<T>& operator>>=(const std::uint32_t r_shift);
    bitset<T>& operator<<=(const std::uint32_t l_shift);

    bitset<T> operator&(const bitset<T>& set);
    bitset<T> operator&(const T number);
    bitset<T>& operator&=(const bitset<T>& set);
    bitset<T>& operator&=(const T number);

    bitset<T> operator|(const bitset<T>& set);
    bitset<T> operator|(const T number); 
    bitset<T>& operator|=(const bitset<T>& set);
    bitset<T>& operator|=(const T number);

    bitset<T> operator^(const bitset<T>& set);
    bitset<T> operator^(const T number); 
    bitset<T>& operator^=(const bitset<T>& set);
    bitset<T>& operator^=(const T number);

    bitset<T> operator~();

    // Methods
    std::uint32_t count();
    void set(const std::uint32_t number);
    void reset(const std::uint32_t nubmer);
    void push_back(bool elem);
    void push_front(bool elem);
    void pop_back();
    void pop_front();

private:
    inline T get_mask(const std::uint32_t number);
    static inline T* allocate(const std::size_t size);
    static inline void destroy(T* data, const std::size_t size);

private:
    std::uint32_t size;     // size of bit set
    std::uint32_t capacity; // count of elements of type T
    T* data;
};

/// Bitset implementation
template <typename T>
T* bitset<T>::allocate(const std::size_t size)
{
    return reinterpret_cast<T*>(operator new(size * sizeof(T)));
}

template <typename T>
void bitset<T>::destroy(T* data, const std::size_t size)
{
    for (std::size_t i = 0; i < size; ++i)
        (data + i)->~T();
    operator delete(data);
}

template <typename T>
bitset<T>::bitset(const std::uint32_t size) :
    size(size),
    capacity(get_element_count<T>(size)),
    data(allocate(capacity))
{
    for (std::uint32_t i = 0; i < capacity; ++i)
        new (data + i) T(0);
}

template <typename T>
bitset<T>::bitset(const bitset<T>& copy) :
    size(copy.size),
    capacity(copy.capacity),
    data(allocate(capacity))
{
    for (std::uint32_t i = 0; i < size; ++i)
        new (data + i) T(copy.data[i]);
}

template <typename T>
bitset<T>::bitset(bitset<T>&& copy) :
    size(copy.size),
    capacity(copy.capacity),
    data(copy.data)
{
    copy.size = 0;
    copy.capacity = 0;
    copy.data = nullptr;
}

template <typename T>
bitset<T>::~bitset()
{
    destroy(data, capacity);
}

template <typename T>
inline T bitset<T>::get_mask(const std::uint32_t number)
{
    return (T)1 << number;
}

template <typename T>
std::uint32_t bitset<T>::count()
{
    return size;
}

template <typename T>
bool bitset<T>::operator[](const std::uint32_t number)
{
    std::uint32_t index_in_array = number / (sizeof(T) * 8);
    T elem_mask = get_mask(number % (sizeof(T) * 8));
    return data[index_in_array] & elem_mask;
}

template <typename T>
bitset<T>& bitset<T>::operator=(const bitset<T>& copy)
{
    if (this != &copy) {
        if (capacity == copy.capacity) {
            for (std::uint32_t i = 0; i < copy.capacity; ++i)
                data[i] = copy.data[i];
        } else {
            T* tmp = allocate(copy.capacity);
            for (std::size_t i = 0; i < copy.capacity; ++i)
                new (tmp + i) T(copy.data[i]);
            destroy(data, capacity);
            data = tmp;
            capacity = copy.capacity;
        }
        size = copy.size;
    }
    return *this;
}

template <typename T>
bitset<T>& bitset<T>::operator=(bitset<T>&& copy)
{
    if (this != &copy) {
        destroy(data, capacity);
        data = copy.data;
        size = copy.size;
        capacity = copy.capacity;

        copy.data = nullptr;
        copy.size = 0;
        copy.capacity = 0;
    }
    return *this;
}

template <typename T>
bool bitset<T>::operator==(const bitset<T>& set)
{
    if (size != set.size || capacity != set.capacity)
        return false;
    for (std::uint32_t i = 0; i < capacity; ++i)
        if (data[i] != set.data[i])
            return false;
    return true;
}

template <typename T>
bool bitset<T>::operator!=(const bitset<T>& set)
{
    if (size != set.size || capacity != set.capacity)
        return true;
    for (std::uint32_t i = 0; i < capacity; ++i)
        if (data[i] != set.data[i])
            return true;
    return false;
}

/// TODO: replase with move semantic to avoid extra data copy
///
template <typename T>
bitset<T> bitset<T>::operator>>(const std::uint32_t r_shift)
{
    std::uint32_t data_index = r_shift / (sizeof(T) * 8);
    std::uint32_t elem_index = r_shift % (sizeof(T) * 8);
    std::uint32_t remainder  = sizeof(T) * 8 - elem_index;
    T mask = ~((T)0) >> remainder;
    bitset<T> res(*this);
    for (std::uint32_t i = data_index; i < res.capacity - 1; ++i) {
        T tmp = (res.data[i + 1] & mask) << remainder;
        res.data[i - data_index] = tmp | (res.data[i] >> elem_index);
    }
    res.size -= r_shift;
    return res;
}

template <typename T>
bitset<T> bitset<T>::operator<<(const std::uint32_t l_shift)
{
    std::uint32_t data_index = r_shift / (sizeof(T) * 8);
    std::uint32_t elem_index = r_shift % (sizeof(T) * 8);
    std::uint32_t remainder  = sizeof(T) * 8 - elem_index;
    T mask = ~((T)0) << remainder;
    std::uint32_t new_size = size + l_shift;
    bitset<T> res(new_size);
    for (std::uint32_t i = 0; i < res.capacity - data_index; ++i) {
        T tmp = (data[i] & mask) >> remainder;
        res.data[i + data_index] = (data[i] << elem_index) | tmp;
    }
    return res;
}

template <typename T>
bitset<T>& bitset<T>::operator>>=(const std::uint32_t r_shift)
{
    std::uint32_t data_index = r_shift / (sizeof(T) * 8);
    std::uint32_t elem_index = r_shift % (sizeof(T) * 8);
    std::uint32_t remainder  = sizeof(T) * 8 - elem_index;
    T mask = ~((T)0) >> remainder;
    for (std::uint32_t i = data_index; i < capacity - 1; ++i) {
        T tmp = (data[i + 1] & mask) << remainder;
        data[i - data_index] = tmp | (data[i] >> elem_index);
    }
    size -= r_shift;
    return *this;
}

template <typename T>
bitset<T>& bitset<T>::operator<<=(const std::uint32_t l_shift)
{
    std::uint32_t data_index = r_shift / (sizeof(T) * 8);
    std::uint32_t elem_index = r_shift % (sizeof(T) * 8);
    std::uint32_t remainder  = sizeof(T) * 8 - elem_index;
    T mask = ~((T)0) << remainder;
    std::uint32_t new_size = size + l_shift;
    bitset<T> copy(*this);
    for (std::uint32_t i = 0; i < res.capacity - data_index; ++i) {
        T tmp = (copy.data[i] & mask) >> remainder;
        data[i + data_index] = (copy.data[i] << elem_index) | tmp;
    }
    return *this;
}

template <typename T>
void bitset<T>::set(const std::uint32_t number)
{
    std::uint32_t index_in_array = number / (sizeof(T) * 8);
    T elem_mask = get_mask(number % (sizeof(T) * 8));
    data[index_in_array] | elem_mask;
}

template <typename T>
void bitset<T>::reset(const std::uint32_t number)
{
    std::uint32_t index_in_array = number / (sizeof(T) * 8);
    T elem_mask = get_mask(number % (sizeof(T) * 8));
    data[index_in_array] ^ elem_mask;
}

template <typename T>
void bitset<T>::push_back(bool elem)
{
    if (size == capacity * sizeof(T) * 8) {
        T* tmp = allocate(capacity + 1);
        for (std::uint32_t i = 0; i < capacity; ++i)
            new (tmp + i) T(data[i]);
        new (tmp + capacity) T(0);
        destroy(data, capacity);
        data = tmp;
        ++capacity;
    }
    elem ? set(size) : reset(size);
    ++size;
}

template <typename T>
void bitset<T>::push_front(bool elem)
{
    *this <<= 1u;
    elem ? set(size) : reset(size);
}

template <typename T>
void bitset<T>::pop_back()
{
    --size;
}

template <typename T>
void bitset<T>::pop_front()
{
    *this >>= 1u;
}

}
