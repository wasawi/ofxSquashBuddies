#pragma once

#include "Message.h"

#include <string>
using namespace std;

#define FRONT_TAG_DELIMITER "[/F]"
#define BACK_TAG_DELIMITER "[/B]"
#define TAG_SEPARATOR " "

namespace ofxSquashBuddies {

	class TaggedMessage : public Message {
	public:
		TaggedMessage();
		~TaggedMessage();
		TaggedMessage(const string &, const Message &);
		void clear();

		void addTag(const string &, const Message &);
		void addTags(const vector<string> &, const Message &);

		bool tagExists(const string &);
		bool hasTags(const Message &);

		bool extractTags(const Message &, vector <string> &, string &);
		const Message & getTaggedMessage();

	protected:
		Message message;

		vector <string> tagCollection;
//		uint16_t		maxTagCollectionChars = 1000;
	};
}