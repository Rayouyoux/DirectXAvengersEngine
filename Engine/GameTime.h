#pragma once

class GameTime {

public:

    // Constructor
    GameTime();

    // Deconstructor
    ~GameTime();

    // Sets Seconds Per Time Unit & Starting Time
    void        Initialize();

    // Updates Delta Time & Total Time
    void        Tick();

    // Resets All Times & Initializes App Again
    void        Reset();

    // Get Methods
    double      TotalTime() const;
    double      DeltaTime() const;


private:

    // Private Members
    __int64     m_startTime;
    __int64     m_previousTime;
    __int64     m_currentTime;
    __int64     m_secondsPerCount;
    __int64     m_deltaTime;
    __int64     m_totalTime;

};