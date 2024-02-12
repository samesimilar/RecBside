#ifndef _HID_LINUX_H
#define _HID_LINUX_H


/*
 * these are exported so they can be used in the implementation of [linuxhid]
 */
void hid_grabber_print(t_hid_grabber* x);
t_int hid_grabber_open_device(t_hid_grabber *x, t_symbol *device_name);
t_int hid_grabber_close_device(t_hid_grabber *x);
t_int hid_grabber_build_device_list(t_hid_grabber *x);

//MWS
t_int hid_grabber_grab_device(t_hid_grabber *x);
t_int hid_grabber_ungrab_device(t_hid_grabber *x);

#endif /* ! _HID_LINUX_H */
