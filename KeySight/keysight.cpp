
#include "../../lib/keysight.h"

using namespace InstrumentApi;

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


bool mBgpib = false;

KeySight::KeySight(){

}

KeySight::~KeySight(){

}

Ks34970A_2A::Ks34970A_2A(unsigned int portNo, int baudRate){
	mpCommunicate = static_cast<std::shared_ptr<CommunicateDrose::CommunicateInterface>>(std::make_shared<CommunicateDrose::ComPortOne>(portNo, baudRate, 10, 5000));
}

Ks34970A_2A::Ks34970A_2A(){

}

Ks34970A_2A::~Ks34970A_2A(){

}

bool Ks34970A_2A::cmd(const QString cmd){
	QString tRecv;
	mpCommunicate->communicate(cmd + "\r\n", tRecv);

	return true;
}

bool Ks34970A_2A::init(){
	if (mpCommunicate->init() == false)
	{
		return 0;
	}

	QString tRecv;

//	mpCommunicate->communicate("MEMory:STATe:RECall:AUTO OFF\r\n", tRecv, 0);
	mpCommunicate->communicate("*CLS\r\n", tRecv, 0);
//	mpCommunicate->communicate("*RST\r\n", tRecv, 0);

	//		mpCommunicate->communicate("CONF:VOLT:DC 10,0.001,(@101)\r\n", tRecv);
	//		mpCommunicate->communicate("READ?\r\n", tRecv);
	//		mpCommunicate->communicate("MEASure:VOLTage:DC? (@103)\r\n", tRecv);

	voltageAc = "VOLT:AC?";
	voltageDc = "VOLT:DC?";
	currentAc = "CURR:AC?";
	currentDc = "CURR:DC?";
	frequency = "FREQUENCY?";
	resistance = "RES?";

	return true;
}

void Ks34970A_2A::init(int address)
{
	mBgpib = true;

	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS)
	{
		printf("Could not open a session to the VISA Resource Manager!\n");
		return;
		exit(EXIT_FAILURE);
	}

	status = viOpen(defaultRM, QString("GPIB::%0::INSTR").arg(address).toStdString().c_str(), VI_NULL, VI_NULL, &mInst);

	status = viInstallHandler(mInst, VI_EVENT_IO_COMPLETION, AsyncHandler, uhandle);
	status = viEnableEvent(mInst, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);
}

float Ks34970A_2A::getMeasure(const QString type, QString channel){
	
	QString cmd = "MEAS:" + type + " (@" + channel + ")\r\n";

	if (mBgpib)
	{
		memset(data1, 0, READ_BUFFER_SIZE);
		strcpy(stringinput, cmd.toStdString().c_str());
		BytesToWrite = (ViUInt32)strlen(stringinput);
		status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);
		_sleep(1000);
		status = viReadAsync(mInst, data1, READ_BUFFER_SIZE - 1, &job);
		return QString((char*)data1).toFloat();
	}
	else
	{		
		QString tRecv;
		mpCommunicate->communicate(cmd, tRecv);

		return tRecv.toFloat();
	}
	
}

bool Ks34970A_2A::getMeasure(const QString type, QString channel, float& value){
	
	if (mBgpib)
	{

	}
	else
	{
		QString cmd = "MEAS:" + type + " (@" + channel + ")\r\n";
		QString tRecv;
		mpCommunicate->communicate(cmd, tRecv);

		value = tRecv.toFloat();

		return true;
	}
	
}
