direction for use:
(1) initialization
1. Set the number of iterations. In platform.cpp file, it is controlled by the epoch parameter.
2. Setting up the optimizer. The default is ADAM, gradient descent is also integrated into the platform, Setting through lines 354-357 of platform.cpp file.
3. Set ratio of the test set and training. In line 161 of platform.cpp file.
(2) compile
This platform does not require the installation of other third-party libraries.
Compiler version: gcc version 7.5.0
Compilation command of call the full amplitude simulator: g++  -I ./  calculategk_FullAmplitude.cpp platform.cpp simulator_FA.cpp -O3 -o plt
Compilation command of call the tensor network simulator: g++  -I ./ -I include/ calculategk_TensorNetwork.cpp platform.cpp simulator_DMTN.cpp -lpthread -O3 -o plt
(3) running
Examples of running command: ./plt network_2.data moon.data
plt is the executable file after compile.
network_2.data is model file, write specification reference LFQAP: A lightweight and flexible quantum artificial intelligence application platform.
moon.data is the sample file, each sample occupies a row, sample features are separated by spaces, at the end is label.
(4) Self-defined functionality
1. Adding new optimizer. This platform integrates two categories of optimizer: ADAM and gradient descent, the user can add one through lines 354-357 in platform.cpp file.
2. Add a new executor. This may be relatively complicated, we add compiler module, when a new executor was added, we need modify or add the compiler file, translate our instruction set to the target instruction, the compiler module in calculategk file. When a new executor added, we usually add a corresponding compiler file, such as calculategk_FullAmplitude.cpp and calculategk_TensorNetwork.cpp.
3. Other self-defined functionality. We can also easily calculate other parameters based on quantum states, such as entropy and mutual information.

Announcements:
This project is currently only available for two or three classification, more categories need to be modified in the file platform.cpp.
