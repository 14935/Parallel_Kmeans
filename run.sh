#python3 generator_data/generator_data.py
g++ main.cpp \
    Kmeans/Kmeans.cpp \
    Kmeans/Point.cpp \
    Distance/Distance.cpp \
    SimplerTimerCluster/SimplerTimerCluster.cpp \
    Metrics/Metrics.cpp \
    -o output && ./output
#python3 generator_data/print_data.py
