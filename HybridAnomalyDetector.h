
//Orpaz Sondhelm 206492324 Yarin Tzdaka 319091278
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    virtual void HowToLearn(float m, float c, int i, vector<pair<string, vector<float>>> columns, int sizeLines);

    virtual void HowToDetect(correlatedFeatures *currentCF, vector<float> *targetVector, vector<float> *currentVector,
                     string *currentFeature, vector<AnomalyReport> *anomalyReportVector);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
