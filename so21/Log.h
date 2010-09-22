#pragma once

#include <allegro5/allegro.h>

class Log
{
public:
	Log(void);
	void Initialize();
	void LogWrite(const char * msg);
	void DestroyLog();
	~Log(void);
	
private:
	ALLEGRO_FILE *_logFile;
};
