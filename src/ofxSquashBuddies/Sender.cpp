#include "Sender.h"
#include "Constants.h"

namespace ofxSquashBuddies {
	//----------
	Sender::Sender() {
		this->setCodec(this->getDefaultCodec());
	}

	//----------
	Sender::~Sender() {
		this->close();
	}

	//----------
	void Sender::init(string ipAddress, int port) {
		this->close();

		//recreate the thread channels
		this->appToCompressor = make_shared<ofThreadChannel<Message>>();
		this->compressorToSocket = make_shared<ThreadChannel<Packet>>();

		this->socket = make_shared<ofxAsio::UDP::Client>();

		{
			auto lock = unique_lock<mutex>(this->configMutex);
			this->config.endPoint = ofxAsio::UDP::EndPoint(ipAddress, port);
		}

		this->threadsRunning = true;
		this->compressThread = thread([this]() {
			this->compressLoop();
		});
		this->socketThread = thread([this]() {
			this->socketLoop();
		});
	}

	//----------
	void Sender::close() {
		this->threadsRunning = false;

		if (this->appToCompressor) {
			this->appToCompressor->close();
		}
		if (this->compressorToSocket) {
			this->compressorToSocket->close();
		}

		if (this->compressThread.joinable()) {
			this->compressThread.join();
		}
		if (this->socketThread.joinable()) {
			this->socketThread.join();
		}

		this->appToCompressor.reset();
		this->compressorToSocket.reset();
	}

	//----------
	void Sender::setCodec(const ofxSquash::Codec & codec) {
		this->codec = codec;
	}

	//----------
	const ofxSquash::Codec & Sender::getCodec() const {
		return this->codec;
	}

	//----------
	bool Sender::send(const void * data, size_t size) {
		return this->send(move(Message(data, size)));
	}

	//----------
	bool Sender::send(const string & data) {
		return this->send(move(Message(data)));
	}

	//----------
	bool Sender::send(const ofPixels & data) {
		return this->send(move(Message(data)));
	}

	//----------
	bool Sender::send(const ofMesh & data) {
		return this->send(move(Message(data)));
	}

	//----------
	bool Sender::send(const Message & message) {
		auto messageCopy = message;
		return this->send(move(messageCopy));
	}

	//----------
	bool Sender::send(Message && message) {
		if (!this->appToCompressor) {
			OFXSQUASHBUDDIES_ERROR << "You cannot call send before you call init";
			return false;
		}
		else {
			if (compressorToSocket->size() < this->maxSocketBufferSize) {
				auto success = this->appToCompressor->send(move(message));
				if (success) {
					this->sendFramerateCounter.addFrame();
					this->sendFramerateCounter.update();
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}
	
	//----------
	bool Sender::send(const string & label, const void * data, size_t size) {
		return this->send(move(Message(label, data, size)));
	}

	//----------
	float Sender::getSendFramerate() const {
		return this->sendFramerateCounter.getFrameRate();
	}

	//----------
	void Sender::setMaxSocketBufferSize(size_t maxSocketBufferSize) {
		this->maxSocketBufferSize = maxSocketBufferSize;
	}

	//----------
	size_t Sender::getMaxSocketBufferSize() const {
		return this->maxSocketBufferSize;
	}

	//----------
	size_t Sender::getCurrentSocketBufferSize() const {
		return this->compressorToSocket->size();
	}

	//----------
	size_t Sender::getPacketSize() const {
		return this->packetSize;
	}

	//----------
	void Sender::setPacketSize(size_t packetSize) {
		if (packetSize > Packet::PacketAllocationSize) {
			OFXSQUASHBUDDIES_WARNING << "Cannot set packet size to [" << packetSize << "] as it is higher than Packet::PacketSize";
			packetSize = Packet::PacketAllocationSize;
		}
		if (packetSize < Packet::HeaderSize + 16) {
			OFXSQUASHBUDDIES_WARNING << "Cannot set packet size to [" << packetSize << "] as it is lower than the minimum packet size";
			packetSize = Packet::HeaderSize + 16;
		}
		this->packetSize = packetSize;
	}

	//----------
	ofxAsio::UDP::EndPoint Sender::getEndPoint() {
		this->configMutex.lock();
		auto endPoint = this->config.endPoint;
		this->configMutex.unlock();
		return endPoint;
	}

	//----------
	void Sender::compressLoop() {
		uint32_t frameIndex = 0;
		while (this->threadsRunning) {
			Message message;
			while (this->appToCompressor->receive(message)) {
				Packet packet;
				packet.header.packetIndex = 0;
				packet.header.frameIndex = frameIndex;

				struct {
					size_t offset = 0;
					size_t availableBytes;
				} payloadState;
				payloadState.availableBytes = this->packetSize - Packet::HeaderSize;

				if (!this->getCodec().isValid()) {
					OFXSQUASHBUDDIES_ERROR << "Codec [" << this->getCodec().getName() << "] is not valid. Are you sure you have the plugins installed correctly?";
					continue;
				}
				ofxSquash::Stream compressStream(this->getCodec(), ofxSquash::Direction::Compress, [this, &packet, &payloadState](const ofxSquash::WriteFunctionArguments & args) {
					//copy incoming data and split into packets

					struct {
						uint8_t * readPosition;
						size_t availableBytes;
					} inputState;
					inputState.readPosition = (uint8_t*)args.data;
					inputState.availableBytes = args.size;

					while (inputState.availableBytes > 0) {
						auto bytesToCopy = min<size_t>(inputState.availableBytes, payloadState.availableBytes);

						memcpy(packet.payload + payloadState.offset, inputState.readPosition, bytesToCopy);
						
						inputState.readPosition += bytesToCopy;
						inputState.availableBytes -= bytesToCopy;
						payloadState.offset += bytesToCopy;
						payloadState.availableBytes -= bytesToCopy;

						if (payloadState.availableBytes == 0) {
							//finish off the packet header and send whenever we have a full packet
							{
								if (payloadState.offset > numeric_limits<uint32_t>::max()) {
									OFXSQUASHBUDDIES_ERROR << "Payload is too big! Sorry baby.";
								}
								else {
									packet.header.payloadSize = (uint32_t)payloadState.offset;
									this->compressorToSocket->send(packet);
								}
								
								packet.header.packetIndex++;
								packet.header.payloadSize = 0;
							}

							//reset the packet for next use
							{
								payloadState.offset = 0;
								payloadState.availableBytes = this->packetSize - Packet::HeaderSize;
							}
						}
					}
				});

				compressStream << message.getMessageString() << ofxSquash::Stream::Finish();

				//send whatever is left over
				if (payloadState.offset > numeric_limits<uint32_t>::max()) {
					OFXSQUASHBUDDIES_ERROR << "Payload is too big! Sorry baby.";
				}
				else {
					packet.header.payloadSize = (uint32_t) payloadState.offset;
					packet.header.isLastPacket = true;
					this->compressorToSocket->send(packet);
				}

				frameIndex++;
			}
		}
	}

	//----------
	void Sender::socketLoop() {
		while (this->threadsRunning) {
			this->configMutex.lock();
			Config config = this->config;
			this->configMutex.unlock();

			Packet packet;
			while (this->compressorToSocket->receive(packet)) {
				if (this->socket) {
					auto dataGram = make_shared<ofxAsio::UDP::DataGram>();
					dataGram->setEndPoint(config.endPoint);

					auto sendSize = packet.size();
					dataGram->getMessage().set(&packet, sendSize);
					this->socket->send(dataGram);
				}
				else {
					OFXSQUASHBUDDIES_WARNING << "Socket not connected, cannot send packets.";
				}
			}
		}
	}
}