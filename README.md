# qUART
Real-time graph and spectrum (of values, not amplitudes) serial plotter written in Qt6 and QCustomPlot

## Screenshots
| Spec | Plot |
| --- | --- |
| ![spectrum](https://user-images.githubusercontent.com/93074662/227128175-3b0d050d-e497-48b4-816b-6d40170cd95a.png) | ![plot](https://user-images.githubusercontent.com/93074662/227128129-62a5b807-43c7-4190-8b37-0c0be9ab1119.png)

## License
- qUART is licensed under GPL-3.0
- Qt 6 is licensed under LGPL-3.0
- QCustomPlot is licensed under GPL-3.0
- Material Symbols are licensed under Apache-2.0

## Features
- show all available ports (name, location, manufacturer, serial number, vendor and product identifiers)
- configure the port (baud rate, data bits, parity, stop bits, flow control)
- write a number to the serial port (can be signed or unsigned, in hex, binary or decimal, 8, 16, 32, 64 bit)
- save and restore serial port configuration
- data representation as a spectrum and/or a plot
- show the last n points
- turn off appending to the plot or the spectrum if not needed
- data types
    - u8, u16, u32, u64 (spectrum)
    - u8, u16, u32, u64, i8, i16, i32, i64, f32, f64 (plot)
- select any value on the chart
- zoom, navigate
- save and open data
- clear the chart
- save the chart as an image

## Saving and opening data
You can save or open data as plain text, it's just numbers separated by new lines. The representation of data depends on the current chart type. If it's a plot, then data is saved as a sequence. If it's a spectrum, then data is saved as frequency of every value.

Opening data works the same way. You should select the appropriate chart type. Note that this program can make a spectrum from a sequence of points (plot), the opposite is impossible.

## Build instructions
You can compile it in all OSs where Qt 6 is available.
If you have already downloaded QCustomPlot you can set QCUSTOMPLOT_SRC_PATH in CMakeLists.txt, otherwise it will download it automatically.
1. git clone https://github.com/ilya-sotnikov/qUART
2. cd qUART
3. mkdir build && cd build
4. cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
5. cmake --install .

## Contributing
Feel free to add features or fix bugs. This project uses the Qt codestyle, you can use the .clang-format file from [here](https://code.qt.io/cgit/qt/qt5.git/tree/_clang-format).

Debug build uses sanitizers (address, undefined, leak) by default.
