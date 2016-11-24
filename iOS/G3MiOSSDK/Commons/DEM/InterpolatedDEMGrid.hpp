//
//  InterpolatedDEMGrid.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/18/16.
//
//

#ifndef InterpolatedDEMGrid_hpp
#define InterpolatedDEMGrid_hpp

#include "DecoratorDEMGrid.hpp"


class InterpolatedDEMGrid : public DecoratorDEMGrid {
private:
  InterpolatedDEMGrid(const DEMGrid*  grid,
                      const Sector&   sector,
                      const Vector2I& extent);

public:
  static InterpolatedDEMGrid* create(const DEMGrid*  grid,
                                     const Vector2S& extent);

  double getElevationAt(int x, int y) const;

};

#endif