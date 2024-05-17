#pragma once
#include <packedvector.hpp>

namespace packedvector {
void Packedvector::resize() { std::cout << "resize" << std::endl; }
void Packedvector::reserve() { std::cout << "reserve" << std::endl; }
void Packedvector::shrink_to_fit() {
  std::cout << "shrink_to_fit" << std::endl;
}
auto Packedvector::begin() { std::cout << "begin" << std::endl; }
auto Packedvector::end() { std::cout << "end" << std::endl; }
auto Packedvector::rbegin() { std::cout << "rbegin" << std::endl; }
auto Packedvector::rend() { std::cout << "rend" << std::endl; }
bool Packedvector::empty() const { std::cout << "empty" << std::endl; }
std::size_t Packedvector::size() const { std::cout << "size" << std::endl; }
std::size_t Packedvector::capacity() const {
  std::cout << "capacity" << std::endl;
}
void Packedvector::insert() { std::cout << "insert" << std::endl; }
void Packedvector::push_back() { std::cout << "push_back" << std::endl; }
void Packedvector::erase() { std::cout << "erase" << std::endl; }

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
