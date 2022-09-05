#include "FRQ.h"

// Intialize the FRQmodule object and configure the UART (Serial) 1 with the given baudrate
FRQmodule::FRQmodule(long baudRate)
{
    Serial1.begin(baudRate);
}

// Read the frequency measurement/measurements via Serial and save it to the according variable
float FRQmodule::getFrequency(bool twoFreq)
{
    // Only read frequency measurements if a certain amount of time has passed
    if (millis() - tLastGet >= 20)
    {
        // Write a 'f' to read the first of the two frequency measurements
        Serial1.write('f');
        delay(1);
        i = 0;
        // Read data while it is at the serial port and save the bytes in the frequency data buffer
        // The incoming bytes are the four seperated bytes of the float value of the frequency measurement
        while (Serial1.available() > 0)
        {
            _freqBuffer1[i] = Serial1.read();
            i++;
        }
        // The four bytes of the frequncy data buffer are copied into a float variable, turning them back into the frequency value
        memcpy(&frequency1, _freqBuffer1, 4);

        // If the twoFreq condition is true, a 'n' is sent to the FRQ module to retrieve the second frequency value in the same manner as before
        if (twoFreq)
        {
            Serial1.write('n');

            delay(1);
            i = 0;

            while (Serial1.available() > 0)
            {
                _freqBuffer2[i] = Serial1.read();
                i++;
            }

            memcpy(&frequency2, _freqBuffer2, 4);
        }
        // If the second frequency is not required, send '0' to end the data transmission
        else
        {
            Serial1.write('0');
        }
        tLastGet = millis();
    }
}