#include "../../lib/communicatelib.h"

using namespace CommunicateDrose;

/*
CommunicateInterface::CommunicateInterface()
{

}

CommunicateInterface::~CommunicateInterface()
{

}
*/

ComPortOne::ComPortOne(unsigned int portNo, int baudRate, int breakLength, int breakDelay)
	:mPortNo(portNo), mBaudRate(baudRate), mBreakDelay(breakDelay)
{
	//	connect(&*mpSerial, SIGNAL(readyRead()), this, SLOT(recvMsg()), Qt::UniqueConnection);

	mpSerial->setPortName(QString("COM%1").arg(portNo));
	mpSerial->setBaudRate(baudRate);
	mpSerial->setParity(QSerialPort::NoParity);
	mpSerial->setDataBits(QSerialPort::Data8);
	mpSerial->setStopBits(QSerialPort::OneStop);
	mpSerial->setFlowControl(QSerialPort::NoFlowControl);

}
ComPortOne::~ComPortOne()
{
	mpSerial->close();
}

void ComPortOne::close()
{
	qDebug() << "close com" << mPortNo << "\r";
	mpSerial->close();
}

bool ComPortOne::init()
{
	mpSerial->close();

	if (mpSerial->open(QIODevice::ReadWrite))
	{
		qDebug() << "open com" << mPortNo << " success" << "\r";
		mbInit = true;
		return true;
	}
	else
	{
		qDebug() << "open com" << mPortNo << " failed" << "\r";
		return false;
	}
}

qint64 ComPortOne::send(const QString sendData)
{
	mRecData.clear();
	mpSerial->clear();
	QByteArray tSend = sendData.toLocal8Bit();

	qDebug() << "";
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mPortNo << "send:" << tSend;

	return mpSerial->write(tSend);
}

inline void ComPortOne::recData(int tBreakLength)
{
	mRecData.clear();
	QTime time;
	time.start();

	forever
	{	
		while (mpSerial->waitForReadyRead(50))
		{
			QByteArray tRec = mpSerial->readAll();
			mRecData += tRec.data();
		}

		if (mRecData.length() >= tBreakLength || time.elapsed() >= mBreakDelay)
		{
			break;
		}

		
	}

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mPortNo << "rec:" << mRecData;
}

/*
QString ComPortOne::getRec()
{
	return mRecData;
}
*/
bool ComPortOne::getRec(QString& msg)
{
	if (false == this->mpSerial->waitForReadyRead(2000))
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
		qDebug() << "Server does not response!";
		return false;
	}

	QByteArray datagram = mpSerial->readAll();;

	msg = datagram.data();

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << msg;

	return true;
}

void ComPortOne::communicate(const QString tSend){
	if (!mbInit){
		qDebug() << "mbInit:false";
		return;
	}
	send(tSend);
}

bool ComPortOne::communicate(const QString tSend, QString& mRecv, int tBreakLength)
{
	if (!mbInit){
		qDebug() << "mbInit:false";
		return false;
	}

	auto a = send(tSend);

	qDebug() << "send success?" << a << "\r";

	recData(tBreakLength);
	mRecv = mRecData;

//	getRec(mRecv);

	return true;
}

bool ComPortOne::communicate(const QByteArray tSend, QByteArray& mRecv, int tBreakLength)
{
	if (!mbInit){
		qDebug() << "mbInit:false";
		return false;
	}

	auto a = mpSerial->write(tSend);

	qDebug() << "send success?" << a << "\r";

	QTime time;
	time.start();

	forever
	{
		if (mRecv.size() > tBreakLength || time.elapsed() > mBreakDelay)
		{
			break;
		}

		mpSerial->waitForReadyRead(50);
		mRecv = mpSerial->readAll();
	}

	return true;
}

void ComPortOne::recvMsg()
{
	mRecData.clear();

	QByteArray tRec = mpSerial->readAll();
	mRecData += tRec.data();

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mPortNo << "rec:" << mRecData;
}

std::string _gethostMac()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning)
			&& !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			qDebug() << strMacAddr;
			break;
		}
	}
	return strMacAddr.toStdString();
}


void _dongle(){
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
	QStringList macList;
	for (size_t i = 0; i < nets.count(); i++)
	{
		macList.append(nets[i].hardwareAddress());
	}

	if (!macList.contains("E4:42:A6:F7:2E:48"))
	{
		qDebug() << "_dongle";
		_ASSERT(false);
	}

}
//
TcpClient::TcpClient(QString ip, int port) : mIp(ip), mPort(port)
{
	_dongle();

	mpQHostAddress = new QHostAddress();
	mpTcpSocket = new QTcpSocket();
}
TcpClient::~TcpClient()
{
	delete mpTcpSocket;
	delete mpQHostAddress;
}

bool TcpClient::init()
{
	if (!mpQHostAddress->setAddress(mIp))
	{
		qDebug() << mIp << "server ip address error!";
		return false;
	}

	mpTcpSocket->connectToHost(*mpQHostAddress, mPort);

	if (false == mpTcpSocket->waitForConnected(1000))
	{
		qDebug() << mIp << "connect failed.";
		return false;
	}

	qDebug() << mIp << "connect ok.";
	mbInit = true;
	return true;
}

void TcpClient::disConnect()
{
	mpTcpSocket->disconnectFromHost();

}

void TcpClient::send(const QString msg)
{
	mRecData.clear();

	qDebug() << "";
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << mIp << " send:" << msg << "\r";

	auto re = mpTcpSocket->write(msg.toLatin1(), msg.length());
}

bool TcpClient::getRec()
{
	if (false == this->mpTcpSocket->waitForReadyRead())
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
		qDebug() << "Server does not response!";
		return false;
	}


	QByteArray datagram = mpTcpSocket->readAll();;

	//	mpTcpSocket->read(datagram.data(), 4);

	QString msg = datagram.data();

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << msg;

	return true;
}

bool TcpClient::getRec(QString& msg)
{
	if (false == this->mpTcpSocket->waitForReadyRead(300))
	{
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
		qDebug() << "Server does not response!";
		return false;
	}

	QByteArray datagram = mpTcpSocket->readAll();;

	msg = datagram.data();

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") << msg;

	return true;
}

void TcpClient::communicate(QString tSend){
	init();
	send(tSend);
	disConnect();
}

bool TcpClient::communicate(const QString tSend, QString& mRecv, int tBreakLength)
{
	init();
		
	send(tSend);

	if (false == getRec(mRecv)){
		return false;
	}

	disConnect();
	return true;
}

bool TcpClient::communicate(QByteArray, QByteArray&, int tBreakLength){
	if (!mbInit){
		qDebug() << "mbInit:false";
		return false;
	}

	return true;
}

/*
void slotConnected()
{
	qDebug() << mIp << "slotConnected success.";
	Sleep(0);
}

void slotDisconnected()
{
	qDebug() << mIp << "slotDisconnected.";
}
*/

