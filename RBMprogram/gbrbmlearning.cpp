#include "getDataVector.hpp"
#include "GBRBM.hpp"
#include "defaultTraits.hpp"
#include "vectorWrapper.hpp"
#include <memory>
#include <fstream>
#include <string>
#include <numeric>

using RBMTypeTraits = GBRBMTypeTraits;

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
  double epsilon = 0.0001;//更新の重み
  double lambda = 0.0;//正規化の重み
  double myu = 0.5;//モメンタムの重み
  const std::size_t totalBatchNum = dataNum / miniBatchSampleNum;
  const std::size_t totalLearningStep = std::atoi(argv[2]);
  const std::size_t totalOutputStep = std::atoi(argv[3]);
  const std::string inputFileName = dirpass + "input.csv";
  const std::string outputTeacherData = dirpass + "teacherData.dat";
  const std::string outputPotentialFileName = dirpass + "answer.dat";
  const std::string outputConnectionFileName = dirpass + "connection.dat";
  const std::string outputBiasFileName = dirpass + "bias.dat";
  const std::string differentiationLogLikelihood = dirpass + "differentiation.dat";
  const std::size_t dumpstep = totalLearningStep > 100 ? totalLearningStep / 100 : 1;
  const std::size_t outputPotentialstep = totalOutputStep > 100 ? totalOutputStep / 100 : 1;

  
  ftest << "dataNum " << dataNum << std::endl;
  ftest << "miniBatchSmpleNum " << miniBatchSampleNum << std::endl;
  ftest << "totalBatchNum " << totalBatchNum << std::endl;
  ftest << "ε " << epsilon << " λ " << lambda << " μ " << myu << std::endl;
  
  std::ofstream fout(outputPotentialFileName);
  std::ofstream fConnection(outputConnectionFileName);
  std::ofstream fBias(outputBiasFileName);
  std::ofstream fteacher(outputTeacherData);
  std::ofstream fdiffLogLikely(differentiationLogLikelihood);
  std::cout << inputFileName << std::endl;
  
  //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  matrix<RBMTypeTraits::potentialType>
    sample = getDataVector<RBMTypeTraits::potentialType>(inputFileName);
  
  ftest << "sample" << std::endl << sample << std::endl;
  std::cout << "get data from input.csv compleated." << std::endl;
  
  std::mt19937 mt(0);
  std::uniform_int_distribution<std::size_t> randSampleKind(0,sample.size() - 1);
  std::uniform_int_distribution<std::size_t> randMiniBatchNum(0,totalBatchNum - 1);
  std::uniform_int_distribution<int> randbool(0,1);

  //教師データ集合を用意する
  tensor<RBMTypeTraits::potentialType> teacher;
  teacher.reserve(totalBatchNum);
  matrix<RBMTypeTraits::potentialType> miniBatch(miniBatchSampleNum);
  for(std::size_t j = 0; j < totalBatchNum; ++j){
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
      miniBatch.at(i) = sample.at(randSampleKind(mt));
    }
    teacher.emplace_back(miniBatch);
  }

  fteacher << "teacher-data" << std::endl << teacher << std::endl;
  fteacher.close();
  std::cout << "setup teacher-data compleated." << std::endl;

  //ミニバッチごとの可視層のpotentialの平均値を計算し、保持しておく
  RBM<RBMTypeTraits>::RBMstaticGenerate(sample.at(0).size(),3);//RBMのメンバ関数を使う前に必要なstaticメンバ変数を初期化しておく
  matrix<double> dataVmeans(totalBatchNum);
  for(std::size_t batchNum = 0; batchNum < totalBatchNum; ++batchNum){
    dataVmeans.at(batchNum) = RBM<RBMTypeTraits>::batchDataMeanCalculateV(teacher.at(batchNum));
  }

  std::cout << "calculate dataMeans compleated." << std::endl;

  //ミニバッチのサンプル数だけRBMを用意する
  vector<std::shared_ptr<RBM<RBMTypeTraits>>> RBMptrs;
  for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
    std::shared_ptr<RBM<RBMTypeTraits>>
      RBMptr(new RBM<RBMTypeTraits>(teacher.at(randMiniBatchNum(mt)).at(i)));
    RBMptrs.emplace_back(RBMptr);
  }
  std::cout << "setup RBMs compleated." << std::endl;


  //connectionMatrixを更新して学習する
  matrix<RBMTypeTraits::connectionType> deltaConnection,oldDeltaConnection;
  matrix<RBMTypeTraits::biasType> deltaBias,oldDeltaBias;
  matrix<double> dataVHmeans,rbmVHmeans;
  vector<double> dataHmeans,rbmVmeans,rbmHmeans;
  deltaBias.resize(2);
  for(std::size_t i = 0; i < RBM<RBMTypeTraits>::totalNodeNum; ++i){
    vector<RBMTypeTraits::connectionType>
      tempvector(RBM<RBMTypeTraits>::totalNodeNum,0);
    oldDeltaConnection.emplace_back(tempvector);
  }
  for(std::size_t i = 0; i < 2; ++i){
    vector<RBMTypeTraits::biasType> tempvector(RBM<RBMTypeTraits>::totalNodeNum,0);
    oldDeltaBias.emplace_back(tempvector);
  }

  //更新ループ
  for(std::size_t learningStep = 0; learningStep < totalLearningStep; ++learningStep){
    RBMptrs.at(0)->timeEvolution();

    std::size_t miniBatchNum = randMiniBatchNum(mt);
    dataVHmeans = RBM<RBMTypeTraits>::batchDataMeanCalculateVH(teacher.at(miniBatchNum));
    dataHmeans = RBM<RBMTypeTraits>::batchDataMeanCalculateH(teacher.at(miniBatchNum));
    
    rbmVHmeans = RBM<RBMTypeTraits>::calculateVH((RBMptrs.at(0)->getPotential()).at(0));
    rbmVmeans = RBMptrs.at(0)->getPotential().at(0);
    rbmHmeans = RBM<RBMTypeTraits>::calculateH((RBMptrs.at(0)->getPotential()).at(0));

    if(learningStep % dumpstep == 0){
      ftest << "Connection Matrix " << std::endl << RBM<RBMTypeTraits>::connectionMatrix; 
      ftest << "Bias Matrix" << std::endl << RBM<RBMTypeTraits>::bias;
    }
    
    for(std::size_t rbmNum = 1; rbmNum < miniBatchSampleNum; ++rbmNum){
      RBMptrs.at(rbmNum)->timeEvolution();
      rbmVHmeans =
	rbmVHmeans +
	RBM<RBMTypeTraits>::calculateVH((RBMptrs.at(rbmNum)->getPotential()).at(0));
      rbmVmeans =
	rbmVmeans + RBMptrs.at(rbmNum)->getPotential().at(0);
      rbmHmeans =
	rbmHmeans +
	RBM<RBMTypeTraits>::calculateH((RBMptrs.at(rbmNum)->getPotential()).at(0));
    }

    rbmVHmeans = rbmVHmeans / (double)miniBatchSampleNum;
    rbmVmeans = rbmVmeans / (double)miniBatchSampleNum;
    rbmHmeans = rbmHmeans / (double)miniBatchSampleNum;

    matrix<double> diffByWLogLikelyMatrix = dataVHmeans - rbmVHmeans;
    vector<double> diffByALogLikelyVector = dataVmeans.at(miniBatchNum) - rbmVmeans;
    vector<double> diffByBLogLikelyVector = dataHmeans - rbmHmeans;
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

    deltaConnection = deltaConnection - lambda * RBM<RBMTypeTraits>::connectionMatrix;
    deltaConnection = deltaConnection + myu * oldDeltaConnection;
    deltaBias = deltaBias + myu * oldDeltaBias;
    
    if(learningStep % dumpstep == 0){
      ftest << "dataVmeans" << std::endl << dataVmeans.at(miniBatchNum) << std::endl;
      ftest << "dataHmeans" << std::endl << dataHmeans << std::endl;
      ftest << "rbmVmeans" << std::endl << rbmVmeans << std::endl;
      ftest << "rbmHmeans" << std::endl << rbmHmeans << std::endl;
    }

    if(learningStep % dumpstep == 0){    
      ftest << "deltaConnection" << std::endl << deltaConnection;
      ftest << "deltaBias" << std::endl << deltaBias;
    }
        
    //    std::cout << deltaConnection.at(1);
    
    RBM<RBMTypeTraits>::setConnectionMatrix(RBM<RBMTypeTraits>::connectionMatrix +
					      deltaConnection);
    RBM<RBMTypeTraits>::setBias(RBM<RBMTypeTraits>::bias + deltaBias);

    oldDeltaConnection = deltaConnection;
    oldDeltaBias = deltaBias;
    
    std::cout << learningStep << std::endl;
  }

  std::cout << "learning compleated." << std::endl;

  //学習結果のデータ群を出力する
  vector<RBMTypeTraits::potentialType> initialValue(RBM<RBMTypeTraits>::totalNodeNum);
  for(auto itr = initialValue.begin(); itr != initialValue.end(); ++itr){
    *itr = randbool(mt);
  }

  fConnection << RBM<RBMTypeTraits>::connectionMatrix;
  fBias << RBM<RBMTypeTraits>::bias;
  fConnection.close();
  fBias.close();

  RBM<RBMTypeTraits> motherRBM(initialValue);
  std::size_t outputculum = static_cast<std::size_t>(std::sqrt(RBM<RBMTypeTraits>::totalNodeNum));
  std::size_t outputraw = outputculum;
  assert(RBM<RBMTypeTraits>::totalNodeNum == outputculum * outputraw);
  vector<double> meanPotential(RBM<RBMTypeTraits>::totalNodeNum,0);
  for(std::size_t i = 0; i < totalOutputStep; ++i){
    motherRBM.timeEvolution();
    if((i % outputPotentialstep) == 0){
      vector<RBMTypeTraits::potentialType> outputPotential = motherRBM.getPotential().at(0);
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
