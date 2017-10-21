# Thinking outside the synchonisation quadrant [code::dive 2016 talk]

Cost of the execution of a task in multithreaded manner is not t_1 / n. We have to remember about the cost of context
switch between the threads which could exceed the cost of executiong the task on a single core.

It's described by Amdahl's Law.


