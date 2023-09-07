# [Kompjuterska-Grafika-Projekt](https://github.com/Fazlibeqir/Kompjuterska-Grafika-Projekt)


## Summary
This project is a 3D racing game developed using OpenGL as the primary graphics framework. It leverages the power of OpenGL to deliver stunning 3D graphics, realistic physics, and dynamic gameplay. The foundation of the game is built upon the [Glitter](http://polytonic.github.io/Glitter/) boilerplate, streamlining development and ensuring top-notch performance.

## Getting Started
Only on Windows was tested
### Dependencies
To build this project, you'll need:

[Cmake](http://www.cmake.org/download/)

Visual Studio 2022 Compiler

## Clone the Repository

You have two ways to build the project: 

- [Using CMD](#building-with-cmd)
- [Using CLion](#building-with-clion)

In both cases, start by cloning the repository to your desired location. 
For example: C:\User\Name\Desktop\New folder
```bash
git clone  https://github.com/Fazlibeqir/Kompjuterska-Grafika-Projekt.git
```
## Building with CMD
<a name="building-with-cmd"></a>

Once you've cloned the repository, you can build it using the command line. Here are the steps:
1. Navigate to the project directory:
```bash
cd Kompjuterska-Grafika-Projekt
```
2. Create a "build" directory and move into it:
```bash
mkdir build
cd build
```
3. Generate the Visual Studio 2022 project files:
```bash
cmake -G "Visual Studio 17 2022" ..
```
Please note that "Visual Studio 17 2022" is the recommended CMake generator name for Visual Studio 2022.

4. Build the project in Release mode:
```bash
cmake --build . --config Release
```
5. After a successful build, navigate to the "Release" directory:
```bash
cd Kompjuterska-Grafika-Projekt\Release
```
6. Run the executable, which has the same name as the project:
```bash
Kompjuterska-Grafika-Projekt.exe
```
## Building with CLion
<a name="building-with-clion"></a>
Once you've cloned the repository.
1. Open CLion:
Launch CLion, and when the IDE is open, click on "Open" in the initial window, or go to "File" > "Open" and select the cloned project directory.
2. Configure CMake:
CLion should automatically detect the CMakeLists.txt file in your project directory. If not, you can manually configure CMake by going to "File" > "Settings" (or "CLion" > "Preferences" on macOS) and then navigate to "Build, Execution, Deployment" > "CMake." Make sure the "CMake options" field contains the following:
```bash
-G "Visual Studio 17 2022"
```
<img src="https://github.com/Fazlibeqir/Kompjuterska-Grafika-Projekt/raw/main/CLion.png" width="650">
This sets the CMake generator to Visual Studio 2022. If it's already set up correctly, you can skip this step, but "Visual Studio 17 2022" is the recommended CMake generator name for Visual Studio 2022.

3. Build the Project:
Now that you have CLion configured, you can build the project. In the CLion IDE, navigate to "Build" > "Build Project".

4. Run the Executable:
After a successful build, you can run the executable from within CLion. In the "Run" menu, select "Run 'Kompjuterska-Grafika-Projekt'." This will execute the built binary.
## Input Configuration:
Arrow Keys: Up, Down, Left, Right -Car to moves

WASD - Keys for the Camera to move

Space - Car brakes

R- Car rotates and T - Car jumps

Esc- InGame to show/hide the Menu

Arrow Keys: Left, Right - PreGame to choose the Car out of 3 cars

Enter Key- PreGame you can start the game

Q- Quit the game anytime

Tab key to follow/unfollow the Camera the Car 

## Game Development Stack:
Functionality           | Library
----------------------- | ------------------------------------------
Mesh Loading            | [assimp](https://github.com/assimp/assimp)
Physics                 | [bullet](https://github.com/bulletphysics/bullet3)
OpenGL Function Loader  | glad
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
Texture Loading         | [stb](https://github.com/nothings/stb)
Audio                   | [irrKlang](https://www.ambiera.com/irrklang/downloads.html)
