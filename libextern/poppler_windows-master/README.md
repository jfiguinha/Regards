Poppler is a PDF rendering library based on the xpdf-3.0 code base.


This is a fork of poppler-0.45.0 modified in order to build with Microsoft visual studio 2015

* Fix error related to std min max functions
* Add cmake config for libjpeg2
* All thirdparty library are added under thirdparty directpry 


Note : 
* If you want to use qt, update root cmake file and set your qt path
* Only 32 bit build is tested
* There is a msvs project under build directory if you want to rebuild the project your self