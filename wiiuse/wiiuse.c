/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This file is part of wiiuse.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief API source file
 *
 *	The file must be linked to any third party
 *	program that is utilizing wiiuse as an
 *	external library.
 */

#define WIIUSE_API_SRC

#include <stdio.h>
#include "wiiuse.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

typedef int (*entry_func_t)(struct wiiuse_api_t**);

struct wiiuse_api_t* wiiuse_api = NULL;
void* wiiuse_mod = NULL;

/**
 *	@brief Load the wiiuse library and initialize the function pointers.
 *
 *	@param wiiuse_file	The relative or absolute path to the wiiuse library file.
 *
 *	@return The version of the wiiuse library loaded.
 *
 *	@see wiiuse_shutdown()
 *
 *	If the version of wiiuse being used has a different API
 *	version as expected, this function will fail and return 0.
 */
const char* wiiuse_startup(char* wiiuse_file) {
	entry_func_t entry_func = NULL;

	if (wiiuse_api)
		/* already loaded */
		return wiiuse_api->version;

	if (!wiiuse_file)
		return NULL;

	/* load the module */
	wiiuse_mod = dlopen(wiiuse_file, RTLD_NOW);

	if (!wiiuse_mod)
		/* can not load module */
		return NULL;

	/* get the entry point */
	entry_func = (entry_func_t)dlsym(wiiuse_mod, "wiiuse_main");

	if (!entry_func) {
		wiiuse_shutdown();
		return NULL;
	}

	/* call the entry function */
	entry_func(&wiiuse_api);

	/* make sure the API versions are the same */
	if (wiiuse_api->api_version != WIIUSE_API_VERSION) {
		wiiuse_shutdown();
		return NULL;
	}

	/* set all the function pointers */
	wiiuse_init					= wiiuse_api->_wiiuse_init;
	wiiuse_disconnected			= wiiuse_api->_wiiuse_disconnected;
	wiiuse_rumble				= wiiuse_api->_wiiuse_rumble;
	wiiuse_toggle_rumble		= wiiuse_api->_wiiuse_toggle_rumble;
	wiiuse_set_leds				= wiiuse_api->_wiiuse_set_leds;
	wiiuse_motion_sensing		= wiiuse_api->_wiiuse_motion_sensing;
	wiiuse_read_data			= wiiuse_api->_wiiuse_read_data;
	wiiuse_write_data			= wiiuse_api->_wiiuse_write_data;
	wiiuse_status				= wiiuse_api->_wiiuse_status;
	wiiuse_get_by_id			= wiiuse_api->_wiiuse_get_by_id;
	wiiuse_set_flags			= wiiuse_api->_wiiuse_set_flags;
	wiiuse_set_smooth_alpha		= wiiuse_api->_wiiuse_set_smooth_alpha;
	wiiuse_set_ir				= wiiuse_api->_wiiuse_set_ir;
	wiiuse_set_ir_vres			= wiiuse_api->_wiiuse_set_ir_vres;
	wiiuse_set_ir_position		= wiiuse_api->_wiiuse_set_ir_position;
	wiiuse_set_aspect_ratio		= wiiuse_api->_wiiuse_set_aspect_ratio;
	wiiuse_set_bluetooth_stack	= wiiuse_api->_wiiuse_set_bluetooth_stack;
	wiiuse_set_orient_threshold	= wiiuse_api->_wiiuse_set_orient_threshold;
	wiiuse_find					= wiiuse_api->_wiiuse_find;
	wiiuse_connect				= wiiuse_api->_wiiuse_connect;
	wiiuse_disconnect			= wiiuse_api->_wiiuse_disconnect;
	wiiuse_poll					= wiiuse_api->_wiiuse_poll;

	printf("wiiuse v%s loaded ( http://wiiuse.net http://wiiuse.sf.net/ ).\n", wiiuse_api->version);

	return wiiuse_api->version;
}


/**
 *	@brief Unload the library.
 *
 *	@see wiiuse_startup()
 */
void wiiuse_shutdown() {
	if (!wiiuse_mod)
		return;

	/* unload the module */
	dlclose(wiiuse_mod);

	wiiuse_api = NULL;

	wiiuse_init					= NULL;
	wiiuse_disconnected			= NULL;
	wiiuse_rumble				= NULL;
	wiiuse_toggle_rumble		= NULL;
	wiiuse_set_leds				= NULL;
	wiiuse_motion_sensing		= NULL;
	wiiuse_read_data			= NULL;
	wiiuse_write_data			= NULL;
	wiiuse_status				= NULL;
	wiiuse_get_by_id			= NULL;
	wiiuse_set_flags			= NULL;
	wiiuse_set_smooth_alpha		= NULL;
	wiiuse_set_ir				= NULL;
	wiiuse_set_ir_vres			= NULL;
	wiiuse_set_ir_position		= NULL;
	wiiuse_set_aspect_ratio		= NULL;
	wiiuse_set_bluetooth_stack	= NULL;
	wiiuse_set_orient_threshold	= NULL;
	wiiuse_find					= NULL;
	wiiuse_connect				= NULL;
	wiiuse_disconnect			= NULL;
	wiiuse_poll					= NULL;

}
