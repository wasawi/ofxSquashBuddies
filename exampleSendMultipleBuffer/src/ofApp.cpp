#include "ofApp.h"

#include "ofxSquash.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// OF
	ofSetWindowTitle("Sender");
	ofSetVerticalSync(false);
	ofSetFrameRate(1000);

	// UI
	UImarginLeft = 500;
	UImarginTop = 100;
	resize = 2;
	imageCount = 4;

	// asio
	auto port = 4444;
	string ipAddress = "127.0.0.1";
	this->sender.init(ipAddress, port);

	// frame
	image.load("../../../Women players at the Harvard club.jpg");
	image.resize(image.getWidth() / resize, image.getHeight() / resize);

	for (int i = 0; i < imageCount; i++) {
		ofImage mirrored = image;
		int vertical = i < 2;
		int horizontal =  i % 2;
		cout << vertical << endl;
		cout << horizontal << endl;
		mirrored.getPixels().mirror(vertical, horizontal);
		ofSaveImage(mirrored, buffer);
		mirrored.update();
		images.push_back(mirrored);
		buffers.push_back(buffer);
		tags.push_back("image_" + ofToString(i));
	}
	withTags = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	// send 4 frames
	if (withTags) {
		for (int i = 0; i < imageCount; i++) {
			ofxSquashBuddies::TaggedMessage taggedMessage;
			ofxSquashBuddies::Message message;
			message.setData(buffers[i].getBinaryBuffer(), buffers[i].size());
			taggedMessage.addTag(tags[i], message);
			sender.send(taggedMessage.getTaggedMessage());
			Sleep(10); // seems to be necessary! why?
		}
	}
	else {
		for (int i = 0; i < imageCount; i++) {
			sender.send(buffers[i].getBinaryBuffer(), buffers[i].size());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	int size = 4;

	int monitorW = image.getWidth() / size;
	int monitorH = image.getHeight() / size;
	
	// draw frames
	for (int i = 0; i < imageCount; i++) {
		int row = i % 2;
		int col = i < 2;
		this->images[i].draw(row * monitorW, col * monitorH, monitorW, monitorH);
		ofDrawBitmapStringHighlight(tags[i], row * monitorW + 5, col * monitorH + 20);
	}

	ofDrawBitmapStringHighlight("press t to toggle tags", 100, 250);

	drawInfo();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 't') {
		withTags = !withTags;
	}
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

	ofDrawBitmapStringHighlight("Frame size " + ofToString(image.getWidth() * image.getHeight() / 1024) + " KB",
		UImarginLeft, lineH * 3);
	//-------
	ofDrawBitmapStringHighlight("getSendFramerate " + ofToString(
		sender.getSendFramerate()),
		UImarginLeft, lineH * 4);

	ofDrawBitmapStringHighlight("sender.getCodecName " +
		sender.getCodec().getName(),
		UImarginLeft, lineH * 5);

	ofDrawBitmapStringHighlight("sender.getCurrentSocketBufferSize " + ofToString(
		sender.getCurrentSocketBufferSize()),
		UImarginLeft, lineH * 6);

	ofDrawBitmapStringHighlight("sender.getMaxSocketBufferSize " + ofToString(
		sender.getMaxSocketBufferSize()),
		UImarginLeft, lineH * 7);

	ofDrawBitmapStringHighlight("sender.getPacketSize " + ofToString(
		sender.getPacketSize()),
		UImarginLeft, lineH * 8);

}