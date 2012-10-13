Project 1 For ECS 175
=====================
Zhang, Chao
-----------
Operating System:`Ubuntu 12.04 amd64`

IDE:Using `Code::Blocks 10.05` for coding, `Make` for compiling and running

G++ Version:`Ubuntu/Linaro 4.6.3-1ubuntu5`

Github:`https://github.com/ComboZhc/OpenGL`

Run using Makefile
------------------
Commands:

cd src

make clean

make

./proj1

Input/Output
------------
Input filename:data.in

Output filename:data.out

Format:corresponding to the fashion in `Syllabus`

Menu(Right-Click Or Press keyboard)
-----------------

Belows are actions that take effect, they are quite self-explanatory

`Next Polygon(N)`

`Prev Polygon(P)`

`Left/C-Clockwise(H)`

`Down/C-Clockwise(J)`

`Up/Clockwise(K)`

`Right/Clockwise(L)`

`Clip(X)`

`Save(W)`


Belows are actions that change state:

`Translate By +-1(T)` #default

`Rotate By +-0.01(R)`

`Scale By +-0.1(S)`

`Viewport Size(Z)`

`Viewport Position(A)`

Belows are actions that switch on/off:

`x10 On/Off(F)` #Ten actions in a row!

`DDA/Bresenham(D)` #Default algo for rastering line is DDA

Overview of program
===================
1. Implement DDA & Bresenham Algo, see `makeLineDDA()` & `makeLineBres()` in `gl.h`

2. Implement a `Polygon` class

3. Implement filling using `Scanline`, boundary cases taken into consideration, `Singularity Algo`, see `Polygon::fill()`

4. Implement translate, rotate, scale by algebra methods(faster than matrix operations and same in essence)

5. Implement clipping using methos similar to `Cohen-Sutherland Line Clipping`, see `Polygon::clip(const Rect& view)`

6. Caculating centroid using diving the polygons into triangles, caculating each of them and caculating as a total.

Step-by-step Instruction
========================
1.Edit input file`data.in` to specify polygons
----------------------------------
5

2
218.6 235.0
317.4 309.0

3
236.8 383.4
337.3 482.8
336.8 382.9

4
319.6 369.8
321.5 395.3
386.5 368.7
357.3 363.7

5
219.1 304.5
169.7 358.7
115.4 309.2
116.9 286.1
177.2 244.2

6
141.6 216.0
141.0 168.6
164.7 144.6
165.0 192.0
212.4 191.7
189.0 215.5

2.Run `proj1`, window would be like `input.png`
-----------------------------------------------
3.Keyboard or Mouse are both available
--------------------------------------
4.Press Keyboard in this sequence:
----------------------------------
`f ntkk nrjj nshh nrll x akk`

Explaination:open x10 mode to speed up,
translate the triangle by (0, 20),
rotate the quadralateral by (0.2rad),
scale the pentagon up down by 1 / (1.1 * 1.1),
rotate the quadralateral by (-0.2rad),
clip,
enlarge the size of view port by (0, 20),

5.This would generate the output image and output in `data.out`, and window would be like `output.png`
----------------------------------------------------------------------------------------------------
5

2
218.6 235.0
317.4 309.0

3
236.8 383.4
337.3 482.8
336.8 382.9

4
319.6 369.8
321.5 395.3
386.5 368.7
357.3 363.7

5
219.1 304.5
169.7 358.7
115.4 309.2
116.9 286.1
177.2 244.2

6
141.6 216.0
141.0 168.6
164.7 144.6
165.0 192.0
212.4 191.7
189.0 215.5

Interface Description
=====================

