#pragma once
#include <string>
#include <vector>

//using namespace std;

class CNTKFunc
{
public:
	CNTKFunc();
	CNTKFunc(std::string fileModel);

	void EvaluatF(std::vector<float>& data, std::vector<int>& result_class, std::vector<float>& result_weight);
	void EvaluatF(std::vector<float>& data, std::vector<std::vector<float>>& outputData);
	void EvaluatD(std::vector<float>& data, std::vector<int>& result_class, std::vector<float>& result_weight);

	~CNTKFunc();

private:
	void* model_ptr;

};
