#include "../../lib/mysignal.h"
#include <QDateTime>

using namespace Drose;

MySignalUi::MySignalUi()
{
	connect(this, SIGNAL(textSignal(QPushButton*, QString)), this, SLOT(textSlot(QPushButton*, QString)));
	connect(this, SIGNAL(textSignal(QLabel*, QString)), this, SLOT(textSlot(QLabel*, QString)));
	connect(this, SIGNAL(textSignal(QTextBrowser*, QString)), this, SLOT(textSlot(QTextBrowser*, QString)));
	connect(this, SIGNAL(textSignal(QTableWidgetItem*, QString)), this, SLOT(textSlot(QTableWidgetItem*, QString)));
	connect(this, SIGNAL(proBarSignal(QProgressBar*, int)), this, SLOT(proBarSlot(QProgressBar*, int)));
	connect(this, SIGNAL(colorSignal(QPushButton*, const QString)), this, SLOT(colorSlot(QPushButton*, const QString)));
	connect(this, SIGNAL(colorSignal(QCheckBox*, const QString)), this, SLOT(colorSlot(QCheckBox*, const QString)));
	connect(this, SIGNAL(colorSignal(QLabel*, const QString)), this, SLOT(colorSlot(QLabel*, const QString)));
	connect(this, SIGNAL(colorSignal(QTableWidgetItem*, const QColor)), this, SLOT(colorSlot(QTableWidgetItem*, const QColor)));
	connect(this, SIGNAL(ableSignal(QPushButton*, bool)), this, SLOT(ableSlot(QPushButton*, bool)));
	connect(this, SIGNAL(showMsgSignal(QTextBrowser*, const QString&)), this, SLOT(showMsg(QTextBrowser*, const QString &)));
	connect(this, SIGNAL(showDialogSignal(const QString&)), this, SLOT(showDialog(const QString &))/*, Qt::BlockingQueuedConnection*/);
	
}

MySignalUi::~MySignalUi()
{

}

void MySignalUi::textSlot(QPushButton* p, QString t){

	p->setText(t);
}


void MySignalUi::textSlot(QLabel* p, QString t){

	p->setText(t);
}

void MySignalUi::textSlot(QTextBrowser* p, QString t){
	p->append(t);
}

void MySignalUi::textSlot(QTableWidgetItem* p, QString t){
	p->setText(t);
}

void MySignalUi::proBarSlot(QProgressBar* p, int t)
{
	p->setValue(t);
}
void MySignalUi::colorSlot(QPushButton* p, const QString t){ 
	p->setStyleSheet(t); 
}

void MySignalUi::ableSlot(QPushButton* p, bool b){ p->setEnabled(b); }
void MySignalUi::colorSlot(QCheckBox* p, const QString t){ p->setStyleSheet(t); }
void MySignalUi::colorSlot(QLabel* p, const QString t){ p->setStyleSheet(t); }
void MySignalUi::colorSlot(QTableWidgetItem* p, const QColor t){ p->setBackgroundColor(t); }
void MySignalUi::showMsg(QTextBrowser* p, const QString t){
	p->append("[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "] " + t);
}
void MySignalUi::showDialog(const QString &msg){
	QMessageBox::information(&mDialog, "", msg);
}