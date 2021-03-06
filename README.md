# Simple 3D software renderer engine in C++

![](assets/spinning_dodecahedron.gif)

### Controls
F2 - menu  
W, A, S, D - moving (when camera detached)  
Space - moving up  
Left control - moving down  
Left shift - fast move

## Third-party

* ImGui ([GitHub](https://github.com/ocornut/imgui), [MIT License](https://github.com/ocornut/imgui/blob/master/LICENSE.txt))
* ImGui SFML ([GitHub](https://github.com/eliasdaler/imgui-sfml), [MIT License](https://github.com/eliasdaler/imgui-sfml/blob/master/LICENSE))
* SFML ([GitHub](https://github.com/SFML/SFML), [zlib License](https://github.com/SFML/SFML/blob/master/license.md))

## Useful links

Projection, clipping - [link](http://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/slides/06-viewing.pdf)

Intersection of a line and a plane - [link](http://www.songho.ca/math/line/line.html#intersect_lineplane)

Fast Extraction of Viewing Frustum Planes from the WorldView-Projection Matrix - [link](http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf)