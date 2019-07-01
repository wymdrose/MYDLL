#include "../../lib/login.h"
#include "loginui.h"

Login::Login(QString path, QVector<QStringList> vector, int& mode)
{
	LoginUi* ptUi = new LoginUi(path, vector, mode);
	ptUi->exec();
}

Login::Login(QString path, int& mode)
{
	LoginUi* ptUi = new LoginUi(path, mode);
	ptUi->exec();
}

Login::~Login()
{

}