//
//  Constants.h
//  MMI_Kaleidoscope
//
//  Created by Brett Renfer on 10/13/15.
//
//

#pragma once

static float getTime( bool bIncrement = false ){
    static float frameRate  = 1000./30.;
    static float frame      = 0;
    if ( bIncrement ){
        frame += frameRate;
    }
    return frame;
}