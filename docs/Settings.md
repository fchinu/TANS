### Settings description

| Setting               | Description                           |
| -----------           | -----------                           |
| OutFileName           | address of the output file                         |
| AutoFlushOpt          | option to pass to ROOT TTrees [SetAutoFlush](https://root.cern.ch/doc/master/classTTree.html#ad4c7c7d70caf5657104832bcfbd83a9f) function          |
| AutoSaveOpt           | option to pass to ROOT TTrees [SetAutoSave](https://root.cern.ch/doc/master/classTTree.html#a583aa74b14c7ac7147a7bb4efbd5fbc5) function         |
| NEvents               | number of events to be run            |

      
 



#### Multiplicity settings

| Setting               | Description                           |
| -----------           | -----------                           |
| MultType              | Multiplicity distribution (possible options: kConst, kCustom and kUniform)                        |
| MultConst             | Number of generated particles per event (only used if MultType is set to kConst)            |
| MultFile              | Address of the file containing the multiplicity distribution (only used if MultType is set to kConst)                 |
| MultHistoName         | Addres of histogram inside MultFile            |
| MultRange             | Only used if MultType is set to kUniform **or** kCustom, sets the multiplicity range of the events (set to null if one doesn't want to set a range for custom distribution)           |


#### Angular distribution settings

| Setting               | Description                           |
| -----------           | -----------                           |
| DistrType             | Angular distribution (possible options: kConst, kCustom and kUniform)             |
| DistrConst            | Vector containing direction of all the particles created in the simulation (only used if DistrType is set to kConst)              |
| DistrFile             | Address of the file containing the pseudorapidity distribution (only used if MultType is set to kCustom)                |
| DistrHisto            | Address of histogram inside DistrFile            |
| SaveEta               | boolean option to save $ \eta$ distribution of generated particles            |
| SavePhi               | boolean option to save $ \vartheta$ distribution of generated particles            |



#### Vertex dispersion settings

| Setting               | Description                           |
| -----------           | -----------                           |
| DistrTypeZ            | Z distribution (possible options: kGaus  and kUniform)             |
| RangeZ                | Used if MultType is set to both kUniform and kGaus, sets the Z range of the events (set to null if one doesn't want to set a range for gaussian distribution)             |
| SigmaZ              | Z dispersion for gaussian distribution            |
| SigmaX              | X dispersion for gaussian distribution            |
| SigmaY              | Y dispersion for gaussian distribution            |

#### Material budgets settings

| Setting               | Description                           |
| -----------           | -----------                           |
| IsDetector             | List for choosing whether the layer detects particles or not             |
| Radii             | List of radii of the material budgets           |
| Thickness              | List of thicknesses of the material budgets            |
| Lenght              | List of lenghts of the material budgets            |
| Material              | List of materials of the material budgets           |
| DoMultScattering             | Enables multiple scattering         |
| DoSmearing              | Enables smearing           |
| DoNoise             | Enables noise         |
| MeanNoise              | Mean number of noise per event      |


Verbose: Sets verbosity