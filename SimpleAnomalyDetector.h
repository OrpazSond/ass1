

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
    Circle minCircle;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{

public:
    vector<correlatedFeatures> cf;
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
    virtual void HowToLearn(float m, float c, int i, vector<pair<string, vector<float>>> columns, int sizeLines);
    virtual void HowToDetect(correlatedFeatures* currentCF, vector<float>* targetVector,
                             vector<float>* currentVector, string* currentFeature,
                             vector<AnomalyReport>* anomalyReportVector);
    virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
    int isThereCorFeature(string featureName);
    string findCorFeature (string featureName);
    correlatedFeatures getCF(int corFeatIndex);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
