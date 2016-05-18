#include "TaggedMessage.h"
#include "Constants.h"

namespace ofxSquashBuddies {
	//----------
	TaggedMessage::TaggedMessage() {
	}

	TaggedMessage::~TaggedMessage()
	{
		this -> clear();
	}

	//----------
	TaggedMessage::TaggedMessage(const string & tag, const Message & msg) {
		this->addTag(tag, msg);
	}

	//----------
	void TaggedMessage::clear() {
		this->tagCollection.clear();
		Message::clear();
	}

	//----------
	void TaggedMessage::addTag(const string & tag, const Message & msg){
		tagCollection.push_back(tag);
		string body;
		msg.getData(body);
		this->setData(body);
	}

	//----------
	void TaggedMessage::addTags(const vector<string> & tags, const Message & msg){
		for (auto & tag : tags) {
			tagCollection.push_back(tag);
		}
		string body;
		msg.getData(body);
		this->setData(body);
	}

	//----------
	bool TaggedMessage::tagExists(const string & needle){
		if (tagCollection.size() == 0) {
			OFXSQUASHBUDDIES_ERROR << "tagCollection is empty";
		}
		for (auto & tag : tagCollection) {
			if (ofIsStringInString(tag, needle)) {
				return true;
			}
		}
		OFXSQUASHBUDDIES_ERROR << "tag not found in tagCollection";
		return false;
	}

	//----------
	bool TaggedMessage::hasTags(const Message & msg){

		if (ofIsStringInString(msg.getMessageString(), FRONT_TAG_DELIMITER)) {
			OFXSQUASHBUDDIES_NOTICE << "tags found";
			return true;
		}
		else {
			OFXSQUASHBUDDIES_ERROR << "no tags found";
			return false;
		}
	}

	//----------
	bool TaggedMessage::extractTags(const Message & msg, vector<string>& tags, string & splittedMsg)
	{
		vector<string> splitFront = ofSplitString(msg.getMessageString(), FRONT_TAG_DELIMITER, true, true);
		if (splitFront.size() <= 1) {
			OFXSQUASHBUDDIES_ERROR << "error while trying to find FRONT_TAG_DELIMITER";
			return false;
		}

		vector<string> splitBack = ofSplitString(splitFront[1], BACK_TAG_DELIMITER, true, true);
		if (splitBack.size() < 1) {
			OFXSQUASHBUDDIES_ERROR << "error while trying to find BACK_TAG_DELIMITER";
			return false;
		}
		OFXSQUASHBUDDIES_NOTICE << "\"" << splitBack[0] << "\"";
		tagCollection = ofSplitString(splitBack[0], TAG_SEPARATOR, true, true);

		splittedMsg = splitBack[1];
		tags = tagCollection;
		return true;
	}

	//----------
	const Message & TaggedMessage::getTaggedMessage() {
		string data;
		data = FRONT_TAG_DELIMITER;
		for (auto & tag : tagCollection) {
			data += TAG_SEPARATOR + tag;
		}
		data += BACK_TAG_DELIMITER;

		string onlyBody;
		this->getData(onlyBody);

		message.setData(data + onlyBody);
		return message;
	}
}