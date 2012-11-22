Project 4 For ECS 175
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
Filename:`data.txt`

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

Overview of program
===================
1. Implement Bezier Curve

2. Implement Bspline Curve

3. Implement `Rubber band` technique to allow users to interactively specify a point.

4. Data abstractions:Curve, Bezier, Bspline, Point2f are all C++ classes

Step-by-step Instruction
========================
1.Edit input file `data.txt` to specify curves like those in `data_in.txt`

2.Run `proj4`, the screen would be like `input.png`

3.Print instructions below:

x #next curve

`left click` on the second last knot, type `8.500` and `Enter`

r #remove the 1st point in the 2nd curve

`right click` on any area of main window, choose `curve->remove current curve`

m #modify the point

`left click` on any specific point to modify current point

`right click` on any area of main window, choose `curve->add bezier`

x #next curve

x #next curve

a and `left click` #add a new point

a and `left click` #add a new point

a and `left click` #add a new point

a and `left click` #add a new point

s #save

4.This would replace the file with contents like in `data_out.txt`, and the windows would be like `output.png`

Interface Description
=====================
1 Window for displaying curves and information about current curve and current point.

1 Window for displaying and manipulating knots.

Input/Output Description
========================
Input: Contains two Bspline curves and two Bezier curves.

Input: Contains one Bspline curve and three Bezier curves.

