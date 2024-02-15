#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <mutex>
#include "RAII2MutexLock.h"
#ifdef _WIN32
#include "RAII2CSLock.h"
#endif

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
		std::cout << std::setw(28) << text << " timing:" << std::setw(5) << ms << "ms" << std::endl;
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

	std::cout << "Running Benchmark. Please wait...\n\n";

	stopwatch.start("Std Locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		std::lock(a, b);
		std::lock_guard<std::mutex> lka(a, std::adopt_lock);
		std::lock_guard<std::mutex> lkb(b, std::adopt_lock);
		result += i;
	}
	stopwatch.stop();
	std::cout << "result:" << result << std::endl;

	stopwatch.start("scoped_lock locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		std::scoped_lock lock(a, b);
		result += i;
	}
	stopwatch.stop();
	std::cout << "result:" << result << std::endl;

	stopwatch.start("RAII2MutexLock locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		RAII2MutexLock lock(a, b);
		result += i;
	}
	stopwatch.stop();
	std::cout << "result:" << result << std::endl;

#ifdef _WIN32
	CriticalSection csA;
	CriticalSection csB;

	stopwatch.start("RAII2CSLock locking");
	result = 0;
	for (int i = 0; i < max_loop; ++i)
	{
		RAII2CSLock lock(csA, csB);
		result += i;
	}
	stopwatch.stop();
	std::cout << "result:" << result << std::endl;
#endif

	std::cout << "\nBenchmark ended. Please ignore result as it is a\nway to prevent optimizing away the for loop" << std::endl;

	return 0;
}

