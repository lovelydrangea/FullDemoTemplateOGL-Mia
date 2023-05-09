#include <iostream>
#include <vector>

#pragma once
#ifndef LOGGER_H
#define LOGGER_H

namespace LOGGER {
	class LOG {
	private:
		void *WINDOW;
		std::string name;
	public:
		LOG();
		LOG(std::string filename);
		LOG(std::string filename, void* hwnd);
		std::string getLogger();
		void info(const std::string log);
		void info(const char* log);
		void infoMB(const std::string log, const std::string title);
		void infoMB(const char* log, const char* title);
		void setWindow(void* hwnd);
		void* getWindow();
	};

	class LOGS {
	private:
		static std::vector<LOG> log;
	public:
		static LOG getLOGGER();
		static LOG getLOGGER(std::string filename);
	};
}
#endif
