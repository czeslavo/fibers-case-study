#include <boost/context/all.hpp>
#include <iostream>
#include <exception>
#include <stdexcept>

namespace ctx = boost::context;

void rethrow_if_set(std::exception_ptr e)
{
    if (e) std::rethrow_exception(e);
}

int main()
{
    int i = 1;
    std::exception_ptr except_ptr; // to capture exceptions inside c1
    ctx::continuation c1 = ctx::callcc([&](ctx::continuation&& c2) {
        try
        {
            std::cout << "inside c1, i = " << i << '\n';
            ++i;
            c2 = c2.resume();
            
            std::cout << "again inside c1\n";
            throw std::runtime_error("Something went wrong");

            return c2.resume();
        } 
        catch (...) // capture and pass up
        {
            except_ptr = std::current_exception();
            return c2.resume();
        }
    });
    
    std::cout << "inside main, i = " << i << '\n';

    c1 = c1.resume();
    try
    {
        rethrow_if_set(except_ptr);    
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "caught exception: " << e.what() << '\n';
    }
}
