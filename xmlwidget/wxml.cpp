#include "wxml.h"
#include <xmltree.h>

WXml::WXml(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.treeWidget, &QTreeWidget::itemClicked, [this]() {

		QTreeWidgetItem* item = ui.treeWidget->currentItem();
		QTreeWidgetItem* parent = item->parent();
		if (item->text(1).isEmpty())
		{
			ui.groupBoxId->setTitle(item->text(0));
			ui.lineEditKey->setText("");
			ui.lineEditValue->setText("");
		}
		else
		{
			ui.groupBoxId->setTitle(parent->text(0));
			ui.lineEditKey->setText(item->text(0));
			ui.lineEditValue->setText(item->text(1));
		}

	});

	connect(ui.pushButtonUpdate, &QPushButton::clicked, [this]() {
		Treexml::XmlSet t;
		t.updateOnePara(ui.groupBoxId->title(), ui.lineEditKey->text(), ui.lineEditValue->text());
		t.loadXmlToTree(ui.treeWidget);
	});

	connect(ui.pushButtonDelete, &QPushButton::clicked, [this]() {
		Treexml::XmlSet t;
		t.deleteOnePara(ui.groupBoxId->title(), ui.lineEditKey->text());
		t.loadXmlToTree(ui.treeWidget);
	});

}

WXml::~WXml()
{

}
