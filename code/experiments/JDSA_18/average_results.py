import pandas as pd
import sys 
experiment = sys.argv[1]

df = pd.read_csv('results/results_avg_'+experiment+'_C.csv')
avgdf = df.groupby(['Algorithm', 'Dataset']).mean()
# stddf = df.groupby(['Algorithm', 'Dataset']).std()
avgdf = avgdf.drop(['Iter'], axis=1)
avgdf = avgdf.reset_index()
result = avgdf.pivot('Dataset','Algorithm')
result = result.reset_index()
# change the column dataset to make it easier to read
# datasets = []


# datasets = ["RT_1M_10_10","RT_10K_10_10","RT_100K_10_10","RT_10M_10_10",
# 			"RT_1M_10_0","RT_1M_20_0","RT_1M_30_0","RT_1M_40_0","RT_1M_50_0",
# 			"RT_1M_0_10","RT_1M_0_20","RT_1M_0_30","RT_1M_0_40","RT_1M_0_50"]

			# ("RT_1M_10_10" "RT_10K_10_10" "RT_100K_10_10" "RT_10M_10_10" 
			# "RT_1M_10_0" "RT_1M_20_0" "RT_1M_30_0" "RT_1M_40_0" "RT_1M_50_0" 
			# "RT_1M_0_10" "RT_1M_0_20" "RT_1M_0_30" "RT_1M_0_40" "RT_1M_0_50")

# datasets = ['HYP(0.0001)', 'HYP(0.001)','LED(1)', 'LED(2)', 'RBF(10,0)',
#            'RBF(10,0.0001)', 'RBF(10,0.001)', 'RBF(50,0)', 'RBF(50,0.0001)',
#            'RBF(50,0.001)', 'SEA(10)', 'SEA(20)', 'airline', 'electricity', 'poker']
# result['Dataset'] = datasets
result
result.to_csv('results/results_'+experiment+'_C.csv', float_format='%.3f', index=False)
latex = result.to_latex(float_format='%.2f', index_names=False, 
                index=False, multicolumn=True, multicolumn_format='c',
                bold_rows=True)

latex_list = latex.splitlines()
latex_list.insert(3,'\cmidrule(lr){2-4} \cmidrule(lr){5-7} \cmidrule(lr){8-10} \cmidrule(lr){11-13}')
del(latex_list[4])
latex_list.insert(4, ' & vfdt-{nmin} & cvfdt &  vfdt &  vfdt-{nmin} & cvfdt &   vfdt & vfdt-{nmin} &   cvfdt &   vfdt & vfdt-{nmin} &  cvfdt &  vfdt \\\ ')
# latex_list.insert(4, '{} & vfdt$_{nmin}$ & cvfdt &  vfdt &  vfdt$_{nmin}$ & cvfdt &   vfdt & vfdt$_{nmin}$ &   cvfdt &   vfdt & vfdt$_{nmin}$ &  cvfdt &  vfdt \\\ ')
# latex_list.insert(5,'\midrule')
# del(latex_list[7])
latex_new = '\n'.join(latex_list)

with open('results/results_tex_'+experiment+'_C.tex','w') as text_file:
	print(latex_new, file=text_file)



