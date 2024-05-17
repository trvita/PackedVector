#pragma once
#include <iostream>

namespace packedvector {
class Packedvector {
public:
  Packedvector() { std::cout << "constructor" << std::endl; }
  ~Packedvector() { std::cout << "destructor" << std::endl; }
  Packedvector(const Packedvector &other) {
    std::cout << "copy constructor" << std::endl;
  }
  Packedvector(Packedvector &&other) {
    std::cout << "move constructor" << std::endl;
  }
  Packedvector &operator=(const Packedvector &other) {
    std::cout << "copying assignment operator" << std::endl;
  }
  Packedvector &operator=(Packedvector &&other) {
    std::cout << "moving assignment operator" << std::endl;
  }

  Packedvector &operator[](std::size_t index) {
    std::cout << "operator[]" << std::endl;
  }
  const Packedvector &operator[](std::size_t index) const {
    std::cout << "const operator[]" << std::endl;
  }
  Packedvector &at(std::size_t index) { std::cout << "at" << std::endl; }
  const Packedvector &at(std::size_t index) const {
    std::cout << "const at" << std::endl;
  }
  void resize() { std::cout << "resize" << std::endl; }
  void reserve() { std::cout << "reserve" << std::endl; }
  void shrink_to_fit() { std::cout << "shrink_to_fit" << std::endl; }
  auto begin() { std::cout << "begin" << std::endl; }
  auto end() { std::cout << "end" << std::endl; }
  auto rbegin() { std::cout << "rbegin" << std::endl; }
  auto rend() { std::cout << "rend" << std::endl; }
  bool empty() const { std::cout << "empty" << std::endl; }
  std::size_t size() const { std::cout << "size" << std::endl; }
  std::size_t capacity() const { std::cout << "capacity" << std::endl; }
  void insert() { std::cout << "insert" << std::endl; }
  void push_back() { std::cout << "push_back" << std::endl; }
  void erase() { std::cout << "erase" << std::endl; }

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
