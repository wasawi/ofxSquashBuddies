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
	resize = 1;
	
	// asio
	auto port = 4444;
	string ipAddress = "127.0.0.1";
	this->sender.init(ipAddress, port);

	// frame
	image.load("../../../Women players at the Harvard club.jpg");
	secondImage = firstImage = image;
	firstImage.resize(image.getWidth() / resize, image.getHeight() / resize);
	firstImage.update();
	ofSaveImage(firstImage, firstBuffer);

	secondImage.getPixels().mirror(false, true);
	secondImage.update();
	ofSaveImage(secondImage, secondBuffer);

}

//--------------------------------------------------------------
void ofApp::update(){
	
	int size = image.getWidth() * image.getHeight() * 4;
	
	sender.send(firstBuffer.getBinaryBuffer(), firstBuffer.size());
	sender.send(secondBuffer.getBinaryBuffer(), secondBuffer.size());
}

//--------------------------------------------------------------
void ofApp::draw(){

	int lineH = 20;
	int marginL = 20;	//Left Margin
	int size = 4;

	int monitorW = image.getWidth() / size;
	int monitorH = image.getHeight() / size;
	
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