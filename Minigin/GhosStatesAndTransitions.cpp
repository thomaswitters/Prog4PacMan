#include "GhosStatesAndTransitions.h"
#include <iostream>
#include "TransformComponent.h"
#include "FOVComponent.h"

// FSM TODO:
using namespace FSMStates;
using namespace FSMConditions;


//------------
//---STATES---
//------------

//------------ Patrol State ------------
void Patrol::OnEnter() {
    m_MoveCoomponent->SetFollowPath(true);

    m_MoveCoomponent->SetTargetCellIndex(m_TargetCellIndices[0]);
}

void Patrol::OnExit() {
    m_MoveCoomponent->SetCurrentIndex(m_MoveCoomponent->GetCurrentGraphCellId());
}

void Patrol::Update(float deltaTime) {
    int currentCell = m_MoveCoomponent->GetCurrentIndex();
    int nextCell = 0;

    if (m_MoveCoomponent->HasReachedTargetCell()) {
        if (currentCell == m_TargetCellIndices[0])
            nextCell = m_TargetCellIndices[1];
        else if (currentCell == m_TargetCellIndices[1])
            nextCell = m_TargetCellIndices[2];
        else if (currentCell == m_TargetCellIndices[2])
            nextCell = m_TargetCellIndices[3];
        else
            nextCell = m_TargetCellIndices[0];

        m_MoveCoomponent->SetTargetCellIndex(nextCell);
    }

    m_MoveCoomponent->FollowPath(deltaTime, nextCell);
}


//------------ ChasePlayer State ------------
void ChasePlayer::OnEnter() {
    m_Player = m_pFovComponent->GetPlayerInVision();
    m_PlayerMoveComponent = m_Player->GetComponent<PacManMoveComponent>();
    m_MoveCoomponent->SetFollowPath(true); 

    m_MoveCoomponent->SetTargetCellIndex(m_PlayerMoveComponent->GetCurrentIndex());
}

void ChasePlayer::OnExit() {
    m_MoveCoomponent->FollowPath(0.0f, m_PlayerMoveComponent->GetCurrentIndex());
}

void ChasePlayer::Update(float deltaTime) {

    switch (m_FincPathType) 
    {
    case FSMStates::ChasePlayer::FindPathType::NONE:
        break;
    case FSMStates::ChasePlayer::FindPathType::BESTPATH:
        m_MoveCoomponent->FollowPath(deltaTime, m_PlayerMoveComponent->GetCurrentIndex());
        break;
    case FSMStates::ChasePlayer::FindPathType::SECONDBESTPATH:
        m_MoveCoomponent->FollowSecondBestPath(deltaTime, m_PlayerMoveComponent->GetCurrentIndex());
        break;
    default:
        break;
    }
}


//------------ Scatter State ------------
void Scatter::OnEnter() {
    std::cout << "Entering Scatter state" << std::endl;
    // Add logic for entering the scatter state
}

void Scatter::OnExit() {
    std::cout << "Exiting Scatter state" << std::endl;
    // Add logic for exiting the scatter state
}

void Scatter::Update(float) {
    // Add logic for updating the scatter state
    std::cout << "Updating Scatter state" << std::endl;
}


//------------ Frightened State ------------
void Frightened::OnEnter() {
    std::cout << "Entering Frightened state" << std::endl;
    // Add logic for entering the frightened state
}

void Frightened::OnExit() {
    std::cout << "Exiting Frightened state" << std::endl;
    // Add logic for exiting the frightened state
}

void Frightened::Update(float) {
    // Add logic for updating the frightened state
    std::cout << "Updating Frightened state" << std::endl;
}


//------------ ReturnToBase State ------------
void ReturnToBase::OnEnter() {
    std::cout << "Entering ReturnToBase state" << std::endl;
    // Add logic for entering the return to base state
}

void ReturnToBase::OnExit() {
    std::cout << "Exiting ReturnToBase state" << std::endl;
    // Add logic for exiting the return to base state
}

void ReturnToBase::Update(float) {
    // Add logic for updating the return to base state
    std::cout << "Updating ReturnToBase state" << std::endl;
}


//----------------- Conditions -----------------
bool PlayerInSight::Evaluate() const {
    if (m_pFovComponent && m_pFovComponent->GetPlayerInSight())
        return true;

    return false;
}

bool PlayerNotInSight::Evaluate() const {
    if (m_pFovComponent && !m_pFovComponent->GetPlayerInSight() && m_MoveCoomponent->HasReachedTargetCell())
        return true;

    return false;
}

bool PlayerFrightened::Evaluate() const {
    // Add logic to check if the player is frightened
    std::cout << "Evaluating PlayerFrightened condition" << std::endl;
    return false; // Placeholder return value
}

bool PlayerNotFrightened::Evaluate() const {
    // Add logic to check if the player is not frightened
    std::cout << "Evaluating PlayerNotFrightened condition" << std::endl;
    return false;
}

bool AtBase::Evaluate() const {
    // Add logic to check if the ghost is at its base
    std::cout << "Evaluating AtBase condition" << std::endl;
    return false;
}

bool NotAtBase::Evaluate() const {
    // Add logic to check if the ghost is not at its base
    std::cout << "Evaluating NotAtBase condition" << std::endl;
    return false;
}
