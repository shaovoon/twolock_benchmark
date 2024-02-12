#pragma once

// https://jrdodds.blogs.com/blog/2004/08/raii_in_c.html/
// written by Jonathan Dodds
#include <Windows.h>

class CriticalSection
{
public:
	CriticalSection()
	{
		::InitializeCriticalSection(&m_rep);
	}
	~CriticalSection()
	{
		::DeleteCriticalSection(&m_rep);
	}

	void Lock()
	{
		::EnterCriticalSection(&m_rep);
	}
	void Unlock()
	{
		::LeaveCriticalSection(&m_rep);
	}

private:
	// copy ops are private to prevent copying
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);

	CRITICAL_SECTION m_rep;
};