00001 #ifndef VERSION_H
00002 #define VERSION_H
00003 
00004 namespace AutoVersion{
00005     
00006     //Date Version Types
00007     static const char DATE[] = "25";
00008     static const char MONTH[] = "07";
00009     static const char YEAR[] = "2012";
00010     static const char UBUNTU_VERSION_STYLE[] = "12.07";
00011     
00012     //Software Status
00013     static const char STATUS[] = "Release";
00014     static const char STATUS_SHORT[] = "r";
00015     
00016     //Standard Version Type
00017     static const long MAJOR = 1;
00018     static const long MINOR = 1;
00019     static const long BUILD = 0;
00020     static const long REVISION = 8261;
00021     
00022     //Miscellaneous Version Types
00023     static const long BUILDS_COUNT = 8261;
00024     #define RC_FILEVERSION 1,1,0,8261
00025     #define RC_FILEVERSION_STRING "1, 1, 0, 8261\0"
00026     static const char FULLVERSION_STRING[] = "1.1.0.8261";
00027     
00028     //These values are to keep track of your versioning state, don't modify them.
00029     static const long BUILD_HISTORY = 0;
00030     
00031 
00032 }
00033 #endif //VERSION_H
