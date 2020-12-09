#pragma once

#include "CNTKFunc.h"
#include <string.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#ifdef ANGELVISIONCARMODEL_EXPORTS
#define ANGELVISIO_API __declspec(dllexport) 
#else
#define ANGELVISIO_API __declspec(dllimport) 
#endif

namespace Angel {
	namespace Vision {
		namespace CarModel {
			using namespace cv::dnn;
			using namespace cv;
			using namespace std;

			class ANGELVISIO_API CarModelData
			{
			public:				
				string Name;
				string Mark;
				string Model;
				string Generation;
				Rect Position;
				float Probability;

				CarModelData();
				CarModelData(string name, string mark, string model, string generation, Rect position, float probability);

			private:

			};


			class ANGELVISIO_API CarModelRecognizer
			{
			public:
				CarModelRecognizer();
				void Recogniz(const Mat& img, vector<CarModelData>& data);
				void RecognizImg(const Mat& img, vector<CarModelData>& data);

			private:
				CNTKFunc func;
				Net net;
				vector<string> names;
			};
		}
	}
}