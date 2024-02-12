#ifndef _HID_H
#define _HID_H

#include <stdio.h>

#include <m_pd.h>

/* 
 * this is automatically generated from linux/input.h by
 * make-arrays-from-input.h.pl to be the cross-platform event types and codes 
 */
#include "input_arrays.h"

#define HID_MAJOR_VERSION 0
#define HID_MINOR_VERSION 6

static char *version = "$Revision: 1.17 $";

/*------------------------------------------------------------------------------
 *  CLASS DEF
 */
static t_class *hid_grabber_class;

typedef struct _hid_grabber 
{
		t_object            x_obj;
		t_int               x_fd;
		t_symbol            *x_device_name;
		t_int               x_has_ff;
		void                *x_ff_device;
		t_clock             *x_clock;
		t_int               x_delay;
		t_int               x_started;
		t_int               x_device_open;
		t_outlet            *x_data_outlet;
		t_outlet            *x_device_name_outlet;
        t_outlet            *x_null_outlet;
} t_hid_grabber;


/*------------------------------------------------------------------------------
 * GLOBAL DEFINES
 */

#define DEFAULT_DELAY 5


/*------------------------------------------------------------------------------
 *  GLOBAL VARIABLES
 */

/*
 * count the number of instances of this object so that certain free()
 * functions can be called only after the final instance is detroyed.
 */
t_int hid_grabber_instance_count;


/*------------------------------------------------------------------------------
 *  FUNCTION PROTOTYPES FOR DIFFERENT PLATFORMS
 */

/* support functions */
void hid_grabber_output_event( t_hid_grabber *x, char *type, char *code, t_float value );

/* generic, cross-platform functions implemented in a separate file for each
 * platform 
 */
t_int hid_grabber_open_device( t_hid_grabber *x, t_symbol *device_name );
t_int hid_grabber_close_device( t_hid_grabber *x );
t_int hid_grabber_build_device_list( t_hid_grabber* x );
t_int hid_grabber_get_events( t_hid_grabber *x ) ;
void hid_grabber_print( t_hid_grabber* x );
void hid_grabber_platform_specific_free( t_hid_grabber *x );

/* cross-platform force feedback functions */
t_int hid_grabber_ff_autocenter( t_hid_grabber *x, t_float value );
t_int hid_grabber_ff_gain( t_hid_grabber *x, t_float value );
t_int hid_grabber_ff_motors( t_hid_grabber *x, t_float value );
t_int hid_grabber_ff_continue( t_hid_grabber *x );
t_int hid_grabber_ff_pause( t_hid_grabber *x );
t_int hid_grabber_ff_reset( t_hid_grabber *x );
t_int hid_grabber_ff_stopall( t_hid_grabber *x );

// these are just for testing...
t_int hid_grabber_ff_fftest ( t_hid_grabber *x, t_float value);
void hid_grabber_ff_print( t_hid_grabber *x );


//MWS
t_int hid_grabber_grab_device(t_hid_grabber *x);
t_int hid_grabber_ungrab_device(t_hid_grabber *x);


#endif  /* #ifndef _HID_H */
