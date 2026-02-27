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
    properties.needRotation = true;
    properties.rotationAngle = 30;

    std::vector<std::string> extensions{".bmp", ".gif", ".jpg", ".png", ".webp"};
    for (const auto &ext: extensions) {
        auto srcImageFile = "test" + ext;
        overlayWatermarkMask(srcImageFile, properties, "fill_watermark" + ext);
        std::cout << "Fill watermark " << srcImageFile << " finished." << std::endl;

        for (const auto &targetExt: extensions) {
            if (ext != targetExt) {
                auto targetImageFile = srcImageFile + targetExt;
                std::vector<std::uint8_t> result{};
                overlayWatermarkMask(srcImageFile, properties, targetExt, result);
                write2file(targetImageFile, result);
                std::cout << "Transfer " << srcImageFile << " to " << targetImageFile << " success." << std::endl;
            }
        }
    }
    return 0;
}
