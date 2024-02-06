#ifndef _HID_LINUX_H
#define _HID_LINUX_H


/*
 * these are exported so they can be used in the implementation of [linuxhid]
 */
void hid_print(t_hid* x);
t_int hid_open_device(t_hid *x, t_symbol *device_name);
t_int hid_close_device(t_hid *x);
t_int hid_build_device_list(t_hid *x);

//MWS
t_int hid_grab_device(t_hid *x);
t_int hid_ungrab_device(t_hid *x);

#endif /* ! _HID_LINUX_H */
