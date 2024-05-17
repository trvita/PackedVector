#include <packedvector.hpp>

namespace packedvector {
using PackedVector = packedvector::PackedVector;
PackedVector &PackedVector::operator=(const PackedVector &other) {
  std::cout << "copying assignment operator" << std::endl;
}
PackedVector &PackedVector::operator=(PackedVector &&other) {
  std::cout << "moving assignment operator" << std::endl;
}

PackedVector &PackedVector::operator[](std::size_t index) {
  std::cout << "operator[]" << std::endl;
}
const PackedVector &PackedVector::operator[](std::size_t index) const {
  std::cout << "const operator[]" << std::endl;
}
PackedVector &PackedVector::at(std::size_t index) {
  std::cout << "at" << std::endl;
}
const PackedVector &PackedVector::at(std::size_t index) const {
  std::cout << "const at" << std::endl;
}
void PackedVector::resize() { std::cout << "resize" << std::endl; }
void PackedVector::reserve() { std::cout << "reserve" << std::endl; }
void PackedVector::shrink_to_fit() {
  std::cout << "shrink_to_fit" << std::endl;
}
auto PackedVector::begin() { std::cout << "begin" << std::endl; }
auto PackedVector::end() { std::cout << "end" << std::endl; }
auto PackedVector::rbegin() { std::cout << "rbegin" << std::endl; }
auto PackedVector::rend() { std::cout << "rend" << std::endl; }
bool PackedVector::empty() const { std::cout << "empty" << std::endl; }
std::size_t PackedVector::size() const { std::cout << "size" << std::endl; }
std::size_t PackedVector::capacity() const {
  std::cout << "capacity" << std::endl;
}
void PackedVector::insert() { std::cout << "insert" << std::endl; }
void PackedVector::push_back() { std::cout << "push_back" << std::endl; }
void PackedVector::erase() { std::cout << "erase" << std::endl; }

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
