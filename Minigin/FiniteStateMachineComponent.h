#pragma once
#include <map>
#include "BaseComponent.h"
#include "GameObject.h"
#include <vector>
#include "FOVComponent.h"
#include "PacManMoveComponent.h"
#include "RenderComponent.h"
#include "BoxColliderComponent.h"

namespace dae
{
	class FSMState
	{
	public:
		FSMState(std::shared_ptr<GameObject> pOwner) : m_pOwner(pOwner) {
			if (m_pOwner)
			{
				m_pFovComponent = m_pOwner->GetComponent<FOVComponent>();
				m_MoveCoomponent = m_pOwner->GetComponent<PacManMoveComponent>();
				m_RenderComponent = m_pOwner->GetComponent<RenderComponent>();
				m_BoxColliderComponent = m_pOwner->GetComponent<BoxColliderComponent>();
			}
				
		}
		virtual ~FSMState() = default;

		virtual void OnEnter() {}
		virtual void OnExit() {}
		virtual void Update(float) {}

	protected:
		std::shared_ptr<GameObject> m_pOwner;
		std::shared_ptr<PacManMoveComponent> m_MoveCoomponent;
		std::shared_ptr<FOVComponent> m_pFovComponent;
		std::shared_ptr<RenderComponent> m_RenderComponent;
		std::shared_ptr<BoxColliderComponent> m_BoxColliderComponent;
	};

	class FSMCondition
	{
	public:
		FSMCondition(std::shared_ptr<GameObject> pOwner) : m_pOwner(pOwner) {
			if (m_pOwner)
			{
				m_pFovComponent = m_pOwner->GetComponent<FOVComponent>();
				m_MoveCoomponent = m_pOwner->GetComponent<PacManMoveComponent>();
				m_BoxColliderComponent = m_pOwner->GetComponent<BoxColliderComponent>();
			}
		}
		virtual ~FSMCondition() = default;
		virtual bool Evaluate() const = 0;
	protected:
		std::shared_ptr<GameObject> m_pOwner;
		std::shared_ptr<FOVComponent> m_pFovComponent;
		std::shared_ptr<PacManMoveComponent> m_MoveCoomponent;
		std::shared_ptr<BoxColliderComponent> m_BoxColliderComponent;

	};

	class FiniteStateMachine final : public BaseComponent
	{
	public:
		FiniteStateMachine(std::weak_ptr<GameObject> pOwner, FSMState* startState);
		virtual ~FiniteStateMachine();

		void AddTransition(FSMState* startState, FSMState* toState, FSMCondition* transition);
		virtual void Update(float deltaTime) override;

	private:
		void ChangeState(FSMState* newState);
		typedef std::pair<FSMCondition*, FSMState*> TransitionStatePair;
		typedef std::vector<TransitionStatePair> Transitions;

		std::map<FSMState*, Transitions> m_Transitions;
		FSMState* m_pCurrentState;
	};

}


