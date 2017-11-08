md download

wget "https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.bin.WIN32.zip" -OutFile "download/glfw.zip"
Expand-Archive "download/glfw.zip" -DestinationPath "lib/"
Rename-Item "lib/glfw-3.2.1.bin.WIN32" -newName "GLFW"

wget https://github.com/g-truc/glm/releases/download/0.9.8.5/glm-0.9.8.5.zip -OutFile "download/glm.zip"
Expand-Archive "download/glm.zip" -DestinationPath "lib/"

wget https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0-win32.zip -OutFile "download/glew.zip"
Expand-Archive "download/glew.zip" -DestinationPath "lib/"
Rename-Item "lib/glew-2.1.0" -newName "GLEW"

wget https://netix.dl.sourceforge.net/project/openil/DevIL%20Windows%20SDK/1.8.0/DevIL-Windows-SDK-1.8.0.zip -OutFile "download/devil.zip"
Expand-Archive "download/devil.zip" -DestinationPath "lib/"
Rename-Item "lib/DevIL Windows SDK" -newName "DEVIL"

Remove-Item "download" -Force -Recurse
