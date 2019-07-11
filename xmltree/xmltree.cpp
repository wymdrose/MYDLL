#include "../../lib/xmltree.h"

using namespace Treexml;

xmltree::xmltree()
{

}

xmltree::~xmltree()
{

}


void XmlSet::updateOnePara(QString paraId, QString paraKey, QString paraValue)
{
	QFile file("sample.xml");
	if (!file.open(QFile::ReadOnly))
		return;

	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();

	bool needNewId = true;
	bool needNewKey = true;

	while (!n.isNull())
	{
		QDomElement e = n.toElement();

		if (e.attributes().namedItem("id").nodeValue() == paraId)
		{
			needNewId = false;

			QDomNodeList list = e.childNodes();

			for (int i = 0; i< list.count(); i++)
			{
				QDomNode node = list.at(i);
				if (node.isElement() && node.toElement().tagName() == paraKey)
				{
					needNewKey = false;

					QDomNode oldnode = node.toElement().firstChild();
					node.firstChild().setNodeValue(paraValue);
					QDomNode newnode = node.firstChild();
					node.replaceChild(newnode, oldnode);

					break;
				}
			}

			if (needNewKey)
			{
				QDomElement key = doc.createElement(paraKey);
				QDomText value = doc.createTextNode(paraValue);
				n.appendChild(key);
				key.appendChild(value);
			}

		}

		n = n.nextSibling();
	}

	if (needNewId && needNewKey)
	{
		QDomElement id = doc.createElement("item");
		id.setAttribute("id", paraId);

		QDomElement key = doc.createElement(paraKey);
		QDomText value = doc.createTextNode(paraValue);
		key.appendChild(value);
		id.appendChild(key);
		docElem.appendChild(id);
	}


	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return;

	QTextStream out_stream(&file);
	doc.save(out_stream, 4);
	file.close();

}

void XmlSet::deleteOnePara(QString paraId, QString paraKey)
{
	QFile file("sample.xml");
	if (!file.open(QFile::ReadOnly))
		return;

	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement root = doc.documentElement();
	QDomNodeList list = doc.elementsByTagName("item");
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();

	while (!n.isNull()) {
		QDomElement e = n.toElement();

		if (e.attributes().namedItem("id").nodeValue() == paraId)
		{
			QDomNodeList list = e.childNodes();

			for (int i = 0; i< list.count(); i++) {
				QDomNode node = list.at(i);
				if (node.isElement() && node.toElement().tagName() == paraKey)
				{
					e.removeChild(list.at(i));
					break;
				}
			}
		}

		n = n.nextSibling();
	}

	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return;

	QTextStream out_stream(&file);
	doc.save(out_stream, 4);
	file.close();
}

bool XmlSet::getOneParaValue(QString paraId, QString paraKey, QString& paraValue)
{
	QDomDocument doc("message");
	QFile file("sample.xml");
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	QDomElement root = docElem.firstChildElement();

	QDomNodeList item1 = root.elementsByTagName(paraId);

	while (!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		qDebug() << qPrintable(e.tagName()) << qPrintable(e.attribute("id"));

		if (e.attributes().namedItem("id").nodeValue() == paraId)
		{
			QDomNodeList list = e.childNodes();

			for (int i = 0; i< list.count(); i++) {
				QDomNode node = list.at(i);
				if (node.isElement() && node.toElement().tagName() == paraKey)
				{
					paraValue = node.toElement().text();
					return true;
				}
			}
		}

		n = n.nextSibling();
	}

	return false;
}

void XmlSet::loadXmlToTree(QTreeWidget* tw)
{
	tw->clear();
	tw->setWindowTitle("QTreeWidget");
	tw->setHeaderLabels(QStringList() << "Key" << "Value");
	tw->setSortingEnabled(true);
	//	tw->header()->setSortIndicatorShown(true);
	//	tw->header()->setSortIndicator(0, Qt::AscendingOrder);

	QDomDocument doc("message");
	QFile file("sample.xml");
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();

	QDomElement root = doc.documentElement();
	QDomNodeList list = doc.elementsByTagName("item");
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();

	while (!n.isNull()) {
		QDomElement e = n.toElement();

		QTreeWidgetItem* idItem = new QTreeWidgetItem(QStringList() << e.attribute("id"));
		idItem->setIcon(0, QIcon(":/images/a.png"));
		idItem->setCheckState(0, Qt::Checked);
		tw->addTopLevelItem(idItem);

		QDomNodeList list = e.childNodes();
		for (int i = 0; i< list.count(); i++) {
			QDomNode node = list.at(i);


			QStringList columItemList;
			QTreeWidgetItem *child;
			QString key, value;
			key += node.toElement().tagName();
			value += node.toElement().text();
			columItemList << key << value;
			child = new QTreeWidgetItem(columItemList);
			idItem->addChild(child);
			//	tw->setItemWidget(child, 1, new QLineEdit);
		}

		n = n.nextSibling();
	}

	tw->show();
	tw->expandAll();
}
