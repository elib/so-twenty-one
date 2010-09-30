#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_windows.h>

#include "Bass.h"

#include <vector>
using namespace std;

typedef struct MUSICEVENT
{
	MUSICEVENT()
	{
		type = bytestamp = 0;
	}


	int operator < (MUSICEVENT &other)
	{
		return (this->bytestamp < other.bytestamp);
	}

	int type;
	QWORD bytestamp;

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

	vector<MusicEvent> eventsForCurrentFrame;

	QWORD currentPosition;

	void Initialize(ALLEGRO_DISPLAY *display);

	double DoNotLoop();

protected:

	//read/write
	void OpenEventXml();
	void WriteNewEventXml();

	int _nextEventIndex;

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
	int _currentRecordingType;


	//beginning loop
	bool _loop_next_time;
	QWORD _loop_start_pos;
	QWORD _loop_end_pos;
};