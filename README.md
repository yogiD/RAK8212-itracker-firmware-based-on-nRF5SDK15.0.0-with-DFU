# RAK8212-itracker-firmware-based-on-nRF5SDK15.0.0-with-DFU
RAK8212 itracker firmware based on nRF5 SDK15.0.0 with DFU function

program the firmware of RAK8212
RAKWireless provides a whole firmware which is an example for RAK8212 module on their Github page: 
https://github.com/RAKWireless/RAK8212-itracker-firmware-based-on-nRF5SDK15.0.0-with-DFU
This firmware is based on the newest nRF5 SDK 15.0.0 and Softdevice S132 v6.0.0. It includes a Softdevice S132 V6.0.0, a bootloader supported OTA-DFU, and an application for RAK8212 module. 
You can use this firmware directly on RAK8212 module to send “hello world” to Hologram online service via GPRS. You can also send any data from RAK8212 module to anyone via NB-IoT or Cat M easily by developing your own firmware according to the source code of the example firmware provided by RAKWireless on their Github page. 
Before our starting, we should install nRFgo Studio on our PC. It is a programing tool provided by Nordic. Surely, you can use any other programing tools too.
https://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRFgo-Studio/(language)/eng-GB
Program the firmware provided by RAKWireless to RAK8212 module directly
Firstly, let’s download the firmware from RAKWireless page( https://github.com/RAKWireless/RAK8212-itracker-firmware-based-on-nRF5SDK15.0.0-with-DFU), and find out the firmware file “RAK8212_ALL.hex”, which is in the folder “Firmware hex for SWD” and will be used to program into RAK8212 module soon.
Secondly, let’s select a programing method. There are two methods you can select: SWD interface and OTA-DFU.
SWD interface
If there is no firmware on your RAK8212 module, you must select this method for programing. You can also select this method surely even if there is already a firmware on your RAK8212 module.
 
According to the picture above, we connect RAK8212 module with PC via ARM Emulator, and open nRFgo Studio on our PC. Then we select “nRF5x programing” in the Device Manager panel, and the following UI will be shown in front of us:
 
We can click the “Erase all” button to erase the firmware which had been programed into our RAK8212 module before, and it doesn’t matter that we start to program without erasing the old firmware.
Now, let’s program the firmware:
Click the “Browse” button and select the firmware file which is a .hex file;
 
Click the “program” button to start;
 


OTA-DFU
RAKWireless provides a bootloader for RAK8212 module which is included in the firmware provided by RAKWireless on their Github page: https://github.com/RAKWireless/RAK8212-itracker-firmware-based-on-nRF5SDK15.0.0-with-DFU, and this bootloader provides the DFU function. We can program the firmware for RAK8212 module via BLE OTA with the nRF Toolbox app (Android: 1.17 or later, iOS: 4 or later) or the nRF Connect app (4.4 or later, Android only at this time) when it works in the DFU mode. 
Note: Only if the DFU button of the RAK8212 module is pressed during the module startup, it will work in the DFU mode.
To protect the target device against malicious attackers trying to impersonate the rightful sender of the firmware update, the init packet of the firmware package must be signed.
You can either create your own firmware package or use the testing package “RAK8212.zip” provided by RAKWireless in the folder “ Firmware Zip for OTA-DFU”. The provided package is an application of RAK8212, and it has been generated using a private key that corresponds to the default public key in the provided firmware. This public key must not be used in production.
You can program the firmware via OTA-DFU according the following steps:
a.	Transfer the zip firmware package that you want to upload to the mobile device:
	On iOS: Connect the mobile device to your computer and open iTunes. Select the mobile device and then Apps > nRF Toolbox (or nRF Connect). Then you can drag and drop a file directly into the application's internal storage. Alternatively, send an email attachment to the app from the mobile device itself. See the app's documentation for more information.
	On Android: Connect the mobile device to your computer. Enable file transfer between the computer and the device and copy the zip file into a folder of your choice.

b.	Use the nRF Toolbox or nRF Connect app to select a target device and a zip package and to start the process.

Observe that the device resets and runs the new application, bootloader, or SoftDevice.
