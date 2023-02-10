### Vertex reconstruction

The code available in this repository implements vertex reconstruction for simulated events in a vertex detector, evaluating its efficiency and resolution.

### Running the reconstruction
To compile the reconstruction, it is sufficient to run ROOT and execute the following command:
```
.x Compile_Reconstruction.cpp+
```
To run the reconstruction execute the analysis function:
```
RunAnalysis()
```

It is possible to add some arguments to the Compile_Reconstruction.cpp file to activate different options:
- `.x Compile_Reconstruction.cpp+("force")` to force the compilation of the different files
- `.x Compile_Reconstruction.cpp+("clean")` to delete all the compilation outputs coming from previous compilations before compiling

### Configuration of the reconstruction
It is possible to configure a reconstruction using a .yaml file, following these steps:

- Open the .yaml file in a text editor or configuration tool.
- Review the available parameters and their descriptions.
- Modify the values of the parameters as desired to configure the reconstruction.
- Save the .yaml file once you have finished making your changes.

The config file can then be passed to the reconstruction function: `RunAnalysis("Config_file.yaml")`

[Here](https://github.com/Bizzzio/TANS/blob/main/Config_Reconstruction.yaml) is an example of settings used to run a reconstruction.

A description of the settings is available [here](Settings.md).

### Program implementation
Here some details on how the program implements the key steps to implement vertex reconstruction as well as efficiency and resolution evaluation:

- **Tracklet reconstruction:** we consider tracklets all pairs of intersections with first and second detector whose angle difference $ \varphi = |\varphi_1-\varphi_2| < \varphi_{max} $ with $ \varphi_{max} $ to be setted in the configuration file.  

- **Tracklet vertex reconstruction:** for each reconstructed tracklet, vertex is reconstructed as the intersection between the track and a plane orthogonal to the track in the $xy$ plane passing through the $ z $ axis. 

- **Event vertex reconstruction:** for each event, $ z $ vertex coordinates of all tracklets are collected in an histogram. Then, the program considers the range of values defined by lower and upper edge of the bin with highest frequency: the event vertex is estimated averaging vertexes with value of $ z $ in this range. This ensures combinatorial tracklets do not contribute to the estimated value.

- **Setup performance evaluation:** after processing all events of the run, the program saves the following plots in an output file
    + Residuals
    + Efficiency vs Multiplicity
    + Resolution vs Multiplicity
    + Efficiency vs true coordinate $ z $ of vertexes
    + Resolution vs true coordinate $ z $ of vertexes
