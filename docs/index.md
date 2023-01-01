# Simulation of pp collisions
High energy collisions, such as those that occur in particle accelerators, can produce a wide range of fascinating phenomena, including the creation of new particles and the study of fundamental physical interactions. In this project, we developed a simulation tool to study these collisions and how it is possible to use vertex detectors to reconstruct the primary vertex position. The simulation was built using Monte Carlo techniques and only considered high momentum particles moving in a low-intensity magnetic field, i.e. the particles have a linear trajectory.

### Prerequisites
The code in this repository requires to have installed:
- [ROOT](https://root.cern/)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
    - Ubuntu: it can be installed through 
        ```
        sudo apt update 
        sudo apt install libyaml-cpp-dev
        ```
    - MacOs: 
        - via MacPorts: `sudo port install yaml-cpp`
        - via Homebrew: `brew install yaml-cpp`
    - For other OS follow instructions in the [yaml-cpp official github page](https://github.com/jbeder/yaml-cpp)

### Running the simulation
To compile the simulation, it is sufficient to run ROOT and execute the following command:
```
.x Compile_Simulation.cpp+
```
To run the simulation launch the simulation function:
```
Simulation()
```

It is possible to add some parameters to the Complile_Simulation.cpp file to activate different options:
- `.x Compile_Simulation.cpp+("force")` to force the compilation of the different files
- `.x Compile_Simulation.cpp+("clean")` to delete all the compilation outputs coming from previous compilations before compiling
- `.x Compile_Simulation.cpp+("run")` to run the simulation after the compilation ends

It is possible to combine these options: for example, `.x Compile_Simulation.cpp+("cleanrun")` deletes all the compilation outputs coming from previous compilations, then runs the simulation after compilation

### Configuration of the simulation
It is possible to configure a simulation using a .yaml file, following these steps:

- Open the .yaml file in a text editor or configuration tool.
- Review the available parameters and their descriptions.
- Modify the values of the parameters as desired to configure the simulation.
- Save the .yaml file once you have finished making your changes.

[Here](https://github.com/Bizzzio/TANS/blob/main/Config_Run.yaml) CONTROLLARE LINK DOPO PULL REQUEST is an example of settings used to run a simulation.

The config file can then be passed to the simulation function: `Simulation("Config_file.yaml")`
> Be careful: only the parameters included in the example linked above are defined!


To add an image use
![This is an image](https://myoctocat.com/assets/images/base-octocat.svg)

For more info visit [this page](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)
