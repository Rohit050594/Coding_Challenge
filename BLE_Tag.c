#include <stdio.h>
#include <inttypes.h>

void parseAccelerometerFrame(uint8_t *data) {
    // Offset and length information for the accelerometer frame
    int offsets[] = {0, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13, 14, 16, 18, 20};
    int lengths[] = {1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 2, 2, 6};
    
    // Interpret data based on the given structure
    for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++) {
        int offset = offsets[i];
        int length = lengths[i];
        
        printf("Accelerometer Field %d: Offset: %d, Length: %d, Data: ", i, offset, length);
        
        // Print the data based on its length and type
        if (length == 1) {
            printf("%02X\n", data[offset]);
        } else if (length == 2) {
            uint16_t value = (data[offset + 1] << 8) | data[offset];
            printf("%04X\n", value);
        } else if (length == 6) {
            printf("%02X:%02X:%02X:%02X:%02X:%02X\n", data[offset], data[offset + 1], data[offset + 2], data[offset + 3], data[offset + 4], data[offset + 5]);
        }
    }
}

void parseIBeaconFrame(uint8_t *data) {
    // Offset and length information for the iBeacon frame
    int offsets[] = {0, 1, 2, 3, 4, 5, 7, 9, 25, 27, 29};
    int lengths[] = {1, 1, 1, 1, 1, 2, 2, 16, 2, 2, 1};
    
    // Interpret data based on the given structure
    for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++) {
        int offset = offsets[i];
        int length = lengths[i];
        
        printf("iBeacon Field %d: Offset: %d, Length: %d, Data: ", i, offset, length);
        
        // Print the data based on its length and type
        if (length == 1) {
            printf("%02X\n", data[offset]);
        } else if (length == 2) {
            uint16_t value = (data[offset + 1] << 8) | data[offset];
            printf("%04X\n", value);
        } else if (length == 16) {
            printf("%02X%02X%02X%02X%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X\n",
                data[offset], data[offset + 1], data[offset + 2], data[offset + 3],
                data[offset + 4], data[offset + 5], data[offset + 6], data[offset + 7],
                data[offset + 8], data[offset + 9], data[offset + 10], data[offset + 11],
                data[offset + 12], data[offset + 13], data[offset + 14], data[offset + 15]);
        }
    }
}

int main() {
    // Sample data for accelerometer frame
    uint8_t accelerometerData[] = {2, 1, 0x06, 3, 0x03, 0xE1, 0xFF, 18, 0x16, 0xE1, 0xFF, 0xA1, 0x03, 0x64, 0x00, 0x00, 0xFF, 0x80, 0x01, 0x3B, 0x00, 0x90, 0x78, 0x56, 0x34, 12, 34, 56, 78, 90, 0x00};
    
    // Sample data for iBeacon frame
    uint8_t iBeaconData[] = {2, 1, 0x06, 26, 0xFF, 0x4C, 0x00, 0x21, 0x15, 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF0, 0x03, 0xE8, 0x07, 0xD0, 0xC5};
    
    printf("Parsing Accelerometer Frame:\n");
    parseAccelerometerFrame(accelerometerData);
    
    printf("\nParsing iBeacon Frame:\n");
    parseIBeaconFrame(iBeaconData);
    
    return 0;
}
