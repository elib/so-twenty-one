#include "Log.h"

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
#if (defined(_DEBUG) || defined(USE_LOG))
	_logFile = al_fopen("debug.log", "w+");
	LogWrite("Log file open.");
#endif
}

void Log::LogWrite(const char * msg)
{
#if (defined(_DEBUG) || defined(USE_LOG))
	al_fputs(_logFile, msg);
#endif
}