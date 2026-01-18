/*
 * gipMySQL.cpp
 *
 *  Created on: Oct 11, 2025
 *      Author: Noyan Culum
 */

#include "gipMySQL.h"


gipMySQL::gipMySQL() {
	connection_ = nullptr;
	hostname = "";
	username = "";
	password = "";
	database = "";
	port = 3306;
	res = nullptr;
	resrownum = 0;
	resfieldnum = 0;
	mb = false;
	delimiter = ";";
}

gipMySQL::~gipMySQL() {
	mysql_close(connection_);
	gLogi("Connection to MySQL database is closed!");
}

bool gipMySQL::connect(const std::string hostname, const std::string username,
		const std::string password, const std::string database, const int port) {

	connection_ = mysql_init(nullptr);
    if (connection_ == nullptr) {
    	gLoge("Unable to initialize MySQL connection");
    	return false;
    }

    mb = 0;
    mysql_options(connection_, MYSQL_OPT_SSL_ENFORCE, &mb);
    mysql_options(connection_, MYSQL_OPT_SSL_VERIFY_SERVER_CERT, &mb);

    const MYSQL* const success = mysql_real_connect(
        connection_,
        hostname.c_str(),
        username.c_str(),
        password.c_str(),
        database.c_str(),
        port,
        nullptr,
		0);
    if (success == nullptr) {
    	gLoge("Unable to connect to MySQL database");
    	gLoge("ERROR:" + getServerErrorMessage(connection_));
    	mysql_close(connection_);
    	return false;
    }

    this->hostname = hostname;
    this->username = username;
    this->password = password;
    this->database = database;
    this->port = port;

    gLogi("Connected to MySQL database!");
    return true;
}

void gipMySQL::disconnect() {
	mysql_close(connection_);
    gLogi("Disconnected from MySQL database!");
}

void gipMySQL::execute(std::string statement) {
	if (mysql_query(connection_, statement.c_str())) {
		gLoge("QUERY ERROR:" + getServerErrorMessage(connection_));
	  	return;
	}
	res = mysql_store_result(connection_);
	resrownum = mysql_num_rows(res);
	resfieldnum = mysql_num_fields(res);
}

std::string gipMySQL::getSelectData() {
	if(resrownum == 0) return nullptr;
	MYSQL_ROW row = mysql_fetch_row(res);
	std::string resrow = std::string(row[0]);
	for(int i = 1; i < resfieldnum; i++) resrow += delimiter + std::string(row[i]);
	resrownum--;
	if(resrownum == 0) mysql_free_result(res);
	return resrow;
}

std::vector<std::string> gipMySQL::getSelectDataVec() {
	std::vector<std::string> resrow;
	if(resrownum == 0) return resrow;
	MYSQL_ROW row = mysql_fetch_row(res);
	for(int i = 0; i < resfieldnum; i++) resrow.push_back(std::string(row[i]));
	resrownum--;
	if(resrownum == 0) mysql_free_result(res);
	return resrow;
}

bool gipMySQL::hasSelectData() {
	return resrownum != 0;
}

int gipMySQL::getSelectDataNum() {
	return resrownum;
}

int gipMySQL::getSelectDataFieldNum() {
	return resfieldnum;
}

std::vector<std::string> gipMySQL::getTableNames() {
	std::vector<std::string> queryres;
	std::string query = "SHOW TABLES";
	if (mysql_query(connection_, query.c_str())) {
		gLoge("QUERY ERROR:" + getServerErrorMessage(connection_));
	  	return queryres;
	}
	MYSQL_RES *resq = mysql_store_result(connection_);
	int rownum = mysql_num_rows(resq);
	int fieldnum = mysql_num_fields(resq);

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(resq)) != nullptr) {
		queryres.push_back(std::string(row[0]));
		for(int i = 1; i < fieldnum; i++) {
			queryres.push_back(std::string(row[i]));
		}
	}
	mysql_free_result(resq);
	return queryres;
}

std::vector<std::string> gipMySQL::getColumnNames(std::string tableName) {
	std::vector<std::string> queryres;
	std::string query = "SELECT `COLUMN_NAME` "
			"FROM `INFORMATION_SCHEMA`.`COLUMNS` "
			"WHERE `TABLE_SCHEMA`='" + database + "' "
			"AND `TABLE_NAME`='" + tableName + "'";

	if (mysql_query(connection_, query.c_str())) {
		gLoge("QUERY ERROR:" + getServerErrorMessage(connection_));
	  	return queryres;
	}

	MYSQL_RES *resq = mysql_store_result(connection_);
	int rownum = mysql_num_rows(resq);
	int fieldnum = mysql_num_fields(resq);

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(resq)) != nullptr) {
		queryres.push_back(std::string(row[0]));
		for(int i = 1; i < fieldnum; i++) {
			queryres.push_back(std::string(row[i]));
		}
	}
	mysql_free_result(resq);
	return queryres;
}

std::string gipMySQL::getColumnType(std::string tableName, std::string columnName) {
	std::string queryres = "";
	std::string query = "SELECT DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS "
			"WHERE table_name = '" + tableName + "' "
			"AND COLUMN_NAME = '" + columnName + "'";

	if (mysql_query(connection_, query.c_str())) {
		gLoge("QUERY ERROR:" + getServerErrorMessage(connection_));
	  	return "";
	}

	MYSQL_RES *resq = mysql_store_result(connection_);
	MYSQL_ROW row;
	if ((row = mysql_fetch_row(resq)) != nullptr) {
		mysql_free_result(resq);
		return std::string(row[0]);
	}

	mysql_free_result(resq);
	return "";
}

std::string gipMySQL::getServerErrorMessage(const MYSQL* const conn) {
	return mysql_error(const_cast<MYSQL*>(conn));
}

void gipMySQL::setDelimiter(std::string delimiter) {
	this->delimiter = delimiter;
}

std::string gipMySQL::getDelimiter() {
	return delimiter;
}

std::string gipMySQL::getServerInfo() {
	return std::string(mysql_get_server_info(connection_));
}

