#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <tuple>
#include <utility>

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
    bool impl(const T& value) const
    {
        return std::apply([&value](auto&&... args) { return ((args == value) || ...); }, args_);
    };
    
private:
    const std::tuple<const Args&...> args_;
};

template <typename T, typename... Args>
bool operator==(const T& lhs, const any_of<Args...>& rhs)
{
    return rhs.impl(lhs);
}

struct Foo
{
    explicit Foo(int x) : i(x) { }
    Foo(const Foo&) { std::cout << "copy\n"; }
    Foo(Foo&&) { std::cout << "move\n"; }
    
    int i;
};

inline bool operator==(const Foo& lhs, const Foo& rhs)
{
    return lhs.i == rhs.i;
}

int main()
{
    const Foo f1{13};
    const Foo f2{69};

    if (f1 == any_of(f2, Foo{13}))
        return 42;
}
