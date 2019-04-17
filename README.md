# CoRCalculator - Library for centres of rotation computation for [Optimised Centres of Rotation Skinning][1]
An example on how to use the library is given in `example.cpp`.

The CMake file is tested on Linux and Windows.

When compiled with the flag `COR_ENABLE_PROFILING`, the `CoRCalculator` will print the times it has needed for each step.

The library has to be linked with the option `-pthread`, as it uses multithreading ([std::async](http://www.cplusplus.com/reference/future/async/)).

### Dependencies
This library solely depends on the head-only library [`glm`](https://glm.g-truc.net/0.9.9/index.html).
On Linux, installing `libglm-dev` with `apt` suffices.
On Windows, finding GLM may require some changes, as we do it manually.

### Shader
We provide GLSL vertex shader logic for these three skeletal skinning techniques:

* **[Linear Blend Skinning (LBS)](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.14.9310)**
* **[Dual Quaternion Skinning (DQS)](https://dl.acm.org/citation.cfm?id=1230107)**
* **[Optimised Centres of Rotation Skinning (CRS)][1]**.

The file `skeletons.glsl` contains the `perform_skinning` function, that computes one of these three methods, depending on a uniform variable.
The files `quaternion.glsl` and `dualquaternion.glsl` each contain utility functions for their respective mathematics and have to be compiled
together with the vertex shader because they are required by `skeletons.glsl`.
We split these files so that you may pick what you need, since the `skeletons.glsl` file has to be customized anyway (especially the layout locations of the attributes).

[1]: https://dl.acm.org/citation.cfm?id=2925959

### Citation
This repository contains code corresponding to:

[P. Bittner, JP Tauscher, S. Grogorick, M. Magnor. Evaluation of Optimised Centres of Rotation Skinning. In _Proc. International Conference on Computational Visual Media (CVM)_ 2019](https://graphics.tu-bs.de/publications/bittner2019evaluation)

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
copies or substantial portions of the Software. You agree to cite the Evaluation
of Optimised Centres of Rotation Skinning paper in documents and papers that
report on research using this Software.