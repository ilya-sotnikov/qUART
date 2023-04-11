# qUART
Real-time graph and spectrum (of values, not amplitudes) serial plotter written in Qt6 and QCustomPlot.

## Screenshots

### Data representation
| Plot | Spec |
| --- | --- |
| ![plot](https://user-images.githubusercontent.com/93074662/230701589-416c601c-b989-4ac9-9ae6-a1228335f202.png) | ![spectrum](https://user-images.githubusercontent.com/93074662/230701486-989114de-f245-4ae4-8270-8f248d615ca9.png) |

### Settings

| Conf | Info | Data |
| --- | --- | --- |
| ![conf](https://user-images.githubusercontent.com/93074662/230701678-89395b29-93e0-41fe-aa9a-7181ce4a9455.png) | ![info](https://user-images.githubusercontent.com/93074662/230701835-5f8053c5-d2eb-4153-9793-53d716ca4d12.png) | ![data](https://user-images.githubusercontent.com/93074662/230701883-89a8a6d5-69d0-4050-ac80-130be5deb8e8.png) |

## Controls

- LMB to drag (chart or axes)
- mouse wheel to zoom (chart or axes)
- RMB to select zoom
- enter to send a number (0x__ for hex, 0b__ for binary), it will send the number with the minimum possible byte count (1, 2, 4 or 8)
- enter to send a string

## License
- qUART is licensed under GPL-3.0
- Qt 6 is licensed under LGPL-3.0
- QCustomPlot is licensed under GPL-3.0
- Material Symbols are licensed under Apache-2.0

## Features
- show all available ports (name, location, manufacturer, serial number, vendor and product identifiers)
- configure the port (baud rate, data bits, parity, stop bits, flow control)
- write a number to the serial port (can be signed or unsigned, in hex, binary or decimal, 8, 16, 32, 64 bit)
- write a string to the serial port (with or without a newline)
- save and restore settings
- data representation as a plot/spectrum
- show the last n points
- turn off appending to the plot or the spectrum if not needed
- data types
    - u8, u16, u32, u64 (unsigned)
    - i8, i16, i32, i64 (signed)
    - f32, f64 (float and double)
    - ASCII (text)
- change the byte order
    - little-endian
    - big-endian
- select any value on the chart
- zoom (mouse wheel or RMB), navigate
- toggle between log and linear scale
- enable and disable autoscaling for both axes
- save and open plot/spectrum data
- clear the chart
- save the chart as an image
- set a custom chart update interval

## Append to plot/spectrum

You can turn off appending to the plot or the spectrum.

If you don't need information about the data sequence, you can turn off appending to the plot. It can save a lot of memory.

However, if you are in the "Show last N points" mode, the latest data for the spectrum is recreated from the latest plot data, that's why data will always be appended there. But the program will only kepp the latest plot data.

If you don't need information about the frequency distribution of the data, you can turn off appending to the spectrum.

## Installation

Prebuilt executables are in the latest release.

For GNU/Linux x86-64 there is a flatpak bundle, to install it:

`flatpak install --user qUART-linux_x86-64.flatpak`

If you don't like flatpaks, you can build qUART yourself, the process is very simple. See the build instructions.

For Windows x86-64 there is a zip archive. Just unpack it and run qUART.exe.

## Build instructions
You can build it in all OSs where Qt6 is available.

Dependencies:
- Qt6 (base, QSerialPort)
- QCustomPlot (can be downloaded automatically)

Install them using a package manager or using the Qt installer.

If you have already downloaded QCustomPlot you can set `QCUSTOMPLOT_SRC_PATH` in CMakeLists.txt, otherwise it will download it automatically.

1. `git clone https://github.com/ilya-sotnikov/qUART`
2. `cd qUART`
3. `mkdir build && cd build`
4. `cmake -DCMAKE_BUILD_TYPE=Release -G Ninja .. && cmake --build .`
5. `cmake --install .`

## Contributing
Feel free to add features or fix bugs. This project uses the Qt code style, you can use the .clang-format file from [here](https://code.qt.io/cgit/qt/qt5.git/tree/_clang-format).

Debug build uses sanitizers (address, undefined, leak) by default.
