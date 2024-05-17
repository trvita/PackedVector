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
