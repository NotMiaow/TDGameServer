#include "timeSystem.h"

TimeSystem::TimeSystem()
{
	m_currentTime = 0;
	m_curTime = std::chrono::high_resolution_clock::now();
}

TimeSystem::~TimeSystem()
{
}

void TimeSystem::Loop()
{
	m_prevTime = m_curTime;
	m_curTime = std::chrono::high_resolution_clock::now();
	m_currentTime += DeltaTime();
}

const double TimeSystem::GetTime()
{
	return m_currentTime;
}

const double TimeSystem::DeltaTime()
{
	return ((std::chrono::duration<double>)(m_curTime - m_prevTime)).count();
}

