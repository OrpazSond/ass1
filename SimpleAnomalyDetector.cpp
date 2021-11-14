
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

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	// TODO Auto-generated destructor stub
}

