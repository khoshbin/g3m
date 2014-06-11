//
//  GEORasterSymbol.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 7/9/13.
//
//

#ifndef __G3MiOSSDK__GEORasterSymbol__
#define __G3MiOSSDK__GEORasterSymbol__

#include "GEOSymbol.hpp"
#include <vector>

#include "Vector2F.hpp"
class GEORasterProjection;
class ICanvas;
class GEO2DPolygonData;
#include "QuadTree.hpp"

class GEO2DCoordinatesData;

class GEORasterSymbol : public GEOSymbol, public QuadTree_Content {
private:
  GEORasterSymbol(const GEORasterSymbol& that);

  const int _minTileLevel;
  const int _maxTileLevel;

protected:
  const Sector* _sector;

  //static std::vector<std::vector<Geodetic2D*>*>* copyCoordinatesArray(const std::vector<std::vector<Geodetic2D*>*>* coordinatesArray);

  static Sector* calculateSectorFromCoordinates(const std::vector<Geodetic2D*>* coordinates);
  static Sector* calculateSectorFromCoordinatesArray(const std::vector<std::vector<Geodetic2D*>*>* coordinatesArray);

  GEORasterSymbol(const Sector* sector,
                  const int minTileLevel,
                  const int maxTileLevel) :
  _minTileLevel(minTileLevel),
  _maxTileLevel(maxTileLevel)
  {
  }
  
  GEORasterSymbol(const int minTileLevel,
                  const int maxTileLevel) :
  _minTileLevel(minTileLevel),
  _maxTileLevel(maxTileLevel)
  {
  }
  
  

  void rasterLine(const GEO2DCoordinatesData* coordinates,
                  ICanvas*                    canvas,
                  const GEORasterProjection*  projection) const;

  void rasterPolygon(const GEO2DPolygonData*    polygonData,
                     bool                       rasterSurface,
                     bool                       rasterBoundary,
                     ICanvas*                   canvas,
                     const GEORasterProjection* projection) const;

  virtual void rawRasterize(ICanvas*                   canvas,
                            const GEORasterProjection* projection) const = 0;


public:
  virtual ~GEORasterSymbol();

  bool symbolize(const G3MRenderContext* rc,
                 const GEOSymbolizer*    symbolizer,
                 MeshRenderer*           meshRenderer,
                 ShapesRenderer*         shapesRenderer,
                 MarksRenderer*          marksRenderer,
                 GEOTileRasterizer*      geoTileRasterizer) const;

  virtual const Sector* getSector() const = 0;

  void rasterize(ICanvas*                   canvas,
                 const GEORasterProjection* projection,
                 int tileLevel) const;

  // useless, it's here only to make the C++ => Java translator creates an interface intead of an empty class
  void unusedMethod() const {
  }

  static std::vector<Geodetic2D*>* copyCoordinates(const std::vector<Geodetic2D*>* coordinates);

};

#endif
