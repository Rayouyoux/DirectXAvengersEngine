#include "GameTime.h"
#include <windows.h>

GameTime::GameTime() : 
    m_iStartTime(0), 
    m_iPreviousTime(0), 
    m_iCurrentTime(0), 
    m_iSecondsPerCount(0), 
    m_iDeltaTime(0), 
    m_iTotalTime(0) 
{}

GameTime::~GameTime()
{}

void GameTime::Initialize() {

    QueryPerformanceFrequency((LARGE_INTEGER*)&m_iSecondsPerCount);

    QueryPerformanceCounter((LARGE_INTEGER*)&m_iStartTime);
    m_iPreviousTime = m_iStartTime;

}

void GameTime::Tick() {

    QueryPerformanceCounter((LARGE_INTEGER*)&m_iCurrentTime);

    m_iDeltaTime = m_iCurrentTime - m_iPreviousTime;
    m_iPreviousTime = m_iCurrentTime;
    m_iTotalTime += m_iDeltaTime;

}

void GameTime::Reset() {

    m_iStartTime = 0;
    m_iPreviousTime = 0;
    m_iCurrentTime = 0;
    m_iSecondsPerCount = 0;
    m_iDeltaTime = 0;
    m_iTotalTime = 0;

    Initialize();

}

double GameTime::TotalTime() const {

    return static_cast<double>(m_iTotalTime) / m_iSecondsPerCount;

}

double GameTime::DeltaTime() const {

    return static_cast<double>(m_iDeltaTime) / m_iSecondsPerCount;

}
