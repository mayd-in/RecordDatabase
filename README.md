# Record Database
Simple record database manager application in 3 different implementations

<table>
  <tr>
    <th colspan="2">Qt 5.15</td>
  </tr>
  <tr>
    <td>Linux</td>
    <td>Windows</td>
  </tr>
  <tr>
    <td><a href="https://travis-ci.org/github/mayd-in/RecordDatabase" target="_blank"><img src="https://api.travis-ci.org/mayd-in/RecordDatabase.svg?branch=main"/></a></td>
    <td>Soon</td>
  </tr>
</table>

Record Database Editor allows storing information about contacts in individual files.
This application has been implemented in 3 different ways. Native Qt Widgets is used with Python and C++.
QML is used with C++ backend.

## QML and Native Interfaces:

![Record Database QML Implementation](/docs/assets/qml.png)

![Record Database Native Implementation](/docs/assets/native.png)

## Some features:

1. Rich text editor tools
2. Easy record creation and save
3. Powerful search functionality
4. Theme support
5. Multiple language support *(Soon)*

## Requirements

* C++11 compiler
  * Tested with GCC 5.4.0 on Linux (Travis)
  * Other compilers might work, but are not currently tested
* Qt 5.15.x
  * Tested with the binary distributions of Qt 5.15 on Linux
  * Only Qt version higher than 5.15 is supported.
  * Qt 6 might work, but are not currently tested

## License

This application uses MIT License. See LICENSE file for more details
