/* --------------------------------------------------------------------------*/
/*                                                                           */
/* MacOS X object to use HIDs (Human Interface Devices)                      */
/* Written by Hans-Christoph Steiner <hans@at.or.at>                         */
/*                                                                           */
/* Copyright (c) 2004 Hans-Christoph Steiner                                 */
/*                                                                           */
/* This program is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU General Public License               */
/* as published by the Free Software Foundation; either version 2            */
/* of the License, or (at your option) any later version.                    */
/*                                                                           */
/* See file LICENSE for further informations on licensing terms.             */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software Foundation,   */
/* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           */
/*                                                                           */
/* --------------------------------------------------------------------------*/

/* 
	Note - this source has modifications made specifically for the Organelle
	Synth. - Michael Spears, Feb. 2024
*/

#include "hid.h"

/*------------------------------------------------------------------------------
 * LOCAL DEFINES
 */

#define DEBUG(x)
//#define DEBUG(x) x 

/*------------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 */

void hid_grabber_start(t_hid_grabber *x, t_float f);
void hid_grabber_stop(t_hid_grabber *x);
t_int hid_grabber_open(t_hid_grabber *x, t_symbol *s);
t_int hid_grabber_close(t_hid_grabber *x);
t_int hid_grabber_read(t_hid_grabber *x,int fd);
static void hid_grabber_float(t_hid_grabber* x, t_floatarg f);
// MWS
t_int hid_grabber_grab(t_hid_grabber *x);
t_int hid_grabber_ungrab(t_hid_grabber *x);

/*------------------------------------------------------------------------------
 * SUPPORT FUNCTIONS
 */

void hid_grabber_output_event(t_hid_grabber *x, char *type, char *code, t_float value)
{
	t_atom event_data[3];
	
	SETSYMBOL(event_data, gensym(type));	   /* type */
	SETSYMBOL(event_data + 1, gensym(code));	/* code */
	SETFLOAT(event_data + 2, value);	         /* value */

	outlet_anything(x->x_data_outlet,atom_gensym(event_data),2,event_data+1);
}

void hid_grabber_set_from_float(t_hid_grabber *x, t_floatarg f)
{
/* values greater than 1 set the polling delay time */
/* 1 and 0 for start/stop so you can use a [tgl] */
	if (f > 1)
	{
		x->x_delay = (t_int)f;
		hid_grabber_start(x,f);
	}
	else if (f == 1) 
	{
		if (! x->x_started)
		hid_grabber_start(x,f);
	}
	else if (f == 0) 		
	{
		hid_grabber_stop(x);
	}
}

/*------------------------------------------------------------------------------
 * IMPLEMENTATION                    
 */

/* stop polling the device */
void hid_grabber_stop(t_hid_grabber* x) 
{
  DEBUG(post("hid_grabber_stop"););
  
  if (x->x_started) 
  { 
	  clock_unset(x->x_clock);
	  DEBUG(post("[hid_grabber] polling stopped"););
	  x->x_started = 0;
  }
}

/* close the device */
t_int hid_grabber_close(t_hid_grabber *x) 
{
	DEBUG(post("hid_grabber_close"););

/* just to be safe, stop it first */
	hid_grabber_stop(x);	
	//MWS
	if (!x->x_device_open) {
		return(0);
	}
	
	hid_grabber_ungrab(x);
	if(! hid_grabber_close_device(x)) 
	{
		post("[hid_grabber] closed device %s",x->x_device_name->s_name);
		x->x_device_open = 0;
		return (0);
	}

	return (1);
}

// MWS
t_int hid_grabber_grab(t_hid_grabber *x)
{
	if (hid_grabber_grab_device(x)) {
		error("[hid_grabber] couldn't grab device");
		outlet_bang(x->x_null_outlet);
		return 1;
	} else {
		post("[hid_grabber] grabbed device");
		return 0;
	}
	
}
// MWS
t_int hid_grabber_ungrab(t_hid_grabber *x)
{
	if(hid_grabber_ungrab_device(x)) {
		error("[hid_grabber] couldn't ungrab device");
		outlet_bang(x->x_null_outlet);
		return 1;
	} else {
		post("[hid] ungrabbed device");
		return 0;
	}
}


/* hid_open behavoir
 * current state                 action
 * ---------------------------------------
 * closed / same device          open 
 * open / same device            no action 
 * closed / different device     open 
 * open / different device       close open 
 */

t_int hid_grabber_open(t_hid_grabber *x, t_symbol *s) 
{
	DEBUG(post("hid_grabber_open"););

/* store running state to be restored after the device has been opened */
	t_int started = x->x_started;

/* only close the device if its different than the current and open */	
	if ( (s != x->x_device_name) && (x->x_device_open) ) 
		hid_grabber_close(x);

	// if (f > 0)
	// 	x->x_device_number = f;
	// else
	// 	x->x_device_number = 0;
	x->x_device_name = s;

/* if device is open still, that means the same device is trying to be opened,
 * therefore ignore the redundant open request.  To reopen the same device,
 * send a [close( msg, then an [open( msg. */
	if (! x->x_device_open) 
	{
		if (hid_grabber_open_device(x,x->x_device_name))
		{
			error("[hid_grabber] can not open device %s",x->x_device_name->s_name);
			outlet_bang(x->x_null_outlet);
			return (1);
		}
		else
		{
			x->x_device_open = 1;
		}
	}
	

/* restore the polling state so that when I [tgl] is used to start/stop [hid],
 * the [tgl]'s state will continue to accurately reflect [hid]'s state  */
	if(started)
		hid_grabber_set_from_float(x,x->x_delay);

	return (0);
}


t_int hid_grabber_read(t_hid_grabber *x,int fd) 
{
//	DEBUG(post("hid_read"););

	hid_grabber_get_events(x);
	
	if (x->x_started) 
	{
		clock_delay(x->x_clock, x->x_delay);
	}
	
	// TODO: why is this 1? 
	return 1; 
}

void hid_grabber_start(t_hid_grabber* x, t_float f) 
{
	DEBUG(post("hid_grabber_start"););
  
/*	if the user sets the delay less than one, ignore */
	if( f >= 1 ) 	
		x->x_delay = (t_int)f;

	if(!x->x_device_open)
		hid_grabber_open(x,x->x_device_name);
	
   if(!x->x_started) 
	{
		clock_delay(x->x_clock, x->x_delay);
		DEBUG(post("[hid_grabber] polling started"););
		x->x_started = 1;
	} 
}

static void hid_grabber_float(t_hid_grabber* x, t_floatarg f) 
{
	DEBUG(post("hid_grabber_float"););

	hid_grabber_set_from_float(x,f);
}

/* setup functions */
static void hid_grabber_free(t_hid_grabber* x) 
{
	DEBUG(post("hid_grabber_free"););
		
	hid_grabber_close(x);
	clock_free(x->x_clock);
	hid_grabber_instance_count--;
	outlet_free(x->x_null_outlet);
	outlet_free(x->x_device_name_outlet);
	outlet_free(x->x_data_outlet);
	hid_grabber_platform_specific_free(x);
}

/* create a new instance of this class */
static void *hid_grabber_new(void) 
{
  t_hid_grabber *x = (t_hid_grabber *)pd_new(hid_grabber_class);

  DEBUG(post("hid_grabber_new"););

/* only display the version when the first instance is loaded */
  if(!hid_grabber_instance_count) {
	  post("[hid_grabber] %d.%d, written by Hans-Christoph Steiner <hans@eds.org>",
			 HID_MAJOR_VERSION, HID_MINOR_VERSION);  
	  post("With modifications for Organelle by Michael Spears <samesimilar@gmail.com");
	 }

#if !defined(__linux__) && !defined(__APPLE__)
  error("    !! WARNING !! WARNING !! WARNING !! WARNING !! WARNING !! WARNING !!");
  error("     This is a dummy, since this object only works GNU/Linux and MacOS X!");
  error("    !! WARNING !! WARNING !! WARNING !! WARNING !! WARNING !! WARNING !!");
#endif

  /* init vars */
  x->x_has_ff = 0;
  x->x_device_open = 0;
  x->x_started = 0;
  x->x_delay = DEFAULT_DELAY;
  x->x_device_name = gensym("nodevice");

  x->x_clock = clock_new(x, (t_method)hid_grabber_read);

  /* create anything outlet used for HID data */ 
  x->x_data_outlet = outlet_new(&x->x_obj, 0);
  x->x_device_name_outlet = outlet_new(&x->x_obj, 0);
  x->x_null_outlet = outlet_new(&x->x_obj, &s_bang);
  
  /* find and report the list of devices */
  hid_grabber_build_device_list(x);
  
  // Open the device and save settings.  If there is an error, return the object
  //  * anyway, so that the inlets and outlets are created, thus not breaking the
  //  * patch.  
  // if (hid_open(x,s))
	//   error("[hid] device %s did not open",s->s_name);

  hid_grabber_instance_count++;

  return (x);
}

void hid_grabber_setup(void) 
{
	DEBUG(post("hid_grabber_setup"););
	hid_grabber_class = class_new(gensym("hid_grabber"), 
								 (t_newmethod)hid_grabber_new, 
								 (t_method)hid_grabber_free,
								 sizeof(t_hid_grabber),
								 CLASS_DEFAULT, 0);
	
	/* add inlet datatype methods */
	class_addfloat(hid_grabber_class,(t_method) hid_grabber_float);
	class_addbang(hid_grabber_class,(t_method) hid_grabber_read);
	
	/* add inlet message methods */
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_build_device_list,gensym("refresh"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_print,gensym("print"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_open,gensym("open"),A_SYMBOL,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_close,gensym("close"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_start,gensym("start"),A_DEFFLOAT,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_start,gensym("poll"),A_DEFFLOAT,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_stop,gensym("stop"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_stop,gensym("nopoll"),0);
	
	// MWS
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_grab,gensym("grab"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ungrab,gensym("ungrab"),0);
	
   /* force feedback messages */
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_autocenter,
						 gensym("ff_autocenter"),A_DEFFLOAT,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_gain,gensym("ff_gain"),A_DEFFLOAT,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_motors,gensym("ff_motors"),A_DEFFLOAT,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_continue,gensym("ff_continue"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_pause,gensym("ff_pause"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_reset,gensym("ff_reset"),0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_stopall,gensym("ff_stopall"),0);
	/* ff tests */
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_fftest,gensym("fftest"),A_DEFFLOAT,0);
	class_addmethod(hid_grabber_class,(t_method) hid_grabber_ff_print,gensym("ff_print"),0);
}

