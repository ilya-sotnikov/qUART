# qUART
Real-time graph and spectrum serial plotter written in Qt6 and Qwt

## Screenshots
| Spectrum | Plot |
| --- | --- |
| ![spectrum](https://user-images.githubusercontent.com/93074662/195976564-68adb347-fcfd-4e28-9b4b-4cca96922285.png) | ![plot](https://user-images.githubusercontent.com/93074662/195976573-23d1174a-dfcc-4c7a-b954-1f06f620649a.png)

## Build instructions
You can compile it in all OSs where Qt 6 is available.
1. Build Qwt 6.2.0 for Qt 6.
2. Set QWT_LIB_PATH and QWT_INCLUDE in CMakeLists.txt. Example:
    - set(QWT_LIB_PATH "/usr/local/qwt-6.2.0/lib")
    - set(QWT_INCLUDE "/usr/local/qwt-6.2.0/include")
3. Run cmake.
4. Run make.

## Features
- show all available ports (name, location, manufacturer, serial number, vendor and product identifiers)
- configure port (baud rate, data bits, parity, stop bits, flow control)
- plot data as spectrum or graph
- data types
    - u8, u16, u32, u64 for spectrum
    - u8, u16, u32, u64, i8, i16, i32, i64, f32, f64 for graph
- zoom, navigate
- save and open data
- clear the chart
- save the chart as an image
