#include <string>

class BitArray {
public:
    BitArray();
    ~BitArray();

    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);

    void swap(BitArray& b);

    BitArray& operator=(const BitArray& b);

    void resize(int num_bits, bool value = false);
    void clear();
    void push_back(bool bit);

    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);

    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;

    bool operator[](int i) const;

    int size() const;
    bool empty() const;

    std::string to_string() const;

private:
  unsigned long* data_;
  static const int bits_per_long = sizeof(unsigned long) * 8; // Количество бит в одном unsigned long
  int num_bits_;
  int capacity_;
  
  void _reserve(int new_capacity);
  void _set(int index, bool value);
  bool _get(int index) const;
  void _allocate_and_initialize(int num_bits, unsigned long value);
  void _allocate_and_copy(const BitArray& b);

};

bool operator==(const BitArray & a, const BitArray & b);
bool operator!=(const BitArray & a, const BitArray & b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);
