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

            switch (commandData.second)
            {
            case KeyState::keyPressed:
                if (m_pControllers[i]->IsButtonPressed(button))
                {
                    commandData.first->Execute(deltaTime);
                }
                break;
            case KeyState::keyUp:
                if (m_pControllers[i]->IsButtonUp(button))
                {
                    commandData.first->Execute(deltaTime);
                }
                break;
            case KeyState::keyDown:
                if (m_pControllers[i]->IsButtonDown(button))
                {
                    commandData.first->Execute(deltaTime);
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
                command.second->Execute(deltaTime);
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

            switch (commandData.second)
            {
            case KeyState::keyPressed:
                if (m_pKeyboards[i]->IsPressed(key))
                {
                    commandData.first->Execute(deltaTime);
                }
                break;
            case KeyState::keyUp:
                if (m_pKeyboards[i]->IsUp(key))
                {
                    commandData.first->Execute(deltaTime);
                }
                break;
            case KeyState::keyDown:
                if (m_pKeyboards[i]->IsDown(key))
                {
                    commandData.first->Execute(deltaTime);
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

void InputManager::SetGamePadCommand(GamePad::ControllerButton button, Command* command, KeyState inputType)
{
    m_GamePadCommands.insert({ button, std::make_pair(std::unique_ptr<Command>(command), inputType) });
}

void InputManager::SetGamePadStickCommand(GamePad::ControllerStick button, Command* command)
{
    m_GamePadStickCommands.insert({ button, std::unique_ptr<Command>(command)});
}

void InputManager::SetKeyboardCommand(SDL_Scancode key, Command* command, KeyState inputType)
{
    m_KeyboardCommands.insert({ key, std::make_pair(std::unique_ptr<Command>(command), inputType) });
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





	