PRESENTATION
============

My first try to make small physics engine in C++ using SMFL.

Current state : 

- drawing about 18 000 circles at 60 FPS with collisions and verlet physics (optimized with a grid, no multithreading)
- ability to change lots of parameters in the configuration.h file.
- possibility to show an image (formed by the circles) as the final state of the simulation since its deterministic, to do so first run one time the simulation until the end for a given configuration, then press 'S' to save the config on the disk. Next time you run the simultion, the image will be shown.
- ability to make physical objects using circles

# Controls :

<pre>
- Mouse mouvement and mousewheel --> move the view of the simulation
- Mouse left click               --> add a circle to the next object
- Mouse right click              --> spawn the object
- 'R'                            --> move back to default view
- 'S'                            --> save current state on the disk for a given config
- 'Space'                        --> Stop/Start flux of disks 

</pre>

HOW TO USE IT
=============

If you want to test it, you should download:  

* [CMake](https://cmake.org/)
* [git](https://git-scm.com/)

And then you can clone it in Visual Studio Community 2022. That's it ! :)
