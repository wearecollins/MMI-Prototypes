//
//  SvgGrid.h
//  IntroWallGrid
//
//  Created by Brett Renfer on 10/15/15.
//
//

#pragma once

#include "ofMain.h"
#include "Grid.h"
#include "Svg2Mesh.h"

class SvgGrid : public Grid {
public:
    
    SvgGrid():
    Grid(){
        
    }
    
    void setup( string svg ){
        Svg2Mesh::addSvgToMeshVector(svg, meshes);
        totalWidth = 0;
        
        for ( auto & m : meshes ){
            texCoords.push_back(TexCoords());
            texCoords.back().setup( m.getVertices() );
            colors.push_back(ofFloatColor(1.));
            colors.back().setSaturation(.5);
            colors.back().setBrightness(.7);
            colors.back().setHue(ofRandomuf());
            
            auto mx = texCoords.back().dims.x + texCoords.back().origin.x;
            if ( mx > totalWidth ){
                totalWidth = mx;
            }
        }
    }
    
protected:
};