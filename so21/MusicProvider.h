#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_windows.h>

#include "Bass.h"

#include "TinyXml.h"

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
	static const char* _music_file;
	static const char* _xml_file;
	static const char* _out_xml_file;
	HSTREAM _stream;

	TiXmlElement *_recording_events_element;
	TiXmlDocument *_recording_xml_doc;
};