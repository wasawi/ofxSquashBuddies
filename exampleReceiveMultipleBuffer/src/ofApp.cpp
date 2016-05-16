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

}

//--------------------------------------------------------------
void ofApp::update(){
	this->receiver.update();
	if (this->receiver.isFrameNew()) {
		string data;

		// receive unlabeled buffers
		// this will swap buffers randomly because we don't know what image comes first
		/*		
		this->receiver.receive(data);
		firstBuffer.set(data);
		ofLoadImage(firstImage, firstBuffer);
		this->firstImage.update();

		this->receiver.receive(data);
		secondBuffer.set(data);
		ofLoadImage(secondImage, secondBuffer);
		this->secondImage.update();
		*/	

		// receive labeled buffers

		string label;
		this->receiver.receive(label, data);
		if (label == "first") {
			firstBuffer.set(data);
			ofLoadImage(firstImage, firstBuffer);
			this->firstImage.update();
		}
		else {
			secondBuffer.set(data);
			ofLoadImage(secondImage, secondBuffer);
			this->secondImage.update();
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	int lineH = 20;
	int marginL = 20;	//Left Margin
	int size = 4;

	int monitorW = firstImage.getWidth() / size;
	int monitorH = firstImage.getHeight() / size;

	// draw videos
	this->firstImage.draw(0, 0, monitorW, monitorH);
	this->secondImage.draw(monitorW, 0, monitorW, monitorH);

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 20, 20);

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
		ofToString(firstImage.getWidth()) + " x " +
		ofToString(firstImage.getHeight()),
		UImarginLeft, lineH * 2);

	ofDrawBitmapStringHighlight("Frame size " + ofToString(
		firstImage.getWidth() * firstImage.getHeight() / 1024) + " KB",
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