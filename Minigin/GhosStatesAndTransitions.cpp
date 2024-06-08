#include "GhosStatesAndTransitions.h"
#include <iostream>
#include "TransformComponent.h"
#include "FOVComponent.h"
#include "PoweredUpComponent.h"

// FSM TODO:
using namespace dae::FSMStates;
using namespace dae::FSMConditions;


//------------
//---STATES---
//------------
void None::OnEnter() {
    m_pMoveComponent->SetFollowPath(false);
}

void None::OnExit() {
}

void None::Update(float) {

}

void NoneFrightened::OnEnter() {
    m_pMoveComponent->SetFollowPath(false);
    m_pRenderComponent->SetTexture("frightenghost.png");
    m_pMoveComponent->SetSpeed(40);
}

void NoneFrightened::OnExit() {
    m_pRenderComponent->SetTexture(m_pRenderComponent->GetPreviousTexture());
    m_pMoveComponent->SetSpeed(90);
}

void NoneFrightened::Update(float) {

}


//------------ Patrol State ------------
void Patrol::OnEnter() {
    m_pMoveComponent->SetFollowPath(true);

    m_pMoveComponent->SetTargetCellIndex(m_TargetCellIndices[0]);

    m_pMoveComponent->GetStateManager()->SetPatrolPhasePlus(1);

    if (m_pMoveComponent->GetStateManager()->GetPatrolPhase() >= 3)
    {
        m_pMoveComponent->GetStateManager()->SetMaxTimeInPatrolState(5);
    }
}

void Patrol::OnExit() {
}

void Patrol::Update(float deltaTime) {
    int currentCell = m_pMoveComponent->GetCurrentIndex();
    int nextCell = 0;

    if (m_pMoveComponent->HasReachedTargetCell()) {
        if (currentCell == m_TargetCellIndices[0])
            nextCell = m_TargetCellIndices[1];
        else if (currentCell == m_TargetCellIndices[1])
            nextCell = m_TargetCellIndices[2];
        else if (currentCell == m_TargetCellIndices[2])
            nextCell = m_TargetCellIndices[3];
        else
            nextCell = m_TargetCellIndices[0];

        m_pMoveComponent->SetTargetCellIndex(nextCell);
    }

    m_pMoveComponent->FollowPath(deltaTime, nextCell);

    m_pMoveComponent->GetStateManager()->TimeInPatrolStatePlus(deltaTime);
}


//------------ ChasePlayer State ------------
void ChasePlayer::OnEnter() {

    m_pPlayer = m_pFovComponent->GetPlayerInVision();
    if (m_pPlayer != nullptr)
    {
        if (m_pPlayerMoveComponent == nullptr)
        {
            m_pPlayerMoveComponent = m_pPlayer->GetComponent<PacManMoveComponent>();
        }
    }

   
    m_pMoveComponent->SetFollowPath(true);
}

void ChasePlayer::OnExit() {

}

void ChasePlayer::Update(float deltaTime) {

    std::shared_ptr<GameObject> currentPlayer = m_pFovComponent->GetPlayerInVision();
    if (currentPlayer != m_pPreviousPlayer)
    {
        m_pPlayer = currentPlayer;
        if (m_pPlayer != nullptr)
        {
            m_pPlayerMoveComponent = m_pPlayer->GetComponent<PacManMoveComponent>();
        }
        m_pPreviousPlayer = m_pPlayer;
    }

    m_pMoveComponent->GetStateManager()->TimeInChaseStatePlus(deltaTime);
    switch (m_FincPathType) 
    {
    case FSMStates::ChasePlayer::FindPathType::NONE:
        break;
    case FSMStates::ChasePlayer::FindPathType::BESTPATH:
        if (m_pPlayerMoveComponent != nullptr)
        {
            m_pMoveComponent->FollowPathToPlayer(deltaTime, m_pPlayerMoveComponent->GetCurrentIndex());
        }
        break;
    case FSMStates::ChasePlayer::FindPathType::SECONDBESTPATH:
        if (m_pPlayerMoveComponent != nullptr)
        {
            m_pMoveComponent->FollowSecondBestPath(deltaTime, m_pPlayerMoveComponent->GetCurrentIndex());
        }
        break;
    case FSMStates::ChasePlayer::FindPathType::PREDICT:
        if (m_pPlayerMoveComponent != nullptr)
        {
            m_pMoveComponent->FollowToPathThatPacManWillBeAt(deltaTime, m_pPlayerMoveComponent->GetCurrentIndex(), m_pPlayerMoveComponent->GetCurrentDirection());
        }
        break;
    default:
        break;
    }
}



//------------ Frightened State ------------
void Frightened::OnEnter() {
    m_pRenderComponent->SetTexture("frightenghost.png");
    m_pMoveComponent->SetSpeed(40);
    m_pMoveComponent->SetFollowPath(true);

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
    m_pRenderComponent->SetTexture(m_pRenderComponent->GetPreviousTexture());
    m_pMoveComponent->SetSpeed(90);
}

void Frightened::Update(float deltaTime) {
    if (m_PlayerMoveComponent != nullptr)
    {
        m_pMoveComponent->FleeFromPlayer(deltaTime, m_PlayerMoveComponent->GetCurrentIndex());
    }
}


//------------ ReturnToBase State ------------
void ReturnToBase::OnEnter() {
    m_pMoveComponent->SetFollowPath(true);
    m_pMoveComponent->SetTargetCellIndex(144);

    if (!m_pBoxColliderComponent->IsActive())
    {
        m_pRenderComponent->SetTexture("ghostEyes.png");
        m_pMoveComponent->SetSpeed(200);
        m_pMoveComponent->GetStateManager()->SetMaxTimeInBase(3.f);
    }
   
}

void ReturnToBase::OnExit() {
    if (m_pRenderComponent->GetPreviousTexture() != nullptr)
    {
        m_pRenderComponent->SetTexture(m_pRenderComponent->GetPreviousTexture());
    }
    
    m_pMoveComponent->SetSpeed(90);
    m_pBoxColliderComponent->SetActive(true);
}

void ReturnToBase::Update(float deltaTime) {

    m_pMoveComponent->FollowPath(deltaTime, 144);

    if (m_pMoveComponent->GetCurrentIndex() == 144)
    {
        if (m_pRenderComponent->GetPreviousTexture() != nullptr)
        {
            m_pRenderComponent->SetTexture(m_pRenderComponent->GetPreviousTexture());
        }
        m_pMoveComponent->GetStateManager()->TimeInBaseStatePlus(deltaTime);
    }
   
}

bool PlayerTimerChase::Evaluate() const {
    return m_pMoveComponent->GetStateManager()->TimeToSwitchToPatrol();
}

bool PlayerTimerPatrol::Evaluate() const {
    return m_pMoveComponent->GetStateManager()->TimeToSwitchToChase();
}

bool PlayerPoweredUp::Evaluate() const {
    auto player = m_pFovComponent->GetPlayerInVision();
    if (player) {
        auto playerPoweredUp = player->GetComponent<PoweredUpComponent>();
        return playerPoweredUp && playerPoweredUp->IsPoweredUp() && m_pMoveComponent->GetCurrentIndex() != 144;
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
    return m_pMoveComponent->GetCurrentIndex() == baseId &&
        m_pMoveComponent->GetStateManager()->GoOut() &&
        m_pFovComponent->IsPlayerInSight();
}

bool NotAtBase::Evaluate() const {
    int baseId = 144;
    return (!m_pFovComponent->IsPlayerInSight() &&
        m_pMoveComponent->GetCurrentIndex() != baseId) ||
        !m_pBoxColliderComponent->IsActive();
}