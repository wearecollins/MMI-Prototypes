//
//  Lines.h
//  MuppetType
//
//  Created by Brett Renfer on 9/3/15.
//
//

#pragma once

class Text : public ofRectangle {
public:
    
    void setup( string str, ofTrueTypeFont & font ){
        vector<string> split = ofSplitString(str, ":");
        this->text = split[1];
        this->font = &font;
        
        this->width = this->font->stringWidth(this->text);
        this->height = this->font->stringHeight(this->text);
        color.setSaturation(200);
        color.setBrightness(255);
        color.setHue(ofToInt(split[0]) * 75);
        
        hue = color.getHue();
    }
    
    void draw(){
        ofSetColor(color);
        this->font->drawString(text, this->x, this->y);
        color.setHue(hue + ofNoise( this->x + ofGetElapsedTimeMillis() * .001));
    }
    
protected:
    string text;
    ofTrueTypeFont * font;
    ofColor color;
    float hueSpeed;
    float hue;
};

class Line : public ofRectangle {
public:
    
    Line(){
        spacing.set("Spacing", 20, 0, 100);
        speed.set("Speed", ofRandom(-1), -0, -10.);
    }
    
    void setup( vector<string> lines, ofTrueTypeFont & font  ){
        float tx = 0;
        for ( auto & s : lines ){
            textNodes.push_back(Text());
            textNodes.back().setup( s, font );
            textNodes.back().x = tx;
            
            tx += textNodes.back().width + spacing;
        }
    }
    
    void draw(){
        this->x += speed;
        ofPushMatrix();
        ofTranslate(this->x, this->y);
        float minX = FLT_MAX;
        float maxX = FLT_MIN;
        
        for ( auto & t : textNodes ){
            float rp = this->x + t.x;
            if ( rp < ofGetWidth() && rp > -t.width ){
                t.draw();
            }
            if ( t.x < minX ){
                minX = t.x;
            }
            if ( t.x + t.width > maxX ){
                maxX = t.x;
            }
        }
        ofPopMatrix();
        
        if ( speed > 0 ){
            for ( auto it = textNodes.begin(); it != textNodes.end(); ++it){
                if ( this->x + it->x > ofGetWidth()){
                    it->x = minX;
                    minX = it->x - it->width - spacing;
                }
            }
        } else {
            for ( auto it = textNodes.begin(); it != textNodes.end(); ++it){
                if ( this->x + it->width + it->x < 0 ){
                    it->x = maxX + spacing;
                    maxX = it->x + spacing;
                }
            }
        }
    }
    
    ofParameter <float> spacing;
    ofParameter <float> speed;
    
    vector<Text> textNodes;
};
