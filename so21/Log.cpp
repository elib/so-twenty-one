#include "Log.h"

#ifdef _DEBUG
#ifndef USE_LOG
#define USE_LOG
#endif
#endif

Log::Log(void)
{
	_logFile = NULL;
}

Log::~Log(void)
{
	DestroyLog();
}

void Log::DestroyLog()
{
	if(_logFile)
		al_fclose(_logFile);
	_logFile = NULL;
}

void Log::Initialize()
{
#ifdef USE_LOG
	_logFile = al_fopen("debug.log", "w+");
	LogWrite("Log file open.");
#endif
}

void Log::LogWrite(const char * msg)
{
#ifdef USE_LOG
	al_fputs(_logFile, msg);
	al_fflush(_logFile);
#endif
}