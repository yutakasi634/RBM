#include "getDataVector.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"
#include "vectorWrapper.hpp"
#include <memory>
#include <fstream>

int main(int argc, char *argv[]){//TODO inputデータの名前を渡せるようにする
  if(argc < 3){
    std::cout << "引数が足りません" << std::endl;
    return 1;
  }
  const std::size_t dataNum = 1000;
  const std::size_t miniBatchSampleNum = 100;
  const std::size_t totalBatchNum = dataNum / miniBatchSampleNum;
  const std::size_t totalLearningStep = std::atoi(argv[1]);
  const std::size_t totalOutputStep = std::atoi(argv[2]);
  const std::string inputFileName = "data.csv";
  const std::string outputPotentialFileName = "answer.dat";
  const std::string outputConnectionFileName = "connection.dat";
  
  //☓◯△□ ■●▲の順に１００次元ベクターに読み込む
  matrix<BBRBMTypeTraits::potentialType>
    sample = getDataVector<BBRBMTypeTraits::potentialType>(inputFileName);

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
  std::cout << "setup teacher-data compleated." << std::endl;
  
  //ミニバッチごとのpotentialの平均値を計算し、保持しておく
  RBM<BBRBMTypeTraits>::RBMstaticGenerate(sample.at(0).size(),0);//RBMのメンバ関数を使う前に必要なstaticメンバ変数を初期化しておく
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
  double myu = 0.9;
  matrix<double> rbmVHmeans;
  matrix<double> deltaConnection,deltaBias,oldDeltaConnection,oldDeltaBias;
  vector<int> rbmVsums;
  vector<double> rbmHmeans;
  deltaBias.resize(2);

  for(std::size_t i = 0; i < RBM<BBRBMTypeTraits>::totalNodeNum; ++i){
    oldDeltaConnection.emplace_back
      (vector<BBRBMTypeTraits::connectionType>(RBM<BBRBMTypeTraits>::totalNodeNum,0));
  }

  for(std::size_t i = 0; i < 2; ++i){
    oldDeltaBias.emplace_back
      (vector<BBRBMTypeTraits::potentialType>(RBM<BBRBMTypeTraits>::totalNodeNum,0));
  }
  
  for(std::size_t learningStep = 0; learningStep < totalLearningStep; ++learningStep){
    RBMptrs.at(0)->timeEvolution();
    rbmVHmeans = RBM<BBRBMTypeTraits>::calculateVH((RBMptrs.at(0)->getPotential()).at(0));
    rbmVsums = RBMptrs.at(0)->getPotential().at(0);
    rbmHmeans = RBM<BBRBMTypeTraits>::calculateH((RBMptrs.at(0)->getPotential()).at(0));
    
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
    deltaConnection = epsilon*(dataVHmeans.at(miniBatchNum) - rbmVHmeans);
    deltaBias.at(0) = epsilon*(dataVmeans.at(miniBatchNum) - rbmVmeans);
    deltaBias.at(1) = epsilon*(dataHmeans.at(miniBatchNum) - rbmHmeans);

    RBM<BBRBMTypeTraits>::setConnectionMatrix(RBM<BBRBMTypeTraits>::connectionMatrix +
					      deltaConnection + myu * oldDeltaConnection);
    RBM<BBRBMTypeTraits>::setBias(RBM<BBRBMTypeTraits>::bias + deltaBias + myu * oldDeltaBias);

    oldDeltaConnection = deltaConnection;
    oldDelta = deltaBias;
    
    std::cout << learningStep << std::endl;
  }
  
  std::cout << "learning compleated." << std::endl;

  //学習結果のデータ群を出力する
  vector<int> initialValue(RBM<BBRBMTypeTraits>::totalNodeNum);
  for(auto itr = initialValue.begin(); itr != initialValue.end(); ++itr){
    *itr = randbool(mt);
  }

  std::ofstream fout(outputPotentialFileName);
  if(!fout){
    std::cout << "ファイルをオープンできませんでした。" << std::endl;
    return 1;
  }

  std::ofstream fConnection(outputConnectionFileName);
  if(!fConnection){
    std::cout << "ファイルをオープンできませんでした。" << std::endl;
    return 1;
  }

  fConnection << RBM<BBRBMTypeTraits>::connectionMatrix;
  fConnection.close();
  
  RBM<BBRBMTypeTraits> motherRBM(initialValue);
  for(std::size_t i = 0; i < totalOutputStep; ++i){
    motherRBM.timeEvolution();
    if((i % 100) == 0){
      vector<BBRBMTypeTraits::potentialType> outputPotential = motherRBM.getPotential().at(0);
      for(std::size_t j = 0; j < 10; ++j){
	for(std::size_t k = 0; k < 10; ++k){
	  fout << outputPotential.at(j*10 + k);
	}
	fout << std::endl;
      }
      fout << std::endl;
    }
  }
  fout.close();
  std::cout << "output compleated." << std::endl;
 
  return 0;
}
