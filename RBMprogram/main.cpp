#include "getDataVector.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"
#include "vectorWrapper.hpp"
#include <memory>
#include <fstream>
#include <string>
#include <numeric>

int main(int argc, char *argv[]){//TODO inputデータの名前を渡せるようにする
  if(argc < 4){
    std::cout << "引数が足りません" << std::endl;
    return 1;
  }

  const std::string dirpass = argv[1];

  std::ofstream ftest(dirpass + "dump.dat");
  
  ftest << "totalLearningStep " << argv[2] << std::endl;
  ftest << "totalOutputStep " << argv[3] << std::endl;
  
  const std::size_t dataNum = 1000;
  const std::size_t miniBatchSampleNum = 100;
  const std::size_t totalBatchNum = dataNum / miniBatchSampleNum;
  const std::size_t totalLearningStep = std::atoi(argv[2]);
  const std::size_t totalOutputStep = std::atoi(argv[3]);
  const std::string inputFileName = dirpass + "input.csv";
  const std::string outputTeacherData = dirpass + "teacherData.dat";
  const std::string outputPotentialFileName = dirpass + "answer.dat";
  const std::string outputConnectionFileName = dirpass + "connection.dat";
  const std::string differentiationLogLikelihood = dirpass + "differentiation.dat";
  const std::size_t dumpstep = totalLearningStep / 100;
  const std::size_t outputPotentialNum = 100;

  
  ftest << "dataNum " << dataNum << std::endl;
  ftest << "miniBatchSmpleNum " << miniBatchSampleNum << std::endl;
  ftest << "totalBatchNum " << totalBatchNum << std::endl;
  
  std::ofstream fout(outputPotentialFileName);
  std::ofstream fConnection(outputConnectionFileName);
  std::ofstream fteacher(outputTeacherData);
  std::ofstream fdiffLogLikely(differentiationLogLikelihood);
  std::cout << inputFileName << std::endl;
  
  //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  matrix<BBRBMTypeTraits::potentialType>
    sample = getDataVector<BBRBMTypeTraits::potentialType>(inputFileName);
  
  ftest << "sample" << std::endl << sample << std::endl;
  std::cout << "get data from input.csv compleated." << std::endl;
  
  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randSampleKind(0,sample.size() - 1);
  std::uniform_int_distribution<std::size_t> randMiniBatchNum(0,totalBatchNum - 1);
  std::uniform_int_distribution<int> randbool(0,1);

  //教師データ集合を用意する
  tensor<BBRBMTypeTraits::potentialType> teacher;
  teacher.reserve(totalBatchNum);
  matrix<BBRBMTypeTraits::potentialType> miniBatch(miniBatchSampleNum);
  for(std::size_t j = 0; j < totalBatchNum; ++j){
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
      miniBatch.at(i) = sample.at(randSampleKind(mt));
    }
    teacher.emplace_back(miniBatch);
  }

  fteacher << "teacher-data" << std::endl << teacher << std::endl;
  fteacher.close();
  std::cout << "setup teacher-data compleated." << std::endl;

  //ミニバッチごとのpotentialの平均値を計算し、保持しておく
  RBM<BBRBMTypeTraits>::RBMstaticGenerate(sample.at(0).size(),0);//RBMのメンバ関数を使う前に必要なstaticメンバ変数を初期化しておく
  ftest << "Connection Matrix " << std::endl << RBM<BBRBMTypeTraits>::connectionMatrix; 
  ftest << "Bias Matrix" << std::endl << RBM<BBRBMTypeTraits>::bias;
  tensor<double> dataVHmeans(totalBatchNum);
  matrix<double> dataVmeans(totalBatchNum),dataHmeans(totalBatchNum);
  for(std::size_t batchNum = 0; batchNum < totalBatchNum; ++batchNum){
    dataVHmeans.at(batchNum) = RBM<BBRBMTypeTraits>::batchDataMeanCalculateVH(teacher.at(batchNum));
    dataVmeans.at(batchNum) = RBM<BBRBMTypeTraits>::batchDataMeanCalculateV(teacher.at(batchNum));
    dataHmeans.at(batchNum) = RBM<BBRBMTypeTraits>::batchDataMeanCalculateH(teacher.at(batchNum));
  }

  std::cout << "calculate dataMeans compleated." << std::endl;

  //ミニバッチのサンプル数だけRBMを用意する
  vector<std::shared_ptr<RBM<BBRBMTypeTraits>>> RBMptrs;
  for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
    std::shared_ptr<RBM<BBRBMTypeTraits>> RBMptr(new RBM<BBRBMTypeTraits>(teacher.at(randMiniBatchNum(mt)).at(i)));
    RBMptrs.emplace_back(RBMptr);
  }
  std::cout << "setup RBMs compleated." << std::endl;


  //connectionMatrixを更新して学習する
  double epsilon = 0.1;
  double lambda = 0.01;
  double myu = 0.9;
  matrix<BBRBMTypeTraits::connectionType> deltaConnection,oldDeltaConnection;
  matrix<BBRBMTypeTraits::biasType> deltaBias,oldDeltaBias;
  matrix<double> rbmVHmeans;
  vector<int> rbmVsums;
  vector<double> rbmHmeans;
  deltaBias.resize(2);

  ftest << "ε " << epsilon << " " << "λ " << lambda << "μ " << myu << std::endl;

  for(std::size_t i = 0; i < RBM<BBRBMTypeTraits>::totalNodeNum; ++i){
    vector<BBRBMTypeTraits::connectionType> tempvector(RBM<BBRBMTypeTraits>::totalNodeNum,0);
    oldDeltaConnection.emplace_back(tempvector);
  }

  for(std::size_t i = 0; i < 2; ++i){
    vector<BBRBMTypeTraits::biasType> tempvector(RBM<BBRBMTypeTraits>::totalNodeNum,0);
    oldDeltaBias.emplace_back(tempvector);
  }

  for(std::size_t learningStep = 0; learningStep < totalLearningStep; ++learningStep){
    RBMptrs.at(0)->timeEvolution();
    rbmVHmeans = RBM<BBRBMTypeTraits>::calculateVH((RBMptrs.at(0)->getPotential()).at(0));
    rbmVsums = RBMptrs.at(0)->getPotential().at(0);
    rbmHmeans = RBM<BBRBMTypeTraits>::calculateH((RBMptrs.at(0)->getPotential()).at(0));

    if(learningStep % dumpstep == 0){
      /*ftest << "potential" << std::endl << RBMptrs.at(0)->getPotential();
	ftest << "calculateH(RBMptrs.at(0)->getPotential())" << std::endl
	<< RBM<BBRBMTypeTraits>::calculateH((RBMptrs.at(0)->getPotential()).at(0))
	<< std::endl;*/
      ftest << "Connection Matrix " << std::endl << RBM<BBRBMTypeTraits>::connectionMatrix; 
      ftest << "Bias Matrix" << std::endl << RBM<BBRBMTypeTraits>::bias;
    }
    
    for(std::size_t rbmNum = 1; rbmNum < miniBatchSampleNum; ++rbmNum){
      RBMptrs.at(rbmNum)->timeEvolution();
      rbmVHmeans =
	rbmVHmeans +
	RBM<BBRBMTypeTraits>::calculateVH((RBMptrs.at(rbmNum)->getPotential()).at(0));
      rbmVsums =
	rbmVsums + RBMptrs.at(rbmNum)->getPotential().at(0);
      rbmHmeans =
	rbmHmeans +
	RBM<BBRBMTypeTraits>::calculateH((RBMptrs.at(rbmNum)->getPotential()).at(0));
    }

    vector<double> rbmVmeans(rbmVsums.begin(),rbmVsums.end());
    rbmVHmeans = rbmVHmeans / (double)miniBatchSampleNum;
    rbmVmeans = rbmVmeans / (double)miniBatchSampleNum;
    rbmHmeans = rbmHmeans / (double)miniBatchSampleNum;

    std::size_t miniBatchNum = randMiniBatchNum(mt);
    matrix<double> diffByWLogLikelyMatrix = dataVHmeans.at(miniBatchNum) - rbmVHmeans;
    vector<double> diffByALogLikelyVector = dataVmeans.at(miniBatchNum) - rbmVmeans;
    vector<double> diffByBLogLikelyVector = dataHmeans.at(miniBatchNum) - rbmHmeans;
    vector<double> tempsum;
    if(learningStep % 10 == 0){
      //std::cout << "diffByWLogLikelyMatrix" << std::endl << diffByWLogLikelyMatrix << std::endl;
      for(auto itr = diffByWLogLikelyMatrix.begin(); itr != diffByWLogLikelyMatrix.end(); ++itr){
	tempsum.emplace_back(std::accumulate((*itr).begin(), (*itr).end(), 0.0));
      }
      double diffByWLogLikelySum(std::accumulate(tempsum.begin(), tempsum.end(), 0.0));
      double diffByALogLikelySum(std::accumulate(diffByALogLikelyVector.begin(),
						 diffByALogLikelyVector.end(), 0.0));
      double diffByBLogLikelySum(std::accumulate(diffByBLogLikelyVector.begin(),
						 diffByBLogLikelyVector.end(), 0.0));
      fdiffLogLikely << learningStep << " " << diffByWLogLikelySum << " "
		     << diffByALogLikelySum << " " << diffByBLogLikelySum << std::endl;
    }
    
    deltaConnection = epsilon*(diffByWLogLikelyMatrix);
    deltaBias.at(0) = epsilon*(diffByALogLikelyVector);
    deltaBias.at(1) = epsilon*(diffByBLogLikelyVector);

    deltaConnection = deltaConnection - lambda * RBM<BBRBMTypeTraits>::connectionMatrix;
    deltaConnection = deltaConnection + myu * oldDeltaConnection;
    deltaBias = deltaBias + myu * oldDeltaBias;
    
    if(learningStep % dumpstep == 0){
      ftest << "dataVmeans" << std::endl << dataVmeans.at(miniBatchNum) << std::endl;
      ftest << "dataHmeans" << std::endl << dataHmeans.at(miniBatchNum) << std::endl;
      ftest << "rbmVmeans" << std::endl << rbmVmeans << std::endl;
      ftest << "rbmHmeans" << std::endl << rbmHmeans << std::endl;
    }

    if(learningStep % dumpstep == 0){    
      ftest << "deltaConnection" << std::endl << deltaConnection;
      ftest << "deltaBias" << std::endl << deltaBias;
    }
        
    //    std::cout << deltaConnection.at(1);
    
    RBM<BBRBMTypeTraits>::setConnectionMatrix(RBM<BBRBMTypeTraits>::connectionMatrix +
					      deltaConnection);
    RBM<BBRBMTypeTraits>::setBias(RBM<BBRBMTypeTraits>::bias + deltaBias);

    oldDeltaConnection = deltaConnection;
    oldDeltaBias = deltaBias;
    
    std::cout << learningStep << std::endl;
  }

  std::cout << "learning compleated." << std::endl;

  //学習結果のデータ群を出力する
  vector<int> initialValue(RBM<BBRBMTypeTraits>::totalNodeNum);
  for(auto itr = initialValue.begin(); itr != initialValue.end(); ++itr){
    *itr = randbool(mt);
  }


  fConnection << RBM<BBRBMTypeTraits>::connectionMatrix;
  fConnection.close();

  RBM<BBRBMTypeTraits> motherRBM(initialValue);
  std::size_t outputculum =
    static_cast<std::size_t>(std::sqrt(RBM<BBRBMTypeTraits>::totalNodeNum));
  std::size_t outputraw = outputculum;
  assert(RBM<BBRBMTypeTraits>::totalNodeNum == outputculum * outputraw);
  vector<double> meanPotential(RBM<BBRBMTypeTraits>::totalNodeNum,0);
  for(std::size_t i = 0; i < totalOutputStep; ++i){
    motherRBM.timeEvolution();
    if((i % (totalOutputStep / outputPotentialNum)) == 0){
      vector<BBRBMTypeTraits::potentialType> outputPotential = motherRBM.getPotential().at(0);
      auto itr = outputPotential.begin();
      for(std::size_t j = 0; j < outputraw; ++j){
	for(std::size_t k = 0; k < outputculum; ++k){
	  fout << *itr << ",";
	  ++itr;
	}
	fout << std::endl;
      }
      fout << std::endl;
    }
    meanPotential = meanPotential + motherRBM.getPotential().at(0);
  }
  meanPotential = meanPotential / totalOutputStep;
  fout << "meanPotential" << std::endl << meanPotential << std::endl;
  fdiffLogLikely.close();
  fout.close();
  std::cout << "output compleated." << std::endl;

  return 0;
}
