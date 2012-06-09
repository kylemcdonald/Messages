#pragma once

#include "ofMain.h"

class ofAutoShader : public ofShader {
public:
	void setup(string name) {
		this->name = name;
		ofAddListener(ofEvents.update, this, &ofAutoShader::update);
	}
	
	void update(ofEventArgs &args) {	
		bool needsReload = false;
			
		string fragName = name + ".fs";
		ofFile fragFile(fragName);
		if(fragFile.exists()) {
			Poco::Timestamp fragTimestamp = fragFile.getPocoFile().getLastModified();
			if(fragTimestamp != lastFragTimestamp) {
				needsReload = true;
				lastFragTimestamp = fragTimestamp;
			}
		} else {
			fragName = "";
		}
		
		string vertName = name + ".vs";
		ofFile vertFile(vertName);
		if(vertFile.exists()) {
			Poco::Timestamp vertTimestamp = vertFile.getPocoFile().getLastModified();
			if(vertTimestamp != lastVertTimestamp) {
				needsReload = true;
				lastVertTimestamp = vertTimestamp;
			}
		} else {
			vertName = "";
		}
		
		if(needsReload) {
			ofShader::load(vertName, fragName);
		}
	}
private:
	string name;
	Poco::Timestamp lastFragTimestamp, lastVertTimestamp;
};

// t t 12 t 8 63 t 4 // 4 4
// t t 9 t 13 25 t 6 // 4 4
// t t 8 t 15 t 8 20 t 19 5 t t 3 // 7 7
// t t t 9 t 8 63 t 4 // 5 4
// t t 5 t 8 t 16 // 4 2
// t t 11 t 8 123 t 3 // 4 4
// t 4095 255 t t t 13 12 127 t 234 8 3 3 14 // 5 10
// t 6 t t t 16 10 t 11 7 // 5 5
// t t 16 t 6 69 t 9 // 4 4
// t t 9 t 7 t t 11 t 9 // 6 4
// t 5 t 7 t 3 t 4 10 // 4 5
// t 7 t t 6 10 4 t t 13 t 6 // 6 6
// t 4096 t t t 255 t 4 1 t 3 t 8192 t 2 t // 9 7
// t t 8 t 9 46 t 8 t t 13 t 6 // 7 6

//microcode(t*((t>>12|t>>8)&63&t>>4));//viznut-0
//microcode(t*((t>>9|t>>13)&25&t>>6);}//visy-0
//microcode((t*(t>>8*(t>>15|t>>8)&(20|(t>>19)*5>>t|t>>3))));//visy-1
//microcode(t*(t>>((t>>9|t>>8))&63&t>>4));//visy-2
//microcode((t*(t>>5|t>>8))>>(t>>16));//tejeez-0
//microcode(t*(t>>11&t>>8&123&t>>3));//tejeez-1
//microcode(((-t&4095)*(255&t*(t&t>>13))>>12)+(127&t*(234&t>>8&t>>3)>>(3&t>>14)));//tejeez-2
//microcode((t>>6|t|t>>(t>>16))*10+((t>>11)&7), viznut);
//microcode(v=(v>>1)+(v>>4)+t*(((t>>16)|(t>>6))&(69&(t>>9))), pyryp);
//microcode((t|(t>>9|t>>7))*t&(t>>11|t>>9), red-);
//microcode(t*5&(t>>7)|t*3&(t*4>>10), miiro);
//microcode((t>>7|t|t>>6)*10+4*(t&t>>13|t>>6), viznut);
//microcode(((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)), skurk);
//microcode(((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6), xpansive);
	
class OneLiner {
private:
	static string randomOperator() {
		int c = ofRandom(148);
		if(c < 1) return "/";
		else if(c < 2) return "%";
		else if(c < 3) return "<<";
		else if(c < 4) return "^";
		else if(c < 11) return "+";
		else if(c < 19) return "-";
		else if(c < 41) return "|";
		else if(c < 64) return "*";
		else if(c < 92) return "&";
		return ">>";
	}
	static string randomQuantity() {
		int x;
		if(ofRandom(1) < .75) {
			x = 1 + (int) ofRandom(16);
		} else {
			int bases[5] = {24, 48, 64, 128, 255};
			x = bases[(int) ofRandom(5)];
			if(ofRandom(1) < .25) {
				x += ofRandom(-8, +8);
			}
		}
		return ofToString(x);
	}
	static string randomNode(bool useT, int levels) {
		levels--;
		if(levels > 0 && ofRandom(1) < .9) {
			if(ofRandom(1) < .1) {
				return "(" + randomNode(true, levels) + ">0?" + randomNode(true, levels) + ":" + randomNode(false, levels) + ")";
			} else {
				bool curUseT = ofRandom(1) < .8;
				return "(" + randomNode(curUseT, levels) + randomOperator() + randomNode(!curUseT, levels) + ")";
			}
		} else {
			if(useT) {
				return "t";
			} else {
				return randomQuantity();
			}
		}
	}
public:
	static string build() {
		return randomNode(true, 6);
	}
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void audioOut(float* input, int n, int channels);
	ofSoundStream soundStream;	
	
	ofAutoShader shader;
	ofFbo fbo;
	ofPixels audioPixels;
	unsigned long time;
	unsigned int rateDivider;
	int curCount;
};
