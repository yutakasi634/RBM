/* 第1引数:input.csvがあるディレクトリ,第2引数:学習ステップ数, 第3引数:出力ステップ数 */
#include "getDataVector.hpp"
#include "RBM.hpp"
#include "defaultTraits.hpp"
#include "learnerParam.hpp"
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

    using RBMtype = BBRBMTypeTraits;
    using RBM = RBM<RBMtype>;
    using potentialType = RBMtype::potentialType;
    using connectionType = RBMtype::connectionType;
    using biasType = RBMtype::biasType;
    
    const std::string dirpass = argv[1];

    std::ofstream ftest(dirpass + "dump.dat");
  
    ftest << "totalLearningStep " << argv[2] << std::endl;
    ftest << "totalOutputStep " << argv[3] << std::endl;
    
    const std::size_t totalLearningStep = std::atoi(argv[2]);
    const std::size_t totalOutputStep = std::atoi(argv[3]);
    //input指定
    const std::string inputFileName = dirpass + "input.csv";

    //output指定
    const std::string outputTeacherData = dirpass + "teacherData.dat";
    const std::string outputPotentialFileName = dirpass + "answer.dat";
    const std::string outputConnectionFileName = dirpass + "connection.dat";
    const std::string differentiationLogLikelihood =
	dirpass + "differentiation.dat";
    const std::size_t dumpstep = totalLearningStep / 100;
  
    ftest << "dataNum " << dataNum << std::endl;
    ftest << "miniBatchSmpleNum " << miniBatchSampleNum << std::endl;
    ftest << "totalBatchNum " << totalBatchNum << std::endl;
    ftest << "ε " << epsilon << " λ " << lambda << " μ " << myu << std::endl;
  
    std::ofstream fout(outputPotentialFileName);
    std::ofstream fConnection(outputConnectionFileName);
    std::ofstream fteacher(outputTeacherData);
    std::ofstream fdiffLogLikely(differentiationLogLikelihood);

    //inputを読み込んでいる
    matrix<potentialType>
	sample = getDataVector<potentialType>(inputFileName);
  
    ftest << "sample" << std::endl << sample << std::endl;
  
    std::mt19937 mt(0);
    std::uniform_int_distribution<std::size_t>
	randSampleKind(0,sample.size() - 1);
    std::uniform_int_distribution<std::size_t>
	randMiniBatchNum(0,totalBatchNum - 1);
    std::uniform_int_distribution<int> randbool(0,1);

    //教師データ集合teacherを用意する
    tensor<potentialType> teacher; //miniBatchの集合
    teacher.reserve(totalBatchNum);
    matrix<potentialType> miniBatch(miniBatchSampleNum);

    //サンプルからminiBatchを生成する
    for(std::size_t j = 0; j < totalBatchNum; ++j){
	for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
	    miniBatch.at(i) = sample.at(randSampleKind(mt));
	}
	teacher.emplace_back(miniBatch);
    }

    fteacher << "teacher-data" << std::endl << teacher << std::endl;
    fteacher.close();
    std::cout << "setup teacher-data compleated." << std::endl;

    //RBMのメンバ関数を使う前に必要なstaticメンバ変数を初期化しておく
    RBM::RBMstaticGenerate(sample.at(0).size(),0);//ConnectionとBiasを生成
    ftest << "Connection Matrix" <<
	std::endl << RBM::connectionMatrix; 
    ftest << "Bias Matrix" << std::endl << RBM::bias;
    
    ////ミニバッチごとの可視層のpotentialの平均値を計算し、dataVmeansに格納
    matrix<double> dataVmeans(totalBatchNum);
    for(std::size_t batchNum = 0; batchNum < totalBatchNum; ++batchNum){
	dataVmeans.at(batchNum) =
	    RBM::batchDataMeanCalculateV(teacher.at(batchNum));
    }

    std::cout << "calculate dataMeans compleated." << std::endl;

    //ミニバッチのサンプル数だけRBMを用意する
    vector<std::shared_ptr<RBM>> RBMptrs;
    for(std::size_t i = 0; i < miniBatchSampleNum; ++i){
	//ランダムに選んだMiniBatchの各要素を初期状態としてRBMを生成
	std::shared_ptr<RBM>
	    RBMptr(new RBM(teacher.at(randMiniBatchNum(mt)).at(i)));
	RBMptrs.emplace_back(RBMptr);
    }
    std::cout << "setup RBMs compleated." << std::endl;
    
    ////////////////////connectionMatrixの学習////////////////////
    matrix<connectionType> deltaConnection,oldDeltaConnection;
    matrix<biasType> deltaBias,oldDeltaBias;
    matrix<double> dataVHmeans,rbmVHmeans;
    vector<int> rbmVsums;
    vector<double> dataHmeans,rbmHmeans;
    deltaBias.resize(2);
    for(std::size_t i = 0; i < RBM::totalNodeNum; ++i){
	vector<connectionType> tempvector(RBM::totalNodeNum,0);
	oldDeltaConnection.emplace_back(tempvector);
    }
    for(std::size_t i = 0; i < 2; ++i){
	vector<biasType> tempvector(RBM::totalNodeNum,0);
	oldDeltaBias.emplace_back(tempvector);
    }

    //更新ループ
    for(std::size_t learningStep = 0; learningStep < totalLearningStep; ++learningStep){
	std::size_t miniBatchNum = randMiniBatchNum(mt);
	dataVHmeans = RBM::batchDataMeanCalculateVH(teacher.at(miniBatchNum));
	dataHmeans = RBM::batchDataMeanCalculateH(teacher.at(miniBatchNum));

	//rbmVHmeans,rbmVsums,rbmHmeansを初期化
	RBMptrs.at(0)->timeEvolution();
	rbmVHmeans = RBM::calculateVH((RBMptrs.at(0)->getPotential()).at(0));
	rbmVsums = RBMptrs.at(0)->getPotential().at(0);
	rbmHmeans = RBM::calculateH((RBMptrs.at(0)->getPotential()).at(0));

	//ConnectionとBiasをdump
	if(learningStep % dumpstep == 0){
	    ftest << "Connection Matrix " << std::endl <<
		RBM::connectionMatrix; 
	    ftest << "Bias Matrix" << std::endl << RBM::bias;
	}

	////////////////////各RBMについて期待値をサンプリング////////////////////
	for(std::size_t rbmNum = 1; rbmNum < miniBatchSampleNum; ++rbmNum)
	{
	    RBMptrs.at(rbmNum)->timeEvolution();
	    rbmVHmeans =
		rbmVHmeans +
		RBM::calculateVH((RBMptrs.at(rbmNum)->getPotential()).at(0));
	    rbmVsums =
		rbmVsums + RBMptrs.at(rbmNum)->getPotential().at(0);
	    rbmHmeans =
		rbmHmeans +
		RBM::calculateH((RBMptrs.at(rbmNum)->getPotential()).at(0));
	}
	////////////////////各RBMについて期待値をサンプリング(ここまで)////////////////////
	
	vector<double> rbmVmeans(rbmVsums.begin(), rbmVsums.end());
	rbmVHmeans = rbmVHmeans / static_cast<double>(miniBatchSampleNum);
	rbmVmeans = rbmVmeans / static_cast<double>(miniBatchSampleNum);
	rbmHmeans = rbmHmeans / static_cast<double>(miniBatchSampleNum);

	matrix<double> diffByWLogLikelyMatrix = dataVHmeans - rbmVHmeans;
	vector<double> diffByALogLikelyVector =
	    dataVmeans.at(miniBatchNum) - rbmVmeans;
	vector<double> diffByBLogLikelyVector = dataHmeans - rbmHmeans;

	////////////////////対数尤度微分の評価////////////////////
	//対数尤度のConnectionでの微分を全結合についてsumする
	vector<double> tempsum;
	if(learningStep % 10 == 0){
	    for(auto itr = diffByWLogLikelyMatrix.begin();
		itr != diffByWLogLikelyMatrix.end(); ++itr){
		tempsum.emplace_back(
		    std::accumulate((*itr).begin(), (*itr).end(), 0.0));
	    }
	    double diffByWLogLikelySum(
		std::accumulate(tempsum.begin(), tempsum.end(), 0.0));

	    //対数尤度のBiasでの微分を全結合についてsumする	    
	    double diffByALogLikelySum(
		std::accumulate(diffByALogLikelyVector.begin(),
				diffByALogLikelyVector.end(), 0.0));
	    double diffByBLogLikelySum(
		std::accumulate(diffByBLogLikelyVector.begin(),
				diffByBLogLikelyVector.end(), 0.0));
	    //出力
	    fdiffLogLikely << learningStep << " " << diffByWLogLikelySum <<
		" " << diffByALogLikelySum << " " << diffByBLogLikelySum <<
		std::endl;
	}
	////////////////////対数尤度微分の評価(ここまで)////////////////////
	
	deltaConnection = epsilon*(diffByWLogLikelyMatrix);
	deltaBias.at(0) = epsilon*(diffByALogLikelyVector);
	deltaBias.at(1) = epsilon*(diffByBLogLikelyVector);

	////////////////////正規化とモメンタム////////////////////
	deltaConnection = deltaConnection - lambda * RBM::connectionMatrix;
	deltaConnection = deltaConnection + myu * oldDeltaConnection;
	deltaBias = deltaBias + myu * oldDeltaBias;
	////////////////////正規化とモメンタム(ここまで)////////////////////
	
	if(learningStep % dumpstep == 0){
	    ftest << "dataVmeans" << std::endl << dataVmeans.at(miniBatchNum) <<
		std::endl;
	    ftest << "dataHmeans" << std::endl << dataHmeans << std::endl;
	    ftest << "rbmVmeans" << std::endl << rbmVmeans << std::endl;
	    ftest << "rbmHmeans" << std::endl << rbmHmeans << std::endl;
	}

	if(learningStep % dumpstep == 0){    
	    ftest << "deltaConnection" << std::endl << deltaConnection;
	    ftest << "deltaBias" << std::endl << deltaBias;
	}
        
	////////////////////ConnectionとBiasの更新////////////////////
	RBM::setConnectionMatrix(RBM::connectionMatrix +
						  deltaConnection);
	RBM::setBias(RBM::bias + deltaBias);
	////////////////////ConnectionとBiasの更新(ここまで)////////////////////
	
	oldDeltaConnection = deltaConnection;
	oldDeltaBias = deltaBias;
    
	std::cout << learningStep << std::endl;
    }

    std::cout << "learning compleated." << std::endl;

    //学習結果のデータ群を出力する
    vector<int> initialValue(RBM::totalNodeNum);
    for(auto itr = initialValue.begin(); itr != initialValue.end(); ++itr){
	*itr = randbool(mt);
    }


    fConnection << RBM::connectionMatrix;
    fConnection.close();

    RBM motherRBM(initialValue);
    std::size_t outputculum = static_cast<std::size_t>(
	std::sqrt(RBM::totalNodeNum));
    std::size_t outputraw = outputculum;
    assert(RBM::totalNodeNum == outputculum * outputraw);
    vector<double> meanPotential(RBM::totalNodeNum,0);
    for(std::size_t i = 0; i < totalOutputStep; ++i){
	motherRBM.timeEvolution();
	if((i % (totalOutputStep / outputPotentialNum)) == 0){
	    vector<potentialType> outputPotential = motherRBM.getPotential().at(0);
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
