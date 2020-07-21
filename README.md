# A raytracer built in C++ (guided by [Ray Tracing in One Weekend](https://raytracing.github.io/))


### What does this code do?
Produces these images:

![Book Cover](images/book_cover.png "Book Cover")

> The image created alongside the book

![Imported Gem Mesh](images/gem.png "Imported Gem Mesh")

> The image created with my custom mesh import code


### [What is raytracing?](https://en.wikipedia.org/wiki/Ray_tracing_(graphics))
> In computer graphics, ray tracing is a rendering technique for generating an image by tracing the path of light as pixels in an image plane and simulating the effects of its encounters with virtual objects.


### What is this for?
This is a bite-sized project I worked on to potentially serve as a talking point in interviews as well as provide a concrete code sample. I followed along with the book [Ray Tracing in One Weekend](https://raytracing.github.io/), and although I understood and typed the code myself, guide rails were provided by the book for some of the project.


### What is unique in this project versus the project in the book [Ray Tracing in One Weekend](https://raytracing.github.io/)?
I implemented two main features on top of the raytracer built with the book:
1. Multi-threaded the program for 4x speedup in most cases
2. Custom mesh import (in this case, I 3D-modeled a gem)

Feature 1. is a fairly simple implementation, an allows for a linear speedup for how many CPU cores you throw at it.

Feature 2. is much more involved, and serves as my main talking point when discussing this project. You can read more about this below.


### What is feature 2. (custom mesh import), and why is it special?
Rendering non-simple shapes can be a tricky process. When a 3D-model is exported, it is constructed by a series of vertices, that are grouped into triangles defined in 3D space. The overhead of raytracing a 3D-model can be quite high. Rendering an entire sphere can be thought of as roughly equivalent processing wise to a single triangle. To achieve good looking 3D models, many triangles are required. In modern day video games, a character model can consist of 10,000+ triangles.

What I implemented is a feature that reads a .obj file (currently hard-coded to read gem.obj), interprets the file by constructing triangles in the scene, then renders the output. The two (completed) components here are import_mesh/read_mesh.h and import_mesh/triangle.h. As named, read_mesh.h handles the .obj scan process, and adds triangles as defined by triangle.h into the scene. Inside triangle.h, is an algorithm that (with Linear Algebra) handles collision between a ray that is casted, and the triangle.

Inside of import_mesh/octree.h, I have a mostly psuedo-code implementation of a data structure that will greatly speed up the program when finished.

The next major way to improve performance is to speed up the ray-object intersection. By default, for every light ray, we linearly-search the list of all objects and test for an intersection. The author explores speeding this up using a BVH or Bounding Volume Hierarchy approach to reduce the search time. First, the scene is encapsulated by one large box, and collision is checked there first in case of a total miss.

The scene (or rather now, the box) is split up into two equal size boxes. A ray can then check collision across those two to determine
which half of the objects to search across. The idea is to implement a Binary Search to limit the size of the list to then linearly search.

This idea can be expanded much, much further with use of an Octree data structure. We can subdivide the scene into 8 equal boxes, or octants,
and further subdivide those boxes into octants until we have a good level of granularity. Then, the process for search, is searching across this tree
first for which boxes the ray intersects with, then, at the bottom of the tree, we can test collision across all of the objects contained inside that box.

In terms of performance improvement, consider a scene with 128 objects evenly distributed, and an BVH & Octree each with two subdivisions: 
|                | Average (Success) Search     | Worst-Case (Failure) Search |
|----------------|------------------------------|-----------------------------|
| Linear Search: | 128/2 = 64                   | 128                         |
| BVH Search:    | 1 + 1.5 + 1.5 + (128/8) = 20 | 1                           |
| Octree Search: | 1 + 4 + 4 + (128/128) = 10   | 1                           |

Now let's consider a scene with 512 objects, and we add two layers to BVH (to give it a fighting chance): 
|                           | Average (Success) Search    | Worst-Case (Failure) Search |
|---------------------------|-----------------------------|-----------------------------|
| Linear Search:            | 512/2 = 256                 | 512                         |
| BVH Search (4 layers):    | 1 + (1.5*4) + (512/32) = 23 | 1                           |
| Octree Search (2 layers): | 1 + 4 + 4 + (512/128) = 13  | 1                           |


[Definitely check out the Wikipedia page on Octrees!](https://en.wikipedia.org/wiki/Octree)


### What code should I review?
The code in these files is fully written by myself and should be referenced when considering my problem solving ability/technique:
TODO: Link files


## Quickstart Guide
Currently, you have to compile yourself the C++ code. Then, output the logfile into a .ppm image like so:

main.exe > image.ppm

You can also use console commands, the order is (resolution width, number of rays, number of bounces, and number of threads):

main.exe 480 60 24 4 > image.ppm

I reccomend File Viewer Plus for viewing the .ppm image.


TODO: Finish
There are 2 ways to run the program. First, if you are on Windows you can try the TODO: [.exe release](https://www.google.com). There are a few extra steps here, so be sure to read the section below. Second, you can compile the code yourself. If you are on Linux, I assume you can figure this out (the Compile on Windows section could give you some pointers if you get stuck). Finally, I have also written a guide to compiling on Windows.

### The .exe release (Windows)
TODO: Write section

### Compiling on Windows
TODO: Write section

