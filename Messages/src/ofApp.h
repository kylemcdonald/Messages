#pragma once

#include "ofMain.h"
#include "OneLiner.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void audioOut(float* input, int n, int channels);
	
	int side;
	
	ofShader shader;
	ofFbo fbo;
	ofPixels audioPixels;
	unsigned long time;
	unsigned int rateDivider;
	int curCount;
	
	bool updateShader, updatePixels;
	float jpegRating;
	int jpegSize, jpegMax;
	
	unsigned long nextUpdate;
	string core, source;
};
