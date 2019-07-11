#include "../../lib/chroma.h"

using namespace InstrumentApi;

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
	if (mpCommunicate->init() == false)
	{
		return 0;
	}

	//		QString tRecv;
	//		mpCommunicate->communicate("*RST;*CLS\r\n", tRecv);

	return 0;
}

bool Chroma62000H::cmd(const QString cmd)
{
	QString tRecv;
	mpCommunicate->communicate(cmd, tRecv);

	return true;
}

bool Chroma62000H::setVoltage(const QString value)
{
	QString tRecv;
	mpCommunicate->communicate("SOUR:VOLT " + value + "\r\n", tRecv, 0);

	return true;
}

bool Chroma62000H::setCurrent(const QString value)
{
	QString tRecv;
	mpCommunicate->communicate("SOUR:CURR " + value + "\r\n", tRecv, 0);

	return true;
}

void Chroma62000H::confOutput(bool on_off)
{
	QString tRecv;
	mpCommunicate->communicate(on_off ? "CONFigure:OUTPut ON\r\n" : "CONFigure:OUTPut OFF\r\n", tRecv, 0);
}

void Chroma62000H::confOutput(QString on_off)
{
	QString tRecv;
	mpCommunicate->communicate("CONFigure:OUTPut" + on_off + "\r\n", tRecv, 0);
}

//
Chroma63800::Chroma63800(unsigned int portNo, int baudRate)
{
	mBgpib = true;
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate));
}

Chroma63800::Chroma63800()
{

}

Chroma63800::~Chroma63800()
{

}

bool Chroma63800::init(int address)
{
	if (mBgpib)
	{
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