#include "MusicProvider.h"
#include "World.h"

const char* MusicProvider::_music_file = "Resources/03_Sleepy_Town_Manufacture_-_So_Twenty_One.mp3";

MusicProvider::MusicProvider(void)
{
}

MusicProvider::~MusicProvider(void)
{
}


void MusicProvider::Initialize(ALLEGRO_DISPLAY *display)
{
	HWND hwnd = al_get_win_window_handle(display);
	BASS_Init(-1, 44100, 0, hwnd, NULL);

	_stream = BASS_StreamCreateFile(false, _music_file, 0, 0, 0);
	if(_stream == 0)
	{
		int err = BASS_ErrorGetCode();
		LOG_WRITE("BASS error loading stream: %d", err);
		return;
	}

	if(!BASS_ChannelPlay(_stream, false))
	{
		int err = BASS_ErrorGetCode();
		LOG_WRITE("BASS error playing stream: %d", err);
		return;
	}
}