#include "pch.h"
#include "Keyboard.h"
#include <iostream>

Keyboard::Keyboard()
	:m_Keybinds{}
	,m_pKeyStates{SDL_GetKeyboardState(nullptr)}
{
	m_Keybinds.reserve(int(Action::AMOUNTOFKEYBINDS));
	SetDefaultKeybinds();
}

bool Keyboard::IsMoveLeftPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::MOVELEFT)].GetScanCode()];
}

bool Keyboard::IsMoveRightPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::MOVERIGHT)].GetScanCode()];
}

bool Keyboard::IsJumpPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::JUMP)].GetScanCode()];
}

bool Keyboard::IsCrouchPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::CROUCH)].GetScanCode()];
}

bool Keyboard::IsDashPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::DASH)].GetScanCode()];
}

bool Keyboard::IsShootPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::SHOOT)].GetScanCode()];
}

bool Keyboard::IsUpPressed() const
{
	return m_pKeyStates[m_Keybinds[int(Action::UP)].GetScanCode()];
}

bool Keyboard::IsKeyPressed(Action action) const
{
	return m_pKeyStates[m_Keybinds[int(action)].GetScanCode()];
}

void Keyboard::ChangeKeybind(Action action, int scanCode)
{
	m_Keybinds[int(action)].SetScanCode(scanCode);
}

void Keyboard::SetDefaultKeybinds()
{
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_A));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_D));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_SPACE));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_S));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_W));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_LSHIFT));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_E));
	m_Keybinds.push_back(Keybind(SDL_SCANCODE_ESCAPE));
}

std::ostream& operator<<(std::ostream& out, const Keyboard& rhs)
{
	out << "=== Controls ===" << std::endl;
	out << "A: Walk Left / Hang on ledge / Aim Left / Select Left" << std::endl;
	out << "D: Walk Right / Hang on ledge / Aim Right / Select Up" << std::endl;
	out << "Space: Jump / Confirm Select" << std::endl;
	out << "S: Crouch / Aim Down / Select Down" << std::endl;
	out << "W: Look up / Aim Up / Select Up" << std::endl;
	out << "LShift: Dash / Roll" << std::endl;
	out << "E: Load Arrow / Shoot" << std::endl;
	out << "Esc: Unpause" << std::endl;
	out << "I: Show this menu" << std::endl;
	out << "================" << std::endl;
	return out;
}