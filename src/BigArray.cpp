#include "../include/BigArray.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <string>
#include <bitset>

#include <iostream>

// Конструктор по умолчанию
BitArray::BitArray() : num_bits_(0), capacity_(0), data_(nullptr) {}

// Деструктор
BitArray::~BitArray() {
    delete[] data_;
}

// Конструирует массив, хранящий заданное количество бит.
// Первые sizeof(unsigned long) бит можно инициализровать с помощью параметра value.
BitArray::BitArray(int num_bits, unsigned long value) 
    : num_bits_(num_bits), capacity_((num_bits + bits_per_long - 1) / bits_per_long) {
    _allocate_and_initialize(num_bits, value);
}

// Конструктор копирования
BitArray::BitArray(const BitArray& b) : num_bits_(b.num_bits_), capacity_(b.capacity_) {
    _allocate_and_copy(b);
}

// Обменивает значения двух битовых массивов.
void BitArray::swap(BitArray& b) {
    std::swap(num_bits_, b.num_bits_);
    std::swap(capacity_, b.capacity_);
    std::swap(data_, b.data_);
}

// Оператор присваивания
BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        BitArray temp(b);
        swap(temp); //
    }
    return *this;
}

// Изменяет размер массива. В случае расширения, новые элементы
// инициализируются значением value.
void BitArray::resize(int num_bits, bool value) {
    if (num_bits == num_bits_) {
        return;
    }
    
    int new_capacity = (num_bits + bits_per_long - 1) / bits_per_long;

    if (new_capacity > capacity_) {
        _reserve(new_capacity);
    }

    if (num_bits > num_bits_) {
        unsigned long fill_value = value ? ~0UL : 0;
        // Инициализируем новые блоки значением fill_value
        int start_index = (num_bits_ + bits_per_long - 1) / bits_per_long;
        int end_index = new_capacity;
        std::fill_n(data_ + start_index, end_index - start_index, fill_value);

        if (num_bits_ % bits_per_long != 0) {
            int remaining_bits = bits_per_long - (num_bits_ % bits_per_long);
            unsigned long mask = (1UL << remaining_bits) - 1;
            if (value) {
                data_[start_index - 1] |= mask;
            } else {
                data_[start_index - 1] &= ~mask;
            }
        }
    }

    num_bits_ = num_bits;
}

// Очищает массив.
void BitArray::clear() {
    delete[] data_;
    data_ = nullptr;
    num_bits_ = 0;
    capacity_ = 0;
}

// Добавляет новый бит в конец массива. В случае необходимости
// происходит перераспределение памяти.
void BitArray::push_back(bool bit) { // needed to refactor
    if (num_bits_ == capacity_ * bits_per_long) {
        _reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    ++num_bits_;
    _set(num_bits_- 1, bit);
}


// Остальные методы класса

// Битовые операции над массивами.
// Работают только на массивах одинакового размера.
// Обоснование реакции на параметр неверного размера входит в задачу.
BitArray& BitArray::operator&=(const BitArray& b) {
    if (num_bits_ != b.num_bits_) {
        throw std::invalid_argument("BitArray sizes do not match");
    }
    for (int i = 0; i < capacity_; ++i) {
        data_[i] &= b.data_[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (num_bits_ != b.num_bits_) {
        throw std::invalid_argument("BitArray sizes do not match");
    }
    for (int i = 0; i < capacity_; ++i) {
        data_[i] |= b.data_[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (num_bits_ != b.num_bits_) {
        throw std::invalid_argument("BitArray sizes do not match");
    }
    for (int i = 0; i < capacity_; ++i) {
        data_[i] ^= b.data_[i];
    }
    return *this;
}

// Битовый сдвиг с заполнением нулями.
BitArray& BitArray::operator<<=(int n) {        // ?
    if (n < 0) {
        throw std::invalid_argument("Negative shift count");
    }
    if (n >= num_bits_) {
        clear();
        return *this;
    }
    int shift_words = n / bits_per_long;
    int shift_bits = n % bits_per_long;
    if (shift_bits == 0) {
        std::rotate(data_, data_ + shift_words, data_ + capacity_);
    } else {
        for (int i = 0; i < capacity_; ++i) {
            unsigned long high = (i + shift_words < capacity_) ? data_[i + shift_words] : 0;
            unsigned long low = (i + shift_words + 1 < capacity_) ? data_[i + shift_words + 1] : 0;
            data_[i] = (high << shift_bits) | (low >> (bits_per_long - shift_bits));
        }
    }
    std::fill(data_ + capacity_ - shift_words, data_ + capacity_, 0);
    return *this;
}

BitArray& BitArray::operator>>=(int n) {              // ?
    if (n < 0) {
        throw std::invalid_argument("Negative shift count");
    }
    if (n >= num_bits_) {
        clear();
        return *this;
    }
    int shift_words = n / bits_per_long;
    int shift_bits = n % bits_per_long;
    if (shift_bits == 0) {
        std::rotate(data_, data_ + capacity_ - shift_words, data_ + capacity_);
    } else {
        for (int i = capacity_ - 1; i >= 0; --i) {
            unsigned long high = (i - shift_words >= 0) ? data_[i - shift_words] : 0;
            unsigned long low = (i - shift_words - 1 >= 0) ? data_[i - shift_words - 1] : 0;
            data_[i] = (high >> shift_bits) | (low << (bits_per_long - shift_bits));
        }
    }
    std::fill(data_, data_ + shift_words, 0);
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

// Устанавливает бит с индексом n в значение val.
BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= num_bits_) {
        throw std::out_of_range("Index out of range");
    }
    int long_index = n / bits_per_long;
    int bit_index = n % bits_per_long;
    if (val) {
        data_[long_index] |= (1UL << bit_index);
    } else {
        data_[long_index] &= ~(1UL << bit_index);
    }
    return *this;
}

// Заполняет массив истиной.  ??
BitArray& BitArray::set() {
    std::fill(data_, data_ + capacity_, ~0UL);
    if (num_bits_ % bits_per_long != 0) {
        data_[capacity_ - 1] &= (1UL << (num_bits_ % bits_per_long)) - 1;
    }
    return *this;
}

// Устанавливает бит с индексом n в значение false.
BitArray& BitArray::reset(int n) {
    return set(n, false);
}

// Заполняет массив ложью.
BitArray& BitArray::reset() {
    std::fill(data_, data_ + capacity_, 0);
    return *this;
}

// true, если массив содержит истинный бит.
bool BitArray::any() const {
    for (int i = 0; i < capacity_; ++i) {
        if (data_[i] != 0) {
            return true;
        }
    }
    return false;
}

// true, если все биты были ложны
bool BitArray::none() const {
    for (int i = 0; i < capacity_; ++i) {
        if (data_[i] != 0) {
            return false;
        }
    }
    return true;
}

// инверсия битов
BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (int i = 0; i < capacity_; ++i) {
        unsigned long temp = ~data_[i]; 
        result.data_[i] = temp;
    }
    return result;
}

// колличество единичных бит
int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < num_bits_; ++i) {
        if (_get(i)) {
            ++count;
        }
    }
    return count;
}

bool BitArray::operator[](int i) const {
    return _get(i);
}

int BitArray::size() const {
    return num_bits_;
}

bool BitArray::empty() const {
    return num_bits_ == 0;
}

std::string BitArray::to_string() const {
    std::string str;
    str.reserve(num_bits_);
    for (int i = num_bits_ - 1; i >= 0; --i) {
        str.push_back(_get(i) ? '1' : '0');
    }
    return str;
}



// увеличивает размер по capacity
void BitArray::_reserve(int new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }

    unsigned long* new_data = new unsigned long[new_capacity]{0};

    if (data_ != nullptr) {
        std::memcpy(new_data, data_, sizeof(unsigned long) * ((num_bits_ + bits_per_long - 1) / bits_per_long));
        delete[] data_;
    }

    data_ = new_data;
    capacity_ = new_capacity;
}

// Устанавливает значение бита по индексу.
void BitArray::_set(int index, bool value) {
    if (index < 0 || index >= num_bits_) {
        throw std::out_of_range("Index out of range");
    }
    int long_index = index / bits_per_long;
    int bit_index = index % bits_per_long;
    if (value) { // needed to refactor
        data_[long_index] |= (1UL << bit_index);
    } else {
        data_[long_index] &= ~(1UL << bit_index);
    }
}

// Получает значение бита по индексу.
bool BitArray::_get(int index) const {
    if (index < 0 || index >= num_bits_) {
        throw std::out_of_range("Index out of range");
    }
    int long_index = index / bits_per_long;
    int bit_index = index % bits_per_long;
    // return (data_[long_index] >> (num_bits_ % bits_per_long) - bit_index - 1) & 1;
    return (data_[long_index] >> bit_index) & 1;
}

void BitArray::_allocate_and_initialize(int num_bits, unsigned long value) {
    capacity_ = (num_bits + bits_per_long - 1) / bits_per_long;
    data_ = new unsigned long[capacity_]{0};
    if (num_bits > 0) {
        data_[0] = value;
    }
}

void BitArray::_allocate_and_copy(const BitArray& b) {
    data_ = new unsigned long[b.capacity_]{0};
    std::memcpy(data_, b.data_, sizeof(unsigned long) * ((b.num_bits_ + bits_per_long - 1) / bits_per_long));
}


// refactor is indeed !
bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}


// ok
BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}

template<typename T>
void show_Array_splited_by_Byte(const T& a){ 
    for (size_t i = 0; i < 64; i++)
    {   
        if (i % 4 == 0){
            std::cout << ' ';
        }
        std::cout << a[i];
    }
    std::cout << std::endl;
}