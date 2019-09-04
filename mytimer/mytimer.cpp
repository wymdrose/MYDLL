#include "../../lib/mytimer.h"
#include "countdown.h"

using namespace Drose;

Mytimer::Mytimer()
{
	connect(this, SIGNAL(countdownSignal(unsigned int)), this, SLOT(countdownSlot(unsigned int)), Qt::BlockingQueuedConnection);
}

Mytimer::~Mytimer()
{

}

void Mytimer::countdownSlot(unsigned int sec)
{
	Countdown tCountdown(sec);
	tCountdown.exec();
}