#include "database.h"

database::database()
{
	setupdb();
} 

void database::setupdb()
{
	rc = sqlite3_open("ircontrol.db", &db);
}

int database::addbind(char *b, char *c, char *a)
{
	int ret = 0;
	query = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS binds(id INTEGER PRIMARY KEY, bindkey TEXT, command TEXT, args TEXT);"
	"INSERT INTO binds(bindkey, command, args) VALUES('%q', '%q', '%q');", b, c, a);
	/* execute query */
	rc = sqlite3_exec(db, query, 0, 0, &err);
	/* check if query was a success */
	if (rc != SQLITE_OK)
	{
		printf("Query error: %s\n", err);
		sqlite3_free(err);
		sqlite3_close(db);
		ret = 1;
	}	
	sqlite3_free(query);
	return ret;
}

int database::getbinds()
{
	query = "SELECT * FROM binds;";
	rc = sqlite3_exec(db, query, callback, 0, &err);
	if (rc != SQLITE_OK)
	{
		printf("failed to select data\n");
		printf("Query error: %s\n", err);
		sqlite3_free(err);
		sqlite3_close(db);
		//return 1;
	}
	return 0;
}

int callback(void *not_used, int argc, char **argv,
		char **azColName)
{
	not_used = 0;
	printf("%d\n", argc);
	for (int i = 0; i < argc; i++)
	{
		printf("%s=%s\n", azColName[i],
				argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
