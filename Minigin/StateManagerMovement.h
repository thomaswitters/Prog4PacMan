#pragma once

namespace dae
{
    class StateManagerMovement
    {
    public:
        StateManagerMovement(float maxTimeInChaseState, float maxTimeInPatrolState, float maxTimeInBase);

        // Chase state management
        void TimeInChaseStatePlus(float deltaTime);
        bool TimeToSwitchToPatrol();

        // Patrol state management
        void TimeInPatrolStatePlus(float deltaTime);
        void SetMaxTimeInPatrolState(float maxTime);
        bool TimeToSwitchToChase();
        int GetPatrolPhase() const;

        // Base state management
        void TimeInBaseStatePlus(float deltaTime);
        bool GoOut();
        void SetMaxTimeInBase(float maxTimeInBase);

        void SetPatrolPhasePlus(int amount);
    private:
        float m_TimeInChaseState = 0.f;
        float m_MaxTimeInChaseState;
        float m_TimeInPatrolState = 0.f;
        float m_MaxTimeInPatrolState;
        int m_PatrolPhase = 0;
        float m_TimeInBase = 0.f;
        float m_MaxTimeInBase;
    };
}

