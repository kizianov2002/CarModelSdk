#include "pch.h"
#include "Funcs.h"

#include <ctime>

using namespace std;
using namespace cv;
namespace av = Angel::Vision::CarModel;


bool CopyMarkData(const av::CarModelData source, CarDataStruct* dest) {
	// cout << "CopyMarkData() ";

	Str2Char(source.Name.c_str(), dest->Name);
	Str2Char(source.Mark.c_str(), dest->Mark);
	Str2Char(source.Model.c_str(), dest->Modl);
	Str2Char(source.Generation.c_str(), dest->Genr);

	stringstream strstr;
	strstr << (int)(source.Probability * 100);
	string str = strstr.str();
	Str2Char(str.c_str(), dest->ProbMarkStr);

	ClearChar(dest->FileFullName);

	 //cout << "Mark = " << dest->Mark << ", Modl = " << dest->Modl << ", ProbMarkStr = " << dest->ProbMarkStr << std::endl;

	return true;
}


bool CopyTypeData(const av::CarTypeData typeData, CarDataStruct* dest) {
	// cout << "CopyTypeData() ";

	Str2Char(typeData.Name.c_str(), dest->Type);
	Str2Char(typeData.Side.c_str(), dest->Side);

	stringstream strstr;
	strstr << (int)(typeData.Probability * 100);
	string str = strstr.str();
	Str2Char(str.c_str(), dest->ProbTypeStr);

	ClearChar(dest->FileFullName);

	 //cout << "Type = " << dest->Type << ", Side = " << dest->Side << ", ProbTypeStr = " << dest->ProbTypeStr << std::endl;

	return true;
}


bool CopyCarName(const char* text, CarDataStruct* dest) {
	Str2Char(text, dest->Name);
	ClearChar(dest->Mark);
	ClearChar(dest->Modl);
	ClearChar(dest->Genr);
	ClearChar(dest->Type);
	ClearChar(dest->Side);

	ClearChar(dest->ProbMarkStr);
	ClearChar(dest->ProbTypeStr);

	ClearChar(dest->FileFullName);

	return true;
}


bool CopyStringData(const string source, StringStruct* dest) {
	Str2Char(source.c_str(), dest->Text);

	return true;
}


#include <sys/stat.h>
#include <algorithm>
#include <string>

string SaveImage(const char* fileName_c, Mat img, bool remOld, bool renNew) {
	// Out& out = Out::instance();
	// out.lvlUp();
	// -------------

	bool rem = false;
	bool res = false;
	std::string fileName = fileName_c;

	try {
		if (remOld)
			rem = std::remove(fileName_c);

		struct stat sb;
		int cnt = 0;
		std::string fname = GetFileNoExtFromPath(fileName_c);
		std::string fext = GetFileExtFromPath(fileName_c);
		while ( 
			!(stat(fileName.c_str(), &sb) == -1 
			&& errno == ENOENT) 
			)
		{
			cnt++;
			std::stringstream sstr;
			sstr << fname << " (" << cnt << ")." << fext;
			fileName = sstr.str();
			// out.print("  save try -", fileName);
		}
		res = cv::imwrite(fileName, img);
		// out.print("  save OK -", fileName);
	}
	catch (exception e) {
		// out.print("SaveImage EXCEPTION - ", e.what());
	}
	// -------------
	// out.lvlDn();

	return fileName;
}


void Str2Char(const char* in, char* out) {
	bool start = false;
	int pos = 0;

	for (int i = 0; i < 256; i++)
		if (in[i] == '\'')
		{
			start = true;
			out[pos] = '"';
			pos++;
		}
		else
			if (in[i] == 32)
			{
				start = true;
				out[pos] = ' ';
				pos++;
			}
		else
			if (in[i] == '!')
			{
				start = true;
				out[pos] = '#';
				pos++;
			}
		else
			if (in[i] == '>')
			{
				start = true;
				out[pos] = '#';
				pos++;
			}
		else
			if (in[i] == '<')
			{
				start = true;
				out[pos] = '#';
				pos++;
			}
		else
			if (in[i] == ':')
			{
				start = true;
				out[pos] = '-';
				pos++;
			}
		else
			if (in[i] == '?')
			{
				start = true;
				out[pos] = '#';
				pos++;
			}
		else
			if (in[i] == '*')
			{
				start = true;
				out[pos] = '#';
				pos++;
			}
		else
			if (in[i] == '|')
			{
				start = true;
				out[pos] = '#';
				pos++;
			}
		else
			if (in[i] >= 32 && i < 254)
			{
				start = true;
				out[pos] = in[i];
				pos++;
			}
		else {
			if (start) {
				out[pos] = 0;
				pos++;
				break;
			}
		}
}


void ClearChar(char* out) {
	for (int i = 0; i < 256; i++) {
		out[i] = 0;
		break;  // ???
	}
}


std::string CurrentDateTimeString() {
	time_t now = time(0); //ѕолучить метку времени по √ринвичу
	struct tm timeinfo;
	localtime_s(&timeinfo, &now); //Ћокализовать врем€

	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d %H-%M-%S", &timeinfo); //¬ывести врем€ по формату в буфер
	string str_buffer(buffer); //ѕереписать буфер в объект string

	return str_buffer; //и вернуть этот объект
}


std::string GetFileFromPath(std::string path) {
	// Out& out = Out::instance();
	// out.lvlUp();
	// out.print("GetFileFromPath(", path, " )");
	// out.print("length =", std::to_string(path.length()));

	size_t startPos = path.find_last_of("\\");

	if (startPos == std::string::npos)
		startPos = 0;

	// out.print("startPos =", std::to_string(startPos));

	size_t endPos = path.length();

	std::string filename = path.substr(startPos, endPos - startPos);
	// out.print("file =", filename);


	// out.print("GetFileNameFromPath(", path, " ) -> ", filename, " ok");
	// out.lvlDn();

	return filename;
}


std::string GetFileExtFromPath(std::string path) {
	// Out& out = Out::instance();
	// out.lvlUp();
	// out.print("GetFileExtFromPath(", path, " )");
	// out.print("length =", std::to_string(path.length()));

	size_t startPos = path.length();

	for (int i = path.length(); i >= 0; i--) {
		if (path[i] == '.') {
			startPos = i +1;
			//// out.print("startPosstartPos =", std::to_string(startPos));
			break;
		}
	}

	std::string fileExt = path.substr(startPos, path.length() - startPos);

	// out.print("GetFileNameFromPath(", path, " ) -> ", fileExt, " ok");
	// out.lvlDn();

	return fileExt;
}


std::string GetFileNoExtFromPath(std::string path) {
	// Out& out = Out::instance();
	// out.lvlUp();
	// out.print("GetFileNoExtFromPath(", path, " )");
	// out.print("length =", std::to_string(path.length()));

	size_t endPos = path.length();

	for (int i = path.length(); i >= 0; i--) {
		if (path[i] == '.') {
			endPos = i;
			//// out.print("startPosstartPos =", std::to_string(endPos));
			break;
		}
	}

	std::string fileExt = path.substr(0, endPos);

	// out.print("GetFileNoExtFromPath(", path, " ) -> ", fileExt, " ok");
	// out.lvlDn();

	return fileExt;
}


std::string GetFileNameFromPath(std::string path) {
	// Out& out = Out::instance();
	// out.lvlUp();
	// out.print("GetFileNameFromPath(", path, " )");
	// out.print("length =", std::to_string(path.length()));

	size_t startPos = path.find_last_of("\\") +1;

	if (startPos == std::string::npos)
		startPos = 0;

	// out.print("startPos =", std::to_string(startPos));

	size_t endPos = path.length();

	for (int i = path.length(); i > startPos + 1; i--) {
		if (path[i] == '.') {
			endPos = i;
			//// out.print("endPos =", std::to_string(endPos));
			break;
		}
	}

	std::string filename = path.substr(startPos, endPos - startPos);

	// out.print("GetFileNameFromPath(", path, " ) -> ", filename, " ok");
	// out.lvlDn();

	return filename;
}


#define _SUN_LAG_ 10


// прошло дней после зимнего солнцесто€ни€ (мес€ц, день)
int getDaysFromZero(int mon, int day) {
	int days_from_zero = 0;

	// смещение относительно зимного солнцесто€ни€
	switch (mon)
	{
	case 1:
		days_from_zero = _SUN_LAG_ + day;
		break;
	case 2:
		days_from_zero = _SUN_LAG_ + 31 + day;
		break;
	case 3:
		days_from_zero = _SUN_LAG_ + 31 + 29 + day;
		break;
	case 4:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + day;
		break;
	case 5:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + day;
		break;
	case 6:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + day;
		break;
	case 7:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + 30 + day;
		break;
	case 8:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + 30 + 31 + day;
		break;
	case 9:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + day;
		break;
	case 10:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + day;
		break;
	case 11:
		days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + day;
		break;
	default:
		if (day < 31 - _SUN_LAG_)
			days_from_zero = _SUN_LAG_ + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + day;
		else
			days_from_zero = day - (31 - _SUN_LAG_);
		break;
	}
	return days_from_zero;
}

// прошло дней после зимнего солнцесто€ни€ (день в году)
int getDaysFromZero(int days) {
	int days_from_zero = 0;

	// смещение относительно зимного солнцесто€ни€
	if (days < 355)
		days_from_zero = days + _SUN_LAG_;
	else
		days_from_zero = days - 366 + _SUN_LAG_;

	return days_from_zero;
}

// номер мес€ца по номеру дна€ в году
int getMon(int days) {
	if (days <= 0)
		return 12;
	if (days <= 31)
		return 1;
	if (days <= 60)
		return 2;
	if (days <= 91)
		return 3;
	if (days <= 121)
		return 4;
	if (days <= 152)
		return 5;
	if (days <= 182)
		return 6;
	if (days <= 213)
		return 7;
	if (days <= 244)
		return 8;
	if (days <= 274)
		return 9;
	if (days <= 305)
		return 10;
	if (days <= 335)
		return 11;
	if (days <= 366)
		return 12;

	return 12;
}

// номер дн€ в мес€це по номеру дн€ в году
int getDay(int days) {
	if (days <= 0)
		return 31 + days + 1;
	if (days <= 31)
		return days;
	if (days <= 60)
		return days - 31;
	if (days <= 91)
		return days - 60;
	if (days <= 121)
		return days - 91;
	if (days <= 152)
		return days - 121;
	if (days <= 182)
		return days - 152;
	if (days <= 213)
		return days - 182;
	if (days <= 244)
		return days - 213;
	if (days <= 274)
		return days - 244;
	if (days <= 305)
		return days - 274;
	if (days <= 335)
		return days - 305;
	if (days <= 366)
		return days - 335;

	return 0;
}


// расчЄт продолжительности пол€рного дн€ в сутках - по широте
int CalcPloarDayLen_days(double deg) {
	if (deg < 0)
		deg = -deg;

	if (deg <= 67.0)
		return 0;

	if (deg <= 68.0)
		return (int)(((5) * (deg - 67.0)) / (68.0 - 67.0)) + 0;
	if (deg <= 69.0)
		return (int)(((45 - 5) * (deg - 68.0)) / (69.0 - 68.0)) + 5;
	if (deg <= 70.0)
		return (int)(((60 - 45) * (deg - 69.0)) / (70.0 - 69.0)) + 45;
	if (deg <= 73.0)
		return (int)(((88 - 62) * (deg - 70.0)) / (73.0 - 70.0)) + 62;
	if (deg <= 74.0)
		return (int)(((96 - 88) * (deg - 73.0)) / (74.0 - 73.0)) + 88;
	if (deg <= 77.0)
		return (int)(((115 - 96) * (deg - 74.0)) / (77.0 - 74.0)) + 96;
	if (deg <= 78.0)
		return (int)(((123 - 115) * (deg - 77.0)) / (78.0 - 77.0)) + 115;
	if (deg <= 79.0)
		return (int)(((129 - 123) * (deg - 78.0)) / (79.0 - 78.0)) + 123;
	if (deg <= 80.0)
		return (int)(((135 - 129) * (deg - 79.0)) / (80.0 - 79.0)) + 129;
	if (deg <= 81.0)
		return (int)(((141 - 135) * (deg - 80.0)) / (81.0 - 80.0)) + 135;
	if (deg <= 82.0)
		return (int)(((147 - 141) * (deg - 81.0)) / (82.0 - 81.0)) + 141;
	if (deg <= 90.0)
		return (int)(((187 - 147) * (deg - 82.0)) / (90.0 - 82.0)) + 147;

	return 0;
}

// расчЄт продолжительности пол€рной ночи в сутках - по широте
int CalcPloarNightLen_days(double deg) {
	if (deg < 0)
		deg = -deg;

	if (deg < 67.0)
		return 0;
	if (deg <= 68.0)
		return (int)(((5) * (deg - 67.0)) / 1.0);
	if (deg <= 69.0)
		return (int)(((45 - 2) * (deg - 68.0)) / (69.0 - 68.0)) + 2;
	if (deg <= 70.0)
		return (int)(((60 - 40) * (deg - 69.0)) / (70.0 - 69.0)) + 40;
	if (deg <= 73.0)
		return (int)(((80 - 53) * (deg - 70.0)) / (73.0 - 70.0)) + 53;
	if (deg <= 74.0)
		return (int)(((86 - 80) * (deg - 73.0)) / (74.0 - 73.0)) + 80;
	if (deg <= 77.0)
		return (int)(((106 - 86) * (deg - 74.0)) / (77.0 - 74.0)) + 86;
	if (deg <= 78.0)
		return (int)(((112 - 106) * (deg - 77.0)) / (78.0 - 77.0)) + 106;
	if (deg <= 79.0)
		return (int)(((117 - 112) * (deg - 78.0)) / (79.0 - 78.0)) + 112;
	if (deg <= 80.0)
		return (int)(((122 - 117) * (deg - 79.0)) / (80.0 - 79.0)) + 117;
	if (deg <= 81.0)
		return (int)(((128 - 122) * (deg - 80.0)) / (81.0 - 80.0)) + 122;
	if (deg <= 82.0)
		return (int)(((134 - 128) * (deg - 81.0)) / (82.0 - 81.0)) + 128;
	if (deg <= 90.0)
		return (int)(((178 - 134) * (deg - 82.0)) / (90.0 - 82.0)) + 134;

	return 0;
}

// расчЄт минимальной длины летней ночи - по широте
double CalcDarkTimeSummer_mins(double deg) {

	if (deg >= 67.0)
		return 0.0;

	if (deg >= 66.0)
		return ((99 - 0) * (67.0 - deg)) / (67 - 66) + 0;
	if (deg >= 65.0)
		return ((165 - 99) * (66.0 - deg)) / (66 - 65) + 99;
	if (deg >= 63.0)
		return ((247 - 165) * (65.0 - deg)) / (65 - 63) + 165;
	if (deg >= 60.0)
		return ((330 - 247) * (63.0 - deg)) / (63 - 60) + 247;
	if (deg >= 55.0)
		return ((414 - 330) * (60.0 - deg)) / (60 - 55) + 330;
	if (deg >= 50.0)
		return ((470 - 414) * (55.0 - deg)) / (55 - 50) + 414;
	if (deg >= 40.0)
		return ((549 - 470) * (50.0 - deg)) / (50 - 40) + 470;
	if (deg >= 20.0)
		return ((630 - 549) * (40.0 - deg)) / (40 - 20) + 549;
	if (deg >= 0.0)
		return ((720 - 630) * (20.0 - deg)) / (20 - 0) + 630;

	if (deg >= -20.0)
		return ((810 - 720) * (0.0 - deg)) / (20 - 0) + 720;
	if (deg >= -40.0)
		return ((891 - 810) * (-20.0 - deg)) / (40 - 20) + 810;
	if (deg >= -50.0)
		return ((969 - 891) * (-40.0 - deg)) / (50 - 40) + 891;
	if (deg >= -55.0)
		return ((1025 - 969) * (-50.0 - deg)) / (55 - 50) + 969;
	if (deg >= -60.0)
		return ((1110 - 1025) * (-55.0 - deg)) / (60 - 55) + 1025;
	if (deg >= -63.0)
		return ((1192 - 1110) * (-60.0 - deg)) / (63 - 60) + 1110;
	if (deg >= -65.0)
		return ((1275 - 1192) * (-63.0 - deg)) / (65 - 63) + 1192;
	if (deg >= -66.0)
		return ((1341 - 1275) * (-65.0 - deg)) / (66 - 65) + 1275;
	if (deg >= -67.0)
		return ((1440.0 - 1341) * (-66.0 - deg)) / (67 - 66) + 1341;

	return 1440.0;
}

// расчЄт максимальной длины зимней ночи - по широте
double CalcDarkTimeWinter_mins(double deg) {
	return CalcDarkTimeSummer_mins(-1 * deg);
}


// рассчЄт погружени€ в ночное врем€
//double CalcDarkTime_mins(
//	int DarkTimeWinter_min, int DarkTimeSummer_min,
//	int mon, int day, int hour, int min,
//	int offsetDayTime, int offsetTimeZone)
//{
//	double pi = 3.14159265;
//	int days_from_zero = getDaysFromZero(mon, day);
//
//	// рассвет
//	double sunrizeTime =
//		-sin((days_from_zero * 2.0 * pi) / 366.0 - pi * 0.5)
//		* (DarkTimeWinter_min - DarkTimeSummer_min) * 0.25
//		+ (DarkTimeWinter_min + DarkTimeSummer_min) * 0.25
//		+ offsetTimeZone;
//
//	// закат
//	double nightTime = 1440 - sunrizeTime;
//
//	// текущее врем€
//	double currTime = hour * 60.0 + min;
//
//	// погружение в ночь
//	double sunsetTime = 0.0;
//	if (currTime < 720) {
//		sunsetTime = sunrizeTime - currTime;
//	}
//	else {
//		sunsetTime = currTime - nightTime;
//	}
//
//	return sunsetTime - offsetDayTime;
//}


// рассчЄт времени рассвета - минут после полуночи прошедшей ночи
double CalcSunrize_mins(
	int mon, int day,
	double latitudeDeg, 
	int offsetDayTime, int offsetTimeZone) {

	double pi = 3.14159265;

	int polarDay = CalcPloarDayLen_days(latitudeDeg);
	int polarDayToZero = (int)((183 - polarDay) / 2.0);
	int polarDay0_mon = getMon(polarDayToZero + 91 - _SUN_LAG_);
	int polarDay0_day = getDay(polarDayToZero + 91 - _SUN_LAG_);
	int polarDay1_mon = getMon(polarDayToZero + 91 - _SUN_LAG_ + polarDay);
	int polarDay1_day = getDay(polarDayToZero + 91 - _SUN_LAG_ + polarDay);

	double K = 1;

	if (polarDay > 0) {
		double sinT = sin(2 * pi * polarDayToZero / 366.0);
		K = 1.0 / sinT;
	}

	int polarNgt = CalcPloarNightLen_days(latitudeDeg);
	int polarNgtToZero = polarNgt / 2;
	int polarNgt0_mon = getMon(polarNgtToZero - _SUN_LAG_);
	int polarNgt0_day = getDay(polarNgtToZero - _SUN_LAG_);
	int polarNgt1_mon = getMon(366 - polarNgtToZero - _SUN_LAG_);
	int polarNgt1_day = getDay(366 - polarNgtToZero - _SUN_LAG_);

	// рассвет
	int days_from_zero = getDaysFromZero(mon, day);
	int DarkTimeWinter_min = CalcDarkTimeWinter_mins(latitudeDeg);
	int DarkTimeSummer_min = CalcDarkTimeSummer_mins(latitudeDeg);

	double sinT = sin((days_from_zero / 366.0) * pi * 2.0 + pi * 0.5);
	double A = (polarDay > 0)
		? 360
		: (0.25 * (DarkTimeWinter_min + DarkTimeSummer_min));

	double sunrizeTime
		= sinT * 0.25 * (DarkTimeWinter_min - DarkTimeSummer_min) * K
		+ A
		+ offsetDayTime
		+ offsetTimeZone;

	if (sunrizeTime < 0)
		sunrizeTime = 0;

	if (sunrizeTime >= 720)
		sunrizeTime = 720;

	return sunrizeTime;
}

// рассчЄт времени заката - минут до полуночи новой ночи
double CalcSunset_mins(
	int mon, int day,
	double latitudeDeg, 
	int offsetDayTime, int offsetTimeZone) {

	double pi = 3.14159265;

	int polarDay = CalcPloarDayLen_days(latitudeDeg);
	int polarDayToZero = (int)((183 - polarDay) / 2.0);
	int polarDay0_mon = getMon(polarDayToZero + 91 - _SUN_LAG_);
	int polarDay0_day = getDay(polarDayToZero + 91 - _SUN_LAG_);
	int polarDay1_mon = getMon(polarDayToZero + 91 - _SUN_LAG_ + polarDay);
	int polarDay1_day = getDay(polarDayToZero + 91 - _SUN_LAG_ + polarDay);

	double K = 1;

	if (polarDay > 0) {
		double sinT = sin(2 * pi * polarDayToZero / 366.0);
		K = 1.0 / sinT;
	}

	int polarNgt = CalcPloarNightLen_days(latitudeDeg);
	int polarNgtToZero = polarNgt / 2;
	int polarNgt0_mon = getMon(polarNgtToZero - _SUN_LAG_);
	int polarNgt0_day = getDay(polarNgtToZero - _SUN_LAG_);
	int polarNgt1_mon = getMon(366 - polarNgtToZero - _SUN_LAG_);
	int polarNgt1_day = getDay(366 - polarNgtToZero - _SUN_LAG_);

	// закат
	int days_from_zero = getDaysFromZero(mon, day);
	int DarkTimeWinter_min = CalcDarkTimeWinter_mins(latitudeDeg);
	int DarkTimeSummer_min = CalcDarkTimeSummer_mins(latitudeDeg);

	double sinT = sin((days_from_zero / 366.0) * pi * 2.0 - pi * 0.5);
	double A = (polarDay > 0)
		? 1080
		: (1440 - 0.25 * (DarkTimeWinter_min + DarkTimeSummer_min));

	double sunsetTime = sinT
		* 0.25 * (DarkTimeWinter_min - DarkTimeSummer_min) * K
		+ A
		- offsetDayTime
		+ offsetTimeZone;

	if (sunsetTime < 720)
		sunsetTime = 720;

	if (sunsetTime >= 1440)
		sunsetTime = 1440;

	return sunsetTime;
}

