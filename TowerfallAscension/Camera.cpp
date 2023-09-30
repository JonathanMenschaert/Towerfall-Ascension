#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height)
	: m_CameraView{0.f, 0.f, width, height}
	, m_Width{ width}
	, m_Height{ height }
	, m_Scale{1.f}
	, m_ScaleInterval{0.05f}
	, m_ScaleMin{ 1.f }
	, m_ScaleMax{3.f}
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target) const
{
	Transform(Track(target));		
}

void Camera::Transform(const Point2f& target) const
{
	Point2f cameraCenter{ target};
	cameraCenter.x -= m_Width / 2;
	cameraCenter.y -= m_Height / 2;
	Clamp(cameraCenter);
	glScalef(m_Scale, m_Scale, 1.f);
	glTranslatef(-cameraCenter.x, -cameraCenter.y, 0.f);
}


Point2f Camera::Track(const Rectf& target) const
{
	return Point2f{ target.left + target.width / 2 , target.bottom + target.height / 2 };
}
void Camera::Clamp(Point2f& bottomLeftPos) const
{
	bottomLeftPos.x = std::max(m_LevelBoundaries.left * (1.f - 1.f / m_Scale),
		std::min(bottomLeftPos.x, (1.f + 1.f / m_Scale) * m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width));

	bottomLeftPos.y = std::max(0.f, std::min(bottomLeftPos.y, 2 * m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height));
}

void Camera::ZoomIn(float zoomSpeed)
{
	m_Scale += m_ScaleInterval * zoomSpeed;
	if (m_Scale > m_ScaleMax )
	{
		m_Scale = m_ScaleMax;
	}
	m_Width = m_CameraView.width / m_Scale;
	m_Height = m_CameraView.height / m_Scale;
}

void Camera::ZoomOut(float zoomSpeed)
{
	m_Scale -= m_ScaleInterval * zoomSpeed;
	if (m_Scale < m_ScaleMin)
	{
		m_Scale = m_ScaleMin;
	}
	m_Width = m_CameraView.width / m_Scale;
	m_Height = m_CameraView.height / m_Scale;
}

void Camera::ZoomReset()
{
	m_Scale = m_ScaleMin;
	m_Width = m_CameraView.width / m_Scale;
	m_Height = m_CameraView.height / m_Scale;
}

bool Camera::CanZoomIn()
{
	return m_Scale < m_ScaleMax;
}

bool Camera::CanZoomOut()
{
	return m_Scale > m_ScaleMax;
}
