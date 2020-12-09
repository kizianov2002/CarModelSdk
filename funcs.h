#pragma once


#include <iostream>
#include <cstdint>

#include "include/opencv2/opencv.hpp"
#include "include/opencv2/imgproc/imgproc.hpp"
#include "include/opencv2/highgui/highgui.hpp"

#include "include/CarModel.h"
#include "include/CarType.h"
#include "CarDataStruct.h"

#include "out.h"

#include <iostream>
#include <iomanip>
#include <ctime>


using namespace std;
using namespace cv;
namespace av = Angel::Vision::CarModel;


bool CopyMarkData(const av::CarModelData modelData, CarDataStruct* dest);
bool CopyTypeData(const av::CarTypeData typeData, CarDataStruct* dest);
bool CopyMarkName(const char* text, CarDataStruct* dest);

bool CopyStringData(const string source, StringStruct* dest);

void Str2Char(const char* in, char* out);
void ClearChar(char* out);

std::string GetFileFromPath(std::string path);
std::string GetFileNameFromPath(std::string path);
std::string GetFileExtFromPath(std::string path);
std::string GetFileNoExtFromPath(std::string path);

std::string CurrentDateTimeString();

std::string SaveImage(
	const char* fileName_c, Mat img, 
	bool remOld, bool renNew);

int CalcPloarDayLen_days(double latitudeDeg);
int CalcPloarNightLen_days(double latitudeDeg);
double CalcDarkTimeSummer_mins(double latitudeDeg);
double CalcDarkTimeWinter_mins(double latitudeDeg);

//double CalcDarkTime_mins(
//	int DarkTimeWinter_min, int DarkTimeSummer_min,
//	int month, int day, int hour, int min,
//	int offsetDayTime, int offsetTimeZone);

double CalcSunrize_mins(
	int mon, int day,
	double latitudeDeg, 
	int offsetDayTime, int offsetTimeZone);

double CalcSunset_mins(
	int mon, int day,
	double latitudeDeg, 
	int offsetDayTime, int offsetTimeZone);