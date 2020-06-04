#Simple Raytracer built in C++
---
In this project, the goal was to build a simple raytracing program from scratch to simulate lighting effects on spherical objects. The entirety of this project was built without the help of third-party frameworks and libraries. While OpenGL is an included dependency, it is not used to create objects. Three separate scenes were created to test three separate types of materials: matte, chrome, and clear/translucent. Every individual pixel in the image is independently calculated depending on material and light.

##Prerequisites
---
OpenGL, FreeGlut, C++

##Building
---
`$ make`
`$ ./rayT #`

Argument List for #
1 - Scene 1
2 - Scene 2
3 - Scene 3
4 - Scene 4
5 - Renders All Scenes into .bmp files, displays the last scene in OpenGl