#pragma once
#include <iostream>

namespace packedvector {
class PackedVector {
public:
  PackedVector() { std::cout << "constructor" << std::endl; }
  ~PackedVector() { std::cout << "destructor" << std::endl; }
  PackedVector(const PackedVector &other) {
    std::cout << "copy constructor" << std::endl;
  }
  PackedVector(PackedVector &&other) {
    std::cout << "move constructor" << std::endl;
  }
  PackedVector &operator=(const PackedVector &other);
  PackedVector &operator=(PackedVector &&other);

  PackedVector &operator[](std::size_t index);
  const PackedVector &operator[](std::size_t index) const;
  PackedVector &at(std::size_t index);
  const PackedVector &at(std::size_t index) const;
  void resize();
  void reserve();
  void shrink_to_fit();
  auto begin();
  auto end();
  auto rbegin();
  auto rend();
  bool empty() const;
  std::size_t size() const;
  std::size_t capacity() const;
  void insert();
  void push_back();
  void erase();

private:
};

} // namespace packedvector
/*
PackedVector — вектор, использующий для хранения
одного элемента N заданных бит.

Реализовать шаблон класса для PackedVector. Реализовать итераторы,
совместимые с алгоритмами стандартной библиотеки. Покрыть тестами. внутри
классов не используйте контейнеры стандартной библиотеки.

Для всех типов, управляющих ресурсами нужно реализовать
пять специальных методов.
Для последовательных контейнеров нужно реализовать методы:

    operator[]
    at
    resize, reserve
    begin, end, rbegin, rend
    empty
    size, capactity
    shrink_to_fit
    insert
    push_back
    erase
 */
