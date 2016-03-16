//
//  Constants.h
//  EW_Performance
//
//  Created by Brett Renfer on 1/28/16.
//
//

#pragma once

#include "ofColor.h"

namespace mmi {
    enum Mode {
        MODE_INTRO = 0,
        MODE_PREVIEW_1,
        MODE_PREVIEW_2,
        MODE_SELECT,
        MODE_GETREADY,
        MODE_PERFORM,
        MODE_WATCH,
        MODE_SHARE
    };
}

static string FONT_HEAD = "NB International Bold";
static string FONT_LIGHT = "NB International Light";

static int FONT_HEAD_SIZE = 30;
static int FONT_BUTTON_SIZE = 15;

static const ofColor BUTTON_COLOR = ofColor(0,150,10);
static const ofColor BUTTON_COLOR_ACTIVE = ofColor(0,200,50);