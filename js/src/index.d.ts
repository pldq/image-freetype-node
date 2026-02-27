import {Buffer} from 'buffer';

interface Options {
    watermarkText: string;
    fontHeight?: number;
    thickness?: number;
    textColor?: number;
    opacity?: number;
    fontFilename?: string;
    fontIdx?: number;
    rorationAngle?: number;
}

export function overlayWatermarkMask(inputImage: Buffer, ext: ".jpg" | ".png", options: Options): Buffer;

export function overlayWatermarkMask(inputImage: string, ext: ".jpg" | ".png", options: Options): Buffer;



