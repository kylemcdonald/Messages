#pragma once

#include "ofMain.h"
#include "OneLiner.h"

class ofApp : public ofBaseApp {
public:
	void setup();
    void exit();
	void update();
	void draw();

	void audioOut(float* input, int n, int channels);
    unsigned int rateDivider;
    float volume;
    ofPixels audioPixels;
	
    int side;
	
	ofShader shader;
	ofFbo fbo;
	unsigned long time;
	int curCount;
	string curStartup;
	
	bool updateShader, updatePixels;
	float jpegRating;
	int jpegSize, jpegMax;
	
	unsigned long nextUpdate;
	string core, source;
	
	ofTrueTypeFont font;
    
    ofJson config;
};
