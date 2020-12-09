#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

	typedef struct cv_Mat cv_Mat;

	#ifdef CARMODELSDK_EXPORTS
	#define CARMODELSDK_API __declspec(dllexport)
	#else
	#define CARMODELSDK_API __declspec(dllimport)
	#endif


	#include "CarDataStruct.h"
	

	CARMODELSDK_API int32_t __stdcall
		Test(
			int myInt
		);

	CARMODELSDK_API int32_t __stdcall
		SetFileDirectoryIn(	const char* path );

	CARMODELSDK_API int32_t __stdcall
		SetFileDirectoryOut( const char* path );

	CARMODELSDK_API int32_t __stdcall
		GetFileDirectoryIn(StringStruct * text);

	CARMODELSDK_API int32_t __stdcall
		GetFileDirectoryOut(StringStruct * text);


	CARMODELSDK_API int32_t __stdcall
		InitRecognizer();

	CARMODELSDK_API int32_t __stdcall
		GetRecoCarMarkAddress(int64_t * num);

	CARMODELSDK_API int32_t __stdcall
		GetRecoCarTypeAddress(int64_t * num);


	CARMODELSDK_API int32_t __stdcall
		SetRecoCountLim(
			int cntLim);

	CARMODELSDK_API int32_t __stdcall
		SetRecognizeMode(
			int mode);


	CARMODELSDK_API int32_t __stdcall
		SetRecognizeOptions(
			int recognBorder_prc,
			int latitude_deg,
			int offsetDayTime_min,
			int offsetTimeZone_min
		);

	CARMODELSDK_API int32_t __stdcall
		SetOutFileNamesFormat(
			int filename,
			int datetime,
			int name,
			int mark,
			int modl,
			int genr,
			int markProb,
			int type,
			int side,
			int typeProb,
			int filenam2
		);

	CARMODELSDK_API int32_t __stdcall
		SetSaveOptions(
			int filesToDisk,
			int markRect,
			int typeRect,
			int saveRecognized,
			int saveUnrecognized,
			int removeOldFiles,
			int renameNewFiles
		);


	CARMODELSDK_API int32_t __stdcall
		CalcSunrizeCpp_mins(
			int mon, int day);

	CARMODELSDK_API int32_t __stdcall
		CalcSunsetCpp_mins(
			int mon, int day);


	CARMODELSDK_API int32_t __stdcall
		CalcSunrizeCpp_mins_full(
			int mon, int day,
			int latitude_deg, int offsetDayTime_min, int offsetTimeZone_min);

	CARMODELSDK_API int32_t __stdcall
		CalcSunsetCpp_mins_full(
			int mon, int day,
			int latitude_deg, int offsetDayTime_min, int offsetTimeZone_min);


	CARMODELSDK_API int32_t __stdcall
		RecognizeImgCpp(
			CarDataStruct* answer,
			cv_Mat* img,
			const char* fileNameIn);

	CARMODELSDK_API int32_t __stdcall
		RecognizeImageByteArrCpp(
			CarDataStruct* answer,
			char* buf,
			long bufLen,
			const char* fileNameIn);

	CARMODELSDK_API int32_t __stdcall
		RecognizeFileCpp(
			CarDataStruct* answer,
			const char* fileNameIn);

	CARMODELSDK_API int32_t __stdcall
		RecognizeImageByteArrCpp_thread(
			CarDataStruct* answer,
			char* buf,
			long bufLen,

			int mode,
			int recognizeBorder_prc);


	// ?
	CARMODELSDK_API int32_t __stdcall
		RecognizeDirCpp();

#ifdef __cplusplus
}
#endif