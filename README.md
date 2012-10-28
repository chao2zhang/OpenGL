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

./proj2


In case it doesn't work, i have submitted an executive file: proj2

Input/Output
------------
Filename:User specified.

Format:corresponding to the fashion in `Syllabus`

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

Please follow the instructions(esp. with a valid file) 

Overview of program
===================
1. Implement three orthographic projections

2. Implement cavalier and cabinet projections(to XY-Plane)

3. Translate, Rotate(using Quaternion methods), Scale, Normalize(Scale around (0.5, 0.5, 0.5) to ensure objects are inside the bouding box[0, 1] * [0, 1] * [0, 1]), see `Polygon::translate`, `Polygon::rotate`, `Polygon::scale` and so on.

**For normalization, (Xmin, Xmax, Ymin, Ymax, Zmin, Zmax) would be scaled to (0, 1, 0, 1, 0, 1) centered at (0.5, 0.5, 0.5)

4. Caculating centroid, see `Polygon::centroid()`

5. Data abstractions:Point, Line, Polygon, Matrix are all C++ classes

6. GL library calls:`glBegin`, `gluOrtho2D`, `glColor3bv` and so on

7. A more friendly user interface. Draw a console window using `glRaster2D` and `glutBitmapCharacter`

8. Subwindows in main window

Step-by-step Instruction
========================
1.Edit input file`sample1.txt` to specify polygons
----------------------------------
2.Run `proj1`, window would be like `sample_input.png`
-----------------------------------------------
3.Print instructions below:
----------------------------------
sample1.txt #open sample1.txt

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

e #save and quit


4.This would replace the file with contents like in `sample2.txt`, and window would be like `sample_output.png`
------------------------------------------------------------------------------------------------------------------------------


Interface Description
=====================
Window background is grey.
Subwindow background is white.
Selected polygon is red and others are blue.
Rotate axis is orange.

Give instructions in a subWindow like `Console`

Input/Output Description
========================
Input: Contains a triangle, a pentagon, a tetrahedron and a cube.

Input: Contains a triangle, a pentagon, a tetrahedron and a cube.
Every object is normalized.


