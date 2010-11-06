#include "MusicProvider.h"
#include "World.h"

#include <algorithm>
#include "TinyXml.h"

const char* MusicProvider::_music_file = "Resources/03_Sleepy_Town_Manufacture_-_So_Twenty_One.mp3";
const char* MusicProvider::_xml_file = "Resources/script.xml";
const char* MusicProvider::_out_xml_file = "Resources/out_recording.xml";

#ifdef _DEBUG
#ifndef IS_RECORDING
#define IS_RECORDING
#endif
#endif

#define BEGINNING_LOOP_START_SEC	 4.473628
#define BEGINNING_LOOP_END_SEC		 7.752834

//audacity:
//start		4.473628
//end		7.752834


void CALLBACK HitLoopPosition(
							  HSYNC handle,
							  DWORD channel,
							  DWORD data,
							  void *user
							  );

MusicProvider::MusicProvider(void)
{
	_loop_next_time = true;
}

MusicProvider::~MusicProvider(void)
{
	StopMusic();
	DestroyMusic();
}

void MusicProvider::PlayMusic()
{
	LOG_WRITE("Playing song");

	if(!BASS_ChannelPlay(_stream, false))
	{
		int err = BASS_ErrorGetCode();
		LOG_WRITE("BASS error playing stream: %d", err);
	}
}

void MusicProvider::StopMusic()
{
	LOG_WRITE("Stopping song");
	BASS_ChannelStop(_stream);
}

void MusicProvider::DestroyMusic()
{
	LOG_WRITE("Destroying song");
	BASS_StreamFree(_stream);

#ifdef IS_RECORDING
	WriteNewEventXml();
#endif
}

void MusicProvider::OpenEventXml()
{
	TiXmlDocument doc(_xml_file);
	if(!doc.LoadFile())
	{
		LOG_WRITE("Could not load music event XML! Error: %s", doc.ErrorDesc());
		return;
	}

	TiXmlElement *datael = doc.FirstChildElement();
	TiXmlElement *curobj = datael->FirstChildElement();
	unsigned int i = 0;
	while(curobj != NULL)
	{
		MusicEvent event;
		event.bytestamp = _atoi64(curobj->Attribute("position"));
		curobj->QueryIntAttribute("type", &event.type);
		_musicEvents.push_back(event);

		curobj = curobj->NextSiblingElement();
		i++;
	}

	LOG_WRITE("Loaded %d events from xml.", i);
}

void MusicProvider::WriteNewEventXml()
{
	sort(_newMusicEvents.begin(), _newMusicEvents.end());

	LOG_WRITE("RECORDING: writing events to xml.");
	TiXmlDocument doc(_out_xml_file);

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "");  
	doc.LinkEndChild( decl );

	TiXmlElement *events = new TiXmlElement("events");
	doc.LinkEndChild(events);
	unsigned int i;
	for(i = 0; i < _newMusicEvents.size(); i++)
	{
		TiXmlElement *singleEvent = new TiXmlElement("event");
		events->LinkEndChild(singleEvent);

		char tmpbuf[100];
		_i64toa_s(_newMusicEvents[i].bytestamp, tmpbuf, 100, 10);
		singleEvent->SetAttribute("position", tmpbuf);
		singleEvent->SetAttribute("type", _newMusicEvents[i].type);
	}

	if(!doc.SaveFile())
	{
		LOG_WRITE("RECORDING: error writing xml event file! Error: %s", doc.ErrorDesc());
	}
	else
	{
		LOG_WRITE("RECORDING: wrote %d events to xml.", i);
	}
}


void MusicProvider::Initialize(ALLEGRO_DISPLAY *display)
{
	//zero all interesting things
	currentPosition = 0;
	_currentRecordingType = 0;
	eventsForCurrentFrame.clear();
	_nextEventIndex = 0;

	//read events from file
	OpenEventXml();

#ifdef IS_RECORDING
	//make recording XML
	LOG_WRITE("Recording mode active!!!");
	_newMusicEvents = _musicEvents;
#endif

	HWND hwnd = al_get_win_window_handle(display);
	BASS_Init(-1, 44100, 0, hwnd, NULL);

	_stream = BASS_StreamCreateFile(false, _music_file, 0, 0, 0);
	if(_stream == 0)
	{
		int err = BASS_ErrorGetCode();
		LOG_WRITE("BASS error loading stream: %d", err);
		return;
	}

	_loop_end_pos = BASS_ChannelSeconds2Bytes(_stream, BEGINNING_LOOP_END_SEC);
	_loop_end_pos += 16 * 1;
	_loop_start_pos = BASS_ChannelSeconds2Bytes(_stream, BEGINNING_LOOP_START_SEC);
	_loop_start_pos -= 16 * 3;

	//LOG_WRITE("Calculated start pos: %lu bytes, barf %d, end pos: %lu bytes", _loop_start_pos, _loop_end_pos, _loop_end_pos);

	BASS_ChannelSetSync(_stream, BASS_SYNC_POS, _loop_end_pos, HitLoopPosition, (void*)this);
}

double MusicProvider::DoNotLoop()
{
	//turn off loop
	_loop_next_time = false;
	QWORD bytesleft = _loop_end_pos - currentPosition;
	return BASS_ChannelBytes2Seconds(_stream, bytesleft);
}


void MusicProvider::Update()
{
	//set current position for this frame
	currentPosition = BASS_ChannelGetPosition(_stream, BASS_POS_BYTE);

	//clear current events for this frame
	eventsForCurrentFrame.clear();

	if(!_loop_next_time)
	{
		//set current events for external consumption
		unsigned int i = 0;
		while(((_nextEventIndex + i) < _musicEvents.size())
			&& (_musicEvents[_nextEventIndex + i].bytestamp < currentPosition))
		{
			eventsForCurrentFrame.push_back(_musicEvents[_nextEventIndex + i]);
			i++;
		}

		//place ourselves in the next event domain
		//if any events were taken, we're at least 1 ahead
		_nextEventIndex += i;
	}


#ifdef IS_RECORDING

	//draw pretty reminder at top of screen - we are recording.
	char rectext[1024];
	sprintf_s(rectext, "RECORDING TYPE %d !!!!", _currentRecordingType);
	double seconds = BASS_ChannelBytes2Seconds(_stream, currentPosition);
	World::theWorld->textLayer.AddText(rectext, World::theWorld->fonts.SmallFont,
			al_map_rgba_f(1, 0, 0, 1.0), DISPLAY_WIDTH/2, 0, ALLEGRO_ALIGN_CENTRE);
	char postext[1024];
	sprintf_s(postext, "time (sec): %3.3f", seconds);

	World::theWorld->textLayer.AddText(postext, World::theWorld->fonts.SmallFont,
		al_map_rgba_f(1, 1, 1, 1.0), DISPLAY_WIDTH/2, 15, ALLEGRO_ALIGN_CENTRE);

	//end pretty reminder

	if(World::theWorld->Keys.keys_just_down[ALLEGRO_KEY_PAD_0])
	{
		//consume "record" events 
		MusicEvent ev;
		ev.bytestamp = currentPosition;
		ev.type = _currentRecordingType;
		//must be sorted later
		_newMusicEvents.push_back(ev);

		//show this in real time, as well!
		eventsForCurrentFrame.push_back(ev);
	}
	else if(World::theWorld->Keys.keys_just_down[ALLEGRO_KEY_PAD_PLUS])
	{
		_currentRecordingType++;
	}
	else if(World::theWorld->Keys.keys_just_down[ALLEGRO_KEY_PAD_MINUS])
	{
		_currentRecordingType--;
		_currentRecordingType = max(0, _currentRecordingType);
	}

#endif
}

void MusicProvider::Fadeout(double time)
{
	//fade out a bit sooner than requested
	BASS_ChannelSlideAttribute(_stream, BASS_ATTRIB_VOL, 0, (time * 1000) - 0.5);  
}


void MusicProvider::DoLoop()
{
	if(_loop_next_time)
	{
		if(!BASS_ChannelSetPosition(_stream, _loop_start_pos, BASS_POS_BYTE))
		{
			int err = BASS_ErrorGetCode();
			LOG_WRITE("Error setting stream position: %d", err);
		}

		BASS_ChannelUpdate(_stream, 0);
	}
}



void CALLBACK HitLoopPosition(
							  HSYNC handle,
							  DWORD channel,
							  DWORD data,
							  void *user
							  )
{

	MusicProvider *provider = (MusicProvider*)user;
	provider->DoLoop();
}