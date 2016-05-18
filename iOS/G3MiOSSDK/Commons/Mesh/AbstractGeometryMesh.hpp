//
//  AbstractGeometryMesh.h
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 23/06/13.
//
//

#ifndef __G3MiOSSDK__AbstractGeometryMesh__
#define __G3MiOSSDK__AbstractGeometryMesh__

#include "Mesh.hpp"

#include "Vector3D.hpp"
#include "GLState.hpp"

class MutableMatrix44D;
class IFloatBuffer;
class Color;

class AbstractGeometryMesh : public Mesh {
  
protected:
  const int               _primitive;
  const Vector3D          _center;
  const MutableMatrix44D* _translationMatrix;
  const IFloatBuffer*           _vertices;
  const bool              _ownsVertices;
  const IFloatBuffer*           _normals;
  const bool              _ownsNormals;
  const float             _lineWidth;
  const float             _pointSize;
  const bool              _depthTest;
  
  const Vector3D          _northVector;
  const Vector3D          _eastVector;
  const Vector3D          _normalVector;

  
  const bool _polygonOffsetFill;
  const float _polygonOffsetFactor;
  const float _polygonOffsetUnits;
  
  mutable BoundingVolume* _extent;
  BoundingVolume* computeBoundingVolume() const;
  BoundingVolume* computeBoundingBox() const;
  BoundingVolume* computeBoundingOrientedBox() const;
  
  AbstractGeometryMesh(const int       primitive,
                       const Vector3D& center,
                       const Vector3D& northVector,
                       const Vector3D& eastVector,
                       const Vector3D& normalVector,
                       IFloatBuffer*   vertices,
                       bool            ownsVertices,
                       IFloatBuffer*   normals,
                       bool            ownsNormals,
                       float           lineWidth,
                       float           pointSize,
                       bool            depthTest,
                       bool polygonOffsetFill,
                       float polygonOffsetFactor,
                       float polygonOffsetUnits);
  
  GLState* _glState;
  
  void createGLState();
  
  virtual void rawRender(const G3MRenderContext* rc) const = 0;

  mutable bool _showNormals;
  mutable Mesh* _normalsMesh;
  Mesh* createNormalsMesh() const;
  
public:
  ~AbstractGeometryMesh();
  
  BoundingVolume* getBoundingVolume() const;
  BoundingVolume* getBoundingBox() const;
  BoundingVolume* getBoundingOrientedBox() const;
  
  #warning temp_Agustin;
  BoundingVolume* getBoundingOrientedBox() {
    return computeBoundingOrientedBox();
  }
  
  
  size_t getVertexCount() const;
  
  const Vector3D getVertex(size_t i) const;
  
  bool isTransparent(const G3MRenderContext* rc) const {
    return false; //TODO: CHECK
  }
  
  void rawRender(const G3MRenderContext* rc,
                 const GLState* parentGLState) const;

  void showNormals(bool v) const {
    _showNormals = v;
  }
  
};

#endif
