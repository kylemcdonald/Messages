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
    config = ofLoadJson("config.json");
    
    ofHideCursor();
    ofSetVerticalSync(config["window"]["vsync"]);
    
    side = config["side"];
    ofFbo::Settings fboSettings;
    fboSettings.width = side;
    fboSettings.height = side;
    fboSettings.numSamples = 1;
    fboSettings.minFilter = GL_NEAREST;
    fboSettings.maxFilter = GL_NEAREST;
    fbo.allocate(fboSettings);
    time = 0, curCount = 0;
    updateShader = true;
    font.load("uni05_53.ttf", 6, false);
    curStartup = "cache/" + ofGetTimestampString("%Y-%m-%d %H-%M");
    
    if(config["audio"]["enable"]) {
        ofSoundStreamSettings soundSettings;
        soundSettings.setOutListener(ofGetAppPtr());
        soundSettings.numOutputChannels = 2;
        soundSettings.sampleRate = config["audio"]["sampleRate"];
        soundSettings.bufferSize = 256;
        soundSettings.numBuffers = 1;
        volume = config["audio"]["volume"];
        ofSoundStreamSetup(soundSettings);
    }
}

void ofApp::exit() {
    ofSoundStreamClose();
}

void ofApp::update() {
    if(ofGetSystemTimeMillis() > nextUpdate) {
        updateShader = true;
    }
    
    if(updateShader) {
        rateDivider = 1 << ((int) ofRandom(0,5));
        scale = 1 << ((int) ofRandom(0,5));
        xoff = round(ofRandom(0, 1));
        yoff = round(ofRandom(0, 1));
        
        int maxAttempts = 100;
        for(int i = 0; i < maxAttempts; i++) {
            core = OneLiner::build();
            auto result = previous.insert(core);
            if(result.second) {
                break;
            }
        }
        
        source = buildShader(core, side);
        shader.unload();
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, source);
        shader.linkProgram();
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
    ofDrawRectangle(0, 0, side, side);
    shader.end();
    fbo.end();
    
    ofPushMatrix();
    ofScale(scale, scale);
    int offset = (time / rateDivider);
    offset %= side;
    ofTranslate(0, -offset);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    vector<ofColor> colors = {ofColor::red, ofColor::green, ofColor::blue};
    for(int i = 0; i < 3; i++) {
        ofTranslate(xoff, yoff);
        ofSetColor(colors[i]);
        for(int y = 0; y < ofGetHeight() + side; y += side) {
            for(int x = 0; x < ofGetWidth() + side; x += side) {
                fbo.draw(x, y);
            }
        }
    }
    ofDisableBlendMode();
        
    ofPopMatrix();
    
    if(updatePixels) {
        fbo.readToPixels(audioPixels);
        updatePixels = false;

        ofImage img = audioPixels;
        img.setImageType(OF_IMAGE_GRAYSCALE);

        ofBuffer jpegBuffer;
        ofSaveImage(img.getPixels(), jpegBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
        jpegSize = jpegBuffer.size();

        // safety in case there is a bug
        if(jpegSize > side*side || jpegSize <= 4) {
            jpegSize = jpegMax;
        }

        if(jpegSize > jpegMax) {
            jpegMax = jpegSize;

            if(config["cache"]) {
                img.save(curStartup + ".png");
                ofBuffer msg(core.c_str(), core.length());
                ofBufferToFile(curStartup + ".txt", msg);
            }
        }

        jpegRating = (float) jpegSize / jpegMax;

        nextUpdate = ofGetSystemTimeMillis() + ((int) (jpegRating * 4) + 1) * (float) config["duration"];
    }
    
    ofSetColor(255);
    font.drawString(core, 0, ofGetHeight() - 6);
}

void ofApp::audioOut(float* output, int n, int channels) {
    ofPixels& cur = audioPixels;
    unsigned char* pixels = cur.getData();
    int wh = cur.getWidth() * cur.getHeight();
    if(wh > 0) {
        for(int i = 0; i < n; i++) {
            int curTime = (time / rateDivider) % wh;
            int curPixel = cur.getNumChannels() * curTime;
            for(int j = 0; j < channels; j++) {
                int cur = pixels[curPixel + j];
                output[i * channels + j] = cur / 128. - 1.;
                output[i * channels + j] *= volume; // make it quiet
            }
            time++;
        }
    }
}
