#!/bin/bash

# JDSA. 
# 1. Baline experiments

# | # Instances | #Ai | #Af | #Classes |
# |-------------|-----|-----|----------|
# | 1M          | 10  | 10  | 2        |
# | 10k         | 10  | 10  | 2        |
# | 100k        | 10  | 10  | 2        |
# | 10M         | 10  | 10  | 2        |

# **Number of Ai**

# | # Instances | #Ai | #Af | #Classes |
# |-------------|-----|-----|----------|
# | 1M          | 10  | 0   | 2        |
# | 1M          | 20  | 0   | 2        |
# | 1M          | 30  | 0   | 2        |
# | 1M          | 40  | 0   | 2        |
# | 1M          | 50  | 0   | 2        |

# **Number of Af**

# | # Instances | #Ai | #Af | #Classes |
# |-------------|-----|-----|----------|
# | 1M          | 0   | 10  | 2        |
# | 1M          | 0   | 20  | 2        |
# | 1M          | 0   | 30  | 2        |
# | 1M          | 0   | 40  | 2        |
# | 1M          | 0   | 50  | 2        | 

# Goal: Measure energy consumption using Intel Power Gadget, and compare the consumption for different scenarios
# Algorithms: Very Fast Decision Tree (VFDT), VFDT-nmin (VFDT with nmin adaptation), CVFDT (concept drift VFDT)
# Datasets: Random Tree generator with different # of instances, Ai and Af (shown in the table above)
# Package: VFML original authors
# Note: AVFDT is the VFDT-nmin algorithm. 

# Paths
vfdt_nmin="../code/VFDT-nmin/original/src/learners/avfdt/avfdt"
vfdt="../code/VFDT-nmin/original/src/learners/vfdt/vfdt"
cvfdt="../code/VFDT-nmin/original/src/learners/cvfdt/cvfdt"
path_dataset="datasets/baseline/"

# Input variables
datasets=("RT_1M_10_10" "RT_10K_10_10" "RT_100K_10_10" "RT_10M_10_10"\
			"RT_1M_10_0" "RT_1M_20_0" "RT_1M_30_0" "RT_1M_40_0" "RT_1M_50_0"\
			"RT_1M_0_10" "RT_1M_0_20" "RT_1M_0_30" "RT_1M_0_40" "RT_1M_0_50")

# datasets=("RT_1M_0_30")
algorithms=($vfdt $vfdt_nmin $cvfdt)
# algorithms=($vfdt)
runs=5

# Output a .csv table with the setup, energy, and accuracy results. 
echo "Algorithm,Dataset,Accuracy (%),Total Energy(J),Proc Energy (J),DRAM Energy(J)" > results/results_baseline.csv
echo "Iter,Algorithm,Dataset,Accuracy (%),Total Energy(J),Proc Energy (J),DRAM Energy(J)" > results/results_avg_baseline.csv

for (( i=1;i<=$runs;i+=1));
do
	echo "ITER: "$i
	for algorithm in "${algorithms[@]}";
	do
		for dataset in "${datasets[@]}";
		do
			alg_name=`echo $algorithm | awk -F '/' '{print $8}'`
			echo "(info): Running ["$alg_name"] algorithm  ["$dataset"] Dataset"

			# /Applications/Intel\ Power\ Gadget/PowerLog -file ../results/engy_${algorithm}_$dataset.csv -resolution 100 -cmd python experiment_HT.py $algorithm $dataset
			/Applications/Intel\ Power\ Gadget/PowerLog -file results/raw/engy_${alg_name}_$dataset.csv \
			    -resolution 100 -cmd ${algorithm} -source $path_dataset/${dataset} -f ${dataset} -u \
			    > results/raw/acc_${alg_name}_${dataset}.csv
			# /Applications/Intel\ Power\ Gadget/PowerLog -file test.csv -resolution 100 -cmd ${algorithm} -source $path_dataset/${dataset} -f ${dataset} -u 

			# # Parsing the results
			echo "(info): Parsing ["$alg_name"] algorithm  ["$dataset"] Dataset"
			acc=`tail -6 results/raw/acc_${alg_name}_${dataset}.csv | sed -n 1p | awk '{print $1}'`
			acc=`echo "100-$acc" | bc`

			# sed to match the line that contains the pattern ^.../. The line has to start with tthat. 
			total_proc_engy=`sed -n '/^Cumulative Processor Energy_0 (Joules)/p' < results/raw/engy_${alg_name}_$dataset.csv | awk '{print $6}'`
			total_DRAM_engy=`sed -n '/^Cumulative DRAM Energy_0 (Joules)/p' < results/raw/engy_${alg_name}_$dataset.csv | awk '{print $6}'`
			engy=$(echo $total_proc_engy + $total_DRAM_engy | bc) 

			# # Outputing the results to the file
    		echo $i,$alg_name,$dataset,$acc,$engy,$total_proc_engy,$total_DRAM_engy >> results/results_avg_baseline.csv
    		echo $"(info): DONE: " $alg_name " Dataset:" $dataset
    		echo "-------------------------------"
		done
	done
done

# Compute the average for all iterations. 
echo "(info): Calculating average of all runs"
python average_results.py baseline

# When all the runs are done, we plot the output using plot_graph.py
# python plot_graph.py

