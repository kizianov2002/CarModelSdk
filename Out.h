#pragma once
#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


class Out
{
private:
    int lvl = 0;

public:
    Out() {
        lvl = 0;
    }

    static Out& instance()
    {
        static Out recInstance;
        return recInstance;
    }

    ~Out()
    {
        std::cout
            << "OUT - Memory has been cleaned. Good bye."
            << std::endl;
        // std::cin.get();
    }

    void lvlUp() {
        lvl++;
        if (lvl > 20) 
            lvl = 20;
    }

    void lvlDn() {
        lvl--;
        if (lvl < 0) 
            lvl = 0;
    }

    void print(
        std::string str0 = "",
        std::string str1 = "",
        std::string str2 = "",
        std::string str3 = "",
        std::string str4 = "",
        std::string str5 = "",
        std::string str6 = "",
        std::string str7 = "",
        std::string str8 = "",
        std::string str9 = "",
        std::string st10 = "",
        std::string st11 = "",
        std::string st12 = "",
        std::string st13 = "",
        std::string st14 = "")
    { 
//#ifdef _DEBUG
        for (int i=0; i<lvl; i++) 
            std::cout << "    ";

        std::cout
            << str0 << " "
            << str1 << " "
            << str2 << " "
            << str3 << " "
            << str4 << " "
            << str5 << " "
            << str6 << " "
            << str7 << " "
            << str8 << " "
            << str9 << " "
            << st10 << " "
            << st11 << " "
            << st12 << " "
            << st13 << " "
            << st14 << " "
            << std::endl;
//#endif
    }

    std::string rectToStr(cv::Rect rect) {
        std::string result;
        result = "(" + std::to_string(rect.x) + " , " + std::to_string(rect.y) + ") -> (" + std::to_string(rect.width) + " , " + std::to_string(rect.height) + ")";
        return result;
    }
};

