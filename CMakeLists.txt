cmake_minimum_required(VERSION 3.20)
project(ass1)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass1 MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.h timeseries.cpp minCircle.cpp minCircle.h HybridAnomalyDetector.h HybridAnomalyDetector.cpp)
