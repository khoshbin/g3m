package org.glob3.mobile.generated; 
//
//  ProjectedCornersDistanceTileLoDTester.cpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 4/12/15.
//
//

//
//  ProjectedCornersDistanceTileLoDTester.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 4/12/15.
//
//




public class ProjectedCornersDistanceTileLoDTester extends TileLoDTester
{

  protected static class ProjectedCornersDistanceTileLoDTesterData extends TileLoDTesterData
  {

    private double getSquaredArcSegmentRatio(Vector3D a, Vector3D b)
    {
      /*
       Arco = ang * Cuerda / (2 * sen(ang/2))
       */

      final double angleInRadians = Vector3D.angleInRadiansBetween(a, b);
      final double halfAngleSin = java.lang.Math.sin(angleInRadians / 2);
      final double arcSegmentRatio = (halfAngleSin == 0) ? 1 : angleInRadians / (2 * halfAngleSin);
      return (arcSegmentRatio * arcSegmentRatio);
    }

    public double _northArcSegmentRatioSquared;
    public double _southArcSegmentRatioSquared;
    public double _eastArcSegmentRatioSquared;
    public double _westArcSegmentRatioSquared;

    public Vector3D _northWestPoint ;
    public Vector3D _northEastPoint ;
    public Vector3D _southWestPoint ;
    public Vector3D _southEastPoint ;

    public BoundingVolume _bvol;

    public ProjectedCornersDistanceTileLoDTesterData(Tile tile, double mediumHeight, Planet planet)
    {
       super();
       _northWestPoint = new Vector3D(planet.toCartesian(tile._sector.getNW(), mediumHeight));
       _northEastPoint = new Vector3D(planet.toCartesian(tile._sector.getNE(), mediumHeight));
       _southWestPoint = new Vector3D(planet.toCartesian(tile._sector.getSW(), mediumHeight));
       _southEastPoint = new Vector3D(planet.toCartesian(tile._sector.getSE(), mediumHeight));
      final Vector3D normalNW = planet.centricSurfaceNormal(_northWestPoint);
      final Vector3D normalNE = planet.centricSurfaceNormal(_northEastPoint);
      final Vector3D normalSW = planet.centricSurfaceNormal(_southWestPoint);
      final Vector3D normalSE = planet.centricSurfaceNormal(_southEastPoint);

      _northArcSegmentRatioSquared = getSquaredArcSegmentRatio(normalNW, normalNE);
      _southArcSegmentRatioSquared = getSquaredArcSegmentRatio(normalSW, normalSE);
      _eastArcSegmentRatioSquared = getSquaredArcSegmentRatio(normalNE, normalSE);
      _westArcSegmentRatioSquared = getSquaredArcSegmentRatio(normalNW, normalSW);

      //Computing Bounding Volume

      final Mesh mesh = tile.getCurrentTessellatorMesh();
      if (mesh == null)
      {
        ILogger.instance().logError("Problem computing BVolume in ProjectedCornersDistanceTileLoDTesterData");
        _bvol = null;
      }
      else
      {
        _bvol = mesh.getBoundingVolume(); //BV is deleted by mesh
      }

    }

    public final boolean evaluate(Camera camera, double texHeightSquared, double texWidthSquared)
    {

      final double distanceInPixelsNorth = camera.getEstimatedPixelDistance(_northWestPoint, _northEastPoint);
      final double distanceInPixelsSouth = camera.getEstimatedPixelDistance(_southWestPoint, _southEastPoint);
      final double distanceInPixelsWest = camera.getEstimatedPixelDistance(_northWestPoint, _southWestPoint);
      final double distanceInPixelsEast = camera.getEstimatedPixelDistance(_northEastPoint, _southEastPoint);

      final double distanceInPixelsSquaredArcNorth = (distanceInPixelsNorth * distanceInPixelsNorth) * _northArcSegmentRatioSquared;
      final double distanceInPixelsSquaredArcSouth = (distanceInPixelsSouth * distanceInPixelsSouth) * _southArcSegmentRatioSquared;
      final double distanceInPixelsSquaredArcWest = (distanceInPixelsWest * distanceInPixelsWest) * _westArcSegmentRatioSquared;
      final double distanceInPixelsSquaredArcEast = (distanceInPixelsEast * distanceInPixelsEast) * _eastArcSegmentRatioSquared;

      return ((distanceInPixelsSquaredArcNorth <= texHeightSquared) && (distanceInPixelsSquaredArcSouth <= texHeightSquared) && (distanceInPixelsSquaredArcWest <= texWidthSquared) && (distanceInPixelsSquaredArcEast <= texWidthSquared));
    }

  }

  protected final void _onTileHasChangedMesh(int testerLevel, Tile tile)
  {
    //Recomputing data when tile changes tessellator mesh
    tile.setDataForLoDTester(testerLevel, null);
  }

  protected final ProjectedCornersDistanceTileLoDTesterData getData(Tile tile, int testerLevel, G3MRenderContext rc)
  {
    ProjectedCornersDistanceTileLoDTesterData data = (ProjectedCornersDistanceTileLoDTesterData) tile.getDataForLoDTester(testerLevel);
    if (data == null)
    {
      final double mediumHeight = tile.getTessellatorMeshData()._averageHeight;
      data = new ProjectedCornersDistanceTileLoDTesterData(tile, mediumHeight, rc.getPlanet());
      tile.setDataForLoDTester(testerLevel, data);
    }
    return data;
  }

  protected final boolean _meetsRenderCriteria(int testerLevel, Tile tile, G3MRenderContext rc)
  {


    ProjectedCornersDistanceTileLoDTesterData data = getData(tile, testerLevel, rc);

    return data.evaluate(rc.getCurrentCamera(), _texHeightSquared, _texWidthSquared);
  }

  protected final boolean _isVisible(int testerLevel, Tile tile, G3MRenderContext rc)
  {
    ProjectedCornersDistanceTileLoDTesterData data = getData(tile, testerLevel, rc);
    return data._bvol.touchesFrustum(rc.getCurrentCamera().getFrustumInModelCoordinates());
  }

  protected double _texHeightSquared;
  protected double _texWidthSquared;


  public ProjectedCornersDistanceTileLoDTester(double textureWidth, double textureHeight, TileLoDTester nextTesterRightLoD, TileLoDTester nextTesterWrongLoD, TileLoDTester nextTesterVisible, TileLoDTester nextTesterNotVisible)
  {
     super(nextTesterRightLoD, nextTesterWrongLoD, nextTesterVisible, nextTesterNotVisible);
     _texHeightSquared = textureHeight * textureHeight;
     _texWidthSquared = textureWidth * textureWidth;
  }


  public void dispose()
  {
  }

}