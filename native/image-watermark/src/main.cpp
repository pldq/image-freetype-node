#include "watermark.h"

#include "opencv2/freetype.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

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

#ifndef _WIN32
#define min cv::min
#endif

inline cv::Scalar convertRGB2Scalar(const RGBColor v) {
    return {
        static_cast<double>(v & 0xFF),
        static_cast<double>(v >> 8 & 0xFF),
        static_cast<double>(v >> 16 & 0xFF),
        255};
}

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

    const int width = textSize.width;
    const int height = textSize.height + baseline * 2;

    cv::Mat img(height, width,
                CV_8UC4, cv::Scalar::all(0));
    ft2->putText(img, properties.watermarkText,
                 cv::Point(0, -baseline),
                 properties.fontHeight,
                 convertRGB2Scalar(properties.textColor),
                 properties.thickness,
                 cv::LINE_AA,
                 false);
    if (properties.printWatermarkImg) {
        std::cout << "width = " << width << ", height = " << height << std::endl;
        cv::imwrite("watermark.png", img);
    }
    const auto center = cv::Point2d(width / 2.0, height / 2.0);
    if (properties.needRotation) {
        const double angle = properties.rotationAngle;

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
                       cv::Size(static_cast<int>(rotatedWidth), static_cast<int>(rotatedHeight))
        );
        if (properties.printWatermarkImg) {
            std::cout << "rorationAngle = " << angle
                    << ", rorationRad = " << rad
                    << ", after roration size = (" << dest.rows << ", " << dest.cols << ")"
                    << std::endl;
            cv::imwrite("watermark_rotated.png", dest);
        }
        img = dest;
    }
    return img;
}

inline cv::Mat convertBGRA2TargetFormat(const cv::Mat &src, const int channel, const int type) {
    cv::Mat result;
    int code = 0;
    switch (channel) {
        case 1:
            code = cv::COLOR_BGRA2GRAY;
            cv::cvtColor(src, result, code);
            break;
        case 3:
            code = cv::COLOR_BGRA2BGR;
            cv::cvtColor(src, result, code);
            break;
        default:
            std::cout << "Error: Unsupported number of channels: " << channel << std::endl;
            result = src;
    }
    if (result.type() != type) {
        result.convertTo(result, type);
    }
    return result;
}

void overlayImageRegion(const cv::Mat &src, const cv::Mat &overlay, int x, int y,
                           const WATERMARK_PROPERTIES &properties) {
    const int width = src.cols;
    const int height = src.rows;

    const int overlay_width = overlay.cols;
    const int overlay_height = overlay.rows;

    const int w = min(overlay_width, width - x);
    const int h = min(overlay_height, height - y);

    auto roi = src(cv::Rect(x, y, w, h));
    auto overlayRoi = overlay;
    if (w <= overlay_width || h <= overlay_height) {
        overlayRoi = overlay(cv::Rect(0, 0, w, h));
    }
    overlayRoi = convertBGRA2TargetFormat(overlayRoi, src.channels(), src.type());

    cv::addWeighted(roi, 1, overlayRoi, properties.opacity / 255.0, 0, roi);
}

void overlayWatermarkMask(const cv::Mat &src, const WATERMARK_PROPERTIES &properties) {
    const auto watermark = generateWatermarkImg(properties);

    const int watermarkWidth = watermark.cols;
    const int watermarkHeight = watermark.rows;

    const int width = src.cols;
    const int height = src.rows;

    if (watermarkWidth <= width && watermarkHeight <= height) {
        for (int x = 0; x < width; x += watermarkWidth) {
            for (int y = 0; y < height; y += watermarkHeight) {
                overlayImageRegion(src, watermark, x, y, properties);
            }
        }
    } else {
        std::cout << "Target image size (" << width << "x" << height << ")"
                << " is smaller than watermark image size (" << watermarkWidth << "x" << watermarkHeight << ")"
                << ", overlay will be skipped." << std::endl;
    }
}

inline cv::Mat overlayWatermarkMaskFromBytes(const std::vector<std::uint8_t> &buffer, const WATERMARK_PROPERTIES &properties) {
    const auto src = cv::imdecode(buffer, cv::IMREAD_COLOR);
    overlayWatermarkMask(src, properties);
    return src;
}

inline cv::Mat overlayWatermarkMaskFromFile(const std::string &filename, const WATERMARK_PROPERTIES &properties) {
    const auto src = cv::imread(filename);
    overlayWatermarkMask(src, properties);
    return src;
}

bool overlayWatermarkMask(const std::vector<std::uint8_t> &buffer, const WATERMARK_PROPERTIES &properties,
                          const std::string &ext, std::vector<std::uint8_t> &out) {
    const auto src = overlayWatermarkMaskFromBytes(buffer, properties);
    return cv::imencode(ext, src, out);
}

bool overlayWatermarkMask(const std::vector<std::uint8_t> &buffer, const WATERMARK_PROPERTIES &properties,
                          const std::string &targetFileName) {
    const auto src = overlayWatermarkMaskFromBytes(buffer, properties);
    return cv::imwrite(targetFileName, src);
}

bool overlayWatermarkMask(const std::string &filename, const WATERMARK_PROPERTIES &properties,
                          const std::string &ext, std::vector<std::uint8_t> &out) {
    const auto src = overlayWatermarkMaskFromFile(filename, properties);
    return cv::imencode(ext, src, out);
}

bool overlayWatermarkMask(const std::string &filename, const WATERMARK_PROPERTIES &properties,
                          const std::string &targetFileName) {
    const auto src = overlayWatermarkMaskFromFile(filename, properties);
    return cv::imwrite(targetFileName, src);
}