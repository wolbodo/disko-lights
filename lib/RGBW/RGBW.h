#pragma once
#include "FastLED.h"

/* FastLED_RGBW
 * 
 * Hack to enable SK6812 RGBW strips to work with FastLED.
 *
 * Original code by Jim Bumgardner (http://krazydad.com).
 * Modified by David Madison (http://partsnotincluded.com).
 * 
*/
#ifndef FastLED_RGBW_h
#define FastLED_RGBW_h
struct CRGBW  {
	union {
		struct {
			union {
				uint8_t g;
				uint8_t green;
			};
			union {
				uint8_t r;
				uint8_t red;
			};
			union {
				uint8_t b;
				uint8_t blue;
			};
			union {
				uint8_t w;
				uint8_t white;
			};
		};
		uint8_t raw[4];
	};
	CRGBW(){}
	CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht){
		r = rd;
		g = grn;
		b = blu;
		w = wht;
	}
	inline void operator = (const CRGB c) __attribute__((always_inline)){ 
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->white = 0;
	}
};
inline uint16_t getRGBWsize(uint16_t nleds){
	uint16_t nbytes = nleds * 4;
	if(nbytes % 3 > 0) return nbytes / 3 + 1;
	else return nbytes / 3;
}
#endif

#include "ArduinoJson.h"

namespace ARDUINOJSON_NAMESPACE {
template <>
struct Converter<CRGB> {
  static void toJson(const CRGB& src, VariantRef dst) {
    dst["r"] = src.r;
    dst["g"] = src.g;
    dst["b"] = src.b;
  }

  static CRGB fromJson(VariantConstRef src) {
    return CRGB(src["r"], src["g"], src["b"]);
  }

  static bool checkJson(VariantConstRef src) {
    return src["r"].is<uint8_t>() && src["g"].is<uint8_t>() && src["b"].is<uint8_t>();
  }
};
}

namespace ARDUINOJSON_NAMESPACE {
template <>
struct Converter<CHSV> {
  static bool toJson(const CHSV& src, VariantRef dst) {
    dst["h"] = src.h;
    dst["s"] = src.s;
    dst["v"] = src.v;
    return true;
  }

  static CHSV fromJson(VariantConstRef src) {
    return CHSV(src["h"], src["s"], src["v"]);
  }

  static bool checkJson(VariantConstRef src) {
    return src["h"].is<uint8_t>() && src["s"].is<uint8_t>() && src["v"].is<uint8_t>();
  }
};
}

