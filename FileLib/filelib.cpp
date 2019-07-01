#include "../../lib/filelib.h"

using namespace FileIo;


CsvFile::CsvFile(QString filePath)
{
	mpCsvFile = std::shared_ptr<QFile>(new QFile(filePath));
	mpCsvFile->open(QIODevice::ReadWrite | QIODevice::Append);
}

CsvFile::~CsvFile()
{
	mpCsvFile->close();
}

void CsvFile::clear(){
	if (mpCsvFile->isOpen()){ mpCsvFile->close(); }

	mpCsvFile->open(QIODevice::ReadOnly);
	mpCsvFile->close();

	mpCsvFile->open(QIODevice::Truncate);
	mpCsvFile->close();

	mpCsvFile->open(QIODevice::WriteOnly);
	mpCsvFile->close();
}

bool CsvFile::append(const QStringList& tLine){
	if (!mpCsvFile->isOpen())
	{
		mpCsvFile->open(QIODevice::ReadWrite | QIODevice::Append);
	}

	if (!mpCsvFile->isOpen())	{ return false; }

	//if (!mpCsvFile->resize(0))

	/*
	Q_FOREACH(QString str, tLine)
	{
	//mpCsvFile->write(str.toAscii());
	//mpCsvFile->write(str.toLatin1());
	mpCsvFile->write(str.toLocal8Bit());
	mpCsvFile->write(QString(",").toLocal8Bit());
	}
	*/

	for (size_t i = 0; i < tLine.size(); i++)
	{
		mpCsvFile->write(tLine[i].toLocal8Bit());

		if (i == tLine.size() - 1)
		{

		}
		else
		{
			mpCsvFile->write(QString(",").toLocal8Bit());
		}

	}

	mpCsvFile->write(QString("\n").toLocal8Bit());

	mpCsvFile->flush();

	return true;
}

void CsvFile::get(std::vector<QStringList>& vLineAll){
	vLineAll.clear();

	mpCsvFile->seek(0);

	QTextStream tStream(mpCsvFile.get());

	QStringList CSVList;

	while (!tStream.atEnd())
	{
		QStringList oneLine = tStream.readLine().split(",");
		vLineAll.push_back(oneLine);
	}
}


QStringList CsvFile::get()
{
	QStringList tList;
	tList.clear();

	mpCsvFile->seek(0);

	QTextStream tStream(mpCsvFile.get());

	while (!tStream.atEnd())
	{
		tList.push_back(tStream.readLine());
	}

	return tList;
}

QString CsvFile::getLast()
{
	QStringList tList;
	tList.clear();

	mpCsvFile->seek(0);

	QTextStream tStream(mpCsvFile.get());

	while (!tStream.atEnd())
	{
		tList.push_back(tStream.readLine());
	}

	return tList[tList.size() - 1];
}
