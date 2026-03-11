# node-watermark-loader

[![npm version](https://img.shields.io/npm/v/node-watermark-loader.svg)](https://www.npmjs.com/package/node-watermark-loader)
[![License: ISC](https://img.shields.io/badge/License-ISC-blue.svg)](https://opensource.org/licenses/ISC)

A high-performance Node.js image watermarking library based on OpenCV and FreeType. It uses Node.js Addon to call C++ native code, supporting Chinese fonts and customizable watermark styles.

[![NPM Badge](https://nodei.co/npm/node-watermark-loader.png)](https://npmjs.com/package/node-watermark-loader)

## ✨ Features

- 🚀 **High Performance**: Native C++ implementation with JavaScript interface via Node.js Addon
- 🎨 **Rich Customization Options**: Support for font size, color, opacity, rotation angle, and more
- 🌏 **Chinese Font Support**: Based on FreeType font rendering engine with full support for Chinese fonts
- 📦 **Multiple Format Support**: Supports common image formats like JPG, PNG
- 🔧 **Easy to Use**: Clean API design with support for both Buffer and file path inputs
- 🏗️ **Cross-Platform**: Supports Windows, Linux, and other platforms

## 📋 Requirements

- Node.js >= 14.0.0
- CMake >= 3.21 (only needed if building from source)
- C++ compiler (supporting C++23 standard) (only needed if building from source)

**Note:** Pre-built artifacts are available for Ubuntu(22.04) and Windows, so you may not need to build from source.

## 📦 Installation

```bash
npm install node-watermark-loader
# or
pnpm install node-watermark-loader
# or
yarn add node-watermark-loader
```

## 🚀 Quick Start

### Basic Usage

```javascript
const watermarkLoader = require('node-watermark-loader');
const fs = require('fs');

// Add watermark from file path
const imagePath = './test.png';
const fontPath = './watermark.ttf';

const options = {
    watermarkText: 'Hello World，你好世界!',
    fontFilename: fontPath,
    fontHeight: 14,
    thickness: -1,
    textColor: 0xCCCCCC,
    opacity: 0.4 * 255,
    rorationAngle: 30
};

// Output as Buffer
const watermarkedBuffer = watermarkLoader.overlayWatermarkMask(
    imagePath, 
    '.png', 
    options
);

// Save result
fs.writeFileSync('./output.png', watermarkedBuffer);
```

### Using Buffer Input

```javascript
const imageBuffer = fs.readFileSync('./test.png');

const watermarkedBuffer = watermarkLoader.overlayWatermarkMask(
    imageBuffer, 
    '.png', 
    options
);
```

| Source                                | Resolved                                            |
|---------------------------------------|-----------------------------------------------------|
| ![](./testfiles/images/data/test.png) | ![](./testfiles/images/expected/fill_watermark.png) |

## 📖 API Documentation

### `overlayWatermarkMask(inputImage, ext, options)`

Add watermark to an image.

#### Parameters

- `inputImage` (`Buffer` | `string`): Input image, can be a Buffer or file path
- `ext` (`".jpg"` | `".png"`): Output image format
- `options` (`Object`): Watermark configuration options
  - `watermarkText` (`string`, **required**): Watermark text content
  - `fontFilename` (`string`, **required**): Font file path (supports TTF format)
  - `fontHeight` (`number`, optional): Font height, default `14`
  - `thickness` (`number`, optional): Font thickness, default `-1` (auto)
  - `textColor` (`number`, optional): Text color (RGB format, e.g., `0xCCCCCC`), default `0xCCCCCC`
  - `opacity` (`number`, optional): Opacity (0-255), default `102` (0.4 * 255)
  - `fontIdx` (`number`, optional): Font index, default `0`
  - `rorationAngle` (`number`, optional): Rotation angle (degrees), default `30`

#### Returns

- `Buffer`: Image Buffer with watermark added

## 🏗️ Building from Source

If you want to build the project from source, you need to install dependencies first:

### Prerequisites

1. **Node.js** and package manager (npm/pnpm/yarn)
2. **CMake** >= 3.21
3. **C++ Compiler**
   - Windows: Visual Studio 2019 or higher
   - Linux: GCC or Clang (supporting C++23)
   - macOS: Xcode Command Line Tools

### Build Steps

1. Clone the repository:

```bash
git clone https://github.com/pldq/image-freetype-node.git
cd image-freetype-node
```

2. Install dependencies:

```bash
pnpm install
# or
npm install
```

3. Build native module:

When building the addon, you need to specify the OpenCV directory using the `--CDOpenCV_DIR` parameter. The value should be the path to the directory containing `OpenCVConfig.cmake` after building OpenCV.

```bash
npm run build -- --CDOpenCV_DIR=/path/to/opencv/build
# or
pnpm build -- --CDOpenCV_DIR=/path/to/opencv/build
```

**Example:**
```bash
# Windows
npm run build -- --CDOpenCV_DIR=C:/opencv/build

# Linux/macOS
npm run build -- --CDOpenCV_DIR=/usr/local/lib/cmake/opencv4
```

The build script will automatically:
- Compile C++ native code
- Generate Node.js Addon (`.node` file)
- Output compiled results to `addon/<platform>/<arch>/` directory

### Build Script Notes

The project uses `cmake-js` for building, and the build script is located at `script/buildNodeNative.js`. The build process will:

1. Detect current platform and architecture
2. Configure and compile the project using CMake
3. Install the compiled `.node` file to the `addon` directory

**Important**: You must provide the `--CDOpenCV_DIR` parameter pointing to the directory where `OpenCVConfig.cmake` is located. This is typically in the `build` directory after compiling OpenCV, or in the installation directory (e.g., `/usr/local/lib/cmake/opencv4` on Linux).

## 📁 Project Structure

```
image-freetype-node/
├── js/                    # JavaScript layer
│   ├── src/              # Source code
│   │   ├── index.js      # Main entry file
│   │   └── index.d.ts    # TypeScript type definitions
│   └── test/             # Test files
├── native/                # C++ native layer
│   ├── image-watermark/  # Core watermark processing library
│   │   ├── include/      # Header files
│   │   └── src/          # Source files
│   └── node-watermark-loader/  # Node.js Addon bindings
├── addon/                 # Compiled native modules
│   ├── win32/            # Windows platform
│   └── linux/            # Linux platform
├── script/                # Build scripts
├── testfiles/            # Test files
│   ├── fonts/            # Font files
│   └── images/           # Test images
└── CMakeLists.txt        # CMake configuration file
```

## 🧪 Testing

Run tests:

```bash
npm test
# or
pnpm test
```

## 🔧 Tech Stack

- **OpenCV**: Image processing and manipulation
- **FreeType**: Font rendering engine
- **Node.js Addon API**: Node.js native module interface
- **CMake**: Cross-platform build system
- **cmake-js**: CMake build tool for Node.js

## 📝 License

ISC

## 🤝 Contributing

Contributions are welcome! Please feel free to submit an Issue or Pull Request.

## 📮 Contact

If you have any questions or suggestions, please contact us through:

- Submit an [Issue](https://github.com/pldq/node-watermark-loader/issues)
- Email the project maintainer

## 🙏 Acknowledgments

- [OpenCV](https://opencv.org/) - Powerful computer vision library
- [FreeType](https://www.freetype.org/) - Font rendering engine
- [HarfBuzz](https://harfbuzz.github.io/) - Text shaping engine
- [Node.js Addon API](https://nodejs.org/api/addons.html) - Node.js native module development
- [cmake-js](https://github.com/cmake-js/cmake-js) - CMake build system for Node.js native addons

---

**Note**: This project is currently in development, and the API may change. Please test thoroughly before using in production.