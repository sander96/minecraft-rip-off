#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
	Settings();

	unsigned getWidth() { return width; }
	unsigned getHeight() { return height; }
	float getFov() { return fov; }
	float getSensitivity() { return sensitivity; }
	float getViewDistance() { return viewDistance; }
	int getChunkRadius() { return chunkRadius; }

private:
	unsigned width;
	unsigned height;
	float fov;
	float sensitivity;
	float viewDistance;
	int chunkRadius;
};

#endif
