# Veneris OMNET++ modules

These are the Veneris/Opal modules for the [OMNET++/INET framework](https://inet.omnetpp.org/). 
With them you can perform hybrid traffic-network simulations with [Veneris](https://gitlab.com/esteban.egea/veneris) 
and/or use the [Opal GPU-based ray-launching multipath propagation](https://gitlab.com/esteban.egea/opal). 

For more information visit the [Veneris website](http://pcacribia.upct.es/veneris)



## Installation
We have only tested the installation in Linux. Should be similar for Windows.

### Requirements
You need [OMNET++](https://omnetpp.org/) 5.4.1 and  [INET](https://inet.omnetpp.org/) 4.1.0. 
Tested also with [OMNET++](https://omnetpp.org/) 5.5.1 and [INET](https://inet.omnetpp.org/) 4.1.1. 
Use a recent gcc/g++ compiler.  I use g++ 7.4.0

For Opal, you need a modern NVIDIA GPU and updated driver.  CUDA 9.0 or later. Optix 5.1. gcc or Visual Studio.

**Updated to Optix 6.0**  It has been tested with the last Optix version, 6.0, and the performance on the same hardware has improved remarkably, even without using RTX cores. With Optix 6.0 use CUDA 10.0 and requires a NVIDIA driver at least 418.30. Follow exactly the same steps as below, but with Optix 6.0 and CUDA 10.0.

CMake 3.10, see the [Opal documentation](https://gitlab.com/esteban.egea/opal/README.md)

In addition, we are using Boost for the Veneris server
### Build Opal in Linux
Go to [Opal](https://gitlab.com/esteban.egea/opal) and follow instructions.
**Use the latest Opal release: 0.4.3 (at the moment)**

### Install OMNET++/INET
Install OMNET and then INET.

### Install boost
Install the asio boost libraries for your platform.

### Build Veneris OMNET++ modules in Linux
Once you  have installed INET, you should have an `inet` folder in yor workspace. Copy/create/import the folders in our `src` folder in the corresponding folders below the INET directory tree.

Since we are using Opal as an external library, OMNET++ needs to compile and link against it. In addition, you need to include boost. You have to configure this. 
There are several options.

Generate Makefile:
If you do not use the OMNET++ IDE, you can directly generate the Makefile as follows. 

Go to the `src` folder of inet and execute
```bash
opp_makemake --make-so -f --deep -o INET -O out -pINET -I. -I/home/eegea/optix/SDK/opal -I/home/eegea/optix/include -I/usr/local/cuda-10.0/include -L/home/eegea/optix/SDK/lib -L/home/eegea/optix/lib64 -L/usr/local/cuda-10.0/lib64  -L/usr/lib/x86_64-linux-gnu -lopal_s -lboost_system -lcudart -Xinet/applications/voipstream -Xinet/linklayer/ext -Xinet/transportlayer/tcp_lwip -Xinet/transportlayer/tcp_nsc
```
**changing the paths with your own ones**, obviously. You can notice that we are adding the opal and CUDA directories for compiling and adding the Optix, Opal and Boost libraries for linking. 

Now you should have generated the Makefile and  you can compile
```bash
make
```

Alternatively, if you are using the OMNET++ IDE you can include the libraries as follows
1. Open Project>Project features...
2. Go to C/C++>Paths and Symbols..
3. In the Includes tab add the Opal directory and Optix and Cuda `include` directories.
4. In the Libraries tab add the `libopal_s.so`, the `libcuddn.so`, `liboptix.so`, `liboptixu.so` and your `libboost_system...` and `libcudart.so` libraries.
5. In the Libraries Path tab add the `optix/SDK/lib` and the `optix/lib64` paths and additionally the `cuda-10.0/lib64` paths.

This should be all, but it does not seem to link properly sometimes. For some reason OMNET++ IDE refuses to update the above settings if you edit and add something else.

So, to make sure, you *additionally* can 
1. Open Project>Project features...
2. Go to OMNET++>Makemake...
3. Select src:makemake and, on the right, click Makemake Options..
4. In the Compile tab add the include directories as in 3 above (make sure it is relative to the `src` directory as specified, or full paths).
5. In the Link tab add `opal_s`, `cudart` and `boost_system`

the above steps should be equivalent to the generation of Makefile with `opp_makemake`. In fact, it will overwrite the Makefile. 
There are even more alternatives, such as using the `makefrag` in `src`. 

In summary, you have to be able to compile and link against Opal, Optix and Boost.

## Usage
Once you have compiled, you are ready to develop your own models. Check the OMNET++/INET documentation. 

In this repository you have a few tests that can be used to learn how to use it. They are in the `examples` folder. 

They use a simple node module that  we have added to a  `opaltest` folder in the `src`.

### Usage independent of Veneris
Opal can be used as a radio medium with INET independently of Veneris. Conversely, Veneris can be used without Opal, for example  with the usual stochastic propagation models included, such as Nakagami.

If you want to test Opal without Veneris you can run the `examples/opal` scenario. 

### Usage with Veneris
To use Veneris you need to execute a Veneris simulation with Unity to generate the messages for OMNET++. Veneris can send the messages over a network to the OMNET++ VenerisServer or serialize them to a file that can be *replayed*. This is the suggested way if you do not really need *bidirectionally coupled* simulations, which, by the way, are not tested fully yet, so they are not included here. 

You can test it by running the `examples/veneris` scenario. First, you have to run the Chicago scenario (or any other, actually) and make 
Veneris serialize the messages to a `.mes` file. Then add the path of your .mes file to the corresponding entry in `venerisValidation.ini` and run the simulation.


## Contributing
There are a lot of things that can be improved/optimized and a lot of features that can be added, and we are short of people working on it. Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


## License
[MIT](https://choosealicense.com/licenses/mit/)
