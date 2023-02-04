# TANS PROJECT
This is the documentation page for the TANS project made by Fabrizio Chinu and Marcello Di Costanzo

## Introduction

## Experimental setup description
In our experiment the particle beam travels along a $z$ axis and we have $3$ cylindrical layers with $z$ axis corresponding to the trajectory of particles and radii $r_1,\,r_2,\,r_3$ with $r_1<r_2<r_3$. Geometric features and materials of the $3$ layers can be setted in the configuration file, here we discuss the case in which 
-   cylinder with $r=r_1=3\, cm$ has a thickness $t=0,8\,mm$. It is a beam pipe made of Berillium which does not interact with the material  
-   cylinder with $r=r_2=4\, cm$ has a thickness $t=0,2\,mm$. It is a silicon detector  which interacts with the material  
-   cylinder with $r=r_2=7\, cm$ has a thickness $t=0,2\,mm$. It is a silicon detector which interacts with the material  

IMMAGINE SETUP

`this is formatted as code`

For multiple lines of code use triple backtick

```
gSystem->CompileMacro("Simulation/Particle.cpp",opt.Data());
gSystem->CompileMacro("Simulation/MaterialBudget.cpp",opt.Data());
gSystem->CompileMacro("Simulation/Detector.cpp",opt.Data());
```
For quoting use >
> Ciao!

To add an image use
![This is an image](https://myoctocat.com/assets/images/base-octocat.svg)

For more info visit [this page](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax)



## Reconstruction
The second part of the program is about vertex reconstruction for simulated events and evaluation of the experimental setup efficiency and resolution. We leave a brief description of the key steps implemented by the program to achieve these goals:

- **Tracklet reconstruction:** we consider tracklets all pairs of intersections with $1$st and $2$nd detector whose angle difference $\varphi = |\varphi_1-\varphi_2| < \varphi_{max}$ with $\varphi_{max}$ to be setted in the configuration file.  

- **Tracklet vertex reconstruction:** for each reconstructed tracklet, vertex is reconstructed as the intersection between the track and a plane orthogonal to the track in the $xy$ plane passing through the $z$ axis. 

- **Event vertex reconstruction:** for each event, $z$ vertex coordinates of all tracklets are collected in an histogram. Then, the program considers the range of values defined by lower and upper edge of the bin with highest frequency: the event vertex is estimated averaging vertexes with value of $z$ in this range. This ensures combinatorial tracklets do not contribute to the estimated value.

- **Setup performance evaluation:** after processing all events of the run, the program saves the following plots in an output file
    + Residuals
    + Efficiency vs Multiplicity
    + Resolution vs Multiplicity
    + Efficiency vs true coordinate $z$ of vertexes
    + Resolution vs true coordinate $z$ of vertexes
