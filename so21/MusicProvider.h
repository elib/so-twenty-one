#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_windows.h>

#include "Bass.h"

#include "TinyXml.h"

#include <vector>
using namespace std;

typedef struct MUSICEVENT
{
	MUSICEVENT()
	{
		type = bytestamp = 0;
	}

	int type;
	long bytestamp;
} MusicEvent;

class MusicProvider
{
public:
	MusicProvider(void);
	~MusicProvider(void);

	void PlayMusic();
	void StopMusic();
	void DestroyMusic();

	void Update();

	QWORD currentPosition;

	void Initialize(ALLEGRO_DISPLAY *display);

	void OpenEventXml();

protected:
	//files
	static const char* _music_file;
	static const char* _xml_file;
	static const char* _out_xml_file;
	
	//the music stream
	HSTREAM _stream;

	//reading existing ones:
	vector<MusicEvent> _musicEvents;
	//for recordings:
	vector<MusicEvent> _newMusicEvents;
};