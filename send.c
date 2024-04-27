#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus.h>
#include <errno.h>
int main() {
    modbus_t *ctx;
    int rc;
    uint16_t tab_reg[32];

    // Modbus device parameters
    const char *dev = "/dev/ttyUSB0"; // Modify this according to your device
    int baud = 9600;
    char parity = 'N';
    int data_bit = 8;
    int stop_bit = 1;

    // Create a new RTU context
    ctx = modbus_new_rtu(dev, baud, parity, data_bit, stop_bit);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    // Connect to the Modbus device
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Set slave ID (device address)
    int slave_id = 2; // Change this according to your device
    modbus_set_slave(ctx, slave_id);

    // Modbus message data
    uint16_t data[] = {01 ,03 ,00 ,48 ,00 ,02 };
    int num_registers = sizeof(data) / sizeof(data[0]);

    // Write the data to the device
    rc = modbus_write_registers(ctx, 0, num_registers, data);
    if (rc == -1) {
        fprintf(stderr, "Write failed: %s\n", modbus_strerror(errno));
        modbus_close(ctx);
        modbus_free(ctx);
        return -1;  
    } else {
        printf("Write successful\n");
    }

    // Close connection and free the context
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}