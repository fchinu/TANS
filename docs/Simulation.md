### Simulation

The code available in this repository runs a Monte Carlo (MC) simulation of a vertex detector, i.e. a detector that reconstructs the vertex position. The simulation consists of three parts:
- Firstly, the vertex is generated, together with all the particles produced in the collision
- The particles are then transported; they scatter through the beam pipe and the detector's layers and are detected by the detector
- Lastly, the hits on the detector are smeared and noise is simulated

### Running the simulation
To compile the simulation, it is sufficient to run ROOT and execute the following command:
```
.x Compile_Simulation.cpp+
```
To run the simulation execute the simulation function:
```
Simulation()
```

It is possible to add some arguments to the Complile_Simulation.cpp file to activate different options:
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

A description of the settings is available [here](Settings.md#settings-description).

The config file can then be passed to the simulation function: `Simulation("Config_file.yaml")`
> Be careful: only the parameters included in the example linked above are defined!