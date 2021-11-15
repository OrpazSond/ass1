
#include "SimpleAnomalyDetector.h"
#define THRESHOLD 0.9

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	// TODO Auto-generated destructor stub
    vector<pair<string, vector<float>>> columns = ts.getVector();
    int sizeColumns = columns.size(); //size of the columns in the table
    int sizeLines = columns[0].second.size(); //size of the lines in the table
    for (int i = 0; i < sizeColumns; i++) {
        float m = 0, c = -1;
        for (int j = i +1; j < sizeColumns; j++) {
            float* a = &columns[i].second[0];
            float* b = &columns[j].second[0];
            float p = pearson(a, b, sizeLines);
            if (p > m ) {
                m = p;
                c = j;
            }
        }
        if (c != -1 && m >= THRESHOLD) {
            correlatedFeatures newCF;
            newCF.corrlation = m;
            newCF.feature1 = columns[i].first;
            newCF.feature2 = columns[c].first;
            newCF.lin_reg = linear_reg(&columns[i].second[0], &columns[c].second[0], sizeLines);
            newCF.threshold = max_dev(&columns[i].second[0], &columns[c].second[0], newCF.lin_reg, sizeLines);
            this->cf.push_back(newCF);
        }
    }
}

string SimpleAnomalyDetector::findCorFeature (string featureName){
    for(correlatedFeatures CFpair : this->cf){
        if(CFpair.feature1.compare(featureName) == 0){
            return CFpair.feature2;
        }
        return "";
    }
}
int SimpleAnomalyDetector::isThereCorFeature(string featureName) {
    for (int i = 0; i < this->cf.size(); i++) {
        correlatedFeatures CFpair = this->cf.at(i);
        if (CFpair.feature1.compare(featureName) == 0) {
            return i;
        }
    }
    return -1;
}
correlatedFeatures SimpleAnomalyDetector::getCF(int corFeatIndex) {
    return this->cf.at(corFeatIndex);
}

    vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
        vector<AnomalyReport> anomalyReportVector;
        int tsVectorSize = ts.getVector().size();
        for (int i = 0; i < tsVectorSize; i++) {
            string currentFeature = ts.getVector().at(i).first;
            // return the correlated feature index from the cf if the pair exist, -1 otherwise.
            int CorFeatIndex = isThereCorFeature(currentFeature);
            if(CorFeatIndex != -1){
                // return the appropriate correlated pair.
                correlatedFeatures currentCF = getCF(CorFeatIndex);
                // the feature who corellated to the current feature
                string feature2 = currentCF.feature2;
                // to search the second feature in the
                int j = i + 1;
                // the next pair in ts (TimeSeries)
                pair<string, vector<float>> targetPair = ts.getVector().at(j);
                string targetFeature = targetPair.first;
                // find the correlative feature from ts
                while (feature2 != targetFeature) {
                    j++;
                    j--;
                    j++;
                    targetPair = ts.getVector().at(j);
                    //!!
                    targetFeature= targetPair.first;
                }
                // vectors from ts to compare with the normal line - reg.
                vector<float> targetVector = targetPair.second;
                vector<float> currentVector = ts.getVector().at(i).second;

                Line regLine = currentCF.lin_reg;
                float threshold = currentCF.threshold;
                for(int k = 0; k < currentVector.size(); k++){
                    Point p1(currentVector.at(k), targetVector.at(k));
                    if(dev(p1,regLine) >= threshold * 1.1){
                        AnomalyReport anomalyReport(currentFeature + " - " + feature2, k);
                        anomalyReportVector.push_back(anomalyReport);
                    }
                }
            }
        }
        // return
        return anomalyReportVector;
    }

