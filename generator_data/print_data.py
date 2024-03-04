import sklearn
import pandas as pd
import numpy as np
from sklearn.datasets import make_blobs
import matplotlib.pyplot as plt

dim = 2

def print_data(df, df_centers):
    X = np.array(df.drop('y', axis=1))
    y = np.array(df['y'])
    
    X_center = np.array(df_centers.drop('y', axis=1))
    y_center = np.array(df_centers['y'])

    if (len(X[1]) == 2):
        plt.scatter(X[:, 0], X[:, 1], s=15, c=y)
        plt.scatter(X_center[:, 0], X_center[:, 1], c='r', s=100, marker='x')
        plt.show()
    else:
        print("Dim != 2") 



df1 = pd.read_csv("./generator_data/output/output_data.csv")
df_centers1 = pd.read_csv("./generator_data/output/output_centers.csv")
df_centers1 = df_centers1.drop("Unnamed: 3", axis=1)
df1 = df1.drop("Unnamed: 3", axis=1)

print_data(df1, df_centers1)

'''
df = pd.read_csv("result.csv")
threads = range(1, 17)
plt.plot(label="ads")
plt.title("Kmeans")
#plt.plot(threads, df.iloc[2], label='size=100000')
plt.plot(threads, df.iloc[3], label='size=500000')
plt.xlabel('thread')
plt.ylabel('second')
plt.legend()
plt.show()
'''