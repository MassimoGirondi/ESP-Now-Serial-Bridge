# ESP-NOW Serial Bridge

This project allows you to build inexpensive serial links over 2.4 GHz links!

This has been initially designed to be used as a replacement for a SIK Radio telemetry on a Drone-Ground Station Link, but it should work with any serial communication.

The range is highly dependent on your antennas, while baud-rate has been tested up to 57600 baudps. Going further should be possible.

There is (virtually) no control! So any frame could potentially arrive broken or be lost. This is acceptable for a protocol like MAVlink, and in cases where bandwidth and low latency are more important than missing packets.
There are better solutions if what you want is a reliable connection.

# Install

You'll need the [platformio](https://platformio.org) toolchain installed.
Then you edit the `platformio.ini` file with your board characteristics.

Compile with `pio run -t upload -e board1` or `pio run -t upload -e board2`.

# Configuration

In order to allow the devices to communicate, you have to distinguish what is "board 1" and what is "board 2", and their MAC addresses.

You can set the `DEBUG` flag in the `include/config.h` file, upload your code and open a serial monitor with `pio device monitor`.

There you should see the board MAC address, which you have then to put in the `include/config.h` file.

# LICENSE

This is based on the work of Yuri on  [ESP-Now-Serial-Bridge](https://github.com/yuri-rage/ESP-Now-Serial-Bridge).
All rights to him for his pieces of software.

Part of the software was also based on the tutorial by [randomnerdtutorials.com](https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/).


For everything else, the software licensed under the GNU GPL 3 license. See [LICENSE](LICENSE).
