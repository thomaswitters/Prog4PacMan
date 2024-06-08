#include <SDL.h>
#include "InputManager.h"
//#include "imgui/imgui_impl_sdl2.h"

using namespace dae;

InputManager::InputManager()
{
    m_pControllers.push_back(std::make_unique<GamePad>(0));
    m_pKeyboards.push_back(std::make_unique<Keyboard>(0));
}

bool InputManager::ProcessInput(float deltaTime)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }

        //ImGui_ImplSDL2_ProcessEvent(&e);
    }

    for (size_t i = 0; i < m_pControllers.size(); ++i)
    {
        m_pControllers[i]->Update();

        for (auto& commandPair : m_GamePadCommands)
        {
            auto& button = commandPair.first;
            auto& commandData = commandPair.second;

            switch (commandData.inputType)
            {
            case KeyState::keyPressed:
                if (m_pControllers[i]->IsButtonPressed(button))
                {
                    commandData.command->Execute(deltaTime);
                }
                break;
            case KeyState::keyUp:
                if (m_pControllers[i]->IsButtonUp(button))
                {
                    commandData.command->Execute(deltaTime);
                }
                break;
            case KeyState::keyDown:
                if (m_pControllers[i]->IsButtonDown(button))
                {
                    commandData.command->Execute(deltaTime);
                }
                break;
            default:
                continue;
                break;
            }
        }

        for (const auto& command : m_GamePadStickCommands)
        {
            const auto controllerStick = command.first;
            constexpr float deadzone{ 10000 };

            // Get controller stick values
            auto stickValues = GetControllerStickValues(controllerStick);

            if (abs(stickValues.x) > deadzone || abs(stickValues.y) > deadzone)
            {
                command.second.command->Execute(deltaTime);
            }
        }
    }

    for (size_t i = 0; i < m_pKeyboards.size(); ++i)
    {
        m_pKeyboards[i]->Update();

        for (auto& commandPair : m_KeyboardCommands)
        {
            auto& key = commandPair.first;
            auto& commandData = commandPair.second;

            switch (commandData.inputType)
            {
            case KeyState::keyPressed:
                if (m_pKeyboards[i]->IsPressed(key))
                {
                    commandData.command->Execute(deltaTime);
                }
                break;
            case KeyState::keyUp:
                if (m_pKeyboards[i]->IsUp(key))
                {
                    commandData.command->Execute(deltaTime);
                }
                break;
            case KeyState::keyDown:
                if (m_pKeyboards[i]->IsDown(key))
                {
                    commandData.command->Execute(deltaTime);
                }
                break;
            default:
                continue;
                break;
            }
        }
    }

    return true;
}

void InputManager::SetGamePadCommand(GamePad::ControllerButton button, Command* pCommand, KeyState inputType, bool canBeDeleted)
{
    m_GamePadCommands[button] = CommandData(std::unique_ptr<Command>(pCommand), inputType, canBeDeleted);
}

void InputManager::SetGamePadStickCommand(GamePad::ControllerStick button, Command* pCommand, bool canBeDeleted)
{
    m_GamePadStickCommands[button] = CommandData(std::unique_ptr<Command>(pCommand), KeyState::keyPressed, canBeDeleted);
}

void InputManager::SetKeyboardCommand(SDL_Scancode key, Command* pCommand, KeyState inputType, bool canBeDeleted)
{
    m_KeyboardCommands[key] = CommandData(std::unique_ptr<Command>(pCommand), inputType, canBeDeleted);
}

glm::vec2 InputManager::GetControllerStickValues(GamePad::ControllerStick stick) const
{
    glm::vec2 dir;

    if (stick == GamePad::ControllerStick::LeftStick)
    {
        dir.x = m_pControllers[0]->GetLeftThumbstickX();
        dir.y = -m_pControllers[0]->GetLeftThumbstickY();
    }
    else
    {
        dir.x = m_pControllers[0]->GetRightThumbstickX();
        dir.y = -m_pControllers[0]->GetRightThumbstickY();
    }

    return dir;
}

void InputManager::ClearInputs()
{
    for (auto it = m_GamePadCommands.begin(); it != m_GamePadCommands.end(); )
    {
        if (it->second.canBeDeleted)
        {
            it = m_GamePadCommands.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = m_GamePadStickCommands.begin(); it != m_GamePadStickCommands.end(); )
    {
        if (it->second.canBeDeleted)
        {
            it = m_GamePadStickCommands.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = m_KeyboardCommands.begin(); it != m_KeyboardCommands.end(); )
    {
        if (it->second.canBeDeleted)
        {
            it = m_KeyboardCommands.erase(it);
        }
        else
        {
            ++it;
        }
    }
}





	