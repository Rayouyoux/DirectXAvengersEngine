#pragma once

namespace ave 
{

    class GameTime 
    {

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

        void        SetTimeScale(float fTimeScale);

        // Get Methods
        float      TotalTime() const;
        float      DeltaTime() const;

    private:

        // Private Members
        __int64     m_iStartTime;
        __int64     m_iPreviousTime;
        __int64     m_iCurrentTime;
        __int64     m_iSecondsPerCount;
        __int64     m_iDeltaTime;
        __int64     m_iTotalTime;

        float m_fTimeScale;
    };
}