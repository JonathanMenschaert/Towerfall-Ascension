#include "pch.h"
#include "Scenery.h"
#include "ResourceManager.h"

Scenery::Scenery(const Vector2f& offset, const Vector2f& scale, const Rectf& boundaries, ResourceManager* resources)
	:m_Offset{offset}
	,m_Scale{scale}
	,m_LevelBoundaries{ boundaries }
	,m_pResources{resources}
{
}
