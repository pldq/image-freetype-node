#include "watermark.h"

#include "opencv2/freetype.hpp"
#include "opencv2/imgcodecs.hpp"

#include <iostream>

#ifdef _WIN32
#include "windows.h"

BOOL WINAPI DllMain(
    HINSTANCE hinstance,
    const DWORD fdwReason,
    const LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            if (lpvReserved != nullptr) {
                break;
            }
            break;
        default: break;
    }
    return TRUE;
}
#endif

cv::Mat generateWatermarkImg(const WATERMARK_PROPERTIES &properties) {
    const auto ft2 = cv::freetype::createFreeType2();
    ft2->loadFontData(properties.fontFilename, properties.fontIdx);
    int baseline = 0;
    const cv::Size textSize = ft2->getTextSize(properties.watermarkText,
                                               properties.fontHeight, properties.thickness,
                                               &baseline);
    if (properties.thickness > 0) {
        baseline += properties.thickness;
    }

    int width = textSize.width;
    int height = textSize.height + baseline * 2;

    cv::Mat img(height, width,
                CV_8UC4, cv::Scalar::all(0));
    ft2->putText(img, properties.watermarkText,
                 cv::Point(0, -baseline),
                 properties.fontHeight,
                 properties.textColor,
                 properties.thickness,
                 properties.lineStyle,
                 false);
    if (properties.printWatermarkImg) {
        std::cout << "width = " << width << ", height = " << height << std::endl;
        cv::imwrite("watermark.png", img);
    }
    const auto center = cv::Point2f(width / 2.0, height / 2.0);
    if (properties.rorationAngle != nullptr) {
        const double angle = *properties.rorationAngle;

        auto rotationMat = cv::getRotationMatrix2D(
            center,
            angle,
            1
        );
        cv::Mat dest;
        const double rad = angle * CV_PI / 180.0;

        const double rotatedWidth = width * fabs(cos(rad)) + height * fabs(sin(rad));
        const double rotatedHeight = width * fabs(sin(rad)) + height * fabs(cos(rad));


        rotationMat.at<double>(0, 2) += (rotatedWidth - width) / 2;
        rotationMat.at<double>(1, 2) += (rotatedHeight - height) / 2;

        cv::warpAffine(img, dest,
                       rotationMat,
                       cv::Size(rotatedWidth, rotatedHeight)
        );
        if (properties.printWatermarkImg) {
            std::cout << "rorationAngle = " << angle
                    << ", rorationRad = " << rad
                    << ", after roration size = (" << dest.rows << ", " << dest.cols << ")"
                    << std::endl;
            cv::imwrite("watermark_rotated.png", dest);
        }
        img = dest;
        width = img.cols;
        height = img.rows;
    }
    return img;
}
