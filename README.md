# CoRCalculator - Library to Compute Centres of Rotation for [Optimised Centres of Rotation Skinning][3]
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

[P. Bittner, JP Tauscher, S. Grogorick, M. Magnor. Evaluation of Optimised Centres of Rotation Skinning. In _Proc. International Conference on Computational Visual Media (CVM)_ 2019](https://graphics.tu-bs.de/publications/bittner2019evaluation)

[N.  Magnenat-Thalmann,  R.  Laperrire,  and  D. Thalmann. Joint-dependent local deformations for hand animation and object grasping. In _Proceedings on Graphics interface’88. Citeseer_, 1988](1)

[L. Kavan, S. Collins, J.Žára, and C. O'Sullivan. Skinning with dual quaternions. In _Proceedings of the 2007 symposium on Interactive 3D graphics and games, pages 39–46.ACM_, 2007](2)

[B. H. Le and J. K. Hodgins. Real-time skeletal skinning with optimized centers of rotation. _ACM Transactions on Graphics (TOG)_, 35(4):37, 2016](3)

Please cite as:

    @inproceedings{bittner2019evaluation,
      title = {Evaluation of Optimised Centres of Rotation Skinning},
      author = {Bittner, Paul Maximilian and Tauscher, Jan-Philipp and Grogorick, Steve and Magnor, Marcus},
      booktitle = {Proc. International Conference on Computational Visual Media ({CVM})},
      year = {2019}
    }

### License

Copyright (c) 2019 Paul Maximilian Bittner, Technische Universität Braunschweig

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

You understand and agree that the authors are under no obligation to provide
either maintenance services, update services, notices of latent defects, or
corrections of defects with regard to the Software. The authors nevertheless
reserve the right to update, modify, or discontinue the Software at any time.

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software. You agree to cite the papers
"Evaluation of Optimised Centres of Rotation Skinning",
"Joint-dependent local deformations for hand animation and object grasping",
"Skinning with dual quaternions"
and "Real-time skeletal skinning with optimized centers of rotation"
in documents and papers that
report on research using this Software.

[1]: http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.14.9310
[2]: https://dl.acm.org/citation.cfm?id=1230107
[3]: https://dl.acm.org/citation.cfm?id=2925959
[4]: https://glm.g-truc.net/0.9.9/index.html