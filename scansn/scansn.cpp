#include "../../lib/scansn.h"

scansn::scansn()
{

}

scansn::~scansn()
{

}

ScanDlg::ScanDlg(QString& scanString, const int passLength, QWidget* parent) : mScanString(scanString), mPassLength(passLength)
{
	setWindowTitle(QStringLiteral("ÇëÉ¨Âë:"));
	tEdit = new QLineEdit;
	
	//tEdit->setFixedWidth(50);
	//	tEdit->setEchoMode(QLineEdit::EchoMode::Password);	

	tLayout = new QGridLayout(this);
	tLayout->addWidget(tEdit, 0, 0);

	this->resize(300, 100);


	connect(tEdit, &QLineEdit::editingFinished, [this]()
	{
		if (tEdit->text().length() >= mPassLength)
		{
			mScanString = tEdit->text();
			this->close();
		}
	});

	mTimer = new QTimer(this);
	connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
	mTimer->start(3000);
}

void ScanDlg::update()
{
	if (tEdit->text().length() >= mPassLength)
	{
		mScanString = tEdit->text();
		mTimer->stop();
		this->close();
	}
}