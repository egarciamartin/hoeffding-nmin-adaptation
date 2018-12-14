# Hoeffding Trees with nmin adaptation
Eva García-Martín(1), Niklas Lavesson(1,2), Håkan Grahn(1), Emiliano Casalicchio(1,3), and Veselka Boeva(1)    
 (1) Blekinge Institute of Technology, Karlskrona, Sweden  
 (2) Jönköping University, Jönköping, Sweden  
 (3) Sapienza University of Rome, Rome, Italy  

Paper accepted at 2018 IEEE International Conference on Data Science and Advanced Analytics (DSAA) (*to appear*)  

Extension of this paper titled "Energy Aadaptive Very Fast Decision Tree" submitted to "International Journal of Data Science and Analytics (JDSA)", experiments in folder: code/experiments/JDSA_18

## Folders
Code:    code/VFDT-nmin/original/src/learners/avfdt  
Experiments: 
* DSAA: code/experiments/DSAA18/{run.sh,create_plots.ipynb,create_tables.ipynb}
* JDSA: code/experiments/JDSA_18/  

## Experiments DSAA:
VFDT, CVFDT, VFDT-nmin comparison
15 datasets  
Run: 10 times and averaged the results  
Energy measurement: Intel Power Gadget https://software.intel.com/en-us/articles/intel-power-gadget-20

Script that runs the experiments: code/experiments/DSAA18/run.sh

## Experiments JDSA:
VFDT, CVFDT, VFDT-nmin comparison  
29 datasets  
Run: 5 times and averaged the results   
Energy measurement: Intel Power Gadget https://software.intel.com/en-us/articles/intel-power-gadget-20

Script that runs the experiments: code/experiments/JDSA_18/{run_baseline.sh,run_drift.sh,run_real.sh}


### Cite as:
```
@inproceedings{garcia2018hoeffding,  
  title={Hoeffding Trees with nmin adaptation},  
  author={Garcia-Martin, Eva and Lavesson, Niklas and Grahn, H{\aa}kan and Casalicchio, Emiliano and Boeva,Veselka},  
  booktitle={Data Science and Advanced Analytics (DSAA), 2018 IEEE International Conference on},  
  year={2018},  
  organization={IEEE}  
}
```
