#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	unsigned char rec_mess[65];
	unsigned char message[25];
	short accelsx[100], accelsy[100], accelsz[100];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0x3f, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr);

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	//buf[1] = 0x80;
	buf[1] = 0x1;
	printf("Enter the row you'd like to print the message: ");
	scanf("%d",&buf[2]);
	printf("Enter a message: ");	
	scanf("%s",&buf[3]);
	res = hid_write(handle, buf, 65);

	// Request state (cmd 0x81). The first byte is the report number (0x0).
	//buf[0] = 0x0;
	//buf[1] = 0x81;
	//res = hid_write(handle, buf, 65);

	res = hid_read(handle,buf,65);
	
	if(buf[0] == 0x1 && buf[1] == 0x3){
		buf[0] = 0x0;
		buf[1] = 0x2;
		res = hid_write(handle,buf,65);
		printf("Accelerometer reading!\n");
	}
	
	
	// Print out the returned buffer.
	i = 0;
	
	while(i<100){
		// Read requested state
		res = hid_read(handle, buf, 65);
		
		if (buf[0]==0x1 && buf[1]==0x2){
			accelsx[i] = (buf[2]<<8) | buf[3];
//			accelsy[i] = (buf[4]<<8) | buf[5];
//			accelsz[i] = (buf[6]<<8) | buf[7];
			
			buf[0] = 0x0;
			buf[1] = 0x2;
			res = hid_write(handle,buf,65);
			i++;
		}
	//printf("buf[%d]: %d\n", i, buf[i]);
	}
	// Finalize the hidapi library
	
	printf("Accelerometer reading has completed.\n");
	buf[0] = 0x0;
	buf[1] = 0x80;
	res = hid_write(handle,buf,65);
	
	FILE *ofp;
	
	ofp = fopen("accels.txt", "w");

	for (i = 0; i< 100; i++) {

		//fprintf(ofp, "%d %d %d\n", accelsx[i], accelsy[i], accelsz[i]);
		fprintf(ofp, "%d\n", accelsz[i]);
	}

	fclose(ofp);

	res = hid_exit();

	return 0;
}