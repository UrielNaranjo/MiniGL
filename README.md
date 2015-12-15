# MiniGL
MiniGL is an implementation of OpenGL's 3D graphics pipeline.
I only implemented the minigl.cpp file.
Each tests draws a different object to the screen with varying colors. 

##Installation

>$ git clone https://github.com/UrielNaranjo/MiniGL.git

>$ cd MiniGL

>$ make

>$ ./minigl <1-6>

##Dependencies
The OpenGL library is required in order to compile this code.
   
To install OpenGL on linux run: 
>$ sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev

>$ sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev

To install OpenGL on Mac you must install the developer tools that come with Xcode. Xcode can be found on the Appstore.
You must then change the LDFLAGS in the Makefile to: 
```
LDFLAGS = -g -O2 -Wall -framework -OpenGL -framework -GLUT   
```
##Bugs/Limitations/Issues
* MiniGL has stray pixels in test 4 and 5
