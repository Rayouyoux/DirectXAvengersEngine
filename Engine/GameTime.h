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

        // Get Methods
        double      TotalTime() const;
        double      DeltaTime() const;

    private:

        // Private Members
        float     m_iStartTime;
        float     m_iPreviousTime;
        float     m_iCurrentTime;
        float     m_iSecondsPerCount;
        float     m_iDeltaTime;
        float     m_iTotalTime;

    };
}