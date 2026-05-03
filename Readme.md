# C4-Engine examples
This repository is a collection of basic examples for C4Engine (https://c4engine.com/). If you are interested in a specific version, use the particular [tag](https://github.com/nasosi/C4EngineExamples/tags).

<div align="center">
<p float="center">
  <img src="Media/Screenshots/menu.png" height="150" />
  <img src="Media/Screenshots/spacecastle.png" height="150" /> 
  <img src="Media/Screenshots/spacecity.png" height="150" />
</p>
</div>

## Installing the examples
1. Place the "Examples" folder inside a subdirectory named V4 in the main C4 folder. The structure should look like:

2. Open the C4 Visual Studio Solution from {C4 Engine Root Folder}\VisualStudio-Win\C4\C4.sln.
3. In the solution explorer (usually top left), right-click on the solution name ("Solution 'C4' ...), and navigate to Add->New Solution Folder.
4. Set the name for thesolution folder to "Examples".
5. Right-click on the folder you just created and select Add->Existing Project
6. Navigate to {C4Engine Root Director}\V4\Examples\001_Simple, click on the .vcxproj file and click open.
7. Repeat for the rest of the examples

# List of examples
The currently available examples are shown in the table below.

| Name                       |    Description                                                                                                                                                                                                                                                                                                      |
|----------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 001_Simple                 |    A basic example that contains a very simple world. Demonstrates basic concepts, such as finding a spectator marker, placing the camera, and setting up basic input  |
| 002_Text_Display           |    Demonstrates how to display some static text on the screen. |
| 003_Start_Window           |    A basic menu using C4Engine's panels. The user can click to start the game. |
| 004_Improved_Start_Window  |    An improved menu where the user can start, resume or quit the game. |
| 005_Physics_and_Collision  |    A world with basic physics and collision. |
| 006_Simple_shader          |    A simple procedural shader. |
| 007_Simple_Animated_Shader |    An animated procedural shader. |
| 008_Adding_Geometry_with_Collision_Programmatically | Demonstration on how to programmatically add geometry with collision. |
| 009_Custom_Controller | A custom controller used to rotate geometry that has collision enabled. |
| 010_User_Controlled_Object | A tiny space city with a user-controlled spacecraft. |
| 011_texture | Texturing example and automated asset import using CMake targets. |
## Changing the current application
To change the active application, modify the `C4ActiveApp` variable and reconfigure CMake. CMake will update the game.cfg file and other directories and files that may need to be changed. 

# Version format
C4EngineExamples version follows the following pattern: v[C4-Engine version].[C4EngineExamples version for the particular C4-Engine version]. For example for C4-Engine version 7.1.1, and for C4EngineExamples version 2, the version will be: v7.1.1.2
