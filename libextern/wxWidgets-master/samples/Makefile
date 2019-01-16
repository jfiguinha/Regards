#
# Makefile : Builds wxWindows samples for Unix.
#

SAMPLES_SUBDIRS=access animate artprov aui calendar caret clipboard collpane combo config dataview dialogs dialup display dll dnd docview dragimag drawing erase event except exec font fswatcher grid help htlbox html html/about html/help html/helpview html/printing html/test html/virtual html/widget html/zip image internat ipc joytest layout listctrl mdi mediaplayer menu minimal nativdlg notebook oleauto opengl opengl/cube opengl/isosurf opengl/penguin opengl/pyramid ownerdrw popup power preferences printing propgrid regtest render ribbon richtext sashtest scroll secretstore shaped sockets sound splash splitter statbar stc svg taborder taskbar text thread toolbar treectrl treelist typetest uiaction validate vscroll webview widgets wizard wrapsizer xrc 

ALL_TARGETS := all clean distclean

$(ALL_TARGETS): $(SAMPLES_SUBDIRS)

# The use of both MAKECMDGOALS and .TARGETS here allows the same makefile to be
# used both with GNU make and BSD make: only one of these variables will be
# defined for the particular make program flavour.
$(SAMPLES_SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS) $(.TARGETS)

.PHONY: $(ALL_TARGETS) $(SAMPLES_SUBDIRS)
