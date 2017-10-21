#include <iostream>
#include <boost/context/all.hpp>

namespace ctx = boost::context;

int main()
{
    int a;
    ctx::continuation source = ctx::callcc( // callc captures main continuation and passes it to lambda
        [&a](ctx::continuation&& sink) {    // as sink 
            a = 0;
            int b = 1;
            for (;;) {
                sink = sink.resume(); // resume() call invalidates state of continuation, so
                int next = a + b;     // the one returned from it has to be assigned to sink
                a = b;
                b = next;
            }

            return std::move(sink);
        }
    );

    // here source lambda executed once and returned execution by sink.resume()

    for (int i = 0; i < 10; ++i)
    {
        std::cout << a << " "; 
        source = source.resume(); // give execution control back to continuation - just after sink.resume()
    }
}
