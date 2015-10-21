#include "ofApp.h"

string code = "";
string messageBuffer = "";

//--------------------------------------------------------------
void ofApp::setup(){
    ofFile settings;
    settings.open("settings.txt");
    auto buff = settings.readToBuffer();
    auto lines = buff.getLines();
    auto it = lines.begin();
    
    int dev = ofToInt( *it );
    device.setup(dev, 9600);
    
    it++;
    string server = *it;
    
    ofLogError()<<server;
    
    spacebrew.addPublish("serialRead", Spacebrew::TYPE_STRING);
    spacebrew.connect(server, Spacebrew::SPACEBREW_PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    unsigned char bytesReturned[1];
    
    if ( device.available() ){
        while (device.available() > 0)
        {
            // we'll put the incoming bytes into bytesReturned
            device.readBytes(bytesReturned, 1);
            
            // if we find the splitter we put all the buffered messages
            //   in the final message, stop listening for more data and
            //   notify a possible listener
            // else we just keep filling the buffer with incoming bytes.
            if (*bytesReturned == '\n')
            {
                code = messageBuffer;
                messageBuffer = "";
                
                spacebrew.send("serialRead", Spacebrew::TYPE_STRING, code);
                break;
            }
            else
            {
                if (*bytesReturned != '\r')
                    messageBuffer += *bytesReturned;
            }
            //cout << "  messageBuffer: " << messageBuffer << "\n";
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString( code, 20, 20);
}