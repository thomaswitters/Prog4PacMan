#include "StateManagerMovement.h"

namespace dae
{
    StateManagerMovement::StateManagerMovement(float maxTimeInChaseState, float maxTimeInPatrolState, float maxTimeInBase)
        : m_MaxTimeInChaseState(maxTimeInChaseState)
        , m_MaxTimeInPatrolState(maxTimeInPatrolState)
        , m_MaxTimeInBase(maxTimeInBase)
    {
    }

    void StateManagerMovement::TimeInChaseStatePlus(float deltaTime)
    {
        if (m_TimeInChaseState <= m_MaxTimeInChaseState)
        {
            m_TimeInChaseState += deltaTime;
        }
    }

    bool StateManagerMovement::TimeToSwitchToPatrol()
    {
        if (m_TimeInChaseState >= m_MaxTimeInChaseState)
        {
            m_TimeInChaseState = 0.f;
            return true;
        }
        return false;
    }

    void StateManagerMovement::TimeInPatrolStatePlus(float deltaTime)
    {
        if (m_TimeInPatrolState <= m_MaxTimeInPatrolState)
        {
            m_TimeInPatrolState += deltaTime;
        }
    }

    void StateManagerMovement::SetMaxTimeInPatrolState(float maxTime)
    {
        m_MaxTimeInPatrolState = maxTime;
    }

    bool StateManagerMovement::TimeToSwitchToChase()
    {
        if (m_TimeInPatrolState >= m_MaxTimeInPatrolState)
        {
            m_TimeInPatrolState = 0.f;
            return true;
        }
        return false;
    }

    int StateManagerMovement::GetPatrolPhase() const
    {
        return m_PatrolPhase;
    }

    void StateManagerMovement::TimeInBaseStatePlus(float deltaTime)
    {
        if (m_TimeInBase <= m_MaxTimeInBase)
        {
            m_TimeInBase += deltaTime;
        }
    }

    bool StateManagerMovement::GoOut()
    {
        if (m_TimeInBase >= m_MaxTimeInBase)
        {
            m_TimeInBase = 0.f;
            return true;
        }
        return false;
    }

    void StateManagerMovement::SetMaxTimeInBase(float maxTimeInBase)
    {
        m_MaxTimeInBase = maxTimeInBase;
    }

    void StateManagerMovement::SetPatrolPhasePlus(int amount)
    {
        m_PatrolPhase += amount;
    }
}