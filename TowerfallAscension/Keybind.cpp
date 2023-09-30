#include "pch.h"
#include "Keybind.h"

Keybind::Keybind(int scanCode)
	:m_ScanCode{scanCode}
{
}

int Keybind::GetScanCode() const
{
	return m_ScanCode;
}

void Keybind::SetScanCode(int scanCode)
{
	m_ScanCode = scanCode;
}
