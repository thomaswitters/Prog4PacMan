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
    std::cout << "Entering Patrol state" << std::endl;
    m_Transform = m_pOwner.lock()->GetComponent<TransformComponent>();

    m_PatrolPath = GetPatrolPath();
    glm::vec2 currentPosition = m_Transform->GetLocalPosition();
    m_CurrentWaypointIndex = FindClosestWaypointIndex(currentPosition);
}

void Patrol::OnExit() {
}

void Patrol::Update(float deltaTime) {
    if (m_Transform) {
        glm::vec2 currentPosition = m_Transform->GetLocalPosition();

        if (!m_PatrolPath.empty()) {
            glm::vec2 currentWaypoint = m_PatrolPath[m_CurrentWaypointIndex];

            glm::vec2 direction = glm::normalize(currentWaypoint - currentPosition);

            glm::vec2 newPosition = currentPosition + direction * m_Speed * deltaTime;

            float distanceToWaypoint = glm::length(currentWaypoint - newPosition);
            if (distanceToWaypoint < m_Tolerance) {
                m_CurrentWaypointIndex = (m_CurrentWaypointIndex + 1) % m_PatrolPath.size();
            }

            m_Transform->Translate(glm::vec3(newPosition - currentPosition, 0.f));

            if (glm::abs(direction.y) > glm::abs(direction.x)) {
                if (direction.y > 0)
                    m_Transform->SetDirection(glm::f32vec2{ 0.f, 1.f });
                else
                    m_Transform->SetDirection(glm::f32vec2{ 0.f, -1.f });
            }
            else {
                if (direction.x > 0)
                    m_Transform->SetDirection(glm::f32vec2{ 1.f, 0.f });
                else
                    m_Transform->SetDirection(glm::f32vec2{ -1.f, 0.f });
            }
        }
        else {
            std::cerr << "Patrol path is empty!" << std::endl;
        }
    }
    else {
        std::cerr << "TransformComponent not found!" << std::endl;
    }
}
std::vector<glm::vec2> Patrol::GetPatrolPath() const {
    std::vector<glm::vec2> patrolPath;

    patrolPath.push_back(glm::vec2(100.f, 50.f)); // Waypoint 1
    patrolPath.push_back(glm::vec2(200.f, 50.f)); // Waypoint 2
    patrolPath.push_back(glm::vec2(200.f, 150.f)); // Waypoint 3
    patrolPath.push_back(glm::vec2(100.f, 150.f)); // Waypoint 4

    return patrolPath;
}
size_t Patrol::FindClosestWaypointIndex(const glm::vec2& currentPosition) const {
    if (!m_PatrolPath.empty()) {
        size_t closestIndex = 0;
        float closestDistance = glm::length(currentPosition - m_PatrolPath[0]);

        for (size_t i = 1; i < m_PatrolPath.size(); ++i) {
            float distance = glm::length(currentPosition - m_PatrolPath[i]);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestIndex = i;
            }
        }

        return closestIndex;
    }
    else {
        std::cerr << "Patrol path is empty!" << std::endl;
        return 0;
    }
}


//------------ ChasePlayer State ------------

void ChasePlayer::OnEnter() {
    std::cout << "Entering ChasePlayer state" << std::endl;
}

void ChasePlayer::OnExit() {

}

void ChasePlayer::Update(float) {

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
    if (m_pFovComponent && !m_pFovComponent->GetPlayerInSight())
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
