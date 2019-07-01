#include "loginui.h"
#include <QPainter>
#include <QString>

LoginUi::LoginUi(const QString imgPath, QVector<QStringList> vector, int& mode, QDialog *parent)
	: QDialog(parent), mBackImgPath(imgPath), mMode(mode), mVector(vector)
{
	ui.setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowMinMaxButtonsHint;
	flags |= Qt::WindowCloseButtonHint;

	setWindowFlags(flags);

	for (size_t i = 0; i < mVector.size(); i++)
	{
		ui.comboBoxUser->addItem(mVector[i][0]);
	}
	
	connect(ui.buttonBoxOk, &QDialogButtonBox::accepted, [this](){
		
		for (size_t i = 0; i < mVector.size(); i++){
			if (mVector[i][0] == ui.comboBoxUser->currentText() && mVector[i][1] == ui.lineEditPassword->text())
			{
				mMode = i + 1;
				this->close();
			}
		}
	});
	
	connect(ui.buttonBoxOk, &QDialogButtonBox::rejected, [this]()
	{
		mMode = 0;
		this->close();
	});


}

LoginUi::LoginUi(const QString imgPath, int& mode, QDialog *parent)
	: QDialog(parent), mBackImgPath(imgPath), mMode(mode)
{
	ui.setupUi(this);

	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowMinMaxButtonsHint;
	flags |= Qt::WindowCloseButtonHint;

	setWindowFlags(flags);

	QStringList tUserPswList1 = { QStringLiteral("操作员"), "OBC123" };
	QStringList tUserPswList2 = { QStringLiteral("工程师"), "OBC456" };
	QStringList tUserPswList3 = { "ddd", "ddd" };
	mVector.append(tUserPswList1);
	mVector.append(tUserPswList2);
	mVector.append(tUserPswList3);

	for (size_t i = 0; i < mVector.size(); i++)
	{
		ui.comboBoxUser->addItem(mVector[i][0]);
	}

	connect(ui.buttonBoxOk, &QDialogButtonBox::accepted, [this](){

		for (size_t i = 0; i < mVector.size(); i++){
			if (mVector[i][0] == ui.comboBoxUser->currentText() && mVector[i][1] == ui.lineEditPassword->text())
			{
				mMode = i + 1;
				this->close();
			}
		}
	});

	connect(ui.buttonBoxOk, &QDialogButtonBox::rejected, [this]()
	{
		mMode = 0;
		this->close();
	});


}
LoginUi::~LoginUi()
{

}

void LoginUi::paintEvent(QPaintEvent *event){

	QPainter painter(this);
	painter.drawPixmap(rect(), QPixmap(mBackImgPath), QRect());
}
