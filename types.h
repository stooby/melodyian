#pragma once
#include <Arduino.h>
#include <stddef.h>

#define DEBUG true

typedef unsigned char byte;

struct RGBColor {
  byte r;
  byte g;
  byte b;

   bool operator==(const RGBColor& rhs) const {
        return this->r == rhs.r && this->g == rhs.g && this->b == rhs.b;
    }
};