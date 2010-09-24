#include "MusicProvider.h"
#include "World.h"

const char* MusicProvider::_music_file = "Resources/03_Sleepy_Town_Manufacture_-_So_Twenty_One.mp3";
const char* MusicProvider::_xml_file = "Resources/recording.xml";

#define IS_RECORDING

MusicProvider::MusicProvider(void)
{
	currentPosition = 0;
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
	_recording_xml_doc->SaveFile();
	delete _recording_xml_doc;
#endif
}

void MusicProvider::OpenEventXml()
{
	_recording_xml_doc = new TiXmlDocument(_xml_file);

	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "");  
	_recording_xml_doc->LinkEndChild( decl );

	_recording_events_element = new TiXmlElement("events");
	_recording_xml_doc->LinkEndChild(_recording_events_element);
}


void MusicProvider::Initialize(ALLEGRO_DISPLAY *display)
{
#ifdef IS_RECORDING
	//make recording XML
	OpenEventXml();
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

	PlayMusic();
}

void MusicProvider::Update()
{
	//set current position for this frame
	currentPosition = BASS_ChannelGetPosition(_stream, BASS_POS_BYTE);

	al_draw_text(World::theWorld->fonts.SmallFont,
			al_map_rgba_f(1, 0, 0, 1.0), DISPLAY_WIDTH/2, 0, ALLEGRO_ALIGN_CENTRE, "RECORDING!" );

#ifdef IS_RECORDING

	char postext[1024];
	sprintf_s(postext, 1024, "position: %d", currentPosition);

	al_draw_text(World::theWorld->fonts.SmallFont,
		al_map_rgba_f(1, 1, 1, 1.0), DISPLAY_WIDTH/2, 15, ALLEGRO_ALIGN_CENTRE, postext );

	if(World::theWorld->Keys.keys_just_down[ALLEGRO_KEY_PAD_0])
	{
		//mark this frame as recorded
		TiXmlElement *singleevent = new TiXmlElement("event");
		singleevent->SetAttribute("position", currentPosition);
		_recording_events_element->LinkEndChild(singleevent);
	}
#endif
}