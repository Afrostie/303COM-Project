mopViewer [![Build Status](https://travis-ci.org/Afrostie/Mopviewer.svg?branch=master)](https://travis-ci.org/Afrostie/Mopviewer)
=================================================================================================================================

mopViewer is a project I took on for my 3rd Year University Project. mopViewer is a visualisation tool to display the data generated by [Sulaco](https://gitlab.com/carey.pridgeon/sulaco) (Previously known as moody).

Sulaco is a parallel n-body modelling framework for use in moderatelly large modelling scenarios of several thousand particles. The results are recorded in .mop files.

mopViewer can load these .mop files allowing you to explore the simulated scenes in space.

The main interface is made in nCurses which displays the data from the file. The main viewer is made from OpenGL and allows full 3-dimensional movement.

This project is still in it's very early stages and only implements the basics for displaying the files.

*This shows a MopFile with 10,000 particles forming a Globular Cluster* ![cluster](Resources/Images/cluster1.png)*This shows a MopFile with 100,000 particles* ![100000](Resources/Images/100000.png)

Current Features/Usage:
-----------------------

1.	Enter *1* to look at Particle information
2.	Enter *2* to Open mopViewer
3.	Enter any other key on main screen to exit
4.	Use *Up* and *Down* Arrow Keys to increase or decrease the amount of particles to load
5.	Use *Left* and *Right* Arrow Keys scale up or down the distances between particles

Particle Information Screen:
----------------------------

1.	Use the *Left* and *Right* Arrow Keys to cycle between particles
2.	Use *2* to exit program to main menu
3.	Use *Up* Arrow to cycle through states
4.	Use *Down* Arrow to reset to original state

Installation
------------

mopViewer was developed on Ubuntu but has also been tested on Windows 10 with Visual Studio Community 2015

### Required Libraries

1.	OpenGL 3.3 or higher compatible GPU/drivers
2.	[GLFW3](http://www.glfw.org/)
3.	[GLEW](http://glew.sourceforge.net/)
4.	nCurses *(libncurses-dev)*
5.	[glm](http://glm.g-truc.net/0.9.7/index.html)
6.	[STB](https://github.com/nothings/stb) for image loading (Included as a Submodule)
7.	[Assimp](https://github.com/assimp/assimp) for model loading (Use newer version off of github or it won't compile)
8.	[zLib](http://www.zlib.net/) for compression in Mopfile (not in use currently)

#### Important Note

The loading of .mop files has been moved to a seperate repository as it is code shared between mopViewer and Sulaco. It can be found here [Mopfile](http://gitlab.com/carey.pridgeon/Mopfile)

##### See INSTALL to find detailed Installation instructions


MopFile's
---------

1.	MopFiles should be placed in the Resources Folder
2.	When asked for the File Name it automatically adds the file location and the extension.
3.	E.G to use the 10000 particles file simply enter 10000 when prompted
4.	MopFiles too large for github can be downloaded from [here](https://www.dropbox.com/sh/9mq6s7wrj2i3udk/AABcujcJOI9ZtQ-YM0H6_1sRa?dl=0)

Modification's
--------------

This project is in it's very early stages and the code is extremely messy and latest version will often be unstable. Most of the important part happens in [mainApplication/Viewer/gameWindow.cpp](mainApplication/Viewer/gameWindow.cpp)

The Screen Size should default to the size of your screen

To change model used for particles modify:

```c++
Model newModel("Resources/Model/sphere/sphere.obj");
```

For loading large files it can be useful to see how many particles are loaded. In this case uncomment this line found in [Mopfile/MopFile.h](Mopfile/MopFile.h)

```c++
std::cout << "Loaded Object: " << x << " Out of: " << numParticles << std::endl;
```

TODO:
=====

1.	Improve loading times for large particle counts. Can be upwards of 5 minutes for 100,000 particles
2.	Use instanced rendering to improve performance for large amounts of particles
3.	Make improvements to skipping to allow for finer control over the amount of particles loading
4.	Add optional debug mode that will enable/disable output to the console
5.	Add more features!
