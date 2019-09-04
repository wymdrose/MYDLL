#include "../../lib/yokogawa.h"

using namespace InstrumentApi;

yokogawa::yokogawa()
{

}

yokogawa::~yokogawa()
{

}

bool Wt230::init(int address)
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

	//
	strcpy(stringinput, "RC\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "HD0;FL0;LF0;SC0;AG0\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "DA1;EA1;DB2;EB1;DC3;EC1\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "AA1\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "AV1\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	/*
	strcpy(stringinput, "RV7\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(inst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "RA7\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(inst, (ViBuf)stringinput, BytesToWrite, &rcount);
	*/
	strcpy(stringinput, "MN0\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "OFD0;DL0\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	//

	strcpy(stringinput, "H0\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	strcpy(stringinput, "OF1,3,1\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);

	return true;
}

bool Wt230::cmd(QString send, QString& recv){

	memset(data1, 0, READ_BUFFER_SIZE);

	strcpy(stringinput, QString(send + "\n").toStdString().c_str());
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);
	status = viReadAsync(mInst, data1, READ_BUFFER_SIZE - 1, &job);
	recv = (char*)data1;

	return true;
}

void Wt230::close(){
	viClose(mInst);
	viClose(defaultRM);
}

bool Wt230::get(float& recv){

	memset(data1, 0, READ_BUFFER_SIZE);
	strcpy(stringinput, "OD\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);
	status = viReadAsync(mInst, data1, READ_BUFFER_SIZE - 1, &job);

	QStringList tValue = QString((char*)data1).split(",");
	recv = tValue[0].right(tValue[0].length() - 1).toFloat();

	return true;
}

void Wt230::tabChannel(int m)
{
	viPrintf(mInst, QString("DA1;EA%0;DB2;EB%0;DC3;EC%0\n").arg(m).toStdString().c_str());
}

bool Wt230::getPower(int m, QString& value){

	viPrintf(mInst, QString("DA1;EA%0;DB2;EB%0;DC3;EC%0\n").arg(m).toStdString().c_str());
	viPrintf(mInst, QString("OF1,3,%0\n").arg(m).toStdString().c_str());

	/*
	char tData[50];
	viScanf(inst, tData);
	*/

	memset(data1, 0, READ_BUFFER_SIZE);
	strcpy(stringinput, "OD\n");
	BytesToWrite = (ViUInt32)strlen(stringinput);
	status = viWrite(mInst, (ViBuf)stringinput, BytesToWrite, &rcount);
	_sleep(1000);
	status = viReadAsync(mInst, data1, READ_BUFFER_SIZE - 1, &job);
	QStringList tValue = QString((char*)&data1[1]).split(",");


	//	value = tValue[m - 1].right(tValue[m - 1].length() - 1).toFloat();
	value = tValue[0].right(tValue[0].length() - 1);
	return true;
}