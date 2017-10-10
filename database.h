#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <sqlite3.h>
#include <QStringList>

int callback(void *not_used, int argc, char **argv, char **azColName);

class database
{
	public:
		database();
		int getbinds();
		int addbind(char *b, char *c, char *a);

	private:
		char *query;
		sqlite3 *db;
		char *err = 0;
		int rc;
		void setupdb();

};

#endif // DATABASE_H
