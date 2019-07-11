#include "../../lib/xmlwidget.h"
#include "wxml.h"

xmlwidget::xmlwidget()
{
	WXml* ptUi = new WXml();
	ptUi->show();
}

xmlwidget::~xmlwidget()
{

}
