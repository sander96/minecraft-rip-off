#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
	Settings();

	unsigned getWidth() { return width; }
	unsigned getHeight() { return height; }
	int getFov() { return fov; }

private:
	unsigned width;
	unsigned height;
	int fov;
};

#endif