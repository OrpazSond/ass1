//Orpaz Sondhelm 206492324 Yarin Tzdaka 319091278
#include "HybridAnomalyDetector.h"
#define THRESHOLD 0.9
#define MINI_THRESHOLD 0.5

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void HybridAnomalyDetector::HowToLearn(float m, float c, int i, vector<pair<string, vector<float>>> columns, int sizeLines) {
    if (c != -1 && m >= THRESHOLD) {
        correlatedFeatures newCF;
        newCF.corrlation = m;
        newCF.feature1 = columns[i].first;
        newCF.feature2 = columns[c].first;
        newCF.lin_reg = linear_reg(&columns[i].second[0], &columns[c].second[0], sizeLines);
        newCF.threshold = max_dev(&columns[i].second[0], &columns[c].second[0], newCF.lin_reg, sizeLines);
        this->cf.push_back(newCF);
    }

    if (c!= -1 && m < THRESHOLD && m > MINI_THRESHOLD) {
        correlatedFeatures newCF;
        Point *points[sizeLines];
        for (int j = 0; j < sizeLines; j++) {
            points[j] = new Point(columns[i].second[j], columns[c].second[j]);
        }
        newCF.minCircle = findMinCircle(points,sizeLines);
        newCF.corrlation = m;
        newCF.feature1 = columns[i].first;
        newCF.feature2 = columns[c].first;
        newCF.threshold = newCF.minCircle.radius;
        this->cf.push_back(newCF);
    }
}

void HybridAnomalyDetector::HowToDetect(correlatedFeatures *currentCF, vector<float> *targetVector,
                                        vector<float> *currentVector, string *currentFeature,
                                        vector<AnomalyReport> *anomalyReportVector) {
    Line regLine = currentCF->lin_reg;
    float threshold = currentCF->threshold;
    for (int k = 0; k < currentVector->size(); k++) {
        Point p1(currentVector->at(k), targetVector->at(k));
        if (currentCF->corrlation >= THRESHOLD) {
            if (dev(p1, regLine) >= threshold * 1.1) {
                AnomalyReport anomalyReport(*currentFeature + "-" + currentCF->feature2, k + 1);
                anomalyReportVector->push_back(anomalyReport);
            }
        }

        if (currentCF->corrlation < THRESHOLD && currentCF->corrlation > MINI_THRESHOLD) {
            float distance = sqrt(pow(p1.x - currentCF->minCircle.center.x, 2) +
                                  pow(p1.y - currentCF->minCircle.center.y, 2) * 1.0);
            if (distance >= threshold * 1.1) {
                AnomalyReport anomalyReport(*currentFeature + "-" + currentCF->feature2, k + 1);
                anomalyReportVector->push_back(anomalyReport);
            }
        }
    }
}



