[![Thesis](https://img.shields.io/badge/Bachelor's_Thesis-Read-purple)][thesis]
[![License](https://img.shields.io/badge/License-MIT-blue)](LICENSE.MIT)

# CoRCalculator - Library to Compute Centres of Rotation for [Optimised Centres of Rotation Skinning][3]

This is a small C++ library and re-implementation to compute optimized centres of rotation for skinning 3D graphical models with the skinning method proposed by _Le and Hodgins_ in 2016 in their paper [_Real-time Skeletal Skinning with Optimized Centers of Rotation_][3].
Moreover, the library contains GLSL shaders for optimized centres of rotation skinning as well as for the classical [linear blend skinning][1] and [dual-quaternion skinning][2].
Le and Hodgins present their method in [this video](https://www.youtube.com/watch?v=DfIfcQiC2oA).

### Instructions
An example on how to use the library is given in `example.cpp`.

The CMake file is tested on Linux and Windows.

When compiled with the flag `COR_ENABLE_PROFILING`, the `CoRCalculator` will print the times it has needed for each step.

The library has to be linked with the option `-pthread`, as it uses multithreading ([std::async](http://www.cplusplus.com/reference/future/async/)).

### Dependencies
This library solely depends on the header-only library [`glm`][4].
On Linux, installing `libglm-dev` with `apt` suffices.
On Windows, finding [`glm`][4] may require some changes, as we do it manually.

### Shader
We provide GLSL vertex shader logic for these three skeletal skinning techniques:

* **[Linear Blend Skinning (LBS)][1]**
* **[Dual Quaternion Skinning (DQS)][2]**
* **[Optimised Centres of Rotation Skinning (CRS)][3]**.

The file `skeletons.glsl` contains the `perform_skinning` function, that computes one of these three methods, depending on a uniform variable.
The files `quaternion.glsl` and `dualquaternion.glsl` each contain utility functions for their respective mathematics and have to be compiled
together with the vertex shader because they are required by `skeletons.glsl`.
We split these files so that you may pick what you need, since the `skeletons.glsl` file has to be customized anyway (especially the layout locations of the attributes).


### Citation
This repository contains code corresponding to:

[P. Bittner, JP Tauscher, S. Grogorick, M. Magnor. Evaluation of Optimised Centres of Rotation Skinning. _Poster @ International Conference on Computational Visual Media (CVM)_ 2019][thesis]

[N.  Magnenat-Thalmann,  R.  Laperrire,  and  D. Thalmann. Joint-dependent local deformations for hand animation and object grasping. In _Proceedings on Graphics interface’88. Citeseer_, 1988][1]

[L. Kavan, S. Collins, J.Žára, and C. O'Sullivan. Skinning with dual quaternions. In _Proceedings of the 2007 symposium on Interactive 3D graphics and games, pages 39–46.ACM_, 2007][2]

[B. H. Le and J. K. Hodgins. Real-time skeletal skinning with optimized centers of rotation. _ACM Transactions on Graphics (TOG)_, 35(4):37, 2016][3]

Please cite as:

    @misc{bittner2019evaluation,
      title = {Evaluation of Optimised Centres of Rotation Skinning},
      author = {Bittner, Paul Maximilian and Tauscher, Jan-Philipp and Grogorick, Steve and Magnor, Marcus},
      howpublished = {Poster @ International Conference on Computational Visual Media 2019},
      month = {Apr},
      year = {2019}
    }

[1]: http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.14.9310
[2]: https://dl.acm.org/citation.cfm?id=1230107
[3]: https://dl.acm.org/citation.cfm?id=2925959
[4]: https://glm.g-truc.net/0.9.9/index.html
[thesis]: https://graphics.tu-bs.de/publications/bittner2019evaluation