# JDSA 2018
# Experiments for paper: Hoeffding Trees with nmin adaptation

## Folders

VFDT-nmin code: ../code/VFDT-nmin/original/src/learners/avfdt
Datasets: experiments/datasets
Experiments: experiments/

# Experiments 
We have divided the experiments into: baselines, concept-drift, and real. 
We have created 3 different scripts to run them, for simple understanding and not creating too complicated scripts.
We have also created analyze.sh scripts, that analyze the results, creates the tables, and creates the necessary plots

## Plots

## Experimental design:
VFDT, CVFDT, VFDT-nmin comparison
We want to test the algorithms in three different scenarios. 
### Study of the number of instances, A_f, A_i. To do that, we use the random tree generator varying these three parameters.

**Varying the number of instances:**

| # Instances | #Ai | #Af | #Classes |
|-------------|-----|-----|----------|
| 1M          | 10  | 10  | 2        |
| 10k         | 10  | 10  | 2        |
| 100k        | 10  | 10  | 2        |
| 10M         | 10  | 10  | 2        |

**Number of Ai**

| # Instances | #Ai | #Af | #Classes |
|-------------|-----|-----|----------|
| 1M          | 10  | 0   | 2        |
| 1M          | 20  | 0   | 2        |
| 1M          | 30  | 0   | 2        |
| 1M          | 40  | 0   | 2        |
| 1M          | 50  | 0   | 2        |

**Number of Af**

| # Instances | #Ai | #Af | #Classes |
|-------------|-----|-----|----------|
| 1M          | 0   | 10  | 2        |
| 1M          | 0   | 20  | 2        |
| 1M          | 0   | 30  | 2        |
| 1M          | 0   | 40  | 2        |
| 1M          | 0   | 50  | 2        |

## Study of the same datasets with and without drift
We create the dataset without drift, and the same one with gradual or abrupt drift, based on the paper: Adaptive random forests for evolving data stream classification

* LED
* Random RBF
* Waveform

## Real world datasets

* Forest
* Poker
* Electricity
* Airline
* Instrusion detection
* KDD99



Total of 29 datasets.
Run: 5 times and averaged the results
Energy measurement: Intel Power Gadget https://software.intel.com/en-us/articles/intel-power-gadget-20

