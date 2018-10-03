#include "ofApp.h"

int main() {
    ofJson config = ofLoadJson("config.json");
    ofGLFWWindowSettings settings;
    settings.setSize(config["window"]["width"], config["window"]["height"]);
    settings.setPosition({config["window"]["x"], config["window"]["y"]});
    settings.windowMode = config["window"]["fullscreen"] ? OF_FULLSCREEN : OF_WINDOW;
    settings.decorated = config["window"]["decorated"];
    ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
