#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace detail
{

template <typename T>
class is
{
public:
    explicit is(const T& ref) : ref_(ref) {}

    template <typename... Args>
    bool any_of(Args&&... args)
    {
        return any_of_impl(std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    bool none_of(Args&&... args)
    {
        return !any_of(std::forward<Args>(args)...);
    }
    
    template <typename U>
    bool in(const U& container)
    {
        return std::find(container.begin(), container.end(), ref_) != container.end();
    }
    
private:
    template <typename U, typename... Args>
    bool any_of_impl(const U& first, Args&&... args)
    {
        return ref_ == first || any_of_impl(std::forward<Args>(args)...);
    }

    template <typename U>
    bool any_of_impl(const U& last)
    {
        return ref_ == last;
    }
private:    
    const T& ref_;
};
    
} // namespace detail

template <typename T>
detail::is<T> is(const T& ref)
{
    return detail::is(ref);
}

void example_string()
{
    std::cout << "string\n";
    const std::string s1 = "alma";
    const std::string s2 = "banan";
    
    const std::string needle = "almax";
    
    if (is(needle).none_of(s1, s2))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
    
    if (is("alma").any_of(s1, s2))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
    
    if (is('a').in(s2))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;

}

void example_enum()
{
    std::cout << "enum\n";

    enum class MyEnum {OPTION_1, OPTION_2, OPTION_3, OPTION_4};
    
    MyEnum e = MyEnum::OPTION_2;
    
    if (is(e).any_of(MyEnum::OPTION_1, MyEnum::OPTION_4))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
    
    if (is(e).none_of(MyEnum::OPTION_1, MyEnum::OPTION_4))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
    
}

void example_container()
{
    std::cout << "vector\n";

    std::vector<int> v = {1, 2, 3};
    
    if (is(7).in(v))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
}

int main()
{
    example_string();
    std::cout << std::endl;
    example_enum();
    std::cout << std::endl;
    example_container();
}
