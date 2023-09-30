#pragma once
#include "Upgrade.h"
#include "utils.h"

class Sprite;
class ResourceManager;
class Hat final: public Upgrade
{
public:
	Hat(const Point2f& position, ResourceManager* resources);
	virtual ~Hat();
	Hat(const Hat& c) = delete;
	Hat& operator=(const Hat& rhs) = delete;
	Hat(Hat&& c) = delete;
	Hat& operator=(Hat&& rhs) = delete;

	virtual void Update(float elapsedSec, const Level& level) override;
	virtual void Draw() const override;
	virtual bool CheckHit(const Rectf& actorShape, const Keyboard& keyboard) override;

private:
	Sprite* m_pHat;
};

