#ifndef IMAGE_FREETYPE_NODE_WATERMARK_H
#define IMAGE_FREETYPE_NODE_WATERMARK_H

#include "opencv2/imgproc.hpp"

#ifdef WIN32
#ifdef WATERMARKDLL_EXPORTS
#define WATERMARK_API __declspec(dllexport)
#else
#define WATERMARK_API __declspec(dllimport)
#endif
#else
#define WATERMARK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _WATERMARK_PROPERTIES {
    std::string watermarkText;
    int fontHeight;
    int thickness;

    int lineStyle = cv::LINE_AA;

    cv::Scalar textColor = cv::Scalar(255, 255, 255);

    std::string fontFilename;
    int fontIdx = 0;

    bool printWatermarkImg = false;

    double *rorationAngle = nullptr;
} WATERMARK_PROPERTIES;

WATERMARK_API cv::Mat generateWatermarkImg(const WATERMARK_PROPERTIES &properties);

#ifdef __cplusplus
}
#endif

#endif //IMAGE_FREETYPE_NODE_WATERMARK_H
