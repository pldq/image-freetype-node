#ifndef IMAGE_FREETYPE_NODE_WATERMARK_H
#define IMAGE_FREETYPE_NODE_WATERMARK_H

#include <cstdint>
#include <string>
#include <vector>

#ifdef WIN32
#ifdef WATERMARKDLL_EXPORTS
#define WATERMARK_API __declspec(dllexport)
#else
#define WATERMARK_API __declspec(dllimport)
#endif
#else
#define WATERMARK_API
#endif

typedef std::uint32_t RGBColor;

struct WATERMARK_PROPERTIES {
    std::string watermarkText;
    int fontHeight;
    int thickness;

    RGBColor textColor = 0xFFFFFF;

    uint8_t opacity;

    std::string fontFilename;
    int fontIdx = 0;

    bool printWatermarkImg = false;

    double *rorationAngle = nullptr;
};

WATERMARK_API bool overlayWatermarkMask(const std::vector<std::uint8_t> &buffer,
                                        const WATERMARK_PROPERTIES &properties,
                                        const std::string &ext, std::vector<std::uint8_t> &out);

WATERMARK_API bool overlayWatermarkMask(const std::vector<std::uint8_t> &buffer,
                                            const WATERMARK_PROPERTIES &properties,
                                            const std::string &targetFileName);

WATERMARK_API bool overlayWatermarkMask(const std::string &filename, const WATERMARK_PROPERTIES &properties,
                              const std::string &ext, std::vector<std::uint8_t> &out);

WATERMARK_API bool overlayWatermarkMask(const std::string &filename, const WATERMARK_PROPERTIES &properties,
                              const std::string &targetFileName);

#endif //IMAGE_FREETYPE_NODE_WATERMARK_H
