#include <iostream>
#include <string>
#include <fstream>
#include "../WinAPIHeaders/framework.h"
#include "../WinAPIHeaders/resource.h"
#include <windows.h>
#include <windowsx.h>
#include "Logger.h"

using namespace LOGGER;

	LOGGER::LOG::LOG() {
		name = "output.log";
		WINDOW = NULL;
	}
	LOGGER::LOG::LOG(std::string filename) {
		name = filename;
		WINDOW = NULL;
	}
	LOGGER::LOG::LOG(std::string filename, void* hwnd) {
		name = filename;
		WINDOW = hwnd;
	}
	std::string LOGGER::LOG::getLogger() {
		return this->name;
	}
	void LOGGER::LOG::info(const std::string log) {
		info(log.c_str());
	}
	void LOGGER::LOG::info(const char* log) {
		std::ofstream f(this->name, std::ios::app);
		if (f.is_open()) {
			f << log << std::endl;
			f.close();
		}
	}
	void LOGGER::LOG::infoMB(const std::string log, const std::string title) {
		infoMB(log.c_str(), title.c_str());
	}
	void LOGGER::LOG::infoMB(const char* log, const char* title) {
		info(log);
		int len;
		int slength = (int)strlen(log) + 1;
		len = MultiByteToWideChar(CP_ACP, 0, log, slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, log, slength, buf, len);
		slength = (int)strlen(title) + 1;
		len = MultiByteToWideChar(CP_ACP, 0, title, slength, 0, 0);
		wchar_t* bufT = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, title, slength, bufT, len);
		MessageBox((HWND)this->WINDOW, buf, bufT, 0);
		delete[] buf;
		delete[] bufT;
	}
	void LOGGER::LOG::setWindow(void* hwnd) {
		this->WINDOW = hwnd;
	}
	void* LOGGER::LOG::getWindow() {
		return this->WINDOW;
	}

	LOGGER::LOG LOGGER::LOGS::getLOGGER() {
		return getLOGGER("output.log");
	}
	LOGGER::LOG LOGGER::LOGS::getLOGGER(std::string filename) {
		if (log.size() > 0) {
			for (LOG l : log) {
				if (_strcmpi(l.getLogger().c_str(),filename.c_str()) == 0)
					return l;
			}
		}
		LOG l(filename);
		log.push_back(l);
		return l;
	}

	std::vector<LOG> LOGS::log;