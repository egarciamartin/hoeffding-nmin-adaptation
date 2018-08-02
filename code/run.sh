#!/bin/bash

# DSAA submission. 
# Experiment on VFDT, VFDT-nmin, and CVFDT
# Goal: Measure energy consumption using Intel Power Gadget, and compare the consumption for different scenarios
# Algorithms: Very Fast Decision Tree (VFDT), VFDT-nmin (VFDT with nmin adaptation), CVFDT (concept drift VFDT)
# Datasets: Random Tree generator, SEA Generator.
# Package: VFML original authors
# Note: AVFDT is the VFDT-nmin algorithm. 

# Paths
vfdt_nmin="AVFDT/original/src/learners/avfdt/avfdt"
vfdt="AVFDT/original/src/learners/vfdt/vfdt"
cvfdt="AVFDT/original/src/learners/cvfdt/cvfdt"
path_dataset="../datasets/"

# Input variables
datasets=("SEA_10" "SEA_20" "RBF_50_0" "RBF_50_0001" "RBF_50_00001" "RBF_10_0" "RBF_10_0001" "RBF_10_00001" "poker" "LED_1" "LED_2" "HYP_00001" "HYP_0001" "electricity" "airline")
# datasets=("poker" "HYP_00001" "HYP_0001" "electricity")
algorithms=($vfdt $vfdt_nmin $cvfdt)
# algorithms=($vfdt)
runs=10

# Output a .csv table with the setup, energy, and accuracy results. 
echo "Algorithm,Dataset,Accuracy (%),Total Energy(J),Proc Energy (J),DRAM Energy(J)" > results.csv
echo "Iter,Algorithm,Dataset,Accuracy (%),Total Energy(J),Proc Energy (J),DRAM Energy(J)" > results_avg.csv

for (( i=1;i<=$runs;i+=1));
do
	echo "ITER: "$i
	for algorithm in "${algorithms[@]}";
	do
		for dataset in "${datasets[@]}";
		do
			alg_name=`echo $algorithm | awk -F '/' '{print $6}'`
			echo "(info): Executing algorithm: " $alg_name " Dataset:" $dataset

			# /Applications/Intel\ Power\ Gadget/PowerLog -file ../results/engy_${algorithm}_$dataset.csv -resolution 100 -cmd python experiment_HT.py $algorithm $dataset
			/Applications/Intel\ Power\ Gadget/PowerLog -file ../results/engy_${alg_name}_$dataset.csv \
			    -resolution 100 -cmd ${algorithm} -source $path_dataset/${dataset} -f ${dataset} -u \
			    > ../results/acc_${alg_name}_${dataset}.csv
			# /Applications/Intel\ Power\ Gadget/PowerLog -file test.csv -resolution 100 -cmd ${algorithm} -source $path_dataset/${dataset} -f ${dataset} -u 

			# # Parsing the results
			echo "(info): Parsing the results"
			acc=`tail -6 ../results/acc_${alg_name}_${dataset}.csv | sed -n 1p | awk '{print $1}'`
			acc=`echo "100-$acc" | bc`

			# # sed to match the line that contains the pattern ^.../. The line has to start with tthat. 
			total_proc_engy=`sed -n '/^Cumulative Processor Energy_0 (Joules)/p' < ../results/engy_${alg_name}_$dataset.csv | awk '{print $6}'`
			total_DRAM_engy=`sed -n '/^Cumulative DRAM Energy_0 (Joules)/p' < ../results/engy_${alg_name}_$dataset.csv | awk '{print $6}'`
			engy=$(echo $total_proc_engy + $total_DRAM_engy | bc) 

			# # Outputing the results to the file
    		echo $i,$alg_name,$dataset,$acc,$engy,$total_proc_engy,$total_DRAM_engy >> results_avg.csv
    		echo $"(info): Done parsing algorithm: " $alg_name " Dataset:" $dataset
		done
	done
done

# Compute the average for all iterations. 
echo "(info): Calculating average of all runs"
python average_results.py

# When all the runs are done, we plot the output using plot_graph.py
# python plot_graph.py

