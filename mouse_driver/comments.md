#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void handle_data(char* data) {
    char* tilts[] = {"x", "y", "z"};
    int num_tilts = sizeof(tilts) / sizeof(tilts[0]);
    bool touch = false;
    
    for (int i = 0; i < num_tilts; i++) {
        if (strncmp(data, tilts[i], strlen(tilts[i])) == 0) {
            float value = atof(data + strlen(tilts[i]) + 1);
            // Update tilts array with the new value
            break;
        }
    }
    
    if (strncmp(data, "t:", 2) == 0) {
        int t = atoi(data + 2);
        touch = t > 0;
    }
}

# Serial

```c
port = input('Enter device name: ')

if port == '':
    port = '/dev/tty.usbmodem1302'

serial = serial.Serial(port, baudrate=38400)

// repeat
data = serial.readline().decode('utf-8').strip()
```
