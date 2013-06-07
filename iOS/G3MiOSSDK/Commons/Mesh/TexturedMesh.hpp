//
//  TexturedMesh.hpp
//  G3MiOSSDK
//
//  Created by José Miguel S N on 12/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef G3MiOSSDK_TexturedMesh_hpp
#define G3MiOSSDK_TexturedMesh_hpp

#include "Mesh.hpp"
#include "TextureMapping.hpp"
#include "Vector3D.hpp"

#include "GLClient.hpp"


class TexturedMesh: public Mesh
{
private:
  Mesh*           _mesh;
  const TextureMapping* _textureMapping;
  const bool            _ownedMesh;
  const bool            _ownedTexMapping;
  const bool            _transparent;
  
  GLState _glState;
  
  void createGLState();

  
public:
  
  TexturedMesh(Mesh* mesh,
               bool ownedMesh,
               TextureMapping* const textureMapping,
               bool ownedTexMapping,
               bool transparent) :
  _mesh(mesh),
  _ownedMesh(ownedMesh),
  _textureMapping(textureMapping),
  _ownedTexMapping(ownedTexMapping),
  _transparent(transparent)
  {
    //addChild((Mesh*)mesh); //New and only child (not const)!!
    createGLState();
  }
  
  ~TexturedMesh(){
    if (_ownedMesh) {
      delete _mesh;
    } 
    if (_ownedTexMapping){
      delete _textureMapping;
    }
  }
  
  void render(const G3MRenderContext* rc) const;

  Extent* getExtent()  const {
    return (_mesh == NULL) ? NULL : _mesh->getExtent();
  }
  
  int getVertexCount() const {
    return _mesh->getVertexCount();
  }
  
  const Vector3D getVertex(int i) const {
    return _mesh->getVertex(i);
  }
  
  const TextureMapping* const getTextureMapping() const {
    return _textureMapping;
  }

  bool isTransparent(const G3MRenderContext* rc) const {
    return _transparent;
  }
  
  void notifyGLClientChildrenParentHasChanged();
  void modifyGLGlobalState(GLGlobalState& GLGlobalState) const;
  void modifyGPUProgramState(GPUProgramState& progState) const;
  
  //Scene Graph Node
//  void rawRender(const G3MRenderContext* rc, GLStateTreeNode* myStateTreeNode);
//  bool isVisible(const G3MRenderContext* rc);
  void modifiyGLState(GLState* state);
  void updateGPUUniform(GLStateTreeNode* stateNode, GPUProgramState* progState, const std::string& name){}
  
  
  void render(const G3MRenderContext* rc, GLState* parentState);
};

#endif
