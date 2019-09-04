#include "../../lib/mysignal.h"
#include <QDateTime>

using namespace Drose;

MySignalUi::MySignalUi()
{
	connect(this, SIGNAL(textSignal(QLineEdit*, QString)), this, SLOT(textSlot(QLineEdit*, QString)));
	connect(this, SIGNAL(textSignal(QTextEdit*, QString)), this, SLOT(textSlot(QTextEdit*, QString)));
	connect(this, SIGNAL(textSignal(QPushButton*, QString)), this, SLOT(textSlot(QPushButton*, QString)));
	connect(this, SIGNAL(textSignal(QLabel*, QString)), this, SLOT(textSlot(QLabel*, QString)));	
	connect(this, SIGNAL(textSignal(QTextBrowser*, QString)), this, SLOT(textSlot(QTextBrowser*, QString)));
	connect(this, SIGNAL(textSignal(QTableWidgetItem*, QString)), this, SLOT(textSlot(QTableWidgetItem*, QString)));
	connect(this, SIGNAL(proBarSignal(QProgressBar*, int)), this, SLOT(proBarSlot(QProgressBar*, int)));
	connect(this, SIGNAL(colorSignal(QPushButton*, const QString)), this, SLOT(colorSlot(QPushButton*, const QString)));
	connect(this, SIGNAL(colorSignal(QCheckBox*, const QString)), this, SLOT(colorSlot(QCheckBox*, const QString)));
	connect(this, SIGNAL(colorSignal(QLabel*, const QString)), this, SLOT(colorSlot(QLabel*, const QString)));
	connect(this, SIGNAL(colorSignal(QTableWidgetItem*, const QColor, int)), this, SLOT(colorSlot(QTableWidgetItem*, const QColor, int)));
	connect(this, SIGNAL(ableSignal(QPushButton*, bool)), this, SLOT(ableSlot(QPushButton*, bool)));
	connect(this, SIGNAL(showMsgSignal(QTextBrowser*, const QString&)), this, SLOT(showMsg(QTextBrowser*, const QString &)));
	connect(this, SIGNAL(showDialogSignal(const QString, const QString)), this, SLOT(showDialog(const QString, const QString)));
	connect(this, SIGNAL(showBlockSignal(const QString, const QString)), this, SLOT(showBlock(const QString, const QString)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(showBlockSignal(const QString, const QString, bool&)), this, SLOT(showBlock(const QString, const QString, bool&)), Qt::BlockingQueuedConnection);
}

MySignalUi::~MySignalUi()
{

}

void MySignalUi::textSlot(QLineEdit* p, QString t)
{
	p->setText(t);
}

void MySignalUi::textSlot(QTextEdit* p, QString t)
{
	p->setText(t);
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

void MySignalUi::colorSlot(QTableWidgetItem* p, const QColor t, int backOrFore)
{ 
	if (!backOrFore)
	{
		p->setBackgroundColor(t);
	}
	else
	{
		p->setForeground(t);
	}
	

}

void MySignalUi::showMsg(QTextBrowser* p, const QString t){
	p->append("[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "] " + t);
}

void MySignalUi::showDialog(const QString title, const QString msg){
	QMessageBox::information(&mDialog, title, msg);
}

void MySignalUi::showBlock(const QString title, const QString msg)
{
	QMessageBox::information(&mDialog, title, msg);
}

void MySignalUi::showBlock(const QString title, const QString msg, bool& question)
{
	if (QMessageBox::Yes == QMessageBox::question(&mDialog, title, msg))
	{
		question = true;
	}
	else
	{
		question = false;
	}
	
}