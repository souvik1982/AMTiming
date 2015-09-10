# AMTiming -- A software tool for modeling the CMS L1 Track Trigger Hardware

<img width="30%" src="http://home.fnal.gov/~souvik/Work/AMTiming/plots_PARALLEL2AM_Neutrino_PU140_tt27_sf1_nz4_pt3_ml5_20150511/c_tf2.png"/>
<img width="30%" src="http://home.fnal.gov/~souvik/Work/AMTiming/plots_PARALLEL2AM_Neutrino_PU140_tt27_sf1_nz4_pt3_ml5_20150511/c_BlockDiagram_0.999.png"/>

The aim of this project is to model the hardware of the Associative Memory L1 Track Trigger in sufficient detail to estimate the latency of the entire system when in operation during LHC Phase 2 at CMS. Central to the project is the development of a software package that models the hardware at the appropriate level of detail and is informed by algorithmic simulations of the AM pattern finding, track fitting and other components. The software package will also help us identify bottlenecks in latency caused by slow components or expected large fluxes of data, which can be solved by parallelization of these components.

The software tool has to be developed in conjunction with hardware of a Demonstrator system so as to minimize speculation on what parameters can and cannot be tuned in hardware. For example, it is not clear whether the Hit Buffer can have different operational frequencies for accepting patterns from the AM, for accepting stubs from the Stub Mapper, and for outputting patterns to the Combination Builder, till we begin to build some prototype of the component itself. Trying to develop this tool without input from the Demonstrator hardware can quickly divorce it from reality and may require re-organizing the software’s basic components at a later stage.

## Building the software package

* A configuration file format has to be developed that can describe one Demonstrator chain, a single crate containing multiple PRMs, and the entire L1TT system, and yet me easily readable in all cases.
* The package should be able to handle parallelization of the AM, the Hit Buffer, the Combination Builder and the Track Fitter.
* The package should be able to point out which data streams create the most delays in each component.
* There has to be close coordination and involvement with the hardware developers of the StubMapper, the AM (VIPRAM00), the Hit Buffer, the Combination Builder and the Track Fitter to understand if the level of hardware characterization is sufficient.

## Questions to be answered with the package

* What is the latency for an empty event to transit the entire system?
* What is the latency for a minimum bias event with 140 PU? 
* What is the latency for a tttt+140 PU event?
* How does the latency depend on PU? Can we extrapolate to 200 PU?
* How many AMs are required to bring the latency of the PRM down to 4 μs?
* Do the HB, CB, TF also need to be parallelized? Or can we save hardware resources space by not parallelizing them?
* What are the aforementioned latencies of the system for de-scoped versions of the tracker?
