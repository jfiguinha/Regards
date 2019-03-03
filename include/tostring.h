#if _MSC_VER < 1800
#pragma once
#ifndef TOSTRING
#define TOSTRING

wxString to_string(const float & value);

wxString to_string(const int & value);

wxString to_string(const double & value);

wxString to_string(const unsigned int & value);


#endif
#endif
