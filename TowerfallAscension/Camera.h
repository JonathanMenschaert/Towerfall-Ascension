#pragma once
class Camera final
{
public:
	Camera(float width, float height);
	~Camera() = default;
	Camera(const Camera& c) = default;
	Camera& operator=(const Camera& rhs) = default;
	Camera(Camera&& c) = default;
	Camera& operator=(Camera&& rhs) = default;

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target) const;
	void Transform(const Point2f& target) const;

	void ZoomIn(float zoomSpeed = 1.f);
	void ZoomOut(float zoomSpeed = 1.f);
	void ZoomReset();
	bool CanZoomIn();
	bool CanZoomOut();

private:
	float m_Width, m_Height, m_Scale;
	const float m_ScaleInterval, m_ScaleMin, m_ScaleMax;
	Rectf m_LevelBoundaries;
	Rectf m_CameraView;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
};


