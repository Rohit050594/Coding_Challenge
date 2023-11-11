#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <endian.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

// Function to parse accelerometer data from the BLE packet
void parseAccelerometerData(uint8_t *data, int length) {
    // Implement parsing logic based on the provided format
    // This is a placeholder, replace it with the actual logic
    printf("Accelerometer Data: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// Function to parse iBeacon data from the BLE packet
void parseIBeaconData(uint8_t *data, int length) {
    // Implement parsing logic based on the provided format
    // This is a placeholder, replace it with the actual logic
    printf("iBeacon Data: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// Function to check movement status based on accelerometer data
void checkMovementStatus(uint8_t *data) {
    // Implement movement status check based on accelerometer data
    // This is a placeholder, replace it with the actual logic
    printf("Movement Status: ");
    // Example: Check the accelerometer data for movement
    int x_axis = (int8_t)data[14];
    int y_axis = (int8_t)data[16];
    int z_axis = (int8_t)data[18];
    if (x_axis != 0 || y_axis != 0 || z_axis != 0) {
        printf("Moving\n");
    } else {
        printf("Stationary\n");
    }
}

int main() {
    int device_id = hci_get_route(NULL);
    int device_handle = hci_open_dev(device_id);

    if (device_id < 0 || device_handle < 0) {
        perror("Error opening HCI device.");
        exit(1);
    }

    struct hci_filter old_options;
    socklen_t olen = sizeof(old_options);

    if (getsockopt(device_handle, SOL_HCI, HCI_FILTER, &old_options, &olen) < 0) {
        perror("Error getting socket options");
        exit(1);
    }

    struct hci_filter new_options;
    hci_filter_clear(&new_options);
    hci_filter_set_ptype(HCI_EVENT_PKT, &new_options);
    if (setsockopt(device_handle, SOL_HCI, HCI_FILTER, &new_options, sizeof(new_options)) < 0) {
        perror("Error setting socket options");
        exit(1);
    }

    while (1) {
        unsigned char buf[HCI_MAX_EVENT_SIZE];
        ssize_t len = read(device_handle, buf, sizeof(buf));

        if (len > 0) {
            uint8_t *ptr = buf + (1 + HCI_EVENT_HDR_SIZE); // Skip the HCI event header

            // Iterate through HCI events
            while (ptr < buf + len) {
                uint8_t event_type = *ptr; // Event type is the first byte of each event

                if (event_type == EVT_LE_META_EVENT) {
                    ptr += 3; // Skip the subevent code and status
                    uint8_t subevent = *ptr;

                    if (subevent == EVT_LE_ADVERTISING_REPORT) {
                        ptr++; // Skip the subevent code
                        le_advertising_info *info = (le_advertising_info *)ptr;

                        // Extract Mac address and RSSI
                        char mac[18];
                        ba2str(&info->bdaddr, mac);
                        int8_t rssi = (int8_t)info->data[info->length];

                        // Print Mac address and RSSI
                        printf("Mac: %s, RSSI: %d, ", mac, rssi);

                        // Check if it's an Accelerometer Beacon
                        if (info->length == 21 && info->data[1] == 0x06) {
                            // Parse and print Accelerometer data
                            parseAccelerometerData(info->data, info->length);
                            // Check movement status
                            checkMovementStatus(info->data);
                        }

                        // Check if it's an iBeacon
                        if (info->length == 30 && info->data[1] == 0x06) {
                            // Parse and print iBeacon data
                            parseIBeaconData(info->data, info->length);
                        }

                        printf("\n");
                    }
                }

                ptr += 2 + ptr[1]; // Move to the next event
            }
        }
    }

    close(device_handle);

    return 0;
}

