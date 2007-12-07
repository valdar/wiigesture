#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "wiiuse.h"

#ifdef __cplusplus
}
#endif

#define WIIUSE_PATH	"./lib/wiiuse.so"
#include <iostream>

/**
 *	@brief Callback that handles an event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	This function is called automatically by the wiiuse library when an
 *	event occurs on the specified wiimote.
 */
void handle_event(struct wiimote_t* wm) {


    if (wm->btns) {
		/*
		 *	Pressing minus will tell the wiimote we are no longer interested in movement.
		 *	This is useful because it saves battery power.
		 */
		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)){
            printf("\n\n--- EVENT ACC_ON [wiimote id %i] ---\n", wm->unid);
			wiiuse_motion_sensing(wm, 0);
		}

		/*
		 *	Pressing plus will tell the wiimote we are interested in movement.
		 */
		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)){
            printf("\n\n--- EVENT ACC_OFF[wiimote id %i] ---\n", wm->unid);
			wiiuse_motion_sensing(wm, 1);
		}
    }

    /* if the accelerometer is turned on then print angles */
	if (WIIUSE_USING_ACC(wm)) {
		if(wm->btns){
		    if(IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A)){
                printf("INIZIO GESTURE\n");
		    }
		}

        if(wm->btns_held){
		    if(IS_HELD(wm, WIIMOTE_BUTTON_A)){
                printf("wiimote X acc   = %f\n", wm->gforce.x);
                printf("wiimote Y acc   = %f\n", wm->gforce.y);
                printf("wiimote Z acc   = %f\n", wm->gforce.z);
		    }
        }

        if(wm->btns_released){
            if(IS_RELEASED(wm, WIIMOTE_BUTTON_A)){
                printf("FINE GESTURE\n");
		    }
		}
	}
}

/**
 *	@brief Callback that handles a controller status event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *	@param attachment		Is there an attachment? (1 for yes, 0 for no)
 *	@param speaker			Is the speaker enabled? (1 for yes, 0 for no)
 *	@param ir				Is the IR support enabled? (1 for yes, 0 for no)
 *	@param led				What LEDs are lit.
 *	@param battery_level	Battery level, between 0.0 (0%) and 1.0 (100%).
 *
 *	This occurs when either the controller status changed
 *	or the controller status was requested explicitly by
 *	wiiuse_status().
 *
 *	One reason the status can change is if the nunchuk was
 *	inserted or removed from the expansion port.
 */
void handle_ctrl_status(struct wiimote_t* wm, int attachment, int speaker, int ir, int led[4], float battery_level) {
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

	printf("attachment:      %i\n", attachment);
	printf("speaker:         %i\n", speaker);
	printf("ir:              %i\n", ir);
	printf("leds:            %i %i %i %i\n", led[0], led[1], led[2], led[3]);
	printf("battery:         %f %%\n", battery_level);

}

/**
 *	@brief Callback that handles a disconnection event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *
 *	This can happen if the POWER button is pressed, or
 *	if the connection is interrupted.
 */
void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}

/*
 *	These are some identifiers for wiimotes
 *
 *	See below in main() for what they are used for.
 */
#define WIIMOTE_ID_1		1

/**
 *	@brief main()
 *
 *	Connect to up to one wiimotes and print any events
 *	that occur on either device.
 */
int main(int argc, char** argv) {

	/*

	 *	Make a temp array of wiimote ids.

	 *

	 *	Here I only anticipate connecting up to

	 *	two wiimotes.  Each wiimote connected

	 *	will get one of these ids.

	 */

	int ids[] = { WIIMOTE_ID_1 };



	const char* version;

	wiimote** wiimotes;

	int found, connected;


	/*
	 *	Load the wiiuse library
	 *
	 *	This needs to be done before anything else can happen
	 *	wiiuse_startup() will return the version of the library loaded.
	 */

	version = wiiuse_startup(WIIUSE_PATH);

	printf("Wiiuse Version = %s\n", version);
	if (!version) {
		fprintf(stderr, "Failed to load wiiuse library.\n");
		return 0;
	}

	/*
	 *	Initialize an array of wiimote objects.
	 *
	 *	The first parameter is the number of wiimotes
	 *	I want to create.  I only have one wiimotes
	 *	so I'm limiting the test to just 1.
	 *
	 *	Then I get it the array of ids and a couple
	 *	callback functions to invoke when something
	 *	happens on one of the wiimotes.
	 *
	 *	handle_event gets called when a generic event occurs (button press, motion sensing, etc)
	 *	handle_ctrl_status gets called when a response to a status request arrives (battery power, etc)
	 *	handle_disconnect gets called when the wiimote disconnect (holding power button)
	 */
	wiimotes =  wiiuse_init(1, ids, handle_event, handle_ctrl_status, handle_disconnect);

	/*
	 *	Find wiimote devices
	 *
	 *	Now we need to find some wiimotes.
	 *	Give the function the wiimote array we created, and tell it there
	 *	are 1 wiimotes we are interested in.
	 *
	 *	Set the timeout to be 5 seconds.
	 *
	 *	This will return the number of actual wiimotes that are in discovery mode.
	 */

	found = wiiuse_find(wiimotes, 1, 5);
	if (!found)
		return 0;

	/*
	 *	Connect to the wiimotes
	 *
	 *	Now that we found some wiimotes, connect to them.
	 *	Give the function the wiimote array and the number
	 *	of wiimote devices we found.
	 *
	 *	This will return the number of established connections to the found wiimotes.
	 */
	connected = wiiuse_connect(wiimotes, 1);
	if (connected)
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}

	/*
	 *	Now set the LEDs and rumble for a second so it's easy
	 *	to tell which wiimotes are connected (just like the wii does).
	 */
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	wiiuse_rumble(wiimotes[0], 1);
    usleep(200000);
    wiiuse_rumble(wiimotes[0], 0);


	/*
	 *	Maybe I'm interested in the battery power of the 0th
	 *	wiimote.  This should be WIIMOTE_ID_1 but to be sure
	 *	you can get the wiimote assoicated with WIIMOTE_ID_1
	 *	using the wiiuse_get_by_id() function.
	 *
	 *	A status request will return other things too, like
	 *	if any expansions are plugged into the wiimote or
	 *	what LEDs are lit.
	 */
	wiiuse_status(wiimotes[0]);

	/*
	 *	This is the main loop
	 *
	 *	wiiuse_poll() needs to be called with the wiimote array
	 *	and the number of wiimote structures in that array
	 *	(it doesn't matter if some of those wiimotes are not used
	 *	or are not connected).
	 *
	 *	This function will invoke the callbacks set in wiiuse_init()
	 *	when the wiimote has things to report.
	 */
	while (1) {
		wiiuse_poll(wiimotes, 1);
	}

	/*
	 *	Disconnect the wiimotes
	 */
	wiiuse_disconnect(wiimotes[0]);

	/*
	 *	Unload the wiiuse library
	 */
	wiiuse_shutdown();

	return 0;
}
