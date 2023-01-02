# qUART
Real-time graph and spectrum (of values, not amplitudes) serial plotter written in Qt6 and Qwt

## Screenshots
| Spectrum (of values, not amplitudes) | Plot |
| --- | --- |
| ![spectrum](https://user-images.githubusercontent.com/93074662/205555655-4cb67c9c-0a14-43bb-ba4d-97d756e9d073.png) | ![plot](https://user-images.githubusercontent.com/93074662/205555659-1c2f4fb0-b8cf-4367-a49d-d5a40f20f3fc.png)

## License
- qUART is licensed under GPL-3.0
- Qt 6 is licensed under LGPL-3.0
- Qwt is licensed under Qwt-1.0
- Material Symbols are licensed under Apache-2.0

## Features
- show all available ports (name, location, manufacturer, serial number, vendor and product identifiers)
- configure the port (baud rate, data bits, parity, stop bits, flow control)
- data representation as a spectrum and/or a plot
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
1. Build Qwt 6.2.0 for Qt 6.
2. Set QWT_LIB_PATH and QWT_INCLUDE in CMakeLists.txt. Example:
    - set(QWT_LIB_PATH "/usr/local/qwt-6.2.0/lib")
    - set(QWT_INCLUDE "/usr/local/qwt-6.2.0/include")
3. Run cmake with -DCMAKE_BUILD_TYPE=Release.
4. Run make.

## Contributing
Feel free to add features or fix bugs. This project uses the Qt codestyle, you can use the .clang-format file from [here](https://github.com/qt-creator/qt-creator/blob/master/.clang-format).

You can also use sanitizers (address or UB). To do so pass -DSANITIZER=ADDR or -DSANITIZER=UB to cmake.
