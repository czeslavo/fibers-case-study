#include <boost/fiber/all.hpp>

namespace fibers = boost::fibers;

int main()
{
    fibers::fiber helloFiber([]() { std::cout << "Hello Fiber!\n"; });
    helloFiber.join();
}
