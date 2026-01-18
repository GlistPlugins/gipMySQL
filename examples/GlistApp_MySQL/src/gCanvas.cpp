/*
* gCanvas.cpp
*
*  Created on: May 6, 2020
*      Author: noyan
*/


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {
	std::string hostname = "IP number or URL of the MySQL server";
	std::string username = "Your username in the remote MySQL server";
	std::string password = "Your password";
	std::string database = "The name of the remote MySQL database to connect";
	mysql.connect(hostname.c_str(), username.c_str(), password.c_str(), database.c_str());

	gLogi("gCanvas") << "Server Info:" << mysql.getServerInfo();

	std::vector<std::string> tablenames = mysql.getTableNames();
	for(int i = 0; i < tablenames.size(); i++) {
		gLogi("gCanvas") << "Table Name:" << tablenames[i];
		std::vector<std::string> columnnames = mysql.getColumnNames(tablenames[i]);
		for(int j = 0; j < columnnames.size(); j++) gLogi("gCanvas") << "Column Name:" << columnnames[j] << ", Column Type:" << mysql.getColumnType(tablenames[i], columnnames[j]);
	}

	mysql.execute("SELECT * FROM " + tablenames[0]);
	while(mysql.hasSelectData()) {
		gLogi("gCanavs") << "SELECT ROW:" << mysql.getSelectData();
	}

	mysql.disconnect();
}

void gCanvas::update() {
}

void gCanvas::draw() {
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
}

void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void gCanvas::mousePressed(int x, int y, int button) {
//	gLogi("gCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void gCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;
}

void gCanvas::mouseScrolled(int x, int y) {
//	gLogi("gCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseEntered() {

}

void gCanvas::mouseExited() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

