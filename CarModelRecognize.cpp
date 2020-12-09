#include "pch.h"
#include "CarModelRecognize.h"

//#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <time.h>
#include <math.h>
#include <filesystem>
#include <windows.h>

#include <cstdio>
#include <chrono>
#include <ctime>  

#include "Funcs.h"
#include "out.h"
#include "Timer.h"
using namespace Angel::Vision::CarModel;

// /////////////////////////////// //
// CAST C-POINTER TO COMPLEX TYPES //
// /////////////////////////////// //

template<typename>
constexpr static auto dependent_false = false;

template<typename T>
auto& castPtrToCvMat(T* source) {
	if constexpr (std::is_same_v<T, cv_Mat>) {
		return *(static_cast<cv::Mat*>(static_cast<void*>(source)));
	}
	else {
		static_assert (dependent_false<T>, "No type");
	}
}

// /////////////////////////////// //


using namespace std;
using namespace cv;
namespace av = Angel::Vision::CarModel;


av::CarModelRecognizer* RecoCarMark = NULL;
int RecoCarMark_cnt = 0;
av::CarTypeRecognizer* RecoCarType = NULL;
int RecoCarType_cnt = 0;

std::string FileDirectoryIn = "c:\\Temp\\In";
std::string FileDirectoryOut = "c:\\Temp\\Out";

int RecoCountLim = 10000;

int Mode = 3;
int RecognizeBorder_prc = 90;
int Latitude_deg = 45;
int OffsetDayTime_min = 45;
int OffsetTimeZone_min = 0;

bool OutNameAddFileName = true;
bool OutNameAddDateTime = false;
bool OutNameAddName = true;
bool OutNameAddMark = false;
bool OutNameAddModl = false;
bool OutNameAddGenr = true;
bool OutNameAddMarkProb = true;
bool OutNameAddType = true;
bool OutNameAddSide = true;
bool OutNameAddTypeProb = true;
bool OutNameAddFileNam2 = true;

bool OutMarkRect = false;
bool OutTypeRect = false;

bool FilesToDisk = false;
bool SaveRecognized = false;
bool SaveUnrecognized = false;
bool SaveRemoveOld = false;
bool SaveRenameNew = false;


std::int32_t
Test(std::int32_t myInt) {
	return myInt;
}

std::int32_t
InitRecognizer() {
	int res = 0;

	if (Mode < 0)  Mode = 0;
	if (Mode > 3)  Mode = 3;

	if (Mode & 1)
	{
		delete RecoCarMark;
		RecoCarMark = NULL;
		RecoCarMark_cnt = 0;

		try
		{
			RecoCarMark = new av::CarModelRecognizer;
		}
		catch (Exception e)
		{
			cout << e.msg;
		}
	} 	
	res += (RecoCarMark == NULL ? 0 : 1);

	if (Mode & 2) 
	{
		delete RecoCarType;
		RecoCarType = NULL;
		RecoCarType_cnt = 0;

		try
		{
			RecoCarType = new av::CarTypeRecognizer;
		}
		catch (Exception e)
		{
			cout << e.msg;
		}
	}	
	res += (RecoCarType == NULL ? 0 : 1);

	return res;
}

std::int32_t
SetFileDirectoryIn(const char* path_c) {
	FileDirectoryIn = path_c;

	return 1;
}

std::int32_t
SetFileDirectoryOut(const char* path_c) {
	FileDirectoryOut = path_c;

	return 1;
}

std::int32_t
GetFileDirectoryIn(StringStruct* text) {
	bool res = CopyStringData(FileDirectoryIn, text);

	return res ? 1 : 0;
}

std::int32_t
GetFileDirectoryOut(StringStruct* text) {
	bool res = CopyStringData(FileDirectoryOut, text);

	return res ? 1 : 0;
}

std::int32_t
GetRecoCarMarkAddress(std::int64_t* num) {
	(*num) = (std::int64_t)(RecoCarMark);

	return 1;
}

std::int32_t
GetRecoCarTypeAddress(std::int64_t* num) {
	(*num) = (std::int64_t)(RecoCarType);

	return 1;
}


std::int32_t
SetRecoCountLim(
	std::int32_t cntLim)
{
	if (cntLim < 100)  cntLim = 100;
	if (cntLim > 1000000)  cntLim = 100000;
	RecoCountLim = cntLim;

	return 1;
}

std::int32_t
SetRecognizeMode(
	std::int32_t mode)
{
	if (mode < 0)  mode = 0;
	if (mode > 3)  mode = 3;
	Mode = mode;

	return 1;
}


std::int32_t
SetRecognizeOptions(
	std::int32_t recognBorder_prc,
	std::int32_t latitude_deg,
	std::int32_t offsetDayTime_min,
	std::int32_t offsetTimeZone_min)
{
	if (recognBorder_prc < 50)  recognBorder_prc = 50;
	if (recognBorder_prc > 95)  recognBorder_prc = 95;
	RecognizeBorder_prc = recognBorder_prc;

	if (latitude_deg < 10)  latitude_deg = 10;
	if (latitude_deg > 80)  latitude_deg = 80;
	Latitude_deg = latitude_deg;

	if (offsetDayTime_min < -120)  offsetDayTime_min = -120;
	if (offsetDayTime_min > +120)  offsetDayTime_min = +120;
	OffsetDayTime_min = offsetDayTime_min;

	if (offsetTimeZone_min < -60)  offsetTimeZone_min = -60;
	if (offsetTimeZone_min > +60)  offsetTimeZone_min = +60;
	OffsetTimeZone_min = offsetTimeZone_min;

	return 1;
}

std::int32_t
SetOutFileNamesFormat(
	std::int32_t filename,
	std::int32_t datetime,
	std::int32_t name,
	std::int32_t mark,
	std::int32_t modl,
	std::int32_t genr,
	std::int32_t markProb,
	std::int32_t type,
	std::int32_t side,
	std::int32_t typeProb,
	std::int32_t filenam2)
{
	OutNameAddFileName = (filename > 0);
	OutNameAddDateTime = (datetime > 0);
	OutNameAddName = (name > 0);
	OutNameAddMark = (mark > 0);
	OutNameAddModl = (modl > 0);
	OutNameAddGenr = (genr > 0);
	OutNameAddMarkProb = (markProb > 0);
	OutNameAddType = (type > 0);
	OutNameAddSide = (side > 0);
	OutNameAddTypeProb = (typeProb > 0);
	OutNameAddFileNam2 = (filenam2 > 0);

	return 1;
}

std::int32_t
SetSaveOptions(
	int filesToDisk,
	int markRect,
	int typeRect,
	int saveRecognized,
	int saveUnrecognized,
	int removeOldFiles,
	int renameNewFiles)
{
	FilesToDisk = (filesToDisk > 0);
	OutMarkRect = (markRect > 0);
	OutTypeRect = (typeRect > 0);
	SaveRecognized = (saveRecognized > 0);
	SaveUnrecognized = (saveUnrecognized > 0);
	SaveRemoveOld = (removeOldFiles > 0);
	SaveRenameNew = (renameNewFiles > 0);

	//cout << "FilesToDisk = " << filesToDisk << " первая инициализация" << std::endl;

	return 1;
}



std::int32_t
CalcSunrizeCpp_mins(
	int mon, int day) {

	//cout << "c++ CalcSunrizeCpp_mins() " << endl;
	//cout << "* mon = " << mon << endl;
	//cout << "* day = " << day << endl;
	//cout << "+ Latitude_deg = " << Latitude_deg << endl;
	//cout << "+ OffsetDayTime_min = " << OffsetDayTime_min << endl;
	//cout << "+ OffsetTimeZone_min = " << OffsetTimeZone_min << endl;

	int res = (int)CalcSunrize_mins(mon, day, (double)Latitude_deg, OffsetDayTime_min, OffsetTimeZone_min);
	return res;
}

std::int32_t
CalcSunsetCpp_mins(
	int mon, int day) {

	//cout << "c++ CalcSunsetCpp_mins() " << endl;
	//cout << "+ mon = " << mon << endl;
	//cout << "+ day = " << day << endl;
	//cout << "+ Latitude_deg = " << Latitude_deg << endl;
	//cout << "+ OffsetDayTime_min = " << OffsetDayTime_min << endl;
	//cout << "+ OffsetTimeZone_min = " << OffsetTimeZone_min << endl;

	int res = (int)CalcSunset_mins(mon, day, (double)Latitude_deg, OffsetDayTime_min, OffsetTimeZone_min);
	return res;
}



std::int32_t
CalcSunrizeCpp_mins_full(
	int mon, int day,
	int latitude_deg, int offsetDayTime_min, int offsetTimeZone_min) {

	//cout << "c++ CalcSunrizeCpp_mins() " << endl;
	//cout << "- mon = " << mon << endl;
	//cout << "- day = " << day << endl;
	//cout << "- latitude_deg = " << latitude_deg << endl;
	//cout << "- offsetDayTime_min = " << offsetDayTime_min << endl;
	//cout << "- offsetTimeZone_min = " << offsetTimeZone_min << endl;

	int res = (int)CalcSunrize_mins(mon, day, (double)latitude_deg, offsetDayTime_min, offsetTimeZone_min);
	return res;
}

std::int32_t
CalcSunsetCpp_mins_full(
	int mon, int day,
	int latitude_deg, int offsetDayTime_min, int offsetTimeZone_min) {

	//cout << "c++ CalcSunsetCpp_mins() " << endl;
	//cout << "- mon = " << mon << endl;
	//cout << "- day = " << day << endl;
	//cout << "- latitude_deg = " << latitude_deg << endl;
	//cout << "- offsetDayTime_min = " << offsetDayTime_min << endl;
	//cout << "- offsetTimeZone_min = " << offsetTimeZone_min << endl;

	int res = (int)CalcSunset_mins(mon, day, (double)latitude_deg, offsetDayTime_min, offsetTimeZone_min);
	return res;
}


std::int32_t
RecognizeImgCpp(
	CarDataStruct* answer,
	cv_Mat* img,
	const char* fileNameIn)
{
	string fileName = (fileNameIn[0] == 0 ? "-no-name-file-" : fileNameIn);
	string fileNameOut = "";
	string filePathOut = "";

	//-------------
	// Out& out = Out::instance();
	//out.lvlUp();
	//-------------

	int markProb_prc = 0;
	int typeProb_prc = 0;
	vector<av::CarModelData> markVec;
	vector<av::CarTypeData> typeVec;

	int32_t cnt = 0;

	if (castPtrToCvMat(img).empty())
	{
		// cout << " - ERROR: img.empty() - " << std::endl;
		//out.lvlDn();
		return 0;
	}

	if ( (Mode & 1 )
		&& ( RecoCarMark == NULL
		   || RecoCarMark_cnt >= RecoCountLim
		  )
		)
	{
		//Timer t;
		delete RecoCarMark;
		RecoCarMark = new av::CarModelRecognizer();
		RecoCarMark_cnt = 0;

		if (RecoCarMark == NULL)
		{
			// cout << " - ERROR: RecoCarMark == NULL - " << std::endl;
			//out.lvlDn();
			return 0;
		}
		/*if (RecoCarType_cnt % 200 == 0) 
			cout <<  " - Create RecoCarMark : - " << (int)(t.elapsed()*1000.0);*/

		//cout << " - RECO CAR MARK INITIALIZE - " << std::endl;
	}

	if ((Mode & 1 )
		&& RecoCarMark != NULL)
	{
		Timer t;
		RecoCarMark->Recogniz(castPtrToCvMat(img), markVec);
		RecoCarMark_cnt++;
		// cout <<  " - RecoCarMark->Recogniz() - " << (int)(t.elapsed()*1000.0);

		//Timer tr;
		if (markVec.size() > 0) {
			CopyMarkData(markVec.front(), answer);
			markProb_prc = (int)(markVec.front().Probability * 100);
		}
		/*if (RecoCarType_cnt % 200 == 0)
			cout <<  " - CopyMarkData : - " << (int)(tr.elapsed() * 1000.0);*/
	}

	if ((Mode & 2 )
		&& ( RecoCarType == NULL
			|| RecoCarType_cnt >= RecoCountLim
		))
	{
		//Timer t;
		delete RecoCarType;
		RecoCarType = new av::CarTypeRecognizer();
		RecoCarType_cnt = 0;

		if (RecoCarType == NULL)
		{
			// cout << " - ERROR: RecoCarType == NULL - " << std::endl;
			//out.lvlDn();
			return 0;
		}
		/*if (RecoCarType_cnt % 200 == 0)
			cout <<  " - Create RecoCarType : - " << (int)(t.elapsed()*1000.0);*/

		//cout << " - RECO CAR TYPE INITIALIZE - " << std::endl;
	}

	if ((Mode & 2 )
		&& RecoCarType != NULL)
	{
		//Timer t;
		RecoCarType->Recogniz(castPtrToCvMat(img), typeVec);
		/*if (RecoCarType_cnt % 200 == 0)
			cout <<  " - RecoCarType->Recogniz() - " << (int)(t.elapsed()*1000.0);*/

		RecoCarType_cnt++;

		//Timer tr;
		if (typeVec.size() > 0) {
			CopyTypeData(typeVec.front(), answer);
			typeProb_prc = (int)(typeVec.front().Probability * 100);
		}
		/*if (RecoCarType_cnt % 200 == 0)
			cout <<  " - CopyTypeData : - " << (int)(tr.elapsed() * 1000.0);*/
	}


	//Timer tName;

	stringstream fileNameOut_str;
	bool f = false;
	if (OutNameAddFileName && fileName[0] != 0) {
		fileNameOut_str << (f ? " - " : "");
		f = true;
		fileNameOut_str << fileName;
	}
	if (OutNameAddDateTime) {
		fileNameOut_str << (f ? " - " : "");
		f = true;
		fileNameOut_str << CurrentDateTimeString();
	}


	bool f_rec = false;

	//int ttMarkRect = 0;
	if (markVec.size() > 0
		&& markProb_prc >= RecognizeBorder_prc)
	{
		f_rec = true;
		cnt++;

		if (OutMarkRect) {
			//Timer tMarkRect;
			// cout << "[] mark_rect (" << markVec.front().Position.x << ", " << markVec.front().Position.y << ", " << markVec.front().Position.width << ", " << markVec.front().Position.height << std::endl;

			cv::Rect rect = cv::Rect(
				markVec.front().Position.x,
				markVec.front().Position.y,
				markVec.front().Position.width, 
				markVec.front().Position.height);
			cv::rectangle(castPtrToCvMat(img), rect, Scalar(255, 0, 255), 2);
			/*if (RecoCarType_cnt % 200 == 0)
				ttMarkRect = (int)(tMarkRect.elapsed() * 1000.0);*/
		}

		if (OutNameAddName && answer != NULL && answer->Name[0] != 0) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << answer->Name;
		}
		if (OutNameAddMark && answer != NULL && answer->Mark[0] != 0) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << answer->Mark;
		}
		if (OutNameAddModl && answer != NULL && answer->Modl[0] != 0) {
			fileNameOut_str << (f ? " " : "");
			f = true;
			fileNameOut_str << answer->Modl;
		}
		if (OutNameAddGenr && answer != NULL && answer->Genr[0] != 0) {
			fileNameOut_str << (f ? " " : "");
			f = true;
			fileNameOut_str << answer->Genr;
		}
		if (OutNameAddMarkProb) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << markProb_prc << "%";
		}
	}
	else {
		if (OutNameAddName) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << "UNRECOGNIZED";
		}
		if (OutNameAddMarkProb) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << markProb_prc << "%";
		}
		f_rec = false;
	}
	/*if (RecoCarType_cnt % 200 == 0)
		cout <<  " - draw MarkRect : - " << ttMarkRect;*/

	//int ttTypeRect = 0;
	if (typeVec.size() > 0
		&& typeProb_prc >= RecognizeBorder_prc)
	{
		f_rec = true;
		cnt++;

		if (OutTypeRect) {
			//Timer tTypeRect;
			
			cv::Rect rect = cv::Rect(
				typeVec.front().Position.x,
				typeVec.front().Position.y,
				typeVec.front().Position.width, 
				typeVec.front().Position.height);
			cv::rectangle(castPtrToCvMat(img), rect, Scalar(255, 128, 128), 2);
			/*if (RecoCarType_cnt % 200 == 0)
				ttTypeRect = (int)(tTypeRect.elapsed() * 1000.0);*/
		}

		if (OutNameAddType && answer != NULL && answer->Type[0] != 0) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << answer->Type;
		}
		if (OutNameAddSide && answer != NULL && answer->Side[0] != 0) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << answer->Side;
		}
		if (OutNameAddTypeProb) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << typeProb_prc << "%";
		}
	}
	else {
		if (OutNameAddName) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << "UNRECOGNIZED";
		}
		if (OutNameAddMarkProb) {
			fileNameOut_str << (f ? " - " : "");
			f = true;
			fileNameOut_str << typeProb_prc << "%";
		}
		f_rec = false;
	}
	/*if (RecoCarType_cnt % 200 == 0)
		cout <<  " - draw TypeRect : - " << ttTypeRect;*/

	if (OutNameAddFileNam2 && fileName[0] != 0) {
		fileNameOut_str << (f ? " - " : "");
		f = true;
		fileNameOut_str << fileName;
	}

	fileNameOut = fileNameOut_str.str();

	stringstream filePathOut_str;
	filePathOut_str << FileDirectoryOut << "\\" << fileNameOut << ".jpg";
	filePathOut = filePathOut_str.str();

	/*if (RecoCarType_cnt % 200 == 0)
		cout <<  " - generate FileName : - " << (int)(tName.elapsed()*1000.0);*/

	if (FilesToDisk) {
		if ((!f_rec && SaveUnrecognized)
			|| (f_rec && SaveRecognized)) {

			string resPath = SaveImage(filePathOut.c_str(), castPtrToCvMat(img), SaveRemoveOld, SaveRenameNew);

			if (resPath.size() > 0) {
				Str2Char(resPath.c_str(), answer->FileFullName);
			}
		}
		else {
			//out.print(" - not need to save the file - ");
		}
	}

	//out.lvlDn();
	// cout << "return " << cnt << std::endl << std::endl << std::endl;
	return cnt;
}


std::int32_t
RecognizeImgCpp_thread(
	CarDataStruct* answer,
	cv_Mat* img,

	int mode,
	int recognizeBorder_prc)
{
	if (castPtrToCvMat(img).empty())
	{
		// cout << " - ERROR: img.empty() - " << std::endl;
		return 0;
	}

	av::CarModelRecognizer* recoCarMark = NULL;
	av::CarTypeRecognizer* recoCarType = NULL;

	int markProb_prc = 0;
	vector<av::CarModelData> markVec;
	int typeProb_prc = 0;
	vector<av::CarTypeData> typeVec;

	int32_t cnt = 0;

	if ((mode & 1) ) {
		av::CarModelRecognizer* recoCarMark = new av::CarModelRecognizer();
		if (recoCarMark == NULL)
		{
			// cout << " - ERROR: RecoCarMark == NULL - " << std::endl;
			return 0;
		}

		recoCarMark->Recogniz(castPtrToCvMat(img), markVec);
		RecoCarMark_cnt++;

		if (markVec.size() > 0) {
			CopyMarkData(markVec.front(), answer);

			markProb_prc = (int)(markVec.front().Probability * 100);
			// cout << "typeProb_prc = " << typeProb_prc << "%" << std::endl;

			if (markProb_prc >= recognizeBorder_prc) {
				cnt++;
			}
		}
		delete recoCarMark;
	}

	if ((mode & 2) ) {
		// cout << "mode & 2 > 0" << std::endl;

		av::CarTypeRecognizer* recoCarType = new av::CarTypeRecognizer();
		if (recoCarType == NULL)
		{
			// cout << " - ERROR: recoCarType == NULL - " << std::endl;
			return 0;
		}

		recoCarType->Recogniz(castPtrToCvMat(img), typeVec);
		RecoCarType_cnt++;

		if (typeVec.size() > 0) {
			CopyTypeData(typeVec.front(), answer);

			typeProb_prc = (int)(typeVec.front().Probability * 100);
			// cout << "typeProb_prc = " << typeProb_prc << "%" << std::endl;

			if (typeProb_prc >= recognizeBorder_prc) {
				cnt++;
			}
		}
		delete recoCarType;
	}

	return cnt;
}

std::int32_t
RecognizeImgCpp(
	CarDataStruct* answer,
	cv_Mat* img)
{
	//cout << "CarModelSDK -> RecognizeImgCpp()" << std::endl;
	return RecognizeImgCpp(answer, img, "");
}

std::int32_t
RecognizeImageByteArrCpp(
	CarDataStruct* answer,
	char* buf,
	long bufLen,
	const char* fileNameIn)
{ 
	//Timer t0;
	cv::Mat matImg = cv::imdecode(cv::Mat(1, bufLen, CV_8UC1, buf), CV_LOAD_IMAGE_UNCHANGED);
	cv_Mat* ptr = (cv_Mat*)&matImg;

	//Timer t1;
	std::int32_t res = RecognizeImgCpp(answer, ptr, fileNameIn);
	matImg.release();

	//if (RecoCarType_cnt % 200 == 0) {
	//	int tt1 = (int)(t1.elapsed() * 1000.0);
	//	int tt0 = (int)(t0.elapsed() * 1000.0) - tt1;
	//	cout <<  " - Create CvMat : - " << tt0 << " - RecognizeImgCpp() : - " << tt1 << endl;
	//}

	return res;
}

std::int32_t
RecognizeFileCpp(
	CarDataStruct* answer,
	const char* fileNameIn)
{
	//cout << "CarModelSDK -> RecognizeFileCpp()" << std::endl;
	int32_t cnt = 0;

	if (fileNameIn != "")
	{
		stringstream strPath;

		size_t nPos = std::string(fileNameIn).find(":", 0);
		if (nPos == string::npos)
			strPath << FileDirectoryIn << "\\" << fileNameIn;
		else
			strPath << fileNameIn;

		cv::String filePathIn = strPath.str();

		Mat img = imread(filePathIn, cv::IMREAD_COLOR);

		if (!img.empty()) {
			cnt = RecognizeImgCpp(answer, (cv_Mat*)&img, GetFileNameFromPath(fileNameIn).c_str());
		}
		else {
			//out.print("File load ERROR!");
		}
	}

	return cnt;
}



std::int32_t
RecognizeDirCpp()
{
	setlocale(LC_ALL, "Russian");

	wchar_t szFileFormat[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	string searchMask = FileDirectoryIn + "\\*.jpg";
	wstring wsearchMask = wstring(searchMask.begin(), searchMask.end());

	HANDLE hFind = FindFirstFileW(wsearchMask.c_str(), &FindFileData);

	int32_t cnt = 0;

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			wstring ws(FindFileData.cFileName);
			string filePathIn = GetFileFromPath(string(ws.begin(), ws.end()));

			CarDataStruct* answer = new CarDataStruct;

			cnt += RecognizeFileCpp(answer, filePathIn.c_str());

			delete answer;

		} while (FindNextFileW(hFind, &FindFileData) != 0);
	}
	FindClose(hFind);

	return cnt;
}



std::int32_t
RecognizeImageByteArrCpp_thread(
	CarDataStruct* answer,
	char* buf,
	long bufLen,

	int mode,
	int recognizeBorder_prc)
{
	cv::Mat matImg = cv::imdecode(cv::Mat(1, bufLen, CV_8UC1, buf), CV_LOAD_IMAGE_UNCHANGED);
	cv_Mat* ptr = (cv_Mat*)&matImg;

	std::int32_t res = RecognizeImgCpp_thread(
		answer, 
		ptr, 

		mode,
		recognizeBorder_prc);

	return res;
}
