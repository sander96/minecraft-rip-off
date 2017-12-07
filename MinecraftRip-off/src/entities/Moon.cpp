#include "Moon.h"
#include "../utilities/ResourceManager.h"

Moon::Moon()
{
	setTextureHandle(ResourceManager::getInstance().getTextureHandle(Texture::Moon));
	setStartDegrees(180);
	setScale(50);
}
