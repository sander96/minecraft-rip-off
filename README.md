# Minecraft rip-off

### Running on Windows
You need:
* Microsoft Visual C++ 2017 Redistributable

1. Click on `releases` tab.
2. Download the latest version.
3. Extract the downloaded file.
4. Run the executable.

### Compiling from source on Windows
You need:
* Visual Studio 2017

1. Download/clone repository.
2. Run `dependency-downloader.ps1` script.
3. Open `.sln` file.
4. Set `Solution Configuration` to `Release` and `Solution Platforms` to `x86`.
5. `CTRL + F5` to run.

### Settings
**resources/settings.cfg** includes some basic settings that can be changed. These are *width*, *height*, *fov*, *mouse sensitivity*, *view distance* and *chunk radius*. Please note that if the game crashes, it is because the *chunk-radius* is too large. This is because the program has a limited address space available (2GB).

## Gameplay
### Controls

| Button/mouse  | Action             |
| ------------- |--------------------|
| Move mouse    | Look around        |
| W             | Move forward       |
| S             | Move backward      |
| A             | Move left          |
| D             | Move right         |
| Space         | Move up            |
| Shift         | Move down          |
| LMB           | Remove block       |
| RMB           | Add block          |
| 1             | Dirt block         |
| 2             | Grass block        |
| 3             | Sand block         |
| 4             | Stone block        |
| 5             | Wood block         |
| 6             | Cactus block       |
| 7             | Cloud block        |
| 8             | Leaves block       |
| 9             | Water block        |
