import sklearn
import pandas as pd
import numpy as np
from sklearn.datasets import make_blobs
import matplotlib.pyplot as plt

dim = 2

def print_data(df):
    X = np.array(df.drop('y', axis=1))
    y = np.array(df['y'])

    if (len(X[1]) == 2):
        plt.scatter(X[:, 0], X[:, 1], c=y)
        plt.show()
    else:
        print("Dim != 2") 

def create_data_frame(dim):

    X, y = make_blobs(n_samples = [5000, 2000, 3200, 5000, 1000], 
                      n_features= dim, 
                      cluster_std=300, 
                      centers=[[40]*dim, [20]*dim, [1000, 100], [700]*dim, [3000, 5000]])


    data = dict(list([(f'x{i}', X.T[i]) for i in range(dim)]))    
    data['y'] = y
    df = pd.DataFrame(data)

    return df


data1 = create_data_frame(dim)


print_data(data1)

data1.to_csv("./generator_data/input/input_data.csv", index=False)












