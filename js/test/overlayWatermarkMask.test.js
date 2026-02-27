const watermarkLoader = require("../src");
const fs = require("node:fs");
const path = require("node:path");
describe("overlayWatermarkMask", () => {
    const testfiles = path.resolve(__dirname, "../../testfiles");

    const fontFileName = path.resolve(testfiles, "fonts", "watermark.ttf");

    const testFile = "test.png";

    const srcImage = path.resolve(testfiles, "images", "data", testFile);

    const srcImageBuffer = fs.readFileSync(srcImage);

    const options = {
        watermarkText: "Hello World，你好世界!",
        fontFilename: fontFileName,
        fontHeight: 14,
        thickness: -1,
        textColor: 0xCCCCCC,
        opacity: 0.4 * 255,
        rorationAngle: 30
    };
    const supportExtensions = ["jpg", "png"];

    test("check parameters", () => {
        expect(() => watermarkLoader.overlayWatermarkMask(1, ".png", options))
            .toThrow("argument first must be buffer or string type.");
        expect(() => watermarkLoader.overlayWatermarkMask(srcImage, 1, options))
            .toThrow("argument second must be string type.");
        expect(() => watermarkLoader.overlayWatermarkMask("notExistedImage.jpg", ".png", options))
            .toThrow("Image file does not exist or is not accessible: notExistedImage.jpg");
        expect(() => watermarkLoader.overlayWatermarkMask(srcImage, ".png", {}))
            .toThrow("argument options.watermarkText must be string type.");
        expect(() => watermarkLoader.overlayWatermarkMask(srcImage, ".png", {
            watermarkText: 1
        })).toThrow("argument options.watermarkText must be string type.");
        expect(() => watermarkLoader.overlayWatermarkMask(srcImage, ".png", {
            watermarkText: "test",
            fontFilename: 1
        })).toThrow("argument options.fontFilename must be not empty string type.");
        expect(() => watermarkLoader.overlayWatermarkMask(srcImage, ".png", {
            watermarkText: "test",
            fontFilename: "notExistedFont.ttf"
        })).toThrow("Font file does not exist or is not accessible: notExistedFont.ttf");
    });

    supportExtensions.forEach(fmt => {
        const ext = `.${fmt}`;
        const srcExt = path.extname(srcImage);
        const expectFileName = srcExt === ext ? `fill_watermark${srcExt}` : `${testFile}${ext}`;
        test(`check overlayWatermarkMask on buffer [${fmt}]`, () => {
            const expectFile = path.resolve(testfiles, "images", "excepted", expectFileName);
            const buffer = watermarkLoader.overlayWatermarkMask(srcImageBuffer, ext, options);
            expect(buffer).toEqual(fs.readFileSync(expectFile));
        });

        test(`check overlayWatermarkMask on path [${fmt}]`, () => {
            const expectFile = path.resolve(testfiles, "images", "excepted", expectFileName);
            const buffer = watermarkLoader.overlayWatermarkMask(srcImage, ext, options);
            expect(buffer).toEqual(fs.readFileSync(expectFile));
        });

    });
});