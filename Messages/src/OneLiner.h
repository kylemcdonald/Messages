#pragma once

class OneLiner {
private:
	static string randomOperator() {
		int t = 0;
		int p[] = {1, 1, 1, 1, 7, 8, 22, 23, 51, 57};
		string q[] = {"/", "%", "<<", "^", "+", "-", "|", "*", "&", ">>"};
		int sum = 0;
		int m = 0;
		for(int i = 0; i < 10; i++) {
			m += p[i];
		}
		int c = ofRandom(m);
		for(int i = 0; i < 10; i++) {
			t += p[i];
			if(c < t) {
				return q[i];
			}
		}
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