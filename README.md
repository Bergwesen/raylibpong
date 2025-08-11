# PING PONG in Raylib zum "lernen" der Bibliothek

# Raylib-Quickstart
A simple cross platform template for setting up a project with the bleeding edge raylib code.
Works with C or C++.

## Supported Platforms
Quickstart supports the main 3 desktop platforms
* Windows
* Linux
* MacOS

# VSCode Users (all platforms)
* Download the quickstart
* Rename the folder to your game name
* Open the folder in VSCode.
* Press F5 to build
* You are good to go.



## Microsoft Visual Studio
* Run the build-VisualStudio2022.bat
* double click the .sln file that is geneated.
* develop your game
* you are good to go.

# Linux Users
* CD into the build folder
* run ./premake5 gmake2
* CD back to the root
* run make
* you are good to go

# MacOS Users
* CD into the build folder
* run ./premake5.osx gmake2
* CD back to the root
* run make
* you are good to go

# Output files
The built code will be in the bin dir

# Working directories and the resources folder
The example uses a utility function from path_utils.h that will find the resources dir and set it as the current working directory. This is very useful when starting out. If you wish to manage your own working directory you can simply remove the call to the function and the header.

# Changing to C++
Simply rename src/main.c to src/main.cpp and re-run the steps above and do a clean build.

