#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <mutex>
#include "RAIIMutexLock.h"

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
		std::cout << std::setw(21) << text << " timing:" << std::setw(5) << ms << "ms" << std::endl;
	}

private:
	std::string text;
	std::chrono::high_resolution_clock::time_point begin;
};

int main()
{
	timer stopwatch;
	std::int64_t n = 0;
	const int max_loop = 100000000;
	std::int64_t result = 0;
	std::mutex a;
	std::mutex b;

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

	stopwatch.start("scoped_lock locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		std::scoped_lock (a, b);
		result += i;
	}
	std::cout << "result:" << result << std::endl;
	stopwatch.stop();

	stopwatch.start("RAIIMutexLock locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		RAIIMutexLock lock(a, b);
		result += i;
	}
	std::cout << "result:" << result << std::endl;
	stopwatch.stop();

	return 0;
}

