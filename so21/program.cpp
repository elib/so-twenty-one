// hello1.c
// An introduction to keyboard input and text output

#define ALLEGRO_USE_CONSOLE

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

#include "World.h"

struct Data {
	
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;

	ALLEGRO_COLOR bright_green;
} data;


const float FPS = 60;


int main(int argc, char *argv[]) {
	// Initialize Allegro 5 and the font routines
	al_init();
	al_init_font_addon();

	fprintf(stderr, "failed to create timer!\n");

	// Create a window to display things on: 640x480 pixels
	data.display = al_create_display(640, 480);
	if(!data.display) {
		printf("Error creating display.\n");
		return 1;
	}

	// Install the keyboard handler
	if(!al_install_keyboard()) {
		printf("Error installing keyboard.\n");
		return 1;
	}

	data.timer = al_create_timer(1.0 / FPS);
	if(!data.timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//initialize the image addon
	al_init_image_addon();

	//build world
	World world;
	if(!world.Initialize(data.timer))
		return -1;

	// Make and set a color to draw with
	data.bright_green = al_map_rgba_f(0.5, 1.0, 0.5, 1.0);
	//al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ALPHA, ALLEGRO_ONE);

	// Make the backbuffer visible
	al_flip_display();

	// Start the event queue to handle keyboard input
	data.queue = al_create_event_queue();
	al_register_event_source(data.queue, (ALLEGRO_EVENT_SOURCE*)al_get_keyboard_event_source());
	al_register_event_source(data.queue, al_get_display_event_source(data.display));
	al_register_event_source(data.queue, al_get_timer_event_source(data.timer));
	al_start_timer(data.timer);



	// Wait until the user presses escape
	bool redraw = true;
	ALLEGRO_EVENT event;
	while(1)
	{
		al_wait_for_event(data.queue, &event);

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

		if(redraw && al_event_queue_is_empty(data.queue))
		{
			redraw = false;
			world.Update();
			al_flip_display();
		}
	}

	al_destroy_timer(data.timer);

	return 0;
}