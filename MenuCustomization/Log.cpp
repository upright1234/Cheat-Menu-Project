#include "Log.h"
#include <string.h>
#include <cstdarg>
#include <stdio.h>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <iostream>


const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
void Log::Log(const char* fmt, ...)
{
	if (!fmt)	return;
	char text[4096];
	char log[4096];
	va_list		ap;
	va_start(ap, fmt);
	std::string date = "[ " + currentDateTime() + "]";
	vsprintf_s(text, fmt, ap);
	strcpy_s(log, date.c_str());
	strcat_s(log, text);
	va_end(ap);
	std::string path = "C:\\8610110310RSCFG\\Xdf.log";
	std::ofstream logfile(path.c_str(), std::ios::app);
	if (logfile.is_open() && log)	logfile << log << std::endl;
	logfile.close();
}