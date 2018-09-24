# CoRCalculator - Library for centres of rotation computation for [Optimised Centres of Rotation Skinning][1]

An example on how to use the library is given in `example.cpp`.


### Shader
We provide GLSL vertex shader logic for these three skeletal skinning techniques:

* **[Linear Blend Skinning (LBS)](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.14.9310)**
* **[Dual Quaternion Skinning (DQS)](https://dl.acm.org/citation.cfm?id=1230107)**
* **[Optimised Centres of Rotation Skinning (CRS)][1]**.

The file `skeletons.glsl` contains the `perform_skinning` function, that computes one of these three methods, depending on a uniform variable.
The files `quaternion.glsl` and `dualquaternion.glsl` each contain utility function for their respective mathematics and have to be compiled
together with the vertex shader, since because are required by `skeletons.glsl`.
We split these files so that you may pick what you need, since the `skeletons.glsl` file has to be customized anyway (especially the layout locations of the attributes).

[1]: https://www.disneyresearch.com/publication/skinning-with-optimized-cors/