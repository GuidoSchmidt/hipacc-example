# Software Language Engineering
## winter term 2014/15 : assignment 02

A simple image kernel example as an example for the [HIPAcc framework](http://www.hipacc-lang.org), which let you define image processing kernels in a domain specific language. The written code gets compiled into target specific GPGPU code ([CUDA](http://www.nvidia.de/object/cuda-parallel-computing-de.html) or [OpenCL](https://www.khronos.org/opencl/)).

The ```kernel()``` function of ```SimpleKernel``` implements a simple mean filter, which will blur the input image. The width of the kernel mask can be changed by changing ```filter_width```

## How-To

1. Download the [HIPAcc framework](http://www.hipacc-lang.org), build and install it ([github repo of HIPAcc](https://github.com/hipacc/hipacc)). Additional install instructions can be found on [hipacc-lang.org](http://www.hipacc-lang.org)
2. ```git clone https://github.com/GuidoSchmidt/hipacc-example.git```
3. ```cd hipacc-example```
4. ```make```
5. ```make run```



![Example output](./data/screenshot.png)


