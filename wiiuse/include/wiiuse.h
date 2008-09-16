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
 *
 *	@brief API header file.
 *
 *	If this file is included from inside the wiiuse source
 *	and not from a third party program, then wiimote_internal.h
 *	is also included which extends this file.
 */

#ifndef WIIUSE_H_INCLUDED
#define WIIUSE_H_INCLUDED

/* ignore this, this is used internally for wiiuse */
#ifdef __WIIUSE__
	#ifndef WIIUSE_INTERNAL_H_INCLUDED
		#error wiiuse.h included directly.  Must include wiiuse_internal.h instead.
	#endif
	#define WCONST
#else
	#define WCONST		const
#endif

#if defined(WIN32) || defined(__WIN32__)
	/* windows */
	#include <windows.h>
	#include <hidsdi.h>
	#include <setupapi.h>
	#include <hidpi.h>

	#define dlopen(file, x)		(void*)LoadLibrary(file)
	#define dlsym(dll, func)	(void*)GetProcAddress((HMODULE)(dll), (func))
	#define dlclose(dll)		FreeLibrary((HMODULE)(dll))
	#define Dl_info				MEMORY_BASIC_INFORMATION
	#define dladdr(func, inf)	VirtualQuery(func, inf, sizeof(*inf))
#else
	/* nix */
	#include <bluetooth/bluetooth.h>
#endif

/* wiiuse version and API version */
#define WIIUSE_VERSION					"0.9"
#define WIIUSE_API_VERSION				8

/* led bit masks */
#define WIIMOTE_LED_NONE				0x00
#define WIIMOTE_LED_1					0x10
#define WIIMOTE_LED_2					0x20
#define WIIMOTE_LED_3					0x40
#define WIIMOTE_LED_4					0x80

/* button codes */
#define WIIMOTE_BUTTON_TWO				0x0001
#define WIIMOTE_BUTTON_ONE				0x0002
#define WIIMOTE_BUTTON_B				0x0004
#define WIIMOTE_BUTTON_A				0x0008
#define WIIMOTE_BUTTON_MINUS			0x0010
#define WIIMOTE_BUTTON_ZACCEL_BIT6		0x0020
#define WIIMOTE_BUTTON_ZACCEL_BIT7		0x0040
#define WIIMOTE_BUTTON_HOME				0x0080
#define WIIMOTE_BUTTON_LEFT				0x0100
#define WIIMOTE_BUTTON_RIGHT			0x0200
#define WIIMOTE_BUTTON_DOWN				0x0400
#define WIIMOTE_BUTTON_UP				0x0800
#define WIIMOTE_BUTTON_PLUS				0x1000
#define WIIMOTE_BUTTON_ZACCEL_BIT4		0x2000
#define WIIMOTE_BUTTON_ZACCEL_BIT5		0x4000
#define WIIMOTE_BUTTON_UNKNOWN			0x8000
#define WIIMOTE_BUTTON_ALL				0x1F9F

/* nunchul button codes */
#define NUNCHUK_BUTTON_Z				0x01
#define NUNCHUK_BUTTON_C				0x02

/* classic controller button codes */
#define CLASSIC_CTRL_BUTTON_UP			0x0001
#define CLASSIC_CTRL_BUTTON_LEFT		0x0002
#define CLASSIC_CTRL_BUTTON_ZR			0x0004
#define CLASSIC_CTRL_BUTTON_X			0x0008
#define CLASSIC_CTRL_BUTTON_A			0x0010
#define CLASSIC_CTRL_BUTTON_Y			0x0020
#define CLASSIC_CTRL_BUTTON_B			0x0040
#define CLASSIC_CTRL_BUTTON_ZL			0x0080
#define CLASSIC_CTRL_BUTTON_FULL_R		0x0200
#define CLASSIC_CTRL_BUTTON_PLUS		0x0400
#define CLASSIC_CTRL_BUTTON_HOME		0x0800
#define CLASSIC_CTRL_BUTTON_MINUS		0x1000
#define CLASSIC_CTRL_BUTTON_FULL_L		0x2000
#define CLASSIC_CTRL_BUTTON_DOWN		0x4000
#define CLASSIC_CTRL_BUTTON_RIGHT		0x8000

/* wiimote option flags */
#define WIIUSE_SMOOTHING				0x01
#define WIIUSE_CONTINUOUS				0x02
#define WIIUSE_ORIENT_THRESH			0x04
#define WIIUSE_INIT_FLAGS				(WIIUSE_SMOOTHING | WIIUSE_ORIENT_THRESH)

/* IR correction types */
typedef enum ir_position_t {
	WIIUSE_IR_ABOVE,
	WIIUSE_IR_BELOW
} ir_position_t;

/**
 *	@brief Check if a button is pressed.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is pressed, 0 if not.
 */
#define IS_PRESSED(dev, button)		((dev->btns & button) == button)

/**
 *	@brief Check if a button is being held.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is held, 0 if not.
 */
#define IS_HELD(dev, button)			((dev->btns_held & button) == button)

/**
 *	@brief Check if a button is released on this event.					\n\n
 *			This does not mean the button is not pressed, it means		\n
 *			this button was just now released.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is released, 0 if not.
 *
 */
#define IS_RELEASED(dev, button)		((dev->btns_released & button) == button)

/**
 *	@brief Check if a button has just been pressed this event.
 *	@param dev		Pointer to a wiimote_t or expansion structure.
 *	@param button	The button you are interested in.
 *	@return 1 if the button is pressed, 0 if not.
 */
#define IS_JUST_PRESSED(dev, button)	(IS_PRESSED(dev, button) && !IS_HELD(dev, button))

#define WIIUSE_USING_ACC(wm)			((wm->state & 0x10) == 0x10)
#define WIIUSE_USING_EXP(wm)			((wm->state & 0x20) == 0x20)
#define WIIUSE_USING_IR(wm)				((wm->state & 0x40) == 0x40)

/*
 *	Largest known payload is 21 bytes.
 *	Add 2 for the prefix and round up to a power of 2.
 */
#define MAX_PAYLOAD			32

typedef unsigned char byte;
typedef char sbyte;

struct wiimote_t;
struct vec3b_t;
struct orient_t;
struct gforce_t;

/**
 *	@brief Event callback.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param btns		What buttons are currently pressed.  They are OR'ed together.
 *	@param accel	Acceleration of the device along each axis.
 *					This is the raw data reported by the wiimote.
 *	@param orient	Orientation (roll, pitch, yaw) of the device.
 *	@param gforce	Pull of gravity on each axis of the device (measured in gravity units).
 *
 *	@see wiiuse_init()
 *
 *	A registered function of this type is called automatically by the wiiuse
 *	library when an event occurs on the specified wiimote.
 */
typedef void (*wiiuse_event_cb)(struct wiimote_t* wm);

/**
 *	@brief Callback that handles a read event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param data		Pointer to the filled data block.
 *	@param len		Length in bytes of the data block.
 *
 *	@see wiiuse_init()
 *
 *	A registered function of this type is called automatically by the wiiuse
 *	library when the wiimote has returned the full data requested by a previous
 *	call to wiiuse_read_data().
 */
typedef void (*wiiuse_read_cb)(struct wiimote_t* wm, byte* data, unsigned short len);

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
 *	@see wiiuse_init()
 *
 *	A registered function of this type is called automatically by the wiiuse
 *	library when either the controller status changed or the controller
 *	status was requested explicitly by wiiuse_status().
 */
typedef void (*wiiuse_ctrl_status_cb)(struct wiimote_t* wm, int attachment, int speaker, int ir, int led[4], float battery_level);

/**
 *	@brief Callback that handles a disconnection event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *
 *	@see wiiuse_init()
 *
 *	A registered function of this type is called automatically by the wiiuse
 *	library when a disconnection occurs.  This can happen if the POWER button
 *	is pressed or if the connection is interrupted.
 */
typedef void (*wiiuse_dis_cb)(struct wiimote_t* wm);


/**
 *	@struct read_req_t
 *	@brief Data read request structure.
 */
struct read_req_t {
	wiiuse_read_cb cb;			/**< read data callback						*/
	byte* buf;					/**< buffer where read data is written		*/
	unsigned int addr;			/**< the offset that the read started at	*/
	unsigned short size;		/**< the length of the data read			*/
	unsigned short wait;		/**< num bytes still needed to finish read	*/

	struct read_req_t* next;	/**< next read request in the queue			*/
};


/**
 *	@struct vec2b_t
 *	@brief Unsigned x,y byte vector.
 */
typedef struct vec2b_t {
	byte x, y;
} vec2b_t;


/**
 *	@struct vec3b_t
 *	@brief Unsigned x,y,z byte vector.
 */
typedef struct vec3b_t {
	byte x, y, z;
} vec3b_t;


/**
 *	@struct vec3f_t
 *	@brief Signed x,y,z float struct.
 */
typedef struct vec3f_t {
	float x, y, z;
} vec3f_t;


/**
 *	@struct orient_t
 *	@brief Orientation struct.
 *
 *	Yaw, pitch, and roll range from -180 to 180 degrees.
 */
typedef struct orient_t {
	float roll;
	float pitch;
	float yaw;
} orient_t;


/**
 *	@struct gforce_t
 *	@brief Gravity force struct.
 */
typedef struct gforce_t {
	float x, y, z;
} gforce_t;


/**
 *	@struct accel_t
 *	@brief Accelerometer struct. For any device with an accelerometer.
 */
typedef struct accel_t {
	struct vec3b_t cal_zero;		/**< zero calibration					*/
	struct vec3b_t cal_g;			/**< gravity calibration				*/

	float st_roll;					/**< last smoothed roll value			*/
	float st_pitch;					/**< last smoothed roll pitch			*/
	float st_alpha;					/**< alpha value for smoothing [0-1]	*/
} accel_t;


/**
 *	@struct ir_dot_t
 *	@brief A single IR source.
 */
typedef struct ir_dot_t {
	byte visible;					/**< if the IR source is visible		*/

	unsigned int x;					/**< interpolated X coordinate			*/
	unsigned int y;					/**< interpolated Y coordinate			*/

	short rx;						/**< raw X coordinate (0-1023)			*/
	short ry;						/**< raw Y coordinate (0-767)			*/

	byte order;						/**< increasing order by x-axis value	*/

	byte size;						/**< size of the IR dot (0-15)			*/
} ir_dot_t;


/**
 *	@enum aspect_t
 *	@brief Screen aspect ratio.
 */
typedef enum aspect_t {
	WIIUSE_ASPECT_4_3,
	WIIUSE_ASPECT_16_9
} aspect_t;


/**
 *	@struct ir_t
 *	@brief IR struct. Hold all data related to the IR tracking.
 */
typedef struct ir_t {
	struct ir_dot_t dot[4];			/**< IR dots							*/
	byte num_dots;					/**< number of dots at this time		*/

	enum aspect_t aspect;			/**< aspect ratio of the screen			*/

	enum ir_position_t pos;			/**< IR sensor bar position				*/

	unsigned int vres[2];			/**< IR virtual screen resolution		*/
	int offset[2];					/**< IR XY correction offset			*/
	int state;						/**< keeps track of the IR state		*/

	int ax;							/**< absolute X coordinate				*/
	int ay;							/**< absolute Y coordinate				*/

	int x;							/**< calculated X coordinate			*/
	int y;							/**< calculated Y coordinate			*/

	float distance;					/**< pixel distance between first 2 dots*/
	float z;						/**< calculated distance				*/
} ir_t;


/**
 *	@struct joystick_t
 *	@brief Joystick calibration structure.
 *
 *	The angle \a ang is relative to the positive y-axis into quadrant I
 *	and ranges from 0 to 360 degrees.  So if the joystick is held straight
 *	upwards then angle is 0 degrees.  If it is held to the right it is 90,
 *	down is 180, and left is 270.
 *
 *	The magnitude \a mag is the distance from the center to where the
 *	joystick is being held.  The magnitude ranges from 0 to 1.
 *	If the joystick is only slightly tilted from the center the magnitude
 *	will be low, but if it is closer to the outter edge the value will
 *	be higher.
 */
typedef struct joystick_t {
	struct vec2b_t max;				/**< maximum joystick values	*/
	struct vec2b_t min;				/**< minimum joystick values	*/
	struct vec2b_t center;			/**< center joystick values		*/

	float ang;						/**< angle the joystick is being held		*/
	float mag;						/**< magnitude of the joystick (range 0-1)	*/
} joystick_t;


/**
 *	@struct nunchuk_t
 *	@brief Nunchuk expansion device.
 */
typedef struct nunchuk_t {
	struct accel_t accel_calib;		/**< nunchuk accelerometer calibration		*/
	struct joystick_t js;			/**< joystick calibration					*/

	int* flags;						/**< options flag (points to wiimote_t.flags) */

	byte btns;						/**< what buttons have just been pressed	*/
	byte btns_held;					/**< what buttons are being held down		*/
	byte btns_released;				/**< what buttons were just released this	*/

	struct vec3b_t accel;			/**< current raw acceleration data			*/
	struct orient_t orient;			/**< current orientation on each axis		*/
	struct gforce_t gforce;			/**< current gravity forces on each axis	*/
} nunchuk_t;


/**
 *	@struct classic_ctrl_t
 *	@brief Classic controller expansion device.
 */
typedef struct classic_ctrl_t {
	short btns;						/**< what buttons have just been pressed	*/
	short btns_held;				/**< what buttons are being held down		*/
	short btns_released;			/**< what buttons were just released this	*/

	float r_shoulder;				/**< right shoulder button (range 0-1)		*/
	float l_shoulder;				/**< left shoulder button (range 0-1)		*/

	struct joystick_t ljs;			/**< left joystick calibration				*/
	struct joystick_t rjs;			/**< right joystick calibration				*/
} classic_ctrl_t;


/**
 *	@struct expansion_t
 *	@brief Generic expansion device plugged into wiimote.
 */
typedef struct expansion_t {
	enum {
		EXP_NONE,
		EXP_NUNCHUK,
		EXP_CLASSIC
	} type;							/**< type of expansion attached				*/

	union {
		struct nunchuk_t nunchuk;
		struct classic_ctrl_t classic;
	};
} expansion_t;


/**
 *	@enum win32_bt_stack_t
 *	@brief	Available bluetooth stacks for Windows.
 */
typedef enum win_bt_stack_t {
	WIIUSE_STACK_UNKNOWN,
	WIIUSE_STACK_MS,
	WIIUSE_STACK_BLUESOLEIL
} win_bt_stack_t;


typedef struct wiimote_state_t {
	/* expansion_t */
	float exp_ljs_ang;
	float exp_rjs_ang;
	float exp_ljs_mag;
	float exp_rjs_mag;
	byte exp_btns;
	struct orient_t exp_orient;
	float exp_r_shoulder;
	float exp_l_shoulder;

	/* ir_t */
	int ir_ax;
	int ir_ay;
	float ir_distance;

	struct orient_t orient;
	unsigned short btns;
} wiimote_state_t;


/**
 *	@struct wiimote_t
 *	@brief Wiimote structure.
 */
typedef struct wiimote_t {
	WCONST int unid;						/**< user specified id						*/

	#ifndef WIN32
		WCONST bdaddr_t bdaddr;				/**< bt address								*/
		WCONST char bdaddr_str[18];			/**< readable bt address					*/
		WCONST int out_sock;				/**< output socket							*/
		WCONST int in_sock;					/**< input socket 							*/
	#else
		WCONST HANDLE dev_handle;			/**< HID handle								*/
		WCONST OVERLAPPED hid_overlap;		/**< overlap handle							*/
		WCONST enum win_bt_stack_t stack;	/**< type of bluetooth stack to use			*/
	#endif

	WCONST int state;						/**< various state flags					*/
	WCONST int leds;						/**< currently lit leds						*/

	WCONST int flags;						/**< options flag							*/

	WCONST wiiuse_event_cb event_cb;		/**< event callback							*/
	WCONST wiiuse_dis_cb dis_cb;			/**< disconnect callback					*/
	WCONST wiiuse_ctrl_status_cb stat_cb;	/**< controller status callback				*/

	WCONST byte handshake_state;			/**< the state of the connection handshake	*/

	WCONST struct read_req_t* read_req;		/**< list of data read requests				*/
	WCONST struct accel_t accel_calib;		/**< wiimote accelerometer calibration		*/
	WCONST struct expansion_t exp;			/**< wiimote expansion device				*/

	WCONST struct vec3b_t accel;			/**< current raw acceleration data			*/
	WCONST struct orient_t orient;			/**< current orientation on each axis		*/
	WCONST struct gforce_t gforce;			/**< current gravity forces on each axis	*/

	WCONST struct ir_t ir;					/**< IR data								*/

	WCONST unsigned short btns;				/**< what buttons have just been pressed	*/
	WCONST unsigned short btns_held;		/**< what buttons are being held down		*/
	WCONST unsigned short btns_released;	/**< what buttons were just released this	*/

	WCONST float orient_threshold;			/**< threshold for orient to generate an event */

	WCONST struct wiimote_state_t lstate;	/**< last saved state						*/

	WCONST byte event[MAX_PAYLOAD];			/**< event buffer							*/
} wiimote;


/*****************************************
 *
 *	Include API specific stuff
 *
 *****************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* wiiuse.c */
typedef struct wiimote_t** (*_wiiuse_init_fptr)(int wiimotes, int* unids, wiiuse_event_cb event_cb, wiiuse_ctrl_status_cb stat_cb, wiiuse_dis_cb dis_cb);
typedef void (*_wiiuse_disconnected_fptr)(struct wiimote_t* wm);
typedef void (*_wiiuse_rumble_fptr)(struct wiimote_t* wm, int status);
typedef void (*_wiiuse_toggle_rumble_fptr)(struct wiimote_t* wm);
typedef void (*_wiiuse_set_leds_fptr)(struct wiimote_t* wm, int leds);
typedef void (*_wiiuse_motion_sensing_fptr)(struct wiimote_t* wm, int status);
typedef int (*_wiiuse_read_data_fptr)(struct wiimote_t* wm, wiiuse_read_cb read_cb, byte* buffer, unsigned int offset, unsigned short len);
typedef int (*_wiiuse_write_data_fptr)(struct wiimote_t* wm, unsigned int addr, byte* data, byte len);
typedef void (*_wiiuse_status_fptr)(struct wiimote_t* wm);
typedef struct wiimote_t* (*_wiiuse_get_by_id_fptr)(struct wiimote_t** wm, int wiimotes, int unid);
typedef int (*_wiiuse_set_flags_fptr)(struct wiimote_t* wm, int enable, int disable);
typedef float (*_wiiuse_set_smooth_alpha_fptr)(struct wiimote_t* wm, float alpha);
typedef void (*_wiiuse_set_ir_fptr)(struct wiimote_t* wm, int status);
typedef void (*_wiiuse_set_ir_vres_fptr)(struct wiimote_t* wm, unsigned int x, unsigned int y);
typedef void (*_wiiuse_set_ir_position_fptr)(struct wiimote_t* wm, enum ir_position_t pos);
typedef void (*_wiiuse_set_aspect_ratio_fptr)(struct wiimote_t* wm, enum aspect_t aspect);
typedef void (*_wiiuse_set_bluetooth_stack_fptr)(struct wiimote_t** wm, int wiimotes, enum win_bt_stack_t type);
typedef void (*_wiiuse_set_orient_threshold_fptr)(struct wiimote_t* wm, float threshold);

/* connect.c */
typedef int (*_wiiuse_find_fptr)(struct wiimote_t** wm, int max_wiimotes, int timeout);
typedef int (*_wiiuse_connect_fptr)(struct wiimote_t** wm, int wiimotes);
typedef void (*_wiiuse_disconnect_fptr)(struct wiimote_t* wm);

/* events.c */
typedef void (*_wiiuse_poll_fptr)(struct wiimote_t** wm, int wiimotes);

#ifdef __cplusplus
}
#endif

/**
 *	@struct wiiuse_api_t
 *	@brief API structure that is filled by the library
 *			when the entry point is invoked.
 *
 *	Unless you are modifying wiiuse, you probably
 *	will not be interested in any members
 *	that begin with an underscore (_).
 */
struct wiiuse_api_t {
	const char* version;				/**< wiiuse version		*/
	int api_version;					/**< wiiuse API version	*/

	_wiiuse_init_fptr					_wiiuse_init;
	_wiiuse_disconnected_fptr			_wiiuse_disconnected;
	_wiiuse_rumble_fptr					_wiiuse_rumble;
	_wiiuse_toggle_rumble_fptr			_wiiuse_toggle_rumble;
	_wiiuse_set_leds_fptr				_wiiuse_set_leds;
	_wiiuse_motion_sensing_fptr			_wiiuse_motion_sensing;
	_wiiuse_read_data_fptr				_wiiuse_read_data;
	_wiiuse_write_data_fptr				_wiiuse_write_data;
	_wiiuse_status_fptr					_wiiuse_status;
	_wiiuse_get_by_id_fptr				_wiiuse_get_by_id;
	_wiiuse_set_flags_fptr				_wiiuse_set_flags;
	_wiiuse_set_smooth_alpha_fptr		_wiiuse_set_smooth_alpha;
	_wiiuse_set_ir_fptr					_wiiuse_set_ir;
	_wiiuse_set_ir_vres_fptr			_wiiuse_set_ir_vres;
	_wiiuse_set_ir_position_fptr		_wiiuse_set_ir_position;
	_wiiuse_set_aspect_ratio_fptr		_wiiuse_set_aspect_ratio;
	_wiiuse_set_bluetooth_stack_fptr	_wiiuse_set_bluetooth_stack;
	_wiiuse_set_orient_threshold_fptr	_wiiuse_set_orient_threshold;

	_wiiuse_find_fptr					_wiiuse_find;
	_wiiuse_connect_fptr				_wiiuse_connect;
	_wiiuse_disconnect_fptr				_wiiuse_disconnect;

	_wiiuse_poll_fptr					_wiiuse_poll;
};

#ifndef __WIIUSE__

/*
 *	Operating system dependent macros.
 */
#ifdef __WIN32__
	#define dlopen(file, x)         (void*)LoadLibrary(file)
	#define dlsym(dll, func)        (void*)GetProcAddress((HMODULE)(dll), (func))
	#define dlclose(dll)            FreeLibrary((HMODULE)(dll))

	char* _dlerror();
	#define dlerror()				_dlerror()
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* api/wiiuse.c */
const char* wiiuse_startup(char* wiiuse_file);
void wiiuse_shutdown();

#ifdef WIIUSE_API_SRC
	#define WEXTERN
#else
	#define WEXTERN		extern
#endif

WEXTERN _wiiuse_init_fptr					wiiuse_init;
WEXTERN _wiiuse_disconnected_fptr			wiiuse_disconnected;
WEXTERN _wiiuse_rumble_fptr					wiiuse_rumble;
WEXTERN _wiiuse_toggle_rumble_fptr			wiiuse_toggle_rumble;
WEXTERN _wiiuse_set_leds_fptr				wiiuse_set_leds;
WEXTERN _wiiuse_motion_sensing_fptr			wiiuse_motion_sensing;
WEXTERN _wiiuse_read_data_fptr				wiiuse_read_data;
WEXTERN _wiiuse_write_data_fptr				wiiuse_write_data;
WEXTERN _wiiuse_status_fptr					wiiuse_status;
WEXTERN _wiiuse_get_by_id_fptr				wiiuse_get_by_id;
WEXTERN _wiiuse_set_flags_fptr				wiiuse_set_flags;
WEXTERN _wiiuse_set_smooth_alpha_fptr		wiiuse_set_smooth_alpha;
WEXTERN _wiiuse_set_ir_fptr					wiiuse_set_ir;
WEXTERN _wiiuse_set_ir_vres_fptr			wiiuse_set_ir_vres;
WEXTERN _wiiuse_set_ir_position_fptr		wiiuse_set_ir_position;
WEXTERN _wiiuse_set_aspect_ratio_fptr		wiiuse_set_aspect_ratio;
WEXTERN _wiiuse_set_bluetooth_stack_fptr	wiiuse_set_bluetooth_stack;
WEXTERN _wiiuse_set_orient_threshold_fptr	wiiuse_set_orient_threshold;

WEXTERN _wiiuse_find_fptr					wiiuse_find;
WEXTERN _wiiuse_connect_fptr				wiiuse_connect;
WEXTERN _wiiuse_disconnect_fptr				wiiuse_disconnect;

WEXTERN _wiiuse_poll_fptr					wiiuse_poll;

#ifdef __cplusplus
}
#endif

#endif /* __WIIUSE__ */

#endif /* WIIUSE_H_INCLUDED */

