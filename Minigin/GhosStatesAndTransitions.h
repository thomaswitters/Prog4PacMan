#pragma once
#include "FiniteStateMachineComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "HealthComponent.h"

using namespace dae;

//------------
//---STATES---
//------------
namespace FSMStates
{
    class None : public FSMState
    {
    public:
        None(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner){}
        ~None() = default;

        None(const None&) = delete;
        None& operator=(const None&) = delete;
        None(None&&) = delete;
        None& operator=(None&&) = delete;

        void OnEnter();
        void OnExit();
        void Update(float deltaTime);
    };

    class NoneFrightened : public FSMState
    {
    public:
        NoneFrightened(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~NoneFrightened() = default;

        NoneFrightened(const NoneFrightened&) = delete;
        NoneFrightened& operator=(const NoneFrightened&) = delete;
        NoneFrightened(NoneFrightened&&) = delete;
        NoneFrightened& operator=(NoneFrightened&&) = delete;

        void OnEnter();
        void OnExit();
        void Update(float deltaTime);
    };

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
            SECONDBESTPATH,
            PREDICT
        };

        ChasePlayer(std::shared_ptr<GameObject> pOwner, FindPathType findPathType) : FSMState(pOwner), m_FincPathType(findPathType) {};
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
        std::shared_ptr<GameObject> m_PreviousPlayer;
        std::shared_ptr<GameObject> m_Player;

        FindPathType m_FincPathType;
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
    private:
        std::shared_ptr<PacManMoveComponent> m_PlayerMoveComponent;
        std::shared_ptr<GameObject> m_Player;
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
    class PlayerTimerChase : public FSMCondition
    {
    public:
        PlayerTimerChase(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerTimerChase(const PlayerTimerChase&) = delete;
        PlayerTimerChase& operator=(const PlayerTimerChase&) = delete;
        PlayerTimerChase(PlayerTimerChase&&) = delete;
        PlayerTimerChase& operator=(PlayerTimerChase&&) = delete;

        bool Evaluate() const override;
    };
    class PlayerTimerPatrol : public FSMCondition
    {
    public:
        PlayerTimerPatrol(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerTimerPatrol(const PlayerTimerPatrol&) = delete;
        PlayerTimerPatrol& operator=(const PlayerTimerPatrol&) = delete;
        PlayerTimerPatrol(PlayerTimerPatrol&&) = delete;
        PlayerTimerPatrol& operator=(PlayerTimerPatrol&&) = delete;

        bool Evaluate() const override;
    };

    class PlayerPoweredUp : public FSMCondition
    {
    public:
        PlayerPoweredUp(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerPoweredUp(const PlayerPoweredUp&) = delete;
        PlayerPoweredUp& operator=(const PlayerPoweredUp&) = delete;
        PlayerPoweredUp(PlayerPoweredUp&&) = delete;
        PlayerPoweredUp& operator=(PlayerPoweredUp&&) = delete;

        bool Evaluate() const override;
    };

    class PlayerNotPoweredUp : public FSMCondition
    {
    public:
        PlayerNotPoweredUp(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        PlayerNotPoweredUp(const PlayerNotPoweredUp&) = delete;
        PlayerNotPoweredUp& operator=(const PlayerNotPoweredUp&) = delete;
        PlayerNotPoweredUp(PlayerNotPoweredUp&&) = delete;
        PlayerNotPoweredUp& operator=(PlayerNotPoweredUp&&) = delete;

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

