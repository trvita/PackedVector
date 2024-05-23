#pragma once
#include <cstdint>
#include <iterator>
#include <memory>

// N - количество бит на элемент
template <typename T, size_t N> class PackedVector {
  static_assert(N > 0 && N <= (sizeof(T) * 8),
                "N must be within the bit-width of T");

private:
  size_t size_;
  size_t capacity_;
  static constexpr size_t bits = N;
  std::unique_prt<uint8_t[]>
      data_; // объяснить почему именно uint8_t а не другой размер

  void setBits(size_t index, T value);
  T getBits(size_t index) const;
  void resize(size_t newSize);

public:
  // Конструкторы и деструктор
  PackedVector();
  explicit PackedVector(size_t capacity);
  PackedVector(const PackedVector &other);
  PackedVector(PackedVector &&other) noexcept;
  ~PackedVector();

  // Операторы присваивания
  PackedVector &operator=(const PackedVector &other);
  PackedVector &operator=(PackedVector &&other) noexcept;

  // Оператор доступа по индексу
  T &operator[](size_t index);
  T operator[](size_t index) const;

  // Методы доступа
  T &at(size_t index); // бросает исключение!
  T at(size_t index) const;

  // Методы изменения размера
  void resize(size_t newSize);
  void reserve(size_t newCapacity);
  void shrink_to_fit();

  // Итераторы------------------------------------------------------------
  // Неконстантный итератор
  class Iterator {
  private:
    Node *current_;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::random_access_iterator_tag;

    Iterator(Node *current) : current_(current) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    Iterator &operator++() {
      if (current_ != nullptr) {
        current_ = current_->next;
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      ++(*this);
      return temp;
    }

    Iterator &operator--() {
      if (current_ != nullptr) {
        current_ = current_->prev;
      }
      return *this;
    }

    Iterator operator--(int) {
      Iterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const Iterator &other) const {
      return current_ == other.current_;
    }
    bool operator!=(const Iterator &other) const {
      return current_ != other.current_;
    }
  };

  // Константный итератор
  class ConstIterator {
  private:
    const Node *current_;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;
    using iterator_category = std::random_access_iterator_tag;

    ConstIterator(const Node *current) : current_(current) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    ConstIterator &operator++() {
      if (current_ != nullptr) {
        current_ = current_->next;
      }
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator temp = *this;
      ++(*this);
      return temp;
    }

    ConstIterator &operator--() {
      if (current_ != nullptr) {
        current_ = current_->prev;
      }
      return *this;
    }

    ConstIterator operator--(int) {
      ConstIterator temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const ConstIterator &other) const {
      return current_ == other.current_;
    }
    bool operator!=(const ConstIterator &other) const {
      return current_ != other.current_;
    }
  };
  // Неконстантный обратный итератор
  class ReverseIterator {
  private:
    Node *current_;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::random_access_iterator_tag;

    ReverseIterator(Node *current) : current_(current) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    ReverseIterator &operator++() {
      if (current_ != nullptr) {
        current_ = current_->prev;
      }
      return *this;
    }

    ReverseIterator operator++(int) {

      ReverseIterator temp = *this;
      --(*this);
      return temp;
    }

    ReverseIterator &operator--() {

      if (current_ != nullptr) {
        current_ = current_->next;
      }
      return *this;
    }

    ReverseIterator operator--(int) {

      ReverseIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const ReverseIterator &other) const {
      return current_ == other.current_;
    }
    bool operator!=(const ReverseIterator &other) const {
      return current_ != other.current_;
    }
  };

  // Константный обратный итератор
  class ConstReverseIterator {
  private:
    const Node *current_;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;
    using iterator_category = std::random_access_iterator_tag;

    ConstReverseIterator(const Node *current) : current_(current) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    ConstReverseIterator &operator++() {
      if (current_ != nullptr) {
        current_ = current_->prev;
      }
      return *this;
    }

    ConstReverseIterator operator++(int) {
      ConstReverseIterator temp = *this;
      --(*this);
      return temp;
    }

    ConstReverseIterator &operator--() {
      if (current_ != nullptr) {
        current_ = current_->next;
      }
      return *this;
    }

    ConstReverseIterator operator--(int) {
      ConstReverseIterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const ConstReverseIterator &other) const {
      return current_ == other.current_;
    }
    bool operator!=(const ConstReverseIterator &other) const {
      return current_ != other.current_;
    }
  };

  // Методы для работы с итераторами
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;

  ConstIterator cbegin() const;
  ConstIterator cend() const;

  ReverseIterator rbegin();
  ReverseIterator rend();
  ReverseConstIterator rbegin() const;
  ReverseConstIterator rend() const;
  ReverseConstIterator crbegin() const;
  ReverseConstIterator crend() const;

  // Дополнительные методы
  bool empty() const;
  size_t size() const;
  size_t capacity() const;
  void insert(Iterator pos, const T &value);
  void push_back(const T &value);

  void clear(); // нужна ли с учётом того что мы используем умный итератор?

  Iterator erase(Iterator pos);
  Iterator erase(ConstIterator pos);
  Iterator erase(Iterator first, Iterator last);
  Iterator erase(ConstIterator first, ConstIterator last);
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
