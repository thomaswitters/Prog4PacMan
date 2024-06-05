#include "GhosStatesAndTransitions.h"
#include <iostream>
#include "TransformComponent.h"
#include "FOVComponent.h"
#include "PoweredUpComponent.h"

// FSM TODO:
using namespace FSMStates;
using namespace FSMConditions;


//------------
//---STATES---
//------------
void None::OnEnter() {
    m_MoveCoomponent->SetFollowPath(false);
}

void None::OnExit() {
}

void None::Update(float) {

}

void NoneFrightened::OnEnter() {
    m_MoveCoomponent->SetFollowPath(false);
    m_RenderComponent->SetTexture("frightenghost.png");
    m_MoveCoomponent->SetSpeed(40);
}

void NoneFrightened::OnExit() {
    m_RenderComponent->SetTexture(m_RenderComponent->GetPreviousTexture());
    m_MoveCoomponent->SetSpeed(90);
}

void NoneFrightened::Update(float) {

}


//------------ Patrol State ------------
void Patrol::OnEnter() {
    m_MoveCoomponent->SetFollowPath(true);

    m_MoveCoomponent->SetTargetCellIndex(m_TargetCellIndices[0]);

    if (m_MoveCoomponent->GetStateManager()->GetPatrolPhase() == 3)
    {
        m_MoveCoomponent->GetStateManager()->SetMaxTimeInPatrolState(5);
    }
}

void Patrol::OnExit() {
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

    m_MoveCoomponent->GetStateManager()->TimeInPatrolStatePlus(deltaTime);
}


//------------ ChasePlayer State ------------
void ChasePlayer::OnEnter() {

    m_Player = m_pFovComponent->GetPlayerInVision();
    if (m_Player != nullptr)
    {
        if (m_PlayerMoveComponent == nullptr)
        {
            m_PlayerMoveComponent = m_Player->GetComponent<PacManMoveComponent>();
        }
    }
    
   
    m_MoveCoomponent->SetFollowPath(true); 
}

void ChasePlayer::OnExit() {

}

void ChasePlayer::Update(float deltaTime) {

    std::shared_ptr<GameObject> currentPlayer = m_pFovComponent->GetPlayerInVision();
    if (currentPlayer != m_PreviousPlayer)
    {
        m_Player = currentPlayer;
        if (m_Player != nullptr)
        {
            m_PlayerMoveComponent = m_Player->GetComponent<PacManMoveComponent>();
        }
        m_PreviousPlayer = m_Player;
    }

    m_MoveCoomponent->GetStateManager()->TimeInChaseStatePlus(deltaTime);
    switch (m_FincPathType) 
    {
    case FSMStates::ChasePlayer::FindPathType::NONE:
        break;
    case FSMStates::ChasePlayer::FindPathType::BESTPATH:
        if (m_PlayerMoveComponent != nullptr)
        {
            m_MoveCoomponent->FollowPathToPlayer(deltaTime, m_PlayerMoveComponent->GetCurrentIndex());
        }
        break;
    case FSMStates::ChasePlayer::FindPathType::SECONDBESTPATH:
        if (m_PlayerMoveComponent != nullptr)
        {
            m_MoveCoomponent->FollowSecondBestPath(deltaTime, m_PlayerMoveComponent->GetCurrentIndex());
        }
        break;
    case FSMStates::ChasePlayer::FindPathType::PREDICT:
        if (m_PlayerMoveComponent != nullptr)
        {
            m_MoveCoomponent->FollowToPathThatPacManWillBeAt(deltaTime, m_PlayerMoveComponent->GetCurrentIndex(), m_PlayerMoveComponent->GetCurrentDirection());
        }
        break;
    default:
        break;
    }
}



//------------ Frightened State ------------
void Frightened::OnEnter() {
    m_RenderComponent->SetTexture("frightenghost.png");
    m_MoveCoomponent->SetSpeed(40);
    m_MoveCoomponent->SetFollowPath(true);

    m_Player = m_pFovComponent->GetPlayerInVision();
    if (m_Player != nullptr)
    {
        if (m_PlayerMoveComponent == nullptr)
        {
            m_PlayerMoveComponent = m_Player->GetComponent<PacManMoveComponent>();
        }
    }
}

void Frightened::OnExit() {
    m_RenderComponent->SetTexture(m_RenderComponent->GetPreviousTexture());
    m_MoveCoomponent->SetSpeed(90);
}

void Frightened::Update(float deltaTime) {
    if (m_PlayerMoveComponent != nullptr)
    {
        m_MoveCoomponent->FleeFromPlayer(deltaTime, m_PlayerMoveComponent->GetCurrentIndex());
    }
}


//------------ ReturnToBase State ------------
void ReturnToBase::OnEnter() {
    m_MoveCoomponent->SetFollowPath(true);
    m_MoveCoomponent->SetTargetCellIndex(144);

    if (!m_BoxColliderComponent->IsActive())
    {
        m_RenderComponent->SetTexture("ghostEyes.png");
        m_MoveCoomponent->SetSpeed(200);
        m_MoveCoomponent->GetStateManager()->SetMaxTimeInBase(3.f);
    }
   
}

void ReturnToBase::OnExit() {
    if (m_RenderComponent->GetPreviousTexture() != nullptr)
    {
        m_RenderComponent->SetTexture(m_RenderComponent->GetPreviousTexture());
    }
    
    m_MoveCoomponent->SetSpeed(90);
    m_BoxColliderComponent->SetActive(true);
}

void ReturnToBase::Update(float deltaTime) {

    m_MoveCoomponent->FollowPath(deltaTime, 144);

    if (m_MoveCoomponent->GetCurrentIndex() == 144)
    {
        if (m_RenderComponent->GetPreviousTexture() != nullptr)
        {
            m_RenderComponent->SetTexture(m_RenderComponent->GetPreviousTexture());
        }
        m_MoveCoomponent->GetStateManager()->TimeInBaseStatePlus(deltaTime);
    }
   
}


////----------------- Conditions -----------------
//bool PlayerInSight::Evaluate() const {
//    if (m_pFovComponent && m_pFovComponent->GetPlayerInSight())
//        return true;
//
//    return false;
//}
//
//bool PlayerNotInSight::Evaluate() const {
//    if (m_pFovComponent && !m_pFovComponent->GetPlayerInSight() && m_MoveCoomponent->HasReachedTargetCell())
//        return true;
//
//    return false;
//}

bool PlayerTimerChase::Evaluate() const {
    return m_MoveCoomponent->GetStateManager()->TimeToSwitchToPatrol();
}

bool PlayerTimerPatrol::Evaluate() const {
    return m_MoveCoomponent->GetStateManager()->TimeToSwitchToChase();
}

bool PlayerPoweredUp::Evaluate() const {
    auto player = m_pFovComponent->GetPlayerInVision();
    if (player) {
        auto playerPoweredUp = player->GetComponent<PoweredUpComponent>();
        return playerPoweredUp && playerPoweredUp->IsPoweredUp() && m_MoveCoomponent->GetCurrentIndex() != 144;
    }
    return false;
}

bool PlayerNotPoweredUp::Evaluate() const {
    auto player = m_pFovComponent->GetPlayerInVision();
    if (player) { 
        auto playerPoweredUp = player->GetComponent<PoweredUpComponent>();
        return playerPoweredUp && !playerPoweredUp->IsPoweredUp();
    }
    return false;
}

bool AtBase::Evaluate() const {
    int baseId = 144;
    return m_MoveCoomponent->GetCurrentIndex() == baseId &&
        m_MoveCoomponent->GetStateManager()->GoOut() &&
        m_pFovComponent->GetPlayerInSight();
}

bool NotAtBase::Evaluate() const {
    int baseId = 144;
    return (!m_pFovComponent->GetPlayerInSight() &&
        m_MoveCoomponent->GetCurrentIndex() != baseId) ||
        !m_BoxColliderComponent->IsActive();
}