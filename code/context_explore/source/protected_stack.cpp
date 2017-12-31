#include <iostream>
#include <boost/context/all.hpp>

namespace ctx = boost::context;

template <typename Fn>
ctx::continuation callcc_with_protected_stack(Fn&& fn)
{
    return ctx::callcc(std::allocator_arg, ctx::protected_fixedsize_stack{}, std::forward<Fn>(fn));
}

int main()
{
    const auto stack_size = ctx::stack_traits::default_size();
    std::cout << "Stack size = " << stack_size << '\n';
    std::cout << "int size = " << sizeof(int) << '\n';
    std::cout << "To cause overflow = " << stack_size / sizeof(int) << '\n';

    auto c1 = callcc_with_protected_stack([](auto&& c) {
        // create huge array on stack to trigger overflow
        [[maybe_unused]] int a[32800];

        return std::move(c);
    });

}
