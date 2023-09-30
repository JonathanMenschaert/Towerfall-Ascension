#pragma once
class Keybind final
{
public:
	Keybind(int scanCode);
	int GetScanCode() const;
	void SetScanCode(int scanCode);

	~Keybind() = default;
	Keybind(const Keybind& other) = default;
	Keybind& operator=(const Keybind& other) = default;
	Keybind(Keybind&& other) = default;
	Keybind& operator=(Keybind&& other) = default;
	

private:
	int m_ScanCode;	
};

