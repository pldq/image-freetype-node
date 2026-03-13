### Added

- Publish `node-watermark-loader` Node.js module with a native C++ addon (OpenCV + FreeType) for high-performance image watermarking.
- `overlayWatermarkMask(inputImage, ext, options)` API supporting:
  - `inputImage`: `Buffer` or file path
  - `ext`: output format (`.jpg` / `.png`)
  - watermark options: `watermarkText`, `fontFilename`, `fontHeight`, `thickness`, `textColor`, `opacity`, `fontIdx`, `rorationAngle`
- Cross-platform loading of prebuilt/compiled addon from `addon/<platform>/<arch>/node-watermark-loader.node`.
- Source build workflow via `cmake-js` (`script/buildNodeNative.js`) with required `--CDOpenCV_DIR` argument.
- Pre-built artifacts available for **Windows** and **Ubuntu 22.04** (so building from source may be unnecessary).
- TypeScript type definitions shipped (`js/src/index.d.ts`).

### Notes

- Runtime requirement: Node.js >= 14.
- Native build requirement (when building from source): CMake >= 3.21 and a C++23-capable compiler.

