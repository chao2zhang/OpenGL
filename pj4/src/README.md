Project 3 For ECS 175
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

./proj4


In case it doesn't work, I have submitted an executive file: proj4

Input/Output
------------
Filename:User specified.

Format:

One line for T(total number of curves)

For each curve,
	
	One line for N and K(0 for Bezier, other values for Bspline),

	N lines for each point X_i, Y_i,

	One lines for each knot K_i.

Menu
----

Curve: 
	
	Next Curve(x)

	Previous Curve(v)
		
	Add Bezier
	
	Add Bspline
	
	Remove Current Curve

Point:

	Next Point(n)

	Prev Point(p)

	Add Point(a)

	Remove Current Point(r)

	Insert Point(i)

	Modify Current Point(m)

B-spline:

	Increment K(+)

	Decrement K(-)

Load(L) #load from `data.txt` 

Save(S) #save to `data.txt`

Parameters
==========

1. `g` whether display a colored imaged or a grayscale image

2. `h` whether half-toning or not

3. `dist` distance between viewpoint and center of the object box

4. `ka`, `kd`, `ks`, `k`(Replace K with K|l-p|), `ia`, `il`, `n`

5. Input: first input `m` for setting parameter, then input assignment like `g=0` or `ka=1 1 1` to set parameters

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

2.Run `proj3`, the output(7 windows in total) would be like `sample_input.png`

3.Print instructions below:

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

m #parameter

n=4 #set N in Phong-lighting model

e #save

q #quit

4.This would replace the file with contents like in `sout.txt`, and window would be like `sample_output.png`

Interface Description
=====================
1 Window for command and display parameters.
6 Windows for showing objects from FRONT/BACK/LEFT/RIGHT/TOP/DOWN.

Input/Output Description
========================
Input: Contains a triangle, a tetrahedron and a dodecahedron.

Input: Contains a triangle, a tetrahedron and a dodecahedron. Each object is normalized.


