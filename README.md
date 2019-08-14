# twolock_benchmark
Naive locking sequence based on mutex address versus C++11 deadlock avoidance because mutex address are unique as they are not copyable. For the benchmark to work, thread affinity needs to set. poor_man_lock is only marginally better in VC++. In GCC and Clang, -O3 optimize away the C++ 11 locks, rendering them unbenchmarkable.

```Cpp
void poor_man_lock(std::mutex& a, std::mutex& b)
{
    std::ptrdiff_t diff = &a - &b;
    if (diff < 0)
    {
        a.lock();
        b.lock();
        return;
    }
    else if (diff > 0)
    {
        b.lock();
        a.lock();
        return;
    }

    a.lock(); // 0 means they are the same mutex so only lock one of them!
}

stopwatch.start("Std Locking");
result = 0;
for (int i = 0; i < max_loop; ++i)
{
    std::lock(a, b); // deadlock avoidance algorithm
    std::lock_guard<std::mutex> lka(a, std::adopt_lock);
    std::lock_guard<std::mutex> lkb(b, std::adopt_lock);
    result += i;
}
std::cout << "result:" << result << std::endl;
stopwatch.stop();


stopwatch.start("Poor man locking");
result = 0;
for (int i = 0; i < max_loop; ++i)
{
    poor_man_lock(a, b); // lock sequence based on mutex address.
    std::lock_guard<std::mutex> lka(a, std::adopt_lock);
    std::lock_guard<std::mutex> lkb(b, std::adopt_lock);
    result += i;
}
std::cout << "result:" << result << std::endl;
stopwatch.stop();
```