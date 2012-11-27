package org.glob3.mobile.generated; 
//***************************************************************

public class BusyMeshEffect extends EffectWithForce
{
  private BusyMeshRenderer _renderer;


  public BusyMeshEffect(BusyMeshRenderer renderer)
  {
	  super(1, 1);
	  _renderer = renderer;
  }

  public void start(G3MRenderContext rc, TimeInterval when)
  {
  }

  public void doStep(G3MRenderContext rc, TimeInterval when)
  {
	super.doStep(rc, when);
	_renderer.incDegrees(5);
  }

  public void stop(G3MRenderContext rc, TimeInterval when)
  {
  }

  public void cancel(TimeInterval when)
  {
	// do nothing, just leave the effect in the intermediate state
  }

}