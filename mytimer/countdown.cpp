#include "countdown.h"

Countdown::Countdown(unsigned int sec, QWidget *parent)
	: QDialog(parent), mSec(sec)
{
	ui.setupUi(this);

	ui.labelCountdown->setText(QString::number(mSec));

	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(showTimelimit()));
	mTimer->start(1000);	
}

Countdown::~Countdown()
{

}

void Countdown::showTimelimit()
{
	mSec -= 1;
	ui.labelCountdown->setText(QString::number(mSec));

	if (mSec <= 0)
	{
		this->close();

	}
}