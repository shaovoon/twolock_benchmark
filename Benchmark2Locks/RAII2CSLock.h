#pragma once
#include "CriticalSection.h"
//#include <iostream>

class RAII2CSLock
{
public:
	RAII2CSLock(CriticalSection& a_section, CriticalSection& b_section)
		: m_SectionA(a_section)
		, m_SectionB(b_section)
	{
		if (&m_SectionA < &m_SectionB)
		{
			m_SectionA.Lock();
			m_SectionB.Lock();
			//std::cout << "&m_SectionA < &m_SectionB\n";
		}
		else if (&m_SectionB < &m_SectionA)
		{
			m_SectionB.Lock();
			m_SectionA.Lock();
			//std::cout << "&m_SectionB < &m_SectionA\n";
		}
		else // m_SectionA and m_SectionB are actually the same lock
			// because they shared the same address
			// so just need to lock one of them
		{
			m_SectionA.Lock();
			//std::cout << "&m_SectionB == &m_SectionA\n";
		}
	}
	~RAII2CSLock()
	{
		if (&m_SectionA == &m_SectionB)
		{
			m_SectionA.Unlock();
			//std::cout << "m_SectionA.Unlock()\n";
			return;
		}
		// the order of unlock does not matter
		m_SectionA.Unlock();
		m_SectionB.Unlock();
		//std::cout << "m_SectionA.Unlock() and m_SectionB.Unlock()\n";
	}

private:
	// copy ops are private to prevent copying
	RAII2CSLock(const RAII2CSLock&);
	RAII2CSLock& operator=(const RAII2CSLock&);

	CriticalSection& m_SectionA;
	CriticalSection& m_SectionB;
};

