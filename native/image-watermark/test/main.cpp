#include "watermark.h"

int main(int argc, char *argv[]) {
    WATERMARK_PROPERTIES properties;
    properties.watermarkText = "Hello World，你好世界!";
    properties.fontFilename = "watermark.ttf";
    properties.fontHeight = 18;
    properties.thickness = -1;
    properties.printWatermarkImg = true;
    properties.textColor = cv::Scalar(0, 255, 0, 128);
    double rorationAngle = 30;
    properties.rorationAngle = &rorationAngle;
    generateWatermarkImg(properties);
    return 0;
}
