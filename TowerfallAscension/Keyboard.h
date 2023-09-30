#pragma once
#include <vector>
#include "Keybind.h"

class Keyboard final
{
public:
	enum class Action
	{
		MOVELEFT,
		MOVERIGHT,
		JUMP,
		CROUCH,
		UP,
		DASH,
		SHOOT,
		ESC,
		//Add keybinds above
		AMOUNTOFKEYBINDS
	};

	Keyboard();

	~Keyboard() = default;
	Keyboard(const Keyboard& other) = default;
	Keyboard& operator=(const Keyboard& other) = default;
	Keyboard(Keyboard&& other) = default;
	Keyboard& operator=(Keyboard&& other) = default;
	void ChangeKeybind(Action action, int scanCode);

	bool IsMoveLeftPressed() const;
	bool IsMoveRightPressed() const;
	bool IsJumpPressed() const;
	bool IsCrouchPressed() const;
	bool IsUpPressed() const;
	bool IsDashPressed() const;
	bool IsShootPressed() const;
	bool IsKeyPressed(Action action) const;
	friend std::ostream& operator<<(std::ostream& out, const Keyboard& rhs);

private:
	std::vector<Keybind> m_Keybinds;
	const Uint8* m_pKeyStates;
	void SetDefaultKeybinds();
};

std::ostream& operator<<(std::ostream& out, const Keyboard& rhs);

