#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetLogLevel(OF_LOG_SILENT);
    
    // test that GL_STEREO is working on this machine
    GLboolean	isGLCapable = GL_FALSE;
	glGetBooleanv(GL_STEREO,&isGLCapable);
    QuadBufferCapable=isGLCapable;
    
    GLint maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&maxVertexTextureImageUnits);
    
    if (QuadBufferCapable)
    {
        glDrawBuffer(GL_BACK);
        printf(">> testApp: GL_STEREO OK \n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);
    }
	else {
        
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        printf(">> testApp: GL_STEREO KO !!\n MaxVertexTextureImageUnits %d\n",maxVertexTextureImageUnits);
    }
    
        outputX = ofGetScreenWidth();
        outputY = ofGetScreenHeight();

    bGLStereoCapable=QuadBufferCapable;
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //video
    elemV1.setup("./movies/movie.mov","", outputX,outputY, ELM_INPUT_STEREO_LEFTRIGHT, 0 , 0,"Movie", true);
        
    //video starts at frame number 0, paused and no-looping
    elemV1.leftChannelPlayer.play();
    elemV1.rightChannelPlayer.play();
    elemV1.leftChannelPlayer.firstFrame();
    elemV1.rightChannelPlayer.firstFrame();    
    elemV1.leftChannelPlayer.stop();
    elemV1.rightChannelPlayer.stop();
    elemV1.leftChannelPlayer.setLoopState(OF_LOOP_NONE);
    elemV1.rightChannelPlayer.setLoopState(OF_LOOP_NONE);
    if (elemV1.rightChannelPath=="") {
        elemV1.rightChannelPlayer.closeMovie();
    }
    else {
        elemV1.rightChannelPlayer.play();
        elemV1.rightChannelPlayer.firstFrame();    
        elemV1.rightChannelPlayer.stop();
        elemV1.rightChannelPlayer.setLoopState(OF_LOOP_NONE);
    }

    outputMode = ELM_STEREO_ANAGLYPH;
//    outputMode = ELM_MONO;
    swapLeftRight=false;
        
    comandi ="element.essential\n\nSPACEBAR\tplay/pause\nBACKSPACE\tstop\n\n'.'\t\tnext frame\n','\t\tprev frame\n\n'w'\t\toggle quadwarp mode\n'g'\t\ttoggle mesh warp mode\n\n'0'\t\ttoggle calibration image\n'-'\t\tswap left/right frame";
    
    fontSmall.loadFont("NEOSANS.otf", 7);
    fontMedium.loadFont("NEOSANS.otf", 10);
    fontLarge.loadFont("NEOSANS.otf", 18);
    
	fontSmall.setLineHeight(12.0f);
	fontSmall.setLetterSpacing(1.017);
    
	fontLarge.setLineHeight(12.0f);
	fontLarge.setLetterSpacing(1.017);
    
	fontMedium.setLineHeight(12.0f);
	fontMedium.setLetterSpacing(1.017);
    
    logo.loadImage("./utils/elementMapTrasp.png");
    
//    elemV1.warper.load();
//    elemV1.loadSettings();
    
    fboLeft.allocate(outputX,outputY, GL_RGBA);
    cout << fboLeft.getWidth() << "," << fboLeft.getHeight() << endl;
    fboRight.allocate(outputX,outputY, GL_RGBA);
    fboGui.allocate(outputX,outputY, GL_RGBA);

    // clear the buffers from past (thnkx to matteo)
    fboLeft.begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboLeft.end();
    
    fboRight.begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboRight.end();

    fboGui.begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboGui.end();
    
    //setup minimal interface
    minimalGUI = new ofxUICanvas(10, 80, 380, 100);
    minimalGUI->setColorBack(ofColor(0,0,0,125));
    minimalGUI->setDrawOutline(FALSE);
    minimalGUI->setDrawBack(TRUE);

    //maniglia per spostare l'interfaccia
    bMoveGui = false;
    minGuiMove = new ofxUIButton(0,0, 20, 20, bMoveGui, "MOVE GUI");
    minGuiMove->setLabelVisible(false);
    minGuiMove->setDrawOutline(true);
    minimalGUI->addWidget(minGuiMove);
    
    
    minimalGUI->addWidget(new ofxUILabel(35,5, "ELEMENT ESSENTIAL", OFX_UI_FONT_MEDIUM));
    
    minimalGUI->setVisible(true);
    ofAddListener(minimalGUI->newGUIEvent, this, &testApp::guiEvent);

}

//--------------------------------------------------------------
void testApp::mouseMovedEvent(ofMouseEventArgs &args){
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    elemV1.update();
    elemV1.warper.updateCoordinates();
    
    //clear dall'fbo in cui disegno griglie e controlli warp
    fboGui.begin();
    ofClear(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboGui.end();
    
    fboLeft.begin();
    ofClear(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboLeft.end();
    
    
    fboRight.begin();
    ofClear(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboRight.end();
    
    //////////////////////////
    // LEFT FRAME:
    //////////////////////////
    
    fboLeft.begin();
    ofClear(0,0,0,0);
    
    //ofPushMatrix();
    //inserire qui l'eventuale ofTranslate(parallax,0); (a cui si riferiscono i push-pop matrix)
    elemV1.drawLeft(0, 0, outputX, outputY);
    //ofPopMatrix();
    
    fboLeft.end();
    
    //prepara gli "special"
    fboGui.begin();
    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    if (elemV1.warper.bWarpActive || elemV1.warper.bViewGrid) elemV1.warper.drawElementOutline();
    if (elemV1.warper.bWarpActive) elemV1.warper.drawMarkers();
    if (elemV1.warper.bViewGrid) elemV1.warper.drawGrid();
    ofPopStyle();
    
    fboGui.end();
    
    
    //////////////////////////
    // RIGHT FRAME:
    //////////////////////////
    
    fboRight.begin();
    ofClear(0,0,0,0);
    
    //ofPushMatrix();
    //inserire qui l'eventuale ofTranslate(parallax,0); (a cui si riferiscono i push-pop matrix)
    elemV1.drawRight(0, 0, outputX, outputY);
    //ofPopMatrix();
    
    fboRight.end();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
//    if (QuadBufferCapable) glDrawBuffer(GL_BACK_LEFT);
//    else glDrawBuffer(GL_BACK);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ofBackground(0);

    int _x = 0;
    int _y = 0;
    int _width = outputX;
    int _height = outputY;
    
    switch (outputMode) 
    {
            
        case ELM_MONO:
            fboLeft.draw(_x,_y,_width,_height);
            ofPushStyle();
            fboGui.draw(_x,_y,_width,_height);
            ofPopStyle();
            break;
            
        case ELM_STEREO_ANAGLYPH:
            ofPushStyle();
            glColorMask(true, false, false, true);
            if (swapLeftRight) fboRight.draw(_x,_y,_width,_height);
            else fboLeft.draw(_x,_y,_width,_height);
            
            glColorMask(false, true, true, true);
                if (swapLeftRight) fboLeft.draw(_x,_y,_width,_height);
                else fboRight.draw(_x,_y,_width,_height);
            glColorMask(true, true, true, true);
            ofPopStyle();
            
            ofPushStyle();
            fboGui.draw(_x,_y,_width,_height);
            ofPopStyle();
            break;
            
        case ELM_STEREO_OPENGL:
            glDrawBuffer(GL_BACK_LEFT);
            if (swapLeftRight) fboRight.draw(_x,_y,_width,_height);
            else fboLeft.draw(_x,_y,_width,_height);
            fboGui.draw(_x,_y,_width,_height);
            
            glDrawBuffer(GL_BACK_RIGHT);
            if (swapLeftRight) fboLeft.draw(_x,_y,_width,_height);
            else fboRight.draw(_x,_y,_width,_height);
            fboGui.draw(_x,_y,_width,_height);
            break;
            
        case ELM_STEREO_LEFTRIGHT:
            ofPushStyle();
            if (swapLeftRight) {
                fboRight.draw(_x,_y,_width*.5,_height);
                fboLeft.draw(_x+_width*.5,_y,_width*.5,_height);
            } else {
                fboLeft.draw(_x,_y,_width*.5,_height);
                fboRight.draw(_x+_width*.5,_y,_width*.5,_height);
            }
            ofPopStyle();
            
            ofPushStyle();
            fboGui.draw(_x,_y,_width*.5,_height);
            ofPopStyle();
            
            break;
            
        case ELM_STEREO_TOPBOTTOM:
            ofPushStyle();
            if (swapLeftRight) {
                fboRight.draw(_x,_y,_width,_height*.5);
                fboLeft.draw(_x,_y+_height*.5,_width,_height*.5);
            } else {
                fboLeft.draw(_x,_y,_width,_height*.5);
                fboRight.draw(_x,_y+_height*.5,_width,_height*.5);
            }
            ofPopStyle();
            
            ofPushStyle();
            fboGui.draw(_x,_y,_width,_height*.5);
            ofPopStyle();
            
            break;
            
        default:
            break;
    }   

    
    //da togliere in modalitˆ proeizione
//    logo.draw(20, 20, 45, 45);
    

    
}


//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if (name == "MOVE GUI") {
        if (minGuiMove->getValue()) bMoveGui = true;
    }
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == ' ')
    {
        if (elemV1.leftChannelPlayer.isPlaying()) elemV1.element_videoPause();
        else elemV1.element_videoPlay(1);
    }
    
    else if (key== OF_KEY_BACKSPACE) elemV1.element_videoStop();
    
//    else if(key == '.') elemV1.element_frameAvanti();
//    
//    else if(key == ',') elemV1.element_frameIndietro();        
    
    else if(key == 'i') ofSystemAlertDialog(comandi);
    
    else if(key == '-') swapLeftRight=!swapLeftRight;
    
    else if(key == 'e') elemV1.setElementInputType(ELM_INPUT_STEREO_LEFTRIGHT);
    
    else if(key == '0') elemV1.bDrawCalibrationImg=!elemV1.bDrawCalibrationImg;
        
    else elemV1.warper.warpKeyPressedHandler(key);
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
    elemV1.warper.warpKeyReleasedHandler(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    if (bMoveGui) minimalGUI->getRect()->setPosition(x, y);
    else elemV1.warper.mouseDragged(x, y, button);
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    elemV1.warper.mousePressed(x, y, button);        
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bMoveGui = false;
    
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}


//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
}

