#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

// Function to parse accelerometer data and print it
void parseAccelerometerData(uint8_t *data) {
    // Extract accelerometer data based on the provided format
    int16_t x_axis = le16toh(*(int16_t *)&data[14]);
    int16_t y_axis = le16toh(*(int16_t *)&data[16]);
    int16_t z_axis = le16toh(*(int16_t *)&data[18]);

    // Print accelerometer data
    printf("X-axis: %f g, Y-axis: %f g, Z-axis: %f g\n",
           x_axis / 256.0, y_axis / 256.0, z_axis / 256.0);
}

// Function to check movement status based on accelerometer data
void checkMovementStatus(uint8_t *data) {
    // Check movement status based on accelerometer data
    int16_t x_axis = le16toh(*(int16_t *)&data[14]);
    int16_t y_axis = le16toh(*(int16_t *)&data[16]);
    int16_t z_axis = le16toh(*(int16_t *)&data[18]);

    // Determine movement status based on non-zero accelerometer values
    if (x_axis != 0 || y_axis != 0 || z_axis != 0) {
        printf("Movement Status: Moving\n");
    } else {
        printf("Movement Status: Stationary\n");
    }
}

int main() {
    // Get the Bluetooth device ID and open the device
    int device_id = hci_get_route(NULL);
    int device_handle = hci_open_dev(device_id);

    // Check if the device was opened successfully
    if (device_id < 0 || device_handle < 0) {
        perror("Error opening HCI device.");
        exit(1);
    }

    // Save the old socket options to restore them later
    struct hci_filter old_options;
    socklen_t olen = sizeof(old_options);

    if (getsockopt(device_handle, SOL_HCI, HCI_FILTER, &old_options, &olen) < 0) {
        perror("Error getting socket options");
        exit(1);
    }

    // Set the new socket options for receiving only HCI_EVENT_PKT packets
    struct hci_filter new_options;
    hci_filter_clear(&new_options);
    hci_filter_set_ptype(HCI_EVENT_PKT, &new_options);

    if (setsockopt(device_handle, SOL_HCI, HCI_FILTER, &new_options, sizeof(new_options)) < 0) {
        perror("Error setting socket options");
        exit(1);
    }

    // Continuously read and process Bluetooth events
    while (1) {
        unsigned char buf[HCI_MAX_EVENT_SIZE];
        ssize_t len = read(device_handle, buf, sizeof(buf));

        if (len > 0) {
            uint8_t *ptr = buf + (1 + HCI_EVENT_HDR_SIZE);

            while (ptr < buf + len) {
                uint8_t event_type = *ptr;

                // Check for LE Meta Event
                if (event_type == EVT_LE_META_EVENT) {
                    ptr += 3;
                    uint8_t subevent = *ptr;

                    // Check for LE Advertising Report
                    if (subevent == EVT_LE_ADVERTISING_REPORT) {
                        ptr++;
                        le_advertising_info *info = (le_advertising_info *)ptr;

                        char mac[18];
                        ba2str(&info->bdaddr, mac);
                        int8_t rssi = (int8_t)info->data[info->length];

                        printf("Mac: %s, RSSI: %d, ", mac, rssi);

                        // Check if it's an Accelerometer Beacon
                        if (info->length == 21 && info->data[1] == 0x06) {
                            parseAccelerometerData(info->data);
                            checkMovementStatus(info->data);
                        }

                        printf("\n");
                    }
                }

                // Move to the next event in the buffer
                ptr += 2 + ptr[1];
            }
        }
    }

    // Close the Bluetooth device handle
    close(device_handle);

    return 0;
}




