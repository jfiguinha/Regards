wxIA                  {#mainpage}
=========

wxIA is a collection of cross-platform image acquisition classes for the [wxWidgets][1] GUI class library providing access to native image acquisition API's. Current support is for TWAIN on Windows (and soon on Mac, hopefully) and SANE on Linux. It was originally created by Derry Bryson in 2003.

Resources
---------
* [Download](https://github.com/versable/wxia/releases)
* [Documentation](http://versable.github.io/wxia)
* [GitHub](https://github.com/versable/wxia)
* [Changelog](https://github.com/versable/wxia/blob/master/CHANGELOG.md#)

Features
--------
Currently implemented:
* TWAIN image acquisition under Windows
* wxWidgets 3.0.0 support

Currently not implemented:
* SANE image acquisition

Requirements
------------
* [wxWidgets][1]: 3.0.0  or newer
* [SANE][2]: For Unix/Linux
* [TWAIN compatible device][3]: For Windows/OSX

[1]: http://www.wxwidgets.org
[2]: http://sane-project.org/
[3]: http://www.twain.org/
