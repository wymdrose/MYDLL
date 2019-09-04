#include "../../lib/mythread.h"



using namespace Drose;

mythread::mythread()
{

}

mythread::~mythread()
{

}



MyThread2::MyThread2()
{
	this->moveToThread(&mThread);
	mThread.start();

	connect(this, &MyThread2::finished, &mThread, &QThread::quit);
	connect(this, &MyThread2::finished, this, &MyThread2::deleteLater);
	connect(&mThread, &QThread::finished, &mThread, &QThread::deleteLater);

}

MyThread2::MyThread2(int index) :mIndex(index)
{
	this->moveToThread(&mThread);

	connect(this, &MyThread2::finished, &mThread, &QThread::quit);
	connect(this, &MyThread2::finished, this, &MyThread2::deleteLater);
	connect(&mThread, &QThread::finished, &mThread, &QThread::deleteLater);
}

MyThread2::~MyThread2()
{

}

void MyThread2::start()
{
	mThread.start();
}


void MyThread2::startOnce()
{
	connect(&mThread, &QThread::started, this, &Drose::MyThread2::once);
	mThread.start();
}

void MyThread2::once()
{

}