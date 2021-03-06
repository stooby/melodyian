#pragma once
#include "types.h"

// Convert a given HSV (Hue Saturation Value) to RGB (Red Green Blue)
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1

namespace Color {

    RGBColor HSVtoRGB(int h, double s, double v);

    RGBColor ColorWithAdjustedBrightness(RGBColor color, float brightness);
    RGBColor ColorWithAdjustedBrightness(byte r, byte g, byte b, float brightness);
    RGBColor GetRandomColor(float colorJitter, byte r, byte g, byte b);

    // Clamp a float value to a byte
    byte ClampColor(float value);

    // Move color "origin" one step closer to "destination", along each r/g/b axis
    RGBColor CrossFade(RGBColor origin, RGBColor destination);

}