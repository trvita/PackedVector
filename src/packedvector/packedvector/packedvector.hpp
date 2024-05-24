#pragma once
#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>

namespace packedvector {
// N - количество бит на элемент

template <typename T, std::size_t N> class PackedVector {
  static_assert(N > 0 && N <= (sizeof(T) * 8),
                "N must be within the bit-width of T");

private:
  std::size_t size_ = 0;
  std::size_t capacity_ = 0;
  static constexpr std::size_t bits = N;
  std::unique_ptr<uint8_t[]> data_ = std::make_unique < uint8_t[] >= 0;

  T getBits(std::size_t index) const {
    T result = 0;
    std::uint8_t getBit = 0;
    for (std::size_t bit = index * bits; bit < bits * (index + 1); bit++) {
      getBit = (data_[bit / 8U] >> (7U - bit % 8U)) & 1U;
      result |= getBit << (bit % bits);
    }
    return result;
  }

  void setBits(std::size_t index, T value) {
    for (std::size_t bit = index * bits; bit < bits * (index + 1); bit++) {
      std::uint8_t setBit = (value >> (bit % bits)) & 0x1;
      if (setBit != 0) {
        data_[bit / 8U] |= (1U << (7U - bit % 8U));
      } else {
        data_[bit / 8U] &= ~(1U << (7U - bit % 8U));
      }
    }
  }

  void resizeCap(std::size_t newCapacity) {
    std::size_t newByteSize = 0;
    newByteSize = ((newCapacity * bits) + 7) / 8;
    std::unique_ptr<uint8_t[]> newData =
        std::make_unique<uint8_t[]>(newByteSize);
    std::memset(newData.get(), 0, newByteSize);

    if (data_) {
      std::size_t oldByteSize = 0;
      oldByteSize = ((capacity_ * bits) + 7) / 8;
      std::memcpy(newData.get(), data_.get(),
                  std::min(oldByteSize, newByteSize));
    }

    data_ = std::move(newData);
    capacity_ = newCapacity;
  }

public:
  //  Коснтрукторы, деструктор, специальные методы------------------------
  //  Конструктор по умолчанию
  PackedVector() = default;

  // Конструктор по диапазону
  template <typename It> PackedVector(It begin, It end) {
    for (auto it = begin; it != end; ++it) {
      push_back(*it);
    }
  }
  // Конструктор по списку инициализации
  explicit PackedVector(std::initializer_list<T> list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
      push_back(*it);
    }
  }

  // Копирующий конструктор
  PackedVector(const PackedVector &other) {
    for (std::size_t i = 0; i < other.size(); ++i) {
      push_back(other[i]);
    }
  }

  // Перемещающий конструктор
  PackedVector(PackedVector &&other) noexcept noexcept {
    data_ = std::move(other.data_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  // Деструктор
  ~PackedVector() { clear(); }

  // Копирующий оператор присваивания
  PackedVector &operator=(const PackedVector &other) {
    if (this != &other) {
      clear();
      for (std::size_t i = 0; i < other.size(); ++i) {
        push_back(other[i]);
      }
    }
    return *this;
  }

  // Перемещающий оператор присваивания
  PackedVector &operator=(PackedVector &&other) noexcept {
    if (this != &other) {
      data_.reset();
      size_ = 0;
      capacity_ = 0;

      data_ = std::move(other.data_);
      size_ = other.size_;
      capacity_ = other.capacity_;

      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  // Операторы доступа по индексу
  // T &operator[](std::size_t index) { // todo}
  T operator[](std::size_t index) const { return getBits(index); }

  // Методы доступа
  // T &at(std::size_t index) {// todo}
  T at(std::size_t index) const {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    return getBits(index);
  }

  // Методы изменения размера
  void resize(std::size_t newSize) {
    if (newSize < size_) {
      size_ = newSize;
      if (newSize < capacity_) {
        shrink_to_fit();
      }
    } else if (newSize > size_) {
      reserve(newSize);
      while (size_ < newSize) {
        push_back(T());
      }
    }
  }
  void reserve(std::size_t newCapacity) {
    if (newCapacity > capacity_) {
      resizeCap(newCapacity);
    }
  }
  void shrink_to_fit() {
    if (capacity_ > size_) {
      resizeCap(size_);
    }
  }

  void clear() {
    size_ = 0;
    if (data_) {
      std::memset(data_.get(), 0, ((capacity_ * bits) + 7) / 8);
    }
  }

  // Итераторы------------------------------------------------------------
  // Неконстантный итератор
  class Iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

  private:
    PackedVector<T, N> *container = nullptr;
    std::size_t index = 0;

  public:
    explicit Iterator(PackedVector<T, N> &cont = nullptr, std::size_t /*index_*/
                                                          = 0) = default;
    value_type operator*() const { return (*container)[index]; }

    pointer operator->() const { return &((*container)[index]); }

    Iterator &operator++() {
      ++index;
      return *this;
    }

    const Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    Iterator &operator--() {
      --index;
      return *this;
    }

    const Iterator operator--(int) {
      Iterator temp = *this;
      --(*this);
      return temp;
    }
    Iterator &operator+=(difference_type /*n*/) {
      index += n;
      return *this;
    }

    Iterator operator+(difference_type /*n*/) const {
      Iterator temp = *this;
      temp += n;
      return temp;
    }

    Iterator &operator-=(difference_type /*n*/) {
      index -= n;
      return *this;
    }

    Iterator operator-(difference_type /*n*/) const {
      Iterator temp = *this;
      temp -= n;
      return temp;
    }

    bool operator==(const Iterator &other) const {
      return index == other.index;
    }

    bool operator!=(const Iterator &other) const {
      return index != other.index;
    }
  };

  // Неконстантный обратный итератор
  class ReverseIterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

  private:
    PackedVector<T, N> *container = nullptr;
    std::size_t index = 0;

  public:
    explicit ReverseIterator(PackedVector<T, N> &cont = nullptr,
                             std::size_t /*index_*/ = 0) = default;
    value_type operator*() const { return (*container)[index]; }

    pointer operator->() const { return &((*container)[index]); }

    ReverseIterator &operator++() {
      --index;
      return *this;
    }

    const ReverseIterator operator++(int) {
      ReverseIterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const ReverseIterator &other) const {
      return index == other.index;
    }

    bool operator!=(const ReverseIterator &other) const {
      return index != other.index;
    }
    ReverseIterator operator+(difference_type n) const {
      return ReverseIterator(*container, index - n);
    }

    ReverseIterator operator-(difference_type n) const {
      return ReverseIterator(*container, index + n);
    }
  };

  // Методы для работы с итераторами

  Iterator begin() { return Iterator(*this, 0); }
  Iterator end() { return Iterator(*this, size_); }
  ReverseIterator rbegin() { return ReverseIterator(*this, size_ - 1); }
  ReverseIterator rend() { return ReverseIterator(*this, -1); }

  // Дополнительные методы
  bool empty() const { return size_ == 0; }

  std::size_t size() const noexcept { return size_; }

  std::size_t capacity() const noexcept { return capacity_; }

  void insert(std::size_t index, const T &value) {
    if (index > size_) {
      throw std::out_of_range("Index out of range");
    }
    resize(size_ + 1);
    for (std::size_t i = size_ - 1; i > index; --i) {
      setBits(i, getBits(i - 1));
    }
    setBits(index, value);
  }

  Iterator insert(Iterator /*pos*/, const T &value) {
    std::size_t index = 0;
    index = std::distance(begin(), pos);
    resize(size_ + 1);
    for (std::size_t i = size_ - 1; i > index; --i) {
      setBits(i, getBits(i - 1));
    }
    setBits(index, value);

    return Iterator(*this, index);
  }

  void push_back(const T &value) {
    if (size_ >= capacity_) {
      resizeCap(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    setBits(size_, value);
    ++size_;
  }

  void erase(std::size_t /*position*/) {
    if (position >= size_) {
      throw std::out_of_range("Index out of range");
    }

    for (std::size_t i = position; i < size_ - 1; ++i) {
      setBits(i, getBits(i + 1));
    }

    resize(size_ - 1);
  }

  Iterator erase(Iterator /*pos*/) {
    if (pos == end()) {
      throw std::out_of_range("Invalid iterator");
    }
    std::size_t index = 0;
    index = std::distance(begin(), pos);
    for (std::size_t i = index; i < size_ - 1; ++i) {
      setBits(i, getBits(i + 1));
    }
    resize(size_ - 1);
    return Iterator(*this, index);
  }

  const std::unique_ptr<uint8_t[]> &getData() const {
    return data_;
  } // для теста на память
};
/*
PackedVector — вектор, использующий для хранения одного элемента N заданных
бит.
Для всех типов, управляющих ресурсами нужно реализовать пять специальных
методов.
Реализовать шаблон класса для структуры данных в соответствии со своим
вариантом. Реализовать итераторы, совместимые с алгоритмами стандартной
библиотеки. Покрыть тестами.

Считаем, что вы полностью реализуете управление ресурсами, поэтому внутри
классов не используйте контейнеры стандартной библиотеки.
реализовать методы:

    operator[]
    at
    resize, reserve
    begin, end, rbegin, rend
    empty
    size, capactiy
    shrink_to_fit
    insert
    push_back
    erase
*/
} // namespace packedvector
