#include "../../lib/xlsxfile.h"

using namespace FileIo;

void readExcel(QString path, QTableWidget* pTableWidget)
{
	QXlsx::Document xlsx(path);
	QXlsx::Workbook *workBook = xlsx.workbook();
	workBook->sheetCount();
	QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(0));

	auto a = workSheet->dimension();
	auto b = workSheet->dimension().rowCount();
	auto c = workSheet->dimension().columnCount();

	pTableWidget->setRowCount(workSheet->dimension().rowCount());
	pTableWidget->setColumnCount(workSheet->dimension().columnCount());
	QString value;
	for (int i = 1; i <= workSheet->dimension().rowCount(); i++)
	{
		for (int j = 1; j <= workSheet->dimension().columnCount(); j++)
		{
			QXlsx::Cell *cell = workSheet->cellAt(i, j);
			if (cell == NULL) continue;
			if (cell->isDateTime())
			{
				if (cell->dateTime().date().year() == 1899) continue;
				value = cell->dateTime().toString("yyyy/MM/dd hh:mm");
			}
			else
			{
				value = cell->value().toString();
			}

			QTableWidgetItem *item = new QTableWidgetItem(value);
			//	item->setBackground(QBrush(QColor(Qt::lightGray)));
			//	item->setFlags(item->flags() & (~Qt::ItemIsEditable));

			pTableWidget->setItem(i - 1, j - 1, item);
		}
	}

	for (int i = pTableWidget->rowCount() - 1; i >= 0; i--)
	{
		QTableWidgetItem *item = pTableWidget->item(i, 0);
		if (item == NULL)
			pTableWidget->removeRow(i);
		else break;
	}
}
