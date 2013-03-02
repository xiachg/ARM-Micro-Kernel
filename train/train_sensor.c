#include <services.h>
#include <klib.h>
#include <unistd.h>
#include <train.h>

#define SENSOR_AUTO_RESET 192
#define SENSOR_READ_ONE 192
#define SENSOR_READ_MULTI 128

void trainSensorNotifier() {
	int i;
	int parent_tid = MyParentTid();
	int sensor_next = 0;
	int data_changed = FALSE;
	char query = SENSOR_READ_MULTI + SENSOR_DECODER_TOTAL;

	SensorMsg msg;
	msg.type = SENSOR_DATA;
	for(i = 0; i < SENSOR_BYTES_TOTAL; i++) {
		msg.sensor_data[i] = 0;
	}

	Putc(COM1, SENSOR_AUTO_RESET);
	while(1) {
		if(sensor_next == 0) {
			if(data_changed) {
				data_changed = FALSE;
				// Deliver sensor data to the parent
				Send(parent_tid, (char *)(&msg), SENSOR_BYTES_TOTAL, NULL, 0);
			}
			// Request sensor data again
			Putc(COM1, query);
		}

		// Get and save new data
		char new_data = Getc(COM1);
		if(msg.sensor_data[sensor_next] != new_data) {
			msg.sensor_data[sensor_next] = new_data;
			data_changed = TRUE;
		}

		// Increment the counter
		sensor_next = (sensor_next + 1) % SENSOR_BYTES_TOTAL;
	}
}
