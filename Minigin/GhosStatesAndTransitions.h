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
        Patrol(std::weak_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~Patrol() = default;

        void OnEnter();
        void OnExit();
        void Update(float deltaTime);
        std::vector<glm::vec2> GetPatrolPath() const;
        size_t FindClosestWaypointIndex(const glm::vec2& currentPosition) const;
    private:
        std::shared_ptr<TransformComponent> m_Transform;
        std::vector<glm::vec2> m_PatrolPath;
        size_t m_CurrentWaypointIndex = 0;
        float m_Speed = 50.0f;
        float m_Tolerance = 1.0f;
    };

    class ChasePlayer : public FSMState
    {
    public:
        ChasePlayer(std::weak_ptr<GameObject> pOwner) : FSMState(pOwner) {};
        ~ChasePlayer() = default;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;

    };

    class Scatter : public FSMState
    {
    public:
        Scatter(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~Scatter() = default;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;
    };

    class Frightened : public FSMState
    {
    public:
        Frightened(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~Frightened() = default;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;
    };

    class ReturnToBase : public FSMState
    {
    public:
        ReturnToBase(std::shared_ptr<GameObject> pOwner) : FSMState(pOwner) {}
        ~ReturnToBase() = default;

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

        bool Evaluate() const override;
    };

    class PlayerNotInSight : public FSMCondition
    {
    public:
        PlayerNotInSight(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        bool Evaluate() const override;
    };

    class PlayerFrightened : public FSMCondition
    {
    public:
        PlayerFrightened(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        bool Evaluate() const override;
    };

    class PlayerNotFrightened : public FSMCondition
    {
    public:
        PlayerNotFrightened(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        bool Evaluate() const override;
    };

    class AtBase : public FSMCondition
    {
    public:
        AtBase(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        bool Evaluate() const override;
    };

    class NotAtBase : public FSMCondition
    {
    public:
        NotAtBase(std::shared_ptr<GameObject> pOwner) : FSMCondition(pOwner) {}

        bool Evaluate() const override;
    };
}
