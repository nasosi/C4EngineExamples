# C4-Engine examples
<div align="center">
<p float="center">
  <img src="Media/Screenshots/menu.png" height="150" />
  <img src="Media/Screenshots/spacecastle.png" height="150" /> 
  <img src="Media/Screenshots/spacecity.png" height="150" />
</p>
</div>

This repository contains a collection of basic examples for C4Engine (https://c4engine.com/). 

The examples are intentionally incremental, with each one focusing on a single aspect of the engine. As a result, they are generally simpler and more narrowly focused than the official tutorials. Tutorial-style comments are included throughout the code to explain relevant sections in detail. 

To avoid repetition, later examples omit comments for code that was already covered in earlier examples. Some examples introduce little or no new code and instead serve primarily as references for how worlds, materials, or scripts should be structured and designed.

# C4 Engine — Adding Examples to the Visual Studio Solution

To add the examples to the C4 Engine Visual Studio solution, follow the steps below.

A video walkthrough of the process is available in the project wiki:  
https://github.com/nasosi/C4EngineExamples/wiki

---

## 1. Set Up the Folder Structure

Place the `Examples` folder inside a `V4` subdirectory within the main `C4` folder.

Your directory structure should look like this:

```text
C4-Engine/
├── Data/
├── EngineCode/
├── ...
└── V4/
    └── Examples/
        ├── 001_Simple/
        └── 002_Display_Text/
```

### Alternative: Add as a Git Submodule

Instead of creating the folder structure manually, you can add the repository as a Git submodule.

First, create a `V4` folder in the root directory of the `C4-Engine` folder. Then open a command prompt in the `C4-Engine` root directory and run the following commands:

```bash
git submodule add https://github.com/nasosi/C4EngineExamples V4/Examples
git submodule update --init --recursive
```

---

## 2. Open the C4 Solution

Open the Visual Studio solution located at:

```text
{C4 Engine Root Folder}\VisualStudio-Win\C4\C4.sln
```

---

## 3. Create a Solution Folder

In **Solution Explorer** (typically located on the right side of Visual Studio):

1. Right-click the solution (for example, `Solution "C4"`).
2. Select **Add → New Solution Folder**.
3. Name the folder:

```text
Examples
```

---

## 4. Add the First Example Project

1. Right-click the newly created **Examples** solution folder.
2. Select **Add → Existing Project**.
3. Navigate to:

```text
{C4 Engine Root Folder}\V4\Examples\001_Simple
```

4. Select the `.vcxproj` file.
5. Click **Open**.

---

## 5. Add the Remaining Example Projects

Repeat the following steps for each additional example project:

1. Right-click the **Examples** solution folder.
2. Select **Add → Existing Project**.
3. Navigate to the desired example folder.
4. Select its `.vcxproj` file.

---

## 6. Create Data and Import Junctions

Run the following file from the `Examples` directory:

```text
{C4 Engine Root Folder}\V4\Examples\CreateJunctions.bat
```

This will create the required ```Data``` and ```Import``` junctions for the examples.

---
# List of Examples

The currently available examples are listed below.

| Name | Description |
|------|-------------|
| `001_Simple` | A basic example containing a very simple world. Demonstrates core concepts such as locating a spectator marker, positioning the camera, and setting up basic input handling. |
| `002_Text_Display` | Demonstrates how to display static text on the screen. |
| `003_Start_Window` | A basic menu implemented using C4 Engine panels. The user can click a button to start the game. |
| `004_Improved_Start_Window` | An expanded menu that allows the user to start, resume, or quit the game. |
| `005_Physics_and_Collision` | Demonstrates basic physics and collision handling within a world. |
| `006_Simple_Shader` | A simple procedural shader example. |
| `007_Simple_Animated_Shader` | Demonstrates an animated procedural shader. |
| `008_Adding_Geometry_with_Collision_Programmatically` | Demonstrates how to programmatically create geometry with collision enabled. |
| `009_Custom_Controller` | Demonstrates a custom controller used to rotate geometry with collision enabled. |
| `010_User_Controlled_Object` | A small space-themed environment featuring a user-controlled spacecraft. |
| `011_Texture` | Demonstrates texturing and material setup. |
| `011_ParallelProcessing` | Demonstrates per-frame multi-threaded computations. |
