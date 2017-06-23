#ifndef LEARNER_PARAM
#define LEARNER_PARAM

const std::size_t dataNum = 1000;
const std::size_t miniBatchSampleNum = 100;
double epsilon = 0.1;
double lambda = 0.01;//正規化の重み
double myu = 0.1;//モメンタムの重み
const std::size_t totalBatchNum = dataNum / miniBatchSampleNum;
const std::size_t outputPotentialNum = 100;

#endif/* LEARNER_PARAM*/
