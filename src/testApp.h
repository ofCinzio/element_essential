#pragma once

//warp values:
#define MAX_RESOLUTION  17
#define MAX_POINTS 2048

#include "ofMain.h"
#include "elementMain.h"
#include "elementWarp.h"
#include "ofxUI.h"

#define ELM_STEREO_ANAGLYPH     0
#define ELM_MONO                1
#define ELM_STEREO_OPENGL       2
#define ELM_STEREO_LEFTRIGHT    3
#define ELM_STEREO_TOPBOTTOM    4

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawScene();
	void PlaceSceneElements();
    void mouseMovedEvent(ofMouseEventArgs &args);

    void guiEvent(ofxUIEventArgs &e);   

	elementVideo			elemV1;
    
    ofFbo                   fboGui;
    ofFbo                   fboLeft;
    ofFbo                   fboRight;

	int						outputX;
	int						outputY;

    bool                    bGLStereoCapable;
    bool                    QuadBufferCapable;
    
    bool                    bMoveGui;
    
    string                  comandi;
    
    int                     outputMode;
    bool                    swapLeftRight;
    
    ofTrueTypeFont          fontLarge;
    ofTrueTypeFont          fontSmall;
    ofTrueTypeFont          fontMedium;
    
    ofImage                 logo;
    ofImage                 previewGrid;
    ofImage                 sfondo;
    
    //interfaccia minimale
    ofxUICanvas*            minimalGUI;
    ofxUIButton*            minGuiMove;
    ofxUILabelButton*       minPlay;
    ofxUILabelButton*       minPause;
    ofxUILabelButton*       minRev;
    

};
