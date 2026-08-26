#include <header.h>
#include "SplitterImpl.h"
#include "SeparationBar.h"
#include <theme.h>

using namespace Regards::Window;

CSplitterImpl::CSplitterImpl(const CThemeSplitter& theme)
	: posBar(0),
	  horizontal(false),
	  oldWidth(0),
	  oldHeight(0),
	  window1(nullptr),
	  window2(nullptr),
	  separationBar(nullptr),
	  fixPosition(false),
	  taille(0),
	  posWindow(0),
	  diff(0),
	  fixWindow1Size(false),
	  window1Size(0),
	  fixWindow2Size(false),
	  window2Size(0),
	  windowResize(false),
	  fastRender(false),
	  moving(false),
	  windowMinimalSize(100),
	  showSeparationBar(true),
	  themeSplitter(theme)
{
#ifdef WIN32
	fastRender = true;
#else
	fastRender = false;
#endif
}
