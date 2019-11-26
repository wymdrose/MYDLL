#include "../../lib/wensn.h"
#include "../../lib/communicateLib.h"

#pragma comment(lib, "CommunicateLib.lib")

using namespace InstrumentApi;

std::shared_ptr<CommunicateDrose::CommunicateInterface> mpCommunicate;

wensn::wensn()
{

}

wensn::~wensn()
{

}

Wst60m485::Wst60m485(unsigned int portNo, int baudRate){
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>
		(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate, 10, 3000));
}

bool Wst60m485::init(){
	if (mpCommunicate->init() == false){
		return false;
	}
	return true;
}

int Wst60m485::getVoice(){

	unsigned short voice;
	QByteArray tSend;

	//0A 04 00 00 00 03 B1 70
	tSend.resize(8);
	tSend[0] = 0x0A;
	tSend[1] = 0x04;
	tSend[2] = 0x00;
	tSend[3] = 0x00;
	tSend[4] = 0x00;
	tSend[5] = 0x03;
	tSend[6] = 0xB1;
	tSend[7] = 0x70;

	QByteArray tRecv;

	mpCommunicate->communicate(tSend, tRecv);

	unsigned char tArray[2];
	tArray[0] = tRecv[6];
	tArray[1] = tRecv[5];

	memcpy(&voice, tArray, 2);

	return voice;
}

int Wst60m485::getVoices()
{
	QVector<int> vVoices;

	for (size_t i = 0; i < 5; i++)
	{
		int voice = getVoice();
		if (voice)
		{
			vVoices.push_back(voice);
		}
	}

	int voicesum(0);
	for (size_t i = 0; i < vVoices.length(); i++)
	{
		voicesum += vVoices[i];
	}

	return voicesum / vVoices.length();
}

int Wst60m485::getVoiceMax()
{
	int max(0);
	for (size_t i = 0; i < 10; i++)
	{
		int voice = getVoice();
		if (voice > max)
		{
			max = voice;
		}
		_sleep(300);
	}

	return max;
}