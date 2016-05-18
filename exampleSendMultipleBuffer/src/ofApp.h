#pragma once

#include "ofMain.h"
#include "ofxSquashBuddies.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void drawInfo();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxSquashBuddies::Sender sender;

		ofVideoPlayer video;

		vector			<ofImage> images;
		vector			<ofBuffer> buffers;
		vector			<string> tags;

		ofImage			image;
		ofBuffer		buffer;
		string			tag;

		int imageCount;
		int resize;
		int UImarginLeft;
		int UImarginTop;

		bool withTags;
};
