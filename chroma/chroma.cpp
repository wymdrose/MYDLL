#include "../../lib/chroma.h"

using namespace InstrumentApi;
using namespace Visa;

static ViJobId job;
static ViAddr uhandle;
static ViStatus status, StatusSession;
static ViSession Sessionparam;
static ViEventType EventTypeparam;
static ViAddr Addressparam;
static ViUInt32 BytesToWrite;
static ViSession defaultRM;
static ViUInt32 rcount, RdCount;
static volatile ViBoolean stopflag = VI_FALSE;
static int letter;
static char stringinput[256];

static ViStatus _VI_FUNCH AsyncHandler(ViSession vi, ViEventType etype, ViEvent event, ViAddr userHandle){
	Sessionparam = vi;
	EventTypeparam = etype;
	Addressparam = userHandle;
	viGetAttribute(event, VI_ATTR_STATUS, &StatusSession);
	viGetAttribute(event, VI_ATTR_RET_COUNT, &RdCount);
	stopflag = VI_TRUE;
	return VI_SUCCESS;
}

chroma::chroma()
{

}

chroma::~chroma()
{

}

Chroma62000H::Chroma62000H(unsigned int portNo, int baudRate)
{
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate));
}

Chroma62000H::Chroma62000H()
{

}

Chroma62000H::~Chroma62000H()
{

}

bool Chroma62000H::init()
{
	mBgpib = false;

	if (mpCommunicate->init() == false)
	{
		return 0;
	}

	//		QString tRecv;
	//		mpCommunicate->communicate("*RST;*CLS\r\n", tRecv);

	return 0;
}

bool Chroma62000H::init(int address)
{
	mBgpib = true;

	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		printf("Could not open a session to the VISA Resource Manager!\n");
		return false;
		exit(EXIT_FAILURE);
	}

	status = viOpen(defaultRM, QString("GPIB::%0::INSTR").arg(address).toStdString().c_str(), VI_NULL, VI_NULL, &mInst);

	status = viInstallHandler(mInst, VI_EVENT_IO_COMPLETION, AsyncHandler, uhandle);
	status = viEnableEvent(mInst, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);

	return true;
}


bool Chroma62000H::cmd(const QString cmd)
{
	if (mBgpib)
	{

	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate(cmd, tRecv);
	}

	return true;
}

float Chroma62000H::getVolt()
{
	if (mBgpib)
	{
		memset(data1, 0, READ_BUFFER_SIZE);
		strcpy(stringinput, "SOUR:VOLT?\r\n");
		BytesToWrite = (ViUInt32)strlen(stringinput);
		status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);
		_sleep(500);
		status = viReadAsync(mInst, data1, READ_BUFFER_SIZE - 1, &job);
		return QString((char*)data1).toFloat();
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("SOUR:VOLT?\r\n", tRecv);
		return tRecv.toFloat();
	}
	
}

bool Chroma62000H::setVoltage(QString value)
{
//	value = QString::number(value.toFloat() + 0.5);

	if (mBgpib)
	{
		viPrintf(mInst, QString("SOUR:VOLT " + value + "\r\n").toStdString().c_str());
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("SOUR:VOLT " + value + "\r\n", tRecv, 0);
	}

	return true;
}

bool Chroma62000H::setCurrent(const QString value)
{
	if (mBgpib)
	{
		viPrintf(mInst, QString("SOUR:CURR " + value + "\r\n").toStdString().c_str());
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("SOUR:CURR " + value + "\r\n", tRecv, 0);
	}

	return true;
}

void Chroma62000H::confOutput(bool on_off)
{
	if (mBgpib)
	{
		viPrintf(mInst, on_off ? "CONFigure:OUTPut ON\r\n" : "CONFigure:OUTPut OFF\r\n");
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate(on_off ? "CONFigure:OUTPut ON\r\n" : "CONFigure:OUTPut OFF\r\n", tRecv, 0);
	}
}

void Chroma62000H::confOutput(QString on_off)
{
	if (mBgpib)
	{
		viPrintf(mInst, QString("CONFigure:OUTPut" + on_off + "\n").toStdString().c_str());
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("CONFigure:OUTPut" + on_off + "\r\n", tRecv, 0);
	}	
}

//
Chroma63800::Chroma63800(unsigned int portNo, int baudRate)
{
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate));
}

Chroma63800::Chroma63800()
{

}

Chroma63800::~Chroma63800()
{

}

bool Chroma63800::init()
{
	if (mpCommunicate->init() == false)
	{
		return 0;
	}

	QString tRecv;

	mpCommunicate->communicate("MODE POW\r\n", tRecv, 0);
	mpCommunicate->communicate("ABA ON\r\n", tRecv, 0);
	mpCommunicate->communicate("CURR:FALL 200\r\n", tRecv, 0);
	mpCommunicate->communicate("CURR:RISE 200\r\n", tRecv, 0);
	mpCommunicate->communicate("CURR:HIGH 45\r\n", tRecv, 0);
	mpCommunicate->communicate("POW:HIGH 4500\r\n", tRecv, 0);
	mpCommunicate->communicate("CURR:MAX 100\r\n", tRecv, 0);
	mpCommunicate->communicate("CURR:PEAK:MAX 135\r\n", tRecv, 0);

	mpCommunicate->communicate("TIME:MODE HODE\r\n", tRecv, 0);
	mpCommunicate->communicate("TIME:OUT 180\r\n", tRecv, 0);
	

	return 0;
}

bool Chroma63800::init(int address)
{
	mBgpib = true;

	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		printf("Could not open a session to the VISA Resource Manager!\n");
		return false;
		exit(EXIT_FAILURE);
	}

	status = viOpen(defaultRM, QString("GPIB::%0::INSTR").arg(address).toStdString().c_str(), VI_NULL, VI_NULL, &mInst);

	status = viInstallHandler(mInst, VI_EVENT_IO_COMPLETION, AsyncHandler, uhandle);
	status = viEnableEvent(mInst, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);

	viPrintf(mInst, "MODE POW\r\n");
	viPrintf(mInst, "CURR:FALL 200\r\n");
	viPrintf(mInst, "CURR:RISE 200\r\n");
	viPrintf(mInst, "CURR:HIGH 45\r\n");
	viPrintf(mInst, "CURR:MAX 45\r\n");
	viPrintf(mInst, "CURR:PEAK:MAX 135\r\n");

	return true;
}

void Chroma63800::setupMode(QString AC_DC)
{
	if (mBgpib)
	{
		viPrintf(mInst, QString("SYSTem:SETup:MODE " + AC_DC + "\n").toStdString().c_str());
	}
	else
	{

	}
}

void Chroma63800::setInput(QString on_off)
{
	if (mBgpib)
	{
		viPrintf(mInst, QString("LOAD " + on_off + "\r\n").toStdString().c_str());
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("LOAD " + on_off + "\r\n", tRecv, 0);
	}
	
}

void Chroma63800::setPower(QString power)
{
	if (mBgpib)
	{
		viPrintf(mInst, QString("POW " + power + "\r\n").toStdString().c_str());
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("MODE POW\r\n", tRecv, 0);
		mpCommunicate->communicate("POW " + power + "\r\n", tRecv, 0);
		mpCommunicate->communicate("CURR:MAX 100\r\n", tRecv, 0);
	}
	
}

bool Chroma63800::setCurrent(const QString current)
{

	if (mBgpib)
	{
		viPrintf(mInst, QString("CURR " + current + "\r\n").toStdString().c_str());
	}
	else
	{
		QString tRecv;
		mpCommunicate->communicate("MODE CURR\r\n", tRecv, 0);
		mpCommunicate->communicate("CURR " + current + "\r\n", tRecv, 0);
	}

	return true;
}
