### Results

Optimisation has been one of the main aspects we have worked on while developing this project. We wanted to deliver a fast, yet accurate simulation. We think we managed to obtain good results, with a full simulation and reconstruction of 1 million events being run in under 1:30 minutes. (CHECK TIME)

Following are the results of some simulations that we ran with different configurations. A comparison between them can be found at the end of the page

#### Run 1
Configuration: 
- N events: 1 million
- Multiplicity distribution: [Charged-particle multiplicity measurement in proton-proton collisions at sqrt(s) = 7 TeV with ALICE at LHC](https://www.hepdata.net/record/54832)
- Angular distribution: uniform
- $Z_{vertex}$ distribution: gaussian
- $\sigma_{x}=0.01$ cm, $\sigma_{y}=0.01$ cm, $\sigma_{z}=5.3$ cm
- Beam pipe radius: $3$ cm
- Detectors radii: $4$cm, $7$cm
- Mean noise: 10

##### Simulation

Firstly, this is a comparison between the multiplicity distribution probability and the (normalised) generated multiplicity distribution:
|![Multi_comparison](Figures/Run1/fTreeRec_Multi_comparison.png)| 
|:--:| 
|Comparison between the multiplicity distribution probability and the (normalised) generated multiplicity distribution| 

A good between the two distributions is observed.
In order to study the effects of the multiple scattering, it is possible to study the difference between the z coordinate distributions of the generated hit in the two layers and the distribution of the z coordinate of the vertex:

|![Z_generated](Figures/Run1/fTreeGen_Config.z.png)| 
|:--:| 
|Distribution of the generated z coordinate of the vertex. $\mathrm{RMS}=5.3$ cm| 

|![Z_comparison](Figures/Run1/fTreeGen_GenHits_comparison.png)| 
|:--:| 
|Comparison between the distributions of the hits' z coordinate for the first layer (in blue, $\mathrm{RMS}=6.7$ cm) and the second layer (in red, $\mathrm{RMS}=7.7$ cm)| 

To study how the smearing affects the reconstruction, it is possible to study the distributions of the reconstructed z coordinates of the hits on each detector's layer:

|![Z_comparison_reco](Figures/Run1/fTreeRec_RecHits_comparison.png)| 
|:--:| 
|Comparison between the distributions of the reconstructed (smeared) hits' z coordinate for the first layer (in blue, $\mathrm{RMS}=6.8$ cm) and the second layer (in red, $\mathrm{RMS}=7.4$ cm)| 
As expected, smearing does not have any effects on the shape of the distributions, nor does it affects the distributions widths as much as the multiple scattering does.

##### Reconstruction
After the simulation finishes, vertexes are reconstructed and the resolution and effeciency of the detector are evaluated as a function of the event multiplicity and of the event's vertex Z coordinate.

|![ResVsMult](Figures/Run1/ResolutionVsMultiplicity.png)| 
|:--:| 
|Detector's resolution as a function of multiplicity.| 

