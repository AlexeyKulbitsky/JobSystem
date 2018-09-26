#include <type_traits>
#include <iostream>

class Base { };
class Derived : public Base { };
class DerivedDerived : public Derived {};
class NotDerived { };

namespace detail
{
    template<typename T, bool is = std::is_base_of<Base, typename std::remove_pointer<T>::type>::value>
    struct GetType {static void impl() { std::cout << "IS INHERITANT" << std::endl; } };
    
    template<typename T> struct GetType<T, false> { static void impl() { std::cout << "OTHER" << std::endl; } };
    template<> struct GetType<int, false> { static void impl() { std::cout << "INT" << std::endl; } };
    template<> struct GetType<float, false> { static void impl() { std::cout << "FLOAT" << std::endl; } };
    template<> struct GetType<void*, false> { static void impl() { std::cout << "VOID*" << std::endl; } };
}


template<typename T>
void GetType(T)
{
    //F<T>::doStuff();
    detail::GetType<T>::impl();
}

// If the return type of foo() is not void, add where indicated.
template <typename T>
typename std::enable_if<std::is_base_of<Base, typename std::remove_pointer<T>>::value>::type
foo(T)
{
    std::cout << "Derived from Base." << std::endl;
}

// If the return type of foo() is not void, add where indicated.
template <typename T>
typename std::enable_if<!std::is_base_of<Base, T>::value>::type
foo(T)
{
    std::cout << "Empty." << std::endl;
}

template <>
typename std::enable_if<!std::is_base_of<Base, int>::value>::type
foo<int>(int) {
    std::cout << "Int" << std::endl;
}

template <>
typename std::enable_if<!std::is_base_of<Base, float>::value>::type
foo<float>(float) {
    std::cout << "Float" << std::endl;
}

int main()
{
    DerivedDerived* d = new DerivedDerived();
    //DerivedDerived d;
    NotDerived nd;
    int i;
    
    NotDerived* notDer = new NotDerived();
    //foo(d);
    //foo(nd);
    //foo(i);
    GetType(d);
    GetType(1);
    GetType(0.3f);
    GetType(notDer);
	return 0;
}
