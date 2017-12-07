#include "Sun.h"
#include "../utilities/ResourceManager.h"

Sun::Sun()
{
	setTextureHandle(ResourceManager::getInstance().getTextureHandle(Texture::Sun));
	setStartDegrees(0);
	setScale(100);
}
