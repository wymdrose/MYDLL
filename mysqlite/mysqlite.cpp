#include "../../lib/mysqlite.h"

using namespace DatabaseCover;

mysqlite::mysqlite()
{

}

mysqlite::~mysqlite()
{

}

//
QSqlDatabase mDatabase;

MySqLite::MySqLite(QString db)
{
	if (QSqlDatabase::contains("qt_sql_default_connection"))
	{
		mDatabase = QSqlDatabase::database("qt_sql_default_connection");
	}
	else
	{
		mDatabase = QSqlDatabase::addDatabase("QSQLITE");
		mDatabase.setDatabaseName(db);
		mDatabase.setUserName("drose");
		mDatabase.setPassword("drose");
		open();
	}
}

bool MySqLite::open()
{
	if (!mDatabase.open())
	{
		qDebug() << "Error: Failed to connect database." << mDatabase.lastError();
		return false;
	}
	else
	{
		qDebug() << "connect database success.";
	}

	return true;
}

void MySqLite::close()
{
	mDatabase.close();
}

bool MySqLite::insert(QString table, QVector<QStringList> tQvector)
{
	QSqlQuery sql_query;
	QString insert_sql = "INSERT INTO " + table;
	insert_sql += " (";
	for (size_t i = 0; i < tQvector.size(); i++)
	{
		insert_sql += tQvector[i][0];
		if (i != tQvector.size() - 1)
		{
			insert_sql += ", ";
		}
	}
	insert_sql += ") VALUES (";

	for (size_t i = 0; i < tQvector.size(); i++)
	{
		insert_sql += tQvector[i][1];
		if (i != tQvector.size() - 1)
		{
			insert_sql += ", ";
		}
	}
	insert_sql += ")";
	//	sql_query.prepare(insert_sql);
	if (!sql_query.exec(insert_sql))
	{
		qDebug() << sql_query.lastError();
		return false;
	}
	else
	{
		qDebug() << "inserted ok!";
		return true;
	}
}

void MySqLite::select()
{
	QSqlQuery sql_query;
	QString tSql = "select * from drose;";

	if (!sql_query.exec(tSql))
	{
		qDebug() << sql_query.lastError();

	}
	else
	{
		qDebug() << "inserted ok!";

	}
}