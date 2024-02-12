#pragma once
#include <mutex>
//#include <iostream>

class RAIIMutexLock
{
public:
	RAIIMutexLock(std::mutex& a_mut, std::mutex& b_mut)
		: mutA(a_mut)
		, mutB(b_mut)
	{
		if (&mutA < &mutB)
		{
			mutA.lock();
			mutB.lock();
			//std::cout << "&mutA < &mutB\n";
		}
		else if (&mutB < &mutA)
		{
			mutB.lock();
			mutA.lock();
			//std::cout << "&mutB < &mutA\n";
		}
		else // mutA and mutB are actually the same lock
			// because they shared the same address
			// so just need to lock one of them
		{
			mutA.lock();
			//std::cout << "&mutB == &mutA\n";
		}
	}
	~RAIIMutexLock()
	{
		if (&mutA == &mutB)
		{
			mutA.unlock();
			//std::cout << "mutA.Unlock()\n";
			return;
		}
		// the order of unlock does not matter
		mutA.unlock();
		mutB.unlock();
		//std::cout << "mutA.Unlock() and mutB.Unlock()\n";
	}

private:
	// copy ops are private to prevent copying
	RAIIMutexLock(const RAIIMutexLock&);
	RAIIMutexLock& operator=(const RAIIMutexLock&);

	std::mutex& mutA;
	std::mutex& mutB;
};

