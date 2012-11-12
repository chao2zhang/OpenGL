Project 2 For ECS 175
=====================
Zhang, Chao
-----------
Operating System:`Ubuntu 12.04 amd64`

IDE:Using `Code::Blocks 10.05` for coding, `Make` for compiling and running

G++ Version:`Ubuntu/Linaro 4.6.3-1ubuntu5`

Github:`https://github.com/ComboZhc/OpenGL`

Please README on Github, it reads better than just plain text!

Run using Makefile
------------------
Commands:

cd src

make clean

make

./proj3


In case it doesn't work, i have submitted an executive file: proj2

Input/Output
------------
Filename:User specified.

Format:
Number_Of_Points
X0 Y0 Z0 R0 G0 B0
X1 Y1 Z1 R1 G1 B1
X2 Y2 Z2 R2 G2 B2
Xi Yi Zi Ri Gi Bi
Number_Of_Triangles
Px0 Py0 Pz0
Px1 Py1 Pz1
Px2 Py2 Pz2
Pxi Pyi Pzi
Number_Of_Objects
Number_Of_Triangles_Object0 T0 T1 ...
Number_Of_Triangles_Object1 T0 T1 ...
Number_Of_Triangles_Objecti T0 T1 ...

Console Menu(Which likes a real console!)
-----------------

(P)revious 

(N)ext 

(T)ranslate 

(R)otate 

(S)cale 

Normali(Z)e 

Sav(E) 

(Q)uit

Para(M)eter

Please follow the instructions(esp. with a valid file) 

Overview of program
===================
1. Implement a camera-like orthogonal projection

2. Implement Phong-light model(replace constant `K` with `k||l-p||`)

3. Implement Gouraud shading.

4. Implement Half-toning(WITH AND WITHOUT COLOR!)

5. Implement Painter's algorithm

6. Translate, Rotate(using Quaternion methods), Scale(by centroid), Normalize, see `translate()`, `rotate()`, `scale()` and so on.

**For normalization, (Xmin, Xmax, Ymin, Ymax, Zmin, Zmax) would be scaled to (0, 1, 0, 1, 0, 1) centered at (0.5, 0.5, 0.5)

7. Data abstractions:Point, Color, Matrix are all C++ classes

8. A more friendly user interface. Draw a console window using `glRaster2D` and `glutBitmapCharacter`

Step-by-step Instruction
========================
1.Edit input file`s.txt` to specify polygons and polyhedras
----------------------------------
2.Run `proj3`, the output(7 windows in total) would be like `sample_input.png`
-----------------------------------------------
3.Print instructions below:
----------------------------------
s.txt #open s.txt

t #translate

0 0.2 0 #translate vector

n #next

r #rotate

0.5 0.5 0.2 #first point on rotate axis

0.5 0.5 0.8 #second point on rotate axis

30 #rotate angle in degree(direction:counter-clockwise to axis from first to second)

n #next

s #scale

1.5 #scale ratio

z #normalize

e #save

q #quit
4.This would replace the file with contents like in `sout.txt`, and window would be like `sample_output.png`
------------------------------------------------------------------------------------------------------------------------------


Interface Description
=====================
1 Window for command and display parameters.
6 Windows for showing objects from FRONT/BACK/LEFT/RIGHT/TOP/DOWN.

Input/Output Description
========================
Input: Contains a triangle, a tetrahedron and a dodecahedron.

Input: Contains a triangle, a tetrahedron and a dodecahedron. Each object is normalized.


