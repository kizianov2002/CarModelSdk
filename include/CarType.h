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

			class ANGELVISIO_API CarTypeData
			{
			public:
				string Name;
				string Side;
				Rect Position;
				float Probability;

				CarTypeData();
				CarTypeData(string name, string side, Rect position, float probability);

			private:

			};


			class ANGELVISIO_API CarTypeRecognizer
			{
			public:
				CarTypeRecognizer();
				void Recogniz(const Mat& img, vector<CarTypeData>& data);

			private:
				CNTKFunc func;
				Net net;
				vector<string> names;
			};
		}
	}
}