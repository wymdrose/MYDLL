#include "../../lib/itech.h"

using namespace InstrumentApi;

itech::itech()
{

}

itech::~itech()
{

}


ItechIt8600::ItechIt8600(unsigned int portNo, int baudRate){
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>
		(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate));
}

ItechIt8600::ItechIt8600(QString ip, int port){
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>
		(std::make_shared<CommunicateDrose::TcpClient>(ip, port));
}

ItechIt8600::~ItechIt8600(){}

bool ItechIt8600::init(){
	if (mpCommunicate->init() == false){
		return 0;
	}

	QString tRecv;
	mpCommunicate->communicate("SYSTem:VERSion?\r\n", tRecv, 5);
	mpCommunicate->communicate("SYSTem:REMote\r\n", tRecv, 0);
	mpCommunicate->communicate("PROT:AUTO:CLE ON\r\n", tRecv, 0);
	mpCommunicate->communicate("FUNC RES\r\n", tRecv, 0);

	return 0;
}

bool ItechIt8600::cmd(const QString cmd){
	QString tRecv;
	mpCommunicate->communicate(cmd, tRecv);

	return true;
}

void ItechIt8600::setRemote()
{
	QString tRecv;

	mpCommunicate->communicate("SYSTem:REMote\r\n", tRecv, 0);
}

void ItechIt8600::setInput(bool in)
{
	QString tRecv;
	QString tSend = in ? "INPut ON\r\n" : "INPut OFF\r\n";
	mpCommunicate->communicate(tSend, tRecv, 0);
}

bool ItechIt8600::setPower(const QString tPower){
	QString tRecv;

	mpCommunicate->communicate("FUNC POW\r\n", tRecv, 0);

	mpCommunicate->communicate("POW " + tPower + "\r\n", tRecv, 0);

	//	mpCommunicate->communicate("RES " + tPower + "\r\n", tRecv, 0);

	/*
	mpCommunicate->communicate("POW?\r\n", tRecv);
	if (tRecv.compare(tPower)){
	return false;
	}
	*/
	return true;
}


//
ItechIt8800::ItechIt8800(unsigned int portNo, int baudRate)
{
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate));
}

ItechIt8800::ItechIt8800(){}

ItechIt8800::~ItechIt8800(){}

bool ItechIt8800::init(){
	if (mpCommunicate->init() == false)	{
		return 0;
	}

	QString tRecv;
	mpCommunicate->communicate("SYSTem:VERSion?\r\n", tRecv, 5);
	mpCommunicate->communicate("SYSTem:REMote\r\n", tRecv, 0);
	return 0;
}

bool ItechIt8800::cmd(const QString cmd)	{
	QString tRecv;
	mpCommunicate->communicate(cmd, tRecv);

	return true;
}

void ItechIt8800::setRemote()
{
	QString tRecv;

	mpCommunicate->communicate("SYSTem:REMote\r\n", tRecv, 0);
}

void ItechIt8800::setInput(bool in)
{
	QString tRecv;
	QString tSend = in ? "INPut ON\r\n" : "INPut OFF\r\n";
	mpCommunicate->communicate(tSend, tRecv, 0);
}

bool ItechIt8800::setCurrent(const QString tCurrent){
	QString tRecv;

	mpCommunicate->communicate("FUNC CURR\r\n", tRecv, 0);

	mpCommunicate->communicate("CURR " + tCurrent + "\r\n", tRecv, 0);


	return true;
}
