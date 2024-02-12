# RecBside v0.1
## A Recording mod for Organelle
### By Michael Spears <mws@samesimilar.com>
### The author graciously acknowledges the contributions of the authors of pdbuilder, HID, readdir, mother.pd, PureData, and more, which make RecBside possible.
### Please see LICENSE.txt for Copyright, Warranty, Software Licenses, and further acknowledgments.


## Concept
RecBside is designed to be a lightweight "drop-in" modification of mother.pd that adds a convenient recording function to the Organelle. When RecBside recording is activated, RecBside records the Organelle's output: "what you hear" from the Organelle is recorded directly to disk. 

This is a *very early* iteration of this idea, so **please use with caution**. Back up your work, make sure you have enough free space for your recordings, and save often. 

RecBside does not add any operations to the front panel or menu system of the Organelle itself. It is designed to be activated with an attached USB keyboard. You press "spacebar" to start / stop recording in a patch.
 
(Note we are talking about a computer keyboard of the USB, "QWERTY" type, not a USB MIDI keyboard of the musical type.)

The twist here is that RecBside uses the [HID] external API to "capture" the USB keyboard of your choice, so that it is reserved entirely for the purpose of controlling your patch. 

Audio is recorded to stereo files in the `/sdcard/recbside` folder, with sequential names like `rec-1.wav`, `rec-2.wav`, etc.

The maximum length of any RecBside recording is limited (by default) to 4 minutes + 20 seconds. Recording should automatically be stopped. This is to help avoid accidentally filling up your disk with audio. *Available space is not checked.* A future version of RecBside may hopefully implement a way to dynamically check for available space. But for now the assumption is that you have a reasonable amount of free space on your Organelle's SD card. **Again: RecBside is still under testing and development, so please back up any important patches and data from your SD card before using RecBside.**

## Requirements

1. An Organelle M/S. I do not have an original Organelle, so I cannot determine if this would work on it.
2. Enough free space on your Organelle's SD card (`/sdcard`). RecBside should conceivably be able to record to USB also, but I have not tested it yet. For now it is hardcoded to record audio to `/sdcard/recbside`
3. A USB keyboard to plug into the Organelle. NOTE: You cannot use a single keyboard with RecBside and also to control the Organelle's terminal or Desktop (GUI) at the same time. This is due to how RecBside reserves ("grabs") control of the selected USB input device.
4. Patches on the Organelle that you want to modify to add recording. It's recommended to make backup copies of any patches before modifying them.

## Setup Instructions
1. Create the following folder on your Organelle: `/sdcard/recbside`. This folder is used to store your recordings. A shared configuration file that indicates what USB HID device it should listen to as a controller
is stored in /sdcard/HIDSelector-Config.txt.

2. Copy the RecBside folder to your Organelle's Patches folder. 
	
	a. `RecBside/HIDSelectorSetup` is a utility patch you run once or twice to set up RecBside. (It's just a normal Organelle patch.)
	
	b. `RecBside/RecBside-integrate` contains the modified mother.pd and support files that should be copied into other patches that you want to record. 
	
3. Attach the USB keyboard you wish to use to your Organelle.
4. Use the Organelle's patch menu to load HIDSelectorSetup.
5. The HIDSelectorSetup patch will display a list of detected USB devices. If you see your USB keyboard, use the rotary encoder to scroll down to select it. Then press the encoder to enter 'test mode'. 
6. Press some keys on your USB keyboard device. If you see key messages printed on the Organelle's display, you have selected the correct device. If not, go back and try a different device in the list. (My 2.4Ghz USB keyboard receiver, for example, has four 'devices' listed, however only one of those works with the associated wireless keyboard.)
7. If you found the correct device, select 'Save' on the menu. When the LED flashes on the Organelle, your setting is saved. You can exit this patch and select anything else.

### Integration with Existing Patches

8. RecBside has a few files that need to be dropped into any patch that you want to enable for recording:

	a. Copy the `mother.pd` file and the entire `rb-recorder-ext` folder from `RecBside-integrate` together into any patch that you want to use with RecBside. 
	
	b. **Important** In rare circumstances, some patches may already have a customized `mother.pd`. Copying the RecBside version of mother.pd so that it overwrites another version could break the original patch.
	
Once those files have been copied, then RecBside will be available next time you start the desired patch. Note: RecBside is not 'global' to the Organelle. It's only available in the patches you add the above files to.

**Alternatively**: you can copy the contents of `RecBside-integrate` (mother.pd + the rb-recorder-ext folder) into `/sdcard/System`. This will load the modified mother.pd patch and RecBside for any patch you load on your Organelle, unless a local mother.pd is found in the patch folder. Just remember that you have made this addition in case your system acts strangely.

## Persistence of Configuration and Organelle "Desktop Mode"

When you select a device in HIDSelectorSetup, the USB device ID is stored in a .txt file: `/sdcard/HIDSelector-Config.txt` folder. This will persist between re-boots. You don't need to select it again unless you want to use a different device.

Note: If you are using the Organelle in "Desktop" mode with an HDMI cable and USB keyboard / mouse attached, then RecBside can 'grab' a device that you have selected, and the device won't operate the terminal / mouse / desktop cursor. This is by design - you wouldn't want random stuff to happen on your screen while operating RecBside. You can get access to your devices back by selecting a different patch on the Organelle that doesn't have RecBside integrated, or you can open the HIDSelectorSetup patch and select+save the "none" option.

I mainly use the Organelle Desktop over VPN, which is not affected by RecBside grabbing a hardware keyboard attached to the Organelle.

## Operation

If RecBside is installed in a patch, press 'spacebar' on the USB keyboard it was configured to work with (see HIDSelectorSetup, above) to operate RecBside. Spacebar will toggle recording on/off. Recordings are stored in sequential files in `/sdcard/recbside`. 

You should also see a flashing recording indicator (during recording) in the top-right of the Organelle's OLED screen. This is not guaranteed to work well with every patch, though, since that screen space is not reserved.



