//
//  Color.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 13/06/12.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.
//

#ifndef G3MiOSSDK_Color
#define G3MiOSSDK_Color

#include <iostream>
#include <string>
#include <sstream>

#include "ILogger.hpp"
#include "IMathUtils.hpp"
#include <string>
#include "Angle.hpp"

class Color {
private:

  Color(const float red,
        const float green,
        const float blue,
        const float alpha) :
  _red(red),
  _green(green),
  _blue(blue),
  _alpha(alpha)
  {

  }
    
  static bool isValidHex(const std::string &hex) {      
      if (hex[0] == '#') {
          if (hex.length() != 7) {
              return false;
          }
      }
      else {
          if (hex.length() != 6) {
              return false;
          }
      }
        
      #ifdef C_CODE
      if(hex.find_first_not_of("#0123456789abcdefABCDEF") != hex.npos){ return false;}
      #endif
      #ifdef JAVA_CODE
      if(!hex.matches("^#?([a-f]|[A-F]|[0-9]){3}(([a-f]|[A-F]|[0-9]){3})?$")){ return false;}
      #endif
    
      return true;
  }
  
  static Color* hexToRGB(std::string hex) {
      if (!isValidHex(hex)) {
          ILogger::instance()->logError("The value received is not avalid hex string!");
      }
      
      if (hex[0] == '#') {
          hex.erase(0,1);
      }
      
      std::string R = hex.substr(0, 2);
      std::string G = hex.substr(2, 2);
      std::string B = hex.substr(4, 2);
      
      return new Color((float)IMathUtils::instance()->parseIntHex(R)/255,(float)IMathUtils::instance()->parseIntHex(G)/255,(float)IMathUtils::instance()->parseIntHex(B)/255,1);
  }

public:
  const float _red;
  const float _green;
  const float _blue;
  const float _alpha;

  Color(const Color& that):
  _red(that._red),
  _green(that._green),
  _blue(that._blue),
  _alpha(that._alpha) {
  }

  ~Color() {

  }

  static Color* parse(const std::string& str);


  static Color fromRGBA255(const int red,
                           const int green,
                           const int blue,
                           const int alpha) {
    return Color(red   / 255.0f,
                 green / 255.0f,
                 blue  / 255.0f,
                 alpha / 255.0f);
  }

  static Color fromRGBA(const float red,
                        const float green,
                        const float blue,
                        const float alpha) {
    return Color(red, green, blue, alpha);
  }

  static Color* newFromRGBA(const float red,
                            const float green,
                            const float blue,
                            const float alpha) {
    return new Color(red, green, blue, alpha);
  }
    
  static Color* newFromHEX(const std::string hex){
      return hexToRGB(hex);
  }

  static Color fromHueSaturationBrightness(double hueInRadians,
                                           float saturation,
                                           float brightness,
                                           float alpha);

  static Color interpolateColor(const Color& from,
                                const Color& middle,
                                const Color& to,
                                float d);

  static Color transparent() {
    return Color::fromRGBA(0, 0, 0, 0);
  }

  static Color black() {
    return Color::fromRGBA(0, 0, 0, 1);
  }

  static Color gray() {
    return Color::fromRGBA(0.5f, 0.5f, 0.5f, 1);
  }

  static Color darkGray() {
    const float oneThird = 1.0f / 3.0f;
    return Color::fromRGBA(oneThird, oneThird, oneThird, 1);
  }

  static Color lightGray() {
    const float twoThirds = 2.0f / 3.0f;
    return Color::fromRGBA(twoThirds, twoThirds, twoThirds, 1);
  }

  static Color white() {
    return Color::fromRGBA(1, 1, 1, 1);
  }

  static Color yellow() {
    return Color::fromRGBA(1, 1, 0, 1);
  }
  
  static Color cyan() {
    return Color::fromRGBA(0, 1, 1, 1);
  }

  static Color magenta() {
    return Color::fromRGBA(1, 0, 1, 1);
  }
  
  static Color red() {
    return Color::fromRGBA(1, 0, 0, 1);
  }

  static Color green() {
    return Color::fromRGBA(0, 1, 0, 1);
  }

  static Color blue() {
    return Color::fromRGBA(0, 0, 1, 1);
  }

  Color mixedWith(const Color& that,
                  float factor) const {
    float frac1 = factor;
    if (frac1 < 0) frac1 = 0;
    if (frac1 > 1) frac1 = 1;

    const float frac2 = 1 - frac1;

    const float newRed   = (_red   * frac2) + (that._red   * frac1);
    const float newGreen = (_green * frac2) + (that._green * frac1);
    const float newBlue  = (_blue  * frac2) + (that._blue  * frac1);
    const float newAlpha = (_alpha * frac2) + (that._alpha * frac1);

    return Color::fromRGBA(newRed, newGreen, newBlue, newAlpha);
  }

  bool isTransparent() const {
    return (_alpha < 1);
  }

  bool isFullTransparent() const {
    return (_alpha < 0.01);
  }

  bool isEquals(const Color& that) const;

  Color wheelStep(int wheelSize,
                  int step) const;

  float getSaturation() const;

  float getBrightness() const;

  double getHueInRadians() const;

  Angle getHue() const {
    return Angle::fromRadians( getHueInRadians() );
  }

  Color adjustBrightness(float brightness) const {
    const float newBrightness = getBrightness() + brightness;
    return Color::fromHueSaturationBrightness(getHueInRadians(),
                                              getSaturation(),
                                              newBrightness,
                                              _alpha);
  }

  Color adjustSaturationBrightness(float saturation,
                                   float brightness) const {
    const float newSaturation = getSaturation() + saturation;
    const float newBrightness = getBrightness() + brightness;
    return Color::fromHueSaturationBrightness(getHueInRadians(),
                                              newSaturation,
                                              newBrightness,
                                              _alpha);
  }

  Color darker() const {
    return adjustBrightness(-0.08f);
  }

  Color twiceDarker() const {
    return adjustBrightness(-0.16f);
  }

  Color muchDarker() const {
    return adjustBrightness(-0.64f);
  }

  Color lighter() const {
    return adjustSaturationBrightness(-0.03f, 0.08f);
  }

  Color twiceLighter() const {
    return adjustSaturationBrightness(-0.06f, 0.16f);
  }
  
  Color muchLighter() const {
    return adjustSaturationBrightness(-0.24f, 0.64f);
  }

  const std::string toID() const;

  const std::string description() const;
#ifdef JAVA_CODE
  @Override
  public String toString() {
    return description();
  }
#endif

};

#endif
