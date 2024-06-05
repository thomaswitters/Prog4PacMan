#pragma once
#include "FiniteStateMachineComponent.h"
#include "GameObject.h"

using namespace dae;

//------------
//---STATES---
//------------
namespace FSMStates
{
    class Patrol : public FSMState
    {
    public:
        Patrol(std::shared_ptr<GameObject> pOwner, std::vector<int> targetCellIndices) : FSMState(pOwner), m_TargetCellIndices(targetCellIndices) {}
        ~Patrol() = default;

        Patrol(const Patrol&) = delete;
        Patrol& operator=(const Patrol&) = delete;
        Patrol(Patrol&&) = delete;
        Patrol& operator=(Patrol&&) = delete;

        void OnEnter();
        void OnExit();
        void Update(float deltaTime);
    private:
        std::vector<int> m_TargetCellIndices;
    };

    class ChasePlayer : public FSMState
    {
    public:
        enum class FindPathType {
            NONE,
            BESTPATH,
            SECONDBESTPATH
        };

        ChasePlayer(std::shared_ptr<GameObject> pOwner, FindPathType fincPathType) : FSMState(pOwner), m_FincPathType(fincPathType) {};
        ~ChasePlayer() = default;

        ChasePlayer(const ChasePlayer&) = delete;
        ChasePlayer& operator=(const ChasePlayer&) = delete;
        ChasePlayer(ChasePlayer&&) = delete;
        ChasePlayer& operator=(ChasePlayer&&) = delete;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;

        
    private:
        std::shared_ptr<PacManMoveComponent> m_PlayerMoveComponent;
        std::shared_ptr<GameObject> m_Player;

        FindPathType m_FincPathType;
    };

    class Scatter : public FSMState
    {
    public:
        Scatter(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~Scatter() = default;

        Scatter(const Scatter&) = delete;
        Scatter& operator=(const Scatter&) = delete;
        Scatter(Scatter&&) = delete;
        Scatter& operator=(Scatter&&) = delete;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;
    };

    class Frightened : public FSMState
    {
    public:
        Frightened(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~Frightened() = default;

        Frightened(const Frightened&) = delete;
        Frightened& operator=(const Frightened&) = delete;
        Frightened(Frightened&&) = delete;
        Frightened& operator=(Frightened&&) = delete;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;
    };

    class ReturnToBase : public FSMState
    {
    public:
        ReturnToBase(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~ReturnToBase() = default;

        ReturnToBase(const ReturnToBase&) = delete;
        ReturnToBase& operator=(const ReturnToBase&) = delete;
        ReturnToBase(ReturnToBase&&) = delete;
        ReturnToBase& operator=(ReturnToBase&&) = delete;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;
    };
}

//-----------------
//---CONDITIONS---
//-----------------
namespace FSMConditions
{
    class PlayerInSight : public FSMCondition
    {
    public:
        PlayerInSight(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerInSight(const PlayerInSight&) = delete;
        PlayerInSight& operator=(const PlayerInSight&) = delete;
        PlayerInSight(PlayerInSight&&) = delete;
        PlayerInSight& operator=(PlayerInSight&&) = delete;

        bool Evaluate() const override;
    };

    class PlayerNotInSight : public FSMCondition
    {
    public:
        PlayerNotInSight(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerNotInSight(const PlayerNotInSight&) = delete;
        PlayerNotInSight& operator=(const PlayerNotInSight&) = delete;
        PlayerNotInSight(PlayerNotInSight&&) = delete;
        PlayerNotInSight& operator=(PlayerNotInSight&&) = delete;

        bool Evaluate() const override;
    };

    class PlayerFrightened : public FSMCondition
    {
    public:
        PlayerFrightened(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerFrightened(const PlayerFrightened&) = delete;
        PlayerFrightened& operator=(const PlayerFrightened&) = delete;
        PlayerFrightened(PlayerFrightened&&) = delete;
        PlayerFrightened& operator=(PlayerFrightened&&) = delete;

        bool Evaluate() const override;
    };

    class PlayerNotFrightened : public FSMCondition
    {
    public:
        PlayerNotFrightened(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerNotFrightened(const PlayerNotFrightened&) = delete;
        PlayerNotFrightened& operator=(const PlayerNotFrightened&) = delete;
        PlayerNotFrightened(PlayerNotFrightened&&) = delete;
        PlayerNotFrightened& operator=(PlayerNotFrightened&&) = delete;

        bool Evaluate() const override;
    };

    class AtBase : public FSMCondition
    {
    public:
        AtBase(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        AtBase(const AtBase&) = delete;
        AtBase& operator=(const AtBase&) = delete;
        AtBase(AtBase&&) = delete;
        AtBase& operator=(AtBase&&) = delete;

        bool Evaluate() const override;
    };

    class NotAtBase : public FSMCondition
    {
    public:
        NotAtBase(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        NotAtBase(const NotAtBase&) = delete;
        NotAtBase& operator=(const NotAtBase&) = delete;
        NotAtBase(NotAtBase&&) = delete;
        NotAtBase& operator=(NotAtBase&&) = delete;

        bool Evaluate() const override;
    };
}

