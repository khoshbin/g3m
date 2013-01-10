package org.glob3.mobile.generated; 
//
//  DirectMesh.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 12/1/12.
//
//

//
//  DirectMesh.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 12/1/12.
//
//



public class DirectMesh extends AbstractMesh
{
//C++ TO JAVA CONVERTER WARNING: 'const' methods are not available in Java:
//ORIGINAL LINE: void rawRender(const G3MRenderContext* rc, const GLState& parentState) const
  protected final void rawRender(G3MRenderContext rc, GLState parentState)
  {
	GL gl = rc.getGL();
  
	final int verticesCount = getVertexCount();
	gl.drawArrays(_primitive, 0, verticesCount);
  }


  public DirectMesh(int primitive, boolean owner, Vector3D center, IFloatBuffer vertices, float lineWidth, float pointSize, Color flatColor, IFloatBuffer colors)
  {
	  this(primitive, owner, center, vertices, lineWidth, pointSize, flatColor, colors, 0.0f);
  }
  public DirectMesh(int primitive, boolean owner, Vector3D center, IFloatBuffer vertices, float lineWidth, float pointSize, Color flatColor)
  {
	  this(primitive, owner, center, vertices, lineWidth, pointSize, flatColor, null, 0.0f);
  }
  public DirectMesh(int primitive, boolean owner, Vector3D center, IFloatBuffer vertices, float lineWidth, float pointSize)
  {
	  this(primitive, owner, center, vertices, lineWidth, pointSize, null, null, 0.0f);
  }
//C++ TO JAVA CONVERTER NOTE: Java does not allow default values for parameters. Overloaded methods are inserted above.
//ORIGINAL LINE: DirectMesh(const int primitive, boolean owner, const Vector3D& center, IFloatBuffer* vertices, float lineWidth, float pointSize, Color* flatColor = null, IFloatBuffer* colors = null, const float colorsIntensity = 0.0f) : AbstractMesh(primitive, owner, center, vertices, lineWidth, pointSize, flatColor, colors, colorsIntensity)
  public DirectMesh(int primitive, boolean owner, Vector3D center, IFloatBuffer vertices, float lineWidth, float pointSize, Color flatColor, IFloatBuffer colors, float colorsIntensity)
  {
	  super(primitive, owner, center, vertices, lineWidth, pointSize, flatColor, colors, colorsIntensity);
  }

  public void dispose()
  {

  }

}