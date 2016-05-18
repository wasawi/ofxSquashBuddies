#include "ofApp.h"

#include "ofxSquash.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// OF
	ofSetWindowTitle("Receiver");
	ofSetVerticalSync(false);
	ofSetFrameRate(1000);

	// UI
	UImarginLeft = 500;
	UImarginTop = 100;

	// asio
	auto port = 4444;
	this->receiver.init(port);

	imageCount = 4;

	image.allocate(100, 100, OF_IMAGE_COLOR);
	image.update();

	for (int i = 0; i < imageCount; i++) {
		images.push_back(image);
		tags.push_back("");
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	this->receiver.update();
	if (this->receiver.isFrameNew()) {
		string data;
		this->receiver.receive(data);

		ofxSquashBuddies::TaggedMessage taggedMessage;
		ofxSquashBuddies::Message message = ofxSquashBuddies::Message(data);
		vector<string> tagsInMessage;

		// with tags
		if (taggedMessage.extractTags(message, tagsInMessage, data)) {
			for (auto & tag : tagsInMessage) {
				buffer.set(data);
				ofLoadImage(image, buffer);
				this->image.update();
				int num = ofToInt(ofSplitString(tag, "_")[1]);
				images[num] = image;
				tags[num] = tag;
			}
		}else {
			//no tags
			int i = (int)ofRandom(4);
			tags[i] = ("image_" + ofToString(i));
			buffer.set(data);
			ofLoadImage(image, buffer);
			this->image.update();
			images[i] = image;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	int size = 4;

	int monitorW = image.getWidth() / size;
	int monitorH = image.getHeight() / size;

	// draw frames
	int i = 0;
	for (auto & img : images) {
		int row = i % 2;
		int col = i < 2;
		img.draw(row * monitorW, col * monitorH, monitorW, monitorH);
		ofDrawBitmapStringHighlight(tags[i], row * monitorW + 5, col * monitorH + 20);
		i++;
	}

	drawInfo();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



//--------------------------------------------------------------
void ofApp::drawInfo() {


	int lineH = 20;

	// draw info
	ofDrawBitmapStringHighlight("App FPS " +
		ofToString(ofGetFrameRate()),
		UImarginLeft, lineH);

	ofDrawBitmapStringHighlight("Video Resolution " +
		ofToString(image.getWidth()) + " x " +
		ofToString(image.getHeight()),
		UImarginLeft, lineH * 2);

	ofDrawBitmapStringHighlight("Frame size " + ofToString(
		image.getWidth() * image.getHeight() / 1024) + " KB",
		UImarginLeft, lineH * 3);
	//-------
	ofDrawBitmapStringHighlight("getIncomingFramerate " + ofToString(
		receiver.getIncomingFramerate()),
		UImarginLeft, lineH * 4);

	ofDrawBitmapStringHighlight("sender.getCodecName " +
		receiver.getCodec().getName(),
		UImarginLeft, lineH * 5);

	ofDrawBitmapStringHighlight("sender.getDroppedFrames " + ofToString(
		receiver.getDroppedFrames().size()),
		UImarginLeft, lineH * 6);

	ofDrawBitmapStringHighlight("message.getBodySize " + ofToString(
		receiver.getMessage().getBodySize()) + " Bytes",
		UImarginLeft, lineH * 7);

	ofDrawBitmapStringHighlight("message.getHeaderSize " + ofToString(
		receiver.getMessage().getHeaderSize()) + " Bytes",
		UImarginLeft, lineH * 8);

	ofDrawBitmapStringHighlight("getIncomingBitrate " + ofToString(
		(receiver.getMessage().getBodySize() / 1024 *
			receiver.getIncomingFramerate()) / 1024) + " MB/s",
		UImarginLeft, lineH * 9);

}