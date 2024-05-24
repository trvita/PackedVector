#pragma once
#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>

namespace packedvector {
// N - количество бит на элемент
template <typename T, size_t N> class PackedVector {
  static_assert(N > 0 && N <= (sizeof(T) * 8),
                "N must be within the bit-width of T");

private:
  size_t size_;
  size_t capacity_;
  static constexpr size_t bits = N;
  std::unique_ptr<uint8_t[]> data_;

  void setBits(size_t index, T value) {
    size_t bitPos = index * bits;
    size_t byteIndex = bitPos / 8;
    size_t bitOffset = bitPos % 8;

    T mask = (static_cast<T>(1) << bits) - 1;
    value &= mask;

    size_t bitsInFirstByte = 8 - bitOffset;

    if (bits <= bitsInFirstByte) {
      uint8_t clearMask = ~(mask << bitOffset);
      data_[byteIndex] = (data_[byteIndex] & clearMask) | (value << bitOffset);
    } else {
      data_[byteIndex] &= ~(mask << bitOffset);
      data_[byteIndex] |= value << bitOffset;

      size_t bitsRemaining = bits - bitsInFirstByte;
      value >>= bitsInFirstByte;
      ++byteIndex;

      while (bitsRemaining >= 8) {
        data_[byteIndex] = static_cast<uint8_t>(value);
        value >>= 8;
        bitsRemaining -= 8;
        ++byteIndex;
      }

      if (bitsRemaining > 0) {
        uint8_t clearMask = ~(static_cast<uint8_t>((1 << bitsRemaining) - 1));
        data_[byteIndex] &= clearMask;
        data_[byteIndex] |= static_cast<uint8_t>(value);
      }
    }
  }

  T getBits(size_t index) const {
    size_t bitPos = index * bits;
    size_t byteIndex = bitPos / 8;
    size_t bitOffset = bitPos % 8;

    size_t bitsInFirstByte = 8 - bitOffset;

    T result = 0;

    if (bits <= bitsInFirstByte) {
      result = (data_[byteIndex] >> bitOffset) & ((1 << bits) - 1);
    } else {
      result = (data_[byteIndex] >> bitOffset);
      size_t bitsRemaining = bits - bitsInFirstByte;
      size_t shiftAmount = bitsInFirstByte;
      ++byteIndex;

      while (bitsRemaining >= 8) {
        result |= static_cast<T>(data_[byteIndex]) << shiftAmount;
        shiftAmount += 8;
        bitsRemaining -= 8;
        ++byteIndex;
      }

      if (bitsRemaining > 0) {
        result |=
            (static_cast<T>(data_[byteIndex]) & ((1 << bitsRemaining) - 1))
            << shiftAmount;
      }
    }

    return result;
  }

  void resizeCap(size_t newCapacity) {
    size_t newByteSize = ((newCapacity * bits) + 7) / 8;
    std::unique_ptr<uint8_t[]> newData =
        std::make_unique<uint8_t[]>(newByteSize);
    std::memset(newData.get(), 0, newByteSize);

    if (data_) {
      size_t oldByteSize = ((capacity_ * bits) + 7) / 8;
      std::memcpy(newData.get(), data_.get(),
                  std::min(oldByteSize, newByteSize));
    }

    data_ = std::move(newData);
    capacity_ = newCapacity;
  }

public:
  // Коснтрукторы, деструктор, специальные методы------------------------

  // Конструктор по умолчанию
  PackedVector() : size_(0), capacity_(0) {}

  // Конструктор по диапазону
  template <typename It>
  PackedVector(It begin, It end) : size_(0), capacity_(0) {
    for (auto it = begin; it != end; ++it) {
      push_back(*it);
    }
  }
  // Конструктор по списку инициализации
  PackedVector(std::initializer_list<T> list) : size_(0), capacity_(0) {
    for (auto it = list.begin(); it != list.end(); ++it) {
      push_back(*it);
    }
  }

  // Копирующий конструктор
  PackedVector(const PackedVector &other) : size_(0), capacity_(0) {
    for (size_t i = 0; i < other.size(); ++i) {
      push_back(other[i]);
    }
  }

  // Перемещающий конструктор
  PackedVector(PackedVector &&other) noexcept {
    data_ = std::move(other.data_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  // Деструктор
  ~PackedVector() = default;

  // Копирующий оператор присваивания
  PackedVector &operator=(const PackedVector &other) {
    if (this != &other) {
      clear();
      for (size_t i = 0; i < other.size(); ++i) {
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

  // Оператор доступа по индексу
  // T &operator[](size_t index);
  T operator[](size_t index) const { return getBits(index); }

  // Методы доступа
  T &at(size_t index); // бросает исключение!
  T at(size_t index) const;

  // Методы изменения размера
  void resize(size_t newSize);
  void reserve(size_t newCapacity);
  void shrink_to_fit();

  void clear() {
    size_ = 0;
    if (data_) {
      std::memset(data_.get(), 0, ((capacity_ * bits_per_element) + 7) / 8);
    }
  }

  // Итераторы------------------------------------------------------------
  // Неконстантный итератор
  // class Iterator {
  // private:
  //   Node *current_;

  // public:
  //   using difference_type = std::ptrdiff_t;
  //   using value_type = T;
  //   using pointer = T *;
  //   using reference = T &;
  //   using iterator_category = std::random_access_iterator_tag;

  //   Iterator(Node *current) : current_(current) {}

  //   reference operator*() const { return current_->data_; }
  //   pointer operator->() const { return &current_->data_; }

  //   Iterator &operator++() {
  //     if (current_ != nullptr) {
  //       current_ = current_->next;
  //     }
  //     return *this;
  //   }

  //   Iterator operator++(int) {
  //     Iterator temp = *this;
  //     ++(*this);
  //     return temp;
  //   }

  //   Iterator &operator--() {
  //     if (current_ != nullptr) {
  //       current_ = current_->prev;
  //     }
  //     return *this;
  //   }

  //   Iterator operator--(int) {
  //     Iterator temp = *this;
  //     --(*this);
  //     return temp;
  //   }

  //   bool operator==(const Iterator &other) const {
  //     return current_ == other.current_;
  //   }
  //   bool operator!=(const Iterator &other) const {
  //     return current_ != other.current_;
  //   }
  // };

  // // Константный итератор
  // class ConstIterator {
  // private:
  //   const Node *current_;

  // public:
  //   using difference_type = std::ptrdiff_t;
  //   using value_type = T;
  //   using pointer = const T *;
  //   using reference = const T &;
  //   using iterator_category = std::random_access_iterator_tag;

  //   ConstIterator(const Node *current) : current_(current) {}

  //   reference operator*() const { return current_->data_; }
  //   pointer operator->() const { return &current_->data_; }

  //   ConstIterator &operator++() {
  //     if (current_ != nullptr) {
  //       current_ = current_->next;
  //     }
  //     return *this;
  //   }

  //   ConstIterator operator++(int) {
  //     ConstIterator temp = *this;
  //     ++(*this);
  //     return temp;
  //   }

  //   ConstIterator &operator--() {
  //     if (current_ != nullptr) {
  //       current_ = current_->prev;
  //     }
  //     return *this;
  //   }

  //   ConstIterator operator--(int) {
  //     ConstIterator temp = *this;
  //     --(*this);
  //     return temp;
  //   }

  //   bool operator==(const ConstIterator &other) const {
  //     return current_ == other.current_;
  //   }
  //   bool operator!=(const ConstIterator &other) const {
  //     return current_ != other.current_;
  //   }
  // };
  // // Неконстантный обратный итератор
  // class ReverseIterator {
  // private:
  //   Node *current_;

  // public:
  //   using difference_type = std::ptrdiff_t;
  //   using value_type = T;
  //   using pointer = T *;
  //   using reference = T &;
  //   using iterator_category = std::random_access_iterator_tag;

  //   ReverseIterator(Node *current) : current_(current) {}

  //   reference operator*() const { return current_->data_; }
  //   pointer operator->() const { return &current_->data_; }

  //   ReverseIterator &operator++() {
  //     if (current_ != nullptr) {
  //       current_ = current_->prev;
  //     }
  //     return *this;
  //   }

  //   ReverseIterator operator++(int) {

  //     ReverseIterator temp = *this;
  //     --(*this);
  //     return temp;
  //   }

  //   ReverseIterator &operator--() {

  //     if (current_ != nullptr) {
  //       current_ = current_->next;
  //     }
  //     return *this;
  //   }

  //   ReverseIterator operator--(int) {

  //     ReverseIterator temp = *this;
  //     ++(*this);
  //     return temp;
  //   }

  //   bool operator==(const ReverseIterator &other) const {
  //     return current_ == other.current_;
  //   }
  //   bool operator!=(const ReverseIterator &other) const {
  //     return current_ != other.current_;
  //   }
  // };

  // // Константный обратный итератор
  // class ConstReverseIterator {
  // private:
  //   const Node *current_;

  // public:
  //   using difference_type = std::ptrdiff_t;
  //   using value_type = T;
  //   using pointer = const T *;
  //   using reference = const T &;
  //   using iterator_category = std::random_access_iterator_tag;

  //   ConstReverseIterator(const Node *current) : current_(current) {}

  //   reference operator*() const { return current_->data_; }
  //   pointer operator->() const { return &current_->data_; }

  //   ConstReverseIterator &operator++() {
  //     if (current_ != nullptr) {
  //       current_ = current_->prev;
  //     }
  //     return *this;
  //   }

  //   ConstReverseIterator operator++(int) {
  //     ConstReverseIterator temp = *this;
  //     --(*this);
  //     return temp;
  //   }

  //   ConstReverseIterator &operator--() {
  //     if (current_ != nullptr) {
  //       current_ = current_->next;
  //     }
  //     return *this;
  //   }

  //   ConstReverseIterator operator--(int) {
  //     ConstReverseIterator temp = *this;
  //     ++(*this);
  //     return temp;
  //   }

  //   bool operator==(const ConstReverseIterator &other) const {
  //     return current_ == other.current_;
  //   }
  //   bool operator!=(const ConstReverseIterator &other) const {
  //     return current_ != other.current_;
  //   }
  // };

  // // Методы для работы с итераторами
  // Iterator begin();
  // Iterator end();
  // ConstIterator begin() const;
  // ConstIterator end() const;

  // ConstIterator cbegin() const;
  // ConstIterator cend() const;

  // ReverseIterator rbegin();
  // ReverseIterator rend();
  // ReverseConstIterator rbegin() const;
  // ReverseConstIterator rend() const;
  // ReverseConstIterator crbegin() const;
  // ReverseConstIterator crend() const;

  // Iterator erase(Iterator pos);
  // Iterator erase(ConstIterator pos);
  // Iterator erase(Iterator first, Iterator last);
  // Iterator erase(ConstIterator first, ConstIterator last);

  // Дополнительные методы
  bool empty() const;
  size_t size() const noexcept { return size_; }
  size_t capacity() const noexcept { return capacity_; }
  // void insert(Iterator pos, const T &value);
  void push_back(const T &value) {
    if (size_ >= capacity_) {
      resizeCap(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    setBits(size_, value);
    ++size_;
  }
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
