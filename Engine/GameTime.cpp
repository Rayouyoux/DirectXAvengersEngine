#include "GameTime.h"
#include <windows.h>

GameTime::GameTime() : 
    m_startTime(0), 
    m_previousTime(0), 
    m_currentTime(0), 
    m_secondsPerCount(0), 
    m_deltaTime(0), 
    m_totalTime(0) 
{}

GameTime::~GameTime()
{}

void GameTime::Initialize() {

    QueryPerformanceFrequency((LARGE_INTEGER*)&m_secondsPerCount);

    QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
    m_previousTime = m_startTime;

}

void GameTime::Tick() {

    QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

    m_deltaTime = m_currentTime - m_previousTime;
    m_previousTime = m_currentTime;
    m_totalTime += m_deltaTime;

}

void GameTime::Reset() {

    m_startTime = 0;
    m_previousTime = 0;
    m_currentTime = 0;
    m_secondsPerCount = 0;
    m_deltaTime = 0;
    m_totalTime = 0;

    Initialize();

}

double GameTime::TotalTime() const {

    return static_cast<double>(m_totalTime) / m_secondsPerCount;

}

double GameTime::DeltaTime() const {

    return static_cast<double>(m_deltaTime) / m_secondsPerCount;

}
