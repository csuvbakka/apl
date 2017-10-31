#include <type_traits>
#include <tuple>

#include <iostream>
#include <string>

template <typename... Args>
class any_of
{
public:
    explicit any_of(const Args&... args) : args_(std::forward_as_tuple(args...)) { }
    template<
        typename... Ts,
        typename = std::enable_if_t<sizeof...(Ts) == sizeof...(Args)>>
    explicit any_of(Ts&&... args) : args_(std::forward_as_tuple(std::forward<Ts>(args)...)) { }

    template <typename T>
    bool apply(const T& value) const
    {
        return std::apply([&value](auto&&... args) { return ((args == value) || ...); }, args_);
    };
    
private:
    const std::tuple<const Args&...> args_;
};

template <typename T, typename... Args>
bool operator==(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.apply(lhs);
}

template <typename T, typename... Args>
bool operator!=(const T& lhs, const any_of<Args...>& rhs)
{
    return !(lhs == rhs);
}

void example_string()
{
    std::cout << "string\n";
    const std::string s1 = "alma";
    const std::string s2 = "banan";
    
    const std::string needle = "banan";
    
    if (needle != any_of(s1, std::string("citrom"), s2, "dio"))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
    
    if ("banan" == any_of(s1, s2))
        std::cout << "yep" << std::endl;
    else
        std::cout << "nope" << std::endl;
}

struct Foo
{
    explicit Foo(int x) : i(x) { }
    Foo(const Foo&) { std::cout << "copy\n"; }
    Foo(Foo&&) { std::cout << "move\n"; }
    
    int i;
};

bool operator==(const Foo& lhs, const Foo& rhs)
{
    return lhs.i == rhs.i;
}

void example_foo()
{
    std::cout << "Foo\n";
    const Foo f1{13};
    const Foo f2{42};
    
    if (Foo{42} == any_of(f1, Foo{69}, f2))
        std::cout << "yep\n";
    else
        std::cout << "nope\n";
}

int main()
{
    example_string();
    std::cout << std::endl;
    example_foo();
}
