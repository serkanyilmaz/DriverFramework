#pragma once

#include "DFLog.hpp"

namespace DriverFramework
{

class DFTestObj
{
public:
	DFTestObj(const char *name) : m_name(name) {}

	virtual ~DFTestObj() {}

	bool doTests()
	{
		testStart();
		_doTests();
		testEnd();
		return (m_failed_count == 0);
	}

protected:
	virtual void _doTests() = 0;

	void reportResult(const char *name, bool passed)
	{
		DF_LOG_INFO("TEST %s: %s", name, passed ? "PASSED" : "FAILED");

		if (passed) {
			++m_passed_count;

		} else {
			++m_failed_count;
		}
	}

	void startFeatureTest(const char *name)
	{
		DF_LOG_INFO("------- BEGIN FEATURE TEST FOR %s", name);
	}

private:
	void testStart()
	{
		DF_LOG_INFO("======= BEGIN TESTS FOR %s", m_name);
		m_passed_count = 0;
		m_failed_count = 0;
	}

	void testEnd()
	{
		DF_LOG_INFO("======= END OF TESTS FOR %s. %u of %u tests passed", m_name, m_passed_count,
			    m_passed_count + m_failed_count);
	}

	const char *m_name;

	unsigned int 	m_passed_count = 0;
	unsigned int 	m_failed_count = 0;
};
};
