#include "FiniteStateMachineComponent.h"
using namespace dae;

FiniteStateMachine::FiniteStateMachine(std::weak_ptr<GameObject> pOwner, FSMState* startState)
    : BaseComponent(pOwner), 
    m_pCurrentState(nullptr)
{
    ChangeState(startState);
}

FiniteStateMachine::~FiniteStateMachine()
{

}

void FiniteStateMachine::AddTransition(FSMState* startState, FSMState* toState, FSMCondition* condition)
{
    auto it = m_Transitions.find(startState);
    if (it == m_Transitions.end())
    {
        m_Transitions[startState] = Transitions();
    }

    m_Transitions[startState].push_back(std::make_pair(condition, toState));
}

void FiniteStateMachine::Update(float deltaTime)
{
    // FSM TODO:
    //TODO 4: Look if 1 or more condition exists for the current state that we are in
    //Tip: Check the transitions map for a TransitionState pair    
    auto current = m_Transitions.find(m_pCurrentState);
    //TODO 5: if a TransitionState exists
    if (current != m_Transitions.end())
    {
        //TODO 6: Loop over all the TransitionState pairs
        for (auto& transition : current->second)
        {
            FSMCondition* condition = transition.first;
            FSMState* state = transition.second;

            //TODO 7: If the Evaluate function of the FSMCondition returns true => transition to the new corresponding state
            if (condition->Evaluate())
            {
                ChangeState(state);
                break;
            }
        }
    }
    //TODO 8: Update the current state (if one exists)
    if (m_pCurrentState != nullptr)
    {
        m_pCurrentState->Update(deltaTime);
    }
}

void FiniteStateMachine::ChangeState(FSMState* newState)
{
    // FSM TODO:
    //TODO 1. If currently in a state => make sure the OnExit of that state gets called
    if (m_pCurrentState != nullptr)
    {
        m_pCurrentState->OnExit();
    }

    //TODO 2. Change the current state to the new state
    m_pCurrentState = newState;
    //TODO 3. Call the OnEnter of the new state  
    if (m_pCurrentState != nullptr)
    {
        m_pCurrentState->OnEnter();
    }
}