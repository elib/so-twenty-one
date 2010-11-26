// hello1.c
// An introduction to keyboard input and text output

#define ALLEGRO_USE_CONSOLE

#include "../VersionInfo.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "World.h"

#ifdef BOUNDINGBOX_ALLOW
#include <allegro5\allegro_primitives.h>
#endif

struct Data
{
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
} data;


const float FPS = 60;


int main(int argc, char *argv[])
{

	srand(GetTickCount());

	//construct world - including logger.
	World world;

	// Initialize Allegro 5 and the font routines
	al_init();
	al_init_font_addon();
	
	//initialize the image addon
	al_init_image_addon();

	// Create a window to display things on: 640x480 pixels
	al_set_new_display_flags(ALLEGRO_OPENGL_3_0 ); 

		//I like this:
		//| ALLEGRO_NOFRAME

		//fullscreen is surprisingly good-looking
		// | ALLEGRO_FULLSCREEN);

	al_set_new_display_option(ALLEGRO_AUX_BUFFERS, 3, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	
	al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 0, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SWAP_METHOD, 2, ALLEGRO_SUGGEST);

	
	data.display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if(!data.display) {
		LOG_WRITE("Display failed!!");
		return -1;
	}


	al_set_window_title(data.display, "So Twenty-One v" VERSION_STRING);
	LOG_WRITE("Running So Twenty-One v" VERSION_STRING);

	ALLEGRO_BITMAP *iconbitmap = al_load_bitmap("Resources/probe-sprite.png");
	al_set_display_icon(data.display, iconbitmap);

	al_hide_mouse_cursor(data.display);

	// Install the keyboard handler
	if(!al_install_keyboard())
	{
		LOG_WRITE("Keyboard failed!!");
		return -1;
	}

	DWORD ticks = GetTickCount();

	data.timer = al_create_timer(1.0 / FPS);
	if(!data.timer)
	{
		LOG_WRITE("Timer failed!!");
		return -1;
	}

	//build world
	if(!world.Initialize(data.display))
		return -1;

	//al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ALPHA, ALLEGRO_ONE);
	//al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ALPHA);
	//al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA,
	//	ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ZERO);

	// Make the backbuffer visible
	al_flip_display();

	// Start the event queue to handle keyboard input
	data.queue = al_create_event_queue();
	al_register_event_source(data.queue, (ALLEGRO_EVENT_SOURCE*)al_get_keyboard_event_source());
	al_register_event_source(data.queue, al_get_display_event_source(data.display));
	al_register_event_source(data.queue, al_get_timer_event_source(data.timer));
	al_start_timer(data.timer);

#ifdef BOUNDINGBOX_ALLOW
	al_init_primitives_addon();
#endif

	// Wait until the user presses escape
	bool redraw = true;
	ALLEGRO_EVENT event;

	while(1)
	{
		al_wait_for_event(data.queue, &event);

		if(event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
		{
			world.SwitchIn();
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
		{
			world.SwitchOut();
		}

		if(event.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
		}
		else
		{
			if((event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				|| (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE))
			{
				break;
			}
		}

		if(event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			world.Keys.ConsumeKeyDown(event.keyboard);
		}
		else if(event.type == ALLEGRO_EVENT_KEY_UP)
		{
			world.Keys.ConsumeKeyUp(event.keyboard);
		}

		if(redraw && al_event_queue_is_empty(data.queue))
		{
			//DWORD nowticks = GetTickCount();
			//DWORD diffticks = nowticks - ticks;
			//ticks = nowticks;
			//double timepassed = diffticks * (1.0 / 1000.0);
			//LOG_WRITE("Time passed: %f - FPS: %f", timepassed, 1.0/timepassed)

			redraw = false;

			if(!world.Update())
				break;
		}
	}

	al_destroy_timer(data.timer);
	al_destroy_display(data.display);
	al_destroy_bitmap(iconbitmap);
	al_destroy_event_queue(data.queue);

	return 0;
}