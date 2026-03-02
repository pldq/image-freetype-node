#include <filesystem>

#include "napi.h"
#include "watermark.h"
#include <iostream>

inline WATERMARK_PROPERTIES parseWatermarkProperties(const Napi::Value o) {
    const auto env = o.Env();
    auto properties = WATERMARK_PROPERTIES{};

    if (!o.IsObject()) {
        Napi::TypeError::New(env, "argument options must be object type.").ThrowAsJavaScriptException();
        return properties;
    }
    const auto object = o.As<Napi::Object>();

    // Set default values for watermark properties
    properties.fontHeight = 14;
    properties.thickness = -1;
    properties.printWatermarkImg = false;
    properties.textColor = 0xCCCCCC;
    properties.opacity = 0.4 * 255;
    properties.fontIdx = 0;
    properties.needRotation = true;
    properties.rotationAngle = 30;

    if (!object.Has("watermarkText") || !object.Get("watermarkText").IsString()) {
        Napi::TypeError::New(env, "argument options.watermarkText must be string type.").ThrowAsJavaScriptException();
        return properties;
    }
    properties.watermarkText = object.Get("watermarkText").As<Napi::String>();

#define WATERMARK_PROPERTIES_SET_PARAM(property, asType, castType) \
    if (object.Has(#property)) { \
            properties.property = object.Get(#property).As<asType>().castType; \
    }

    WATERMARK_PROPERTIES_SET_PARAM(fontHeight, Napi::Number, Int32Value());
    WATERMARK_PROPERTIES_SET_PARAM(thickness, Napi::Number, Int32Value());
    WATERMARK_PROPERTIES_SET_PARAM(textColor, Napi::Number, Uint32Value());
    WATERMARK_PROPERTIES_SET_PARAM(opacity, Napi::Number, Uint32Value());
    WATERMARK_PROPERTIES_SET_PARAM(fontIdx, Napi::Number, Uint32Value());

    if (object.Has("rorationAngle") && object.Get("rorationAngle").IsNumber()) {
        properties.needRotation = true;
        properties.rotationAngle = object.Get("rorationAngle").As<Napi::Number>().DoubleValue();
    } else {
        properties.needRotation = false;
    }

    if (object.Has("fontFilename") && object.Get("fontFilename").IsString()) {
        const std::string fontFilename = object.Get("fontFilename").As<Napi::String>();
        properties.fontFilename = fontFilename;
        if (!std::filesystem::exists(properties.fontFilename)) {
            Napi::Error::New(env, "Font file does not exist or is not accessible: " + properties.fontFilename)
                    .ThrowAsJavaScriptException();
            return properties;
        }
    } else {
        Napi::Error::New(env, "argument options.fontFilename must be not empty string type.")
                .ThrowAsJavaScriptException();
        return properties;
    }

    return properties;
}

Napi::Value createBufferArrayFromVector(const Napi::Env env, const std::uint8_t *outBytes,
                                                  const std::size_t outByteSize) {
    const auto nodeByteArray = Napi::Buffer<uint8_t>::New(env, outByteSize);
#ifdef WIN32
    memcpy_s(nodeByteArray.Data(), nodeByteArray.ByteLength(),
             outBytes, outByteSize
    );
#else
    memcpy(nodeByteArray.Data(), outBytes, outByteSize);
#endif
    return nodeByteArray;
}

Napi::Value OverlayWatermarkMaskNode(const Napi::CallbackInfo &info) {
    const Napi::Env env = info.Env();
    if (info.Length() != 3) {
        Napi::TypeError::New(env, "Expected exactly 3 arguments").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    auto options = info[2];
    WATERMARK_PROPERTIES watermarkProperties = parseWatermarkProperties(options);
    if (env.IsExceptionPending()) {
        return env.Undefined();
    }

    const auto inputImage = info[0];
    if (!inputImage.IsBuffer() && !inputImage.IsString()) {
        Napi::TypeError::New(env, "argument first must be buffer or string type.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    const auto extNode = info[1];
    if (!extNode.IsString()) {
        Napi::TypeError::New(env, "argument second must be string type.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string ext = extNode.As<Napi::String>();

    if (std::vector<std::string> supportExtensions{".bmp", ".gif", ".jpg", ".png", ".webp"};
        std::ranges::find(supportExtensions, ext) == supportExtensions.end()) {
        std::cout << "Unsupported image format: " << ext << ", using png format." << std::endl;
        ext = ".png";
    }

    if (inputImage.IsBuffer()) {
        const auto inputImageBuffer = inputImage.As<Napi::Buffer<std::uint8_t> >();
        const std::vector inputImageBytes(inputImageBuffer.Data(), inputImageBuffer.Data() + inputImageBuffer.Length());
        std::vector<std::uint8_t> outBytes = {};
        overlayWatermarkMask(inputImageBytes, watermarkProperties, ext, outBytes);
        return createBufferArrayFromVector(env, outBytes.data(), outBytes.size());
    }

    if (inputImage.IsString()) {
        const std::string inputImageFile = inputImage.As<Napi::String>();
        if (!std::filesystem::exists(inputImageFile)) {
            Napi::Error::New(env, "Image file does not exist or is not accessible: " + inputImageFile)
                    .ThrowAsJavaScriptException();
            return env.Undefined();
        }
        std::vector<std::uint8_t> outBytes = {};
        overlayWatermarkMask(inputImageFile, watermarkProperties, ext, outBytes);
        return createBufferArrayFromVector(env, outBytes.data(), outBytes.size());
    }
    return env.Undefined();
}

Napi::Object Init(const Napi::Env env, const Napi::Object exports) {
    exports.Set(
        Napi::String::New(env, "overlayWatermarkMask"),
        Napi::Function::New(env, OverlayWatermarkMaskNode)
    );
    return exports;
}

NODE_API_MODULE(node_watermark_loader, Init)
