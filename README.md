# MiniGL
MiniGL is an implementation of OpenGL's 3D graphics pipeline.
##Installation

>$ git clone https://github.com/UrielNaranjo/MiniGL.git

>$ cd MiniGL

>$ make

>$ ./pipeline 

##Dependencies
The OpenGL library is required in order to compile this code.
   
To install OpenGL on linux run: 
>$ sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
>$ sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev

To install OpenGL on Mac you must install the developer tools that come with Xcode. Xcode is found on the Appstore
You must then change the LDFLAGS in the Makefile to: 
```
LDFLAGS = -g -O2 -Wall -framework -OpenGL -framework -GLUT   
```
