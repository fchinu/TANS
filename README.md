# Simulation of proton-proton collisions
High energy collisions, such as those that occur in particle accelerators, can produce a wide range of fascinating phenomena, including the creation of new particles and the study of fundamental physical interactions. In this project, we developed a simulation tool to study these collisions and how it is possible to use vertex detectors to reconstruct the primary vertex position. The simulation was built using Monte Carlo techniques and only considered high momentum particles moving in a low-intensity magnetic field, i.e. the particles have a linear trajectory.

### Run full simulation and analysis
After checking all [prerequisites](#prerequisites) are satisfied, you can run the full simulation by executing the following shell command:
```
root -l Run_All.cpp+
```
Congratulations! You just run your first simulation, you can check the results obtained by looking at the `outfile.root` file.

Did not work? Check the results we obtained [here](Results.md)

### Prerequisites
The code in this repository requires to have installed:
- [ROOT](https://root.cern/)

# Documentation
The documentation for the project can be found here:
[https://bizzzio.github.io/TANS/](https://bizzzio.github.io/TANS/)