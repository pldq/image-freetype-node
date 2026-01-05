#include <cstdint>

#include "watermark.h"
#include <iostream>
#include <vector>

void write2file(const std::string &filename, const std::vector<std::uint8_t> &result) {
    if (const auto file = fopen(filename.c_str(), "wb")) {
        fwrite(result.data(), sizeof(std::uint8_t), result.size(), file);
        fclose(file);
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}


int main(int argc, char *argv[]) {
    WATERMARK_PROPERTIES properties;
    properties.watermarkText = "Hello World，你好世界!";
    properties.fontFilename = "watermark.ttf";
    properties.fontHeight = 14;
    properties.thickness = -1;
    properties.printWatermarkImg = true;
    properties.textColor = 0xCCCCCC;
    properties.opacity = 0.4 * 255;
    double rorationAngle = 30;
    properties.rorationAngle = &rorationAngle;

    overlayWatermarkMask("43.jpg", properties, "result.png");

    std::vector<std::uint8_t> result{};
    overlayWatermarkMask("BD_Baidu_181.png", properties, ".jpg", result);
    write2file("result_file.jpg", result);
    return 0;
}
