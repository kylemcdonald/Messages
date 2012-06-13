#include "ofApp.h"

int nextPowerOfTwo(int n) {
  return n < 2 ? 1 : 2 * nextPowerOfTwo(n / 2);
}

string buildShader(string bytebeat, int side) {
	return "\
	#extension GL_EXT_gpu_shader4 : enable\n\
	const int width = " + ofToString(side) + ";\n\
	void main() {\n\
	int t = int(gl_FragCoord.y) * width + int(gl_FragCoord.x);\n\
	int x = " + bytebeat + ";\n\
	gl_FragColor = vec4(vec3(float(x % 256) / 256.),1.);\n\
	}";
}

string getSize(string name, int bytes) {
	return name + ": " + ofToString(bytes / 1024., 2) + "KB";
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSoundStreamSetup(2, 0, 48000, 256, 1);
	side = 1024;
	fbo.allocate(side, side);
	time = 0, rateDivider = 8, curCount = 0;
	updateShader = true;
}

void ofApp::update() {	
	if(ofGetSystemTime() > nextUpdate) {
		updateShader = true;		
	}
	
	if(updateShader) {
		core = OneLiner::build();
		source = buildShader(core, side);
		shader.unload();
		shader.setupShaderFromSource(GL_FRAGMENT_SHADER, source);
		shader.linkProgram();
		cout << source << endl;
		updatePixels = true;
		updateShader = false;
	}
}

void ofApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	
	fbo.begin();
	shader.begin();
	ofFill();
	ofRect(0, 0, side, side);
	shader.end();
	fbo.end();
	
	fbo.draw(0, 0);
	
	if(updatePixels) {
		fbo.readToPixels(audioPixels);
		updatePixels = false;
		
		ofImage img = audioPixels;
		img.setImageType(OF_IMAGE_GRAYSCALE);
		
		ofBuffer jpegBuffer;
		ofSaveImage(img.getPixelsRef(), jpegBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
		jpegSize = jpegBuffer.size(), jpegMax = MAX(jpegMax, jpegSize);
		jpegRating = (float) jpegSize / jpegMax;
		
		nextUpdate = ofGetSystemTime() + ((int) (jpegRating * 8) + 1) * 100;
	}
	
	ofTranslate(side, 0);	
	ofTranslate(10, 0);
	
	ofTranslate(0, 20);
	ofDrawBitmapString(getSize("jpeg", jpegSize), 0, 0);
	ofLine(0, 0, side * jpegRating, 0);
	ofDrawBitmapString(source, 0, 20);
}

void ofApp::audioOut(float* output, int n, int channels) {
	ofPixels& cur = audioPixels;
	unsigned char* pixels = cur.getPixels();
	int wh = cur.getWidth() * cur.getHeight();
	int cwh = cur.getNumChannels() * wh;
	if(cwh > 0) {
		for(int i = 0; i < n; i++) {
			int curTime = (time / rateDivider) % wh;
			int curPixel = cur.getNumChannels() * curTime;
			for(int j = 0; j < channels; j++) {
				int cur = pixels[curPixel + j];
				output[i * channels + j] = cur / 128. - 1.;
				output[i * channels + j] *= .05; // make it quiet
			}
			time++;
		}
	}
}