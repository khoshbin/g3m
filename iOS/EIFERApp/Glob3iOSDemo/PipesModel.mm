//
//  PipesModel.cpp
//  EIFER App
//
//  Created by Jose Miguel SN on 23/09/2017.
//
//

#include <stdio.h>

#include "PipesModel.hpp"
#include <G3MiOSSDK/Ditch.hpp>
#include <G3MiOSSDK/CompositeMesh.hpp>

#import <UIKit/UIKit.h>

std::vector<Cylinder *> PipesModel::cylinders;
std::vector<Cylinder::CylinderMeshInfo> PipesModel::cylinderInfo;

void PipesModel::addMeshes(const std::string& fileName, const Planet* p, MeshRenderer* mr, const ElevationData* ed, double heightOffset){
  
  NSString* s = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:fileName.c_str()] ofType:@"csv"] encoding:NSASCIIStringEncoding error:nil ];
  
  int nPipes = 0;
  
  NSArray* lines = [s componentsSeparatedByString:@"\n"];
  for (size_t i = 0; i < lines.count; i++){
    NSArray* numbers = [lines[i] componentsSeparatedByString:@" "];
    
    if (numbers.count == 6){
      
      double lon = ((NSString*)numbers[0]).doubleValue;
      double lat = ((NSString*)numbers[1]).doubleValue;
      double h = ((NSString*)numbers[2]).doubleValue;
      
      double lon2 = ((NSString*)numbers[3]).doubleValue;
      double lat2 = ((NSString*)numbers[4]).doubleValue;
      double h2 = ((NSString*)numbers[5]).doubleValue;
      
      double o1 = ed == NULL? 0.0 : ed->getElevationAt(Geodetic2D::fromDegrees(lat, lon));
      double o2 = ed == NULL? 0.0 : ed->getElevationAt(Geodetic2D::fromDegrees(lat2, lon2));
      
      Geodetic3D g = Geodetic3D::fromDegrees(lat, lon, h + o1 + heightOffset);
      Geodetic3D g2 = Geodetic3D::fromDegrees(lat2, lon2, h2 + o2 + heightOffset);

      //Tubes
      double eDiam = 5;
      Cylinder *c = new Cylinder(p->toCartesian(g), p->toCartesian(g2), eDiam/10);
      
      if (Cylinder::getDitchEnabled()){
          Ditch *ditch = new Ditch(Geodetic3D(g.asGeodetic2D(), g._height - 1.0),
                                   Geodetic3D(g2.asGeodetic2D(), g2._height - 1.0),
                                   eDiam / 5);
              
          Mesh* pipeMesh = (*c).createMesh(Color::fromRGBA255(255,0,0,32), 5, p);
          Mesh* ditchMesh = (*ditch).createMesh(Color::black(), 5, p, ed);
          delete ditch;
              
          CompositeMesh* cm = new CompositeMesh();
              
          cm->addMesh(ditchMesh);
          cm->addMesh(pipeMesh);
              
          mr->addMesh(cm);
      }
      else
      {
          mr->addMesh((*c).createMesh(Color::fromRGBA255(255, 0, 0, 32), 5, p));
      }
      
      cylinderInfo.push_back(Cylinder::CylinderMeshInfo((*c)._info));
      cylinders.push_back(c);
      
      nPipes++;
    }
    
  }
  
  printf("%d pipes created.\n", nPipes);
}

void PipesModel::insertNewCylinder(Geodetic3D &start,Geodetic3D &end, const Planet *p, MeshRenderer *mr, const ElevationData *ed, double heightOffset,
                                   int theId, std::string iMat, std::string eMat,double iDiam, double eDiam, std::string pClass, std::string pType, bool isT, bool isC){
    
    double o1 = (ed == NULL)? 0.0 : ed->getElevationAt(start.asGeodetic2D());
    double o2 = (ed == NULL)? 0.0 : ed->getElevationAt(end.asGeodetic2D());
    double h = start._height;
    double h2 = end._height;
    
    heightOffset = 0;
    
    Geodetic3D g = Geodetic3D(start.asGeodetic2D(),h + o1 + heightOffset);
    Geodetic3D g2 = Geodetic3D(end.asGeodetic2D(),h2 + o2 + heightOffset);
    
    Cylinder *c = new Cylinder(p->toCartesian(g), p->toCartesian(g2), eDiam / 10);
    c->_info.setClassAndType(pClass,pType);
    c->_info.setMaterials(eMat,iMat);
    c->_info.setWidths(iDiam,eDiam);
    c->_info.setTransportComm(isT,isC);
    c->_info.setID(theId);
    
    int red,green;
    if (c->_info.cylinderType.compare("naturalGas") == 0){
        red = 255; green = 0;
    }
    else if (c->_info.cylinderType.compare("High power") == 0){
        red = 255; green = 255;
    }
    else {
        red = 0; green = 255;
    }
    
    if (Cylinder::getDitchEnabled()){
        Ditch *ditch = new Ditch(Geodetic3D(g.asGeodetic2D(), g._height - 1.0),
                                 Geodetic3D(g2.asGeodetic2D(), g2._height - 1.0),
                                 eDiam / 5);
    
        Mesh* pipeMesh = (*c).createMesh(Color::fromRGBA255(red,green,0,32), 5, p);
        Mesh* ditchMesh = (*ditch).createMesh(Color::black(), 5, p, ed);
        delete ditch;
    
        /*CompositeMesh* cm = new CompositeMesh();
    
        cm->addMesh(ditchMesh);
        cm->addMesh(pipeMesh);
    
        mr->addMesh(cm);*/

        //This was changed because of weird behaviour of MeshRenderer->clearMeshes() when CompositeMesh introduced.
        mr->addMesh(ditchMesh);
        mr->addMesh(pipeMesh);
    }
    else {
        mr->addMesh((*c).createMesh(Color::fromRGBA255(red,green,0,32), 5, p));
    }

    cylinderInfo.push_back(Cylinder::CylinderMeshInfo((*c)._info));
    cylinders.push_back(c);
    
}

void PipesModel::reset(){
    for (size_t i=0; i<cylinders.size(); i++){
        delete cylinders.at(i);
    }
    cylinders.clear();
    cylinderInfo.clear(); 
}
