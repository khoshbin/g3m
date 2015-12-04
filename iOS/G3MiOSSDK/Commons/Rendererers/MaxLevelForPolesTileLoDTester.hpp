//
//  MaxLevelForPolesTileLoDTester.hpp
//  G3MiOSSDK
//
//  Created by Jose Miguel SN on 4/12/15.
//
//

#ifndef MaxLevelForPolesTileLoDTester_hpp
#define MaxLevelForPolesTileLoDTester_hpp

#include "TileLoDTester.hpp"
#include "Tile.hpp"

class MaxLevelForPolesTileLoDTester: public TileLoDTester{
protected:

  bool _meetsRenderCriteria(int testerLevel,
                            Tile* tile) const{
    
    if (tile->_sector.touchesPoles()){
      if (tile->_level >= _maxLevelForPoles){
        return true;
      }
    }
    
    return false;
  }
  
  bool _isVisible(int testerLevel,
                  Tile* tile) const{
    return true;
  }
  
  int _maxLevelForPoles;
  
public:
  
  MaxLevelForPolesTileLoDTester(int maxLevelForPoles,
                     TileLoDTester* nextTesterRightLoD,
                     TileLoDTester* nextTesterWrongLoD,
                     TileLoDTester* nextTesterVisible,
                     TileLoDTester* nextTesterNotVisible):
  _maxLevelForPoles(maxLevelForPoles),
  TileLoDTester(nextTesterRightLoD,
                nextTesterWrongLoD,
                nextTesterVisible,
                nextTesterNotVisible){}
  
  
  ~MaxLevelForPolesTileLoDTester(){
  }
  
};

#endif /* MaxLevelForPolesTileLoDTester_hpp */
