/****************************************************************************
 * Copyright (c) 2014 Nitra Games Ltd., Istanbul, Turkey                    *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice should not be      *
 * deleted from the source form of the Software.                            *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/*
 * gipMySQL.h
 *
 *  Created on: Oct 11, 2025
 *      Author: Noyan Culum
*/

#ifndef SRC_GIPMYSQL_H_
#define SRC_GIPMYSQL_H_

#include "gBaseComponent.h"

#include "mysql/mysql.h"


/**
 * gipMySQL is a GlistEngine component that makes queries from a remote MySQL
 * database.
 *
 * The developer should first creates a connection to the remote MySQL server
 * using the connect() function.
 *
 * Once a connection is established, the developer can query SQL statements
 * using execute() function.
 *
 * If the query is a SELECT query, the result can be taken by one of the
 * getSelectData() or getSelectDataVec() functions.
 *
 * The connection should be closed in the end using disconnect() function.
 *
 * This component is tested with a MySQL Server 8.0.44. An example GlistApp
 * can be found in the examples/ folder.
 */
class gipMySQL : public gBaseComponent {
public:
	gipMySQL();
	virtual ~gipMySQL();

	/**
	 * Creates a connection to a MySQL server with the given parameters.
	 *
	 * @param hostname Name of the remote MySQL host. Can be an URL or an IP number.
	 * @param username The username in the remote server. This should ve connected to the database on the host
	 * @param password The password of the user
	 * @param database The database to connect
	 * @param port The port number to use during the connection. Generally 3306 is good
	 *
	 * @return The connection is established or not
	 */
	bool connect(const std::string hostname,
            const std::string username,
            const std::string password,
            const std::string database,
            const int port = 3306);

	/**
	 * Closes the connection from the MySQL server
	 */
	void disconnect();

	/**
	 * Queries a SQL statement. eg. SELECT or UPDATE, etc...
	 */
    void execute(std::string statement);

    /**
     * If the query statement is a SELECT, the result can be taken using getSelectData()
     * function. Each call to this function fetches the first 1 row of the result.
     *
     * @return First row of the SELECT result. Fields are seperated by the delimiter string.
     */
	std::string getSelectData();

    /**
     * If the query statement is a SELECT, the result can be taken using getSelectDataVec()
     * function. Each call to this function fetches the first 1 row of the result.
     *
     * @return First row of the SELECT result. Each field is an element of the returning vector.
     */
	std::vector<std::string> getSelectDataVec();

	/**
	 * Returns if there is still any result data of the SELECT query to fetch
	 *
	 * @return The availability of SELECT result
	 */
	bool hasSelectData();

	/**
	 * Returns the number of the result data of the SELECT query to fetch
	 *
	 * @return The row number of SELECT data to fetch
	 */
	int getSelectDataNum();

	/**
	 * Returns the field number of the result data of the SELECT query
	 *
	 * @return The field number of SELECT data
	 */
	int getSelectDataFieldNum();

	/**
	 * Returns the table names of the connected MySQL database
	 *
	 * @return Table names
	 */
	std::vector<std::string> getTableNames();

	/**
	 * Returns the column names of a table in the connected MySQL database
	 *
	 * @param tableName The name of the table to get its column names
	 *
	 * @return The column names as a string vector
	 */
	std::vector<std::string> getColumnNames(std::string tableName);

	/**
	 * Returns the type of a column of a table. eg. int, text, etc...
	 *
	 * @param tableName The name of the table of the column
	 * @param columnName The name of the column
	 *
	 * @return The type of the column. eg. int, text, etc...
	 */
	std::string getColumnType(std::string tableName, std::string columnName);

	/**
	 * Sets the delimiter for the getSelectData()
	 *
	 * @param delimiter The delimiter to use in the getSelectData() string
	 */
	void setDelimiter(std::string delimiter);

	/**
	 * Gives the delimiter of the getSelectData() string
	 *
	 * @return The delimiter used in the getSelectData() string
	 */
	std::string getDelimiter();

	/**
	 * Returns the server's MySQL version number, eg. 8.0.44
	 */
	std::string getServerInfo();

	/**
	 * Returns the error message if an error occured during the connection or query
	 *
	 * @return Error message generated by the MySQL server
	 */
	std::string getServerErrorMessage(const MYSQL* const conn);

private:
	MYSQL* connection_;
	std::string hostname, username, password, database;
    int port;
	MYSQL_RES *res;	// the results
	int resrownum, resfieldnum;
    my_bool mb;
    std::deque<std::deque<std::string>> response;
    std::string delimiter;
};

#endif /* SRC_GIPMYSQL_H_ */
