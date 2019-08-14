// Benchmark2Locks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <mutex>

class timer
{
public:
	timer() = default;
	void start(const std::string& text_)
	{
		text = text_;
		begin = std::chrono::high_resolution_clock::now();
	}
	void stop()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = end - begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		std::cout << std::setw(20) << text << " timing:" << std::setw(5) << ms << "ms" << std::endl;
	}

private:
	std::string text;
	std::chrono::high_resolution_clock::time_point begin;
};

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

int main()
{
	timer stopwatch;
	std::int64_t n = 0;
	const int max_loop = 50000000;
	std::int64_t result = 0;
	std::mutex a;
	std::mutex b;

	std::cout << "Enter a char and press enter to continue:" << std::endl;

	char temp = ' ';
	std::cin >> temp;
	
	stopwatch.start("Std Locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		std::lock(a, b);
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
		poor_man_lock(a, b);
		std::lock_guard<std::mutex> lka(a, std::adopt_lock);
		std::lock_guard<std::mutex> lkb(b, std::adopt_lock);
		result += i;
	}
	std::cout << "result:" << result << std::endl;
	stopwatch.stop();

	std::cout << "Enter a char and press enter to exit:" << std::endl;

	std::cin >> temp;

}

