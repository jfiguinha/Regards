.PHONY: clean All

All:
	@echo "----------Building project:[ RegardsViewer_Windows - Debug ]----------"
	@cd "RegardsViewer" && "$(MAKE)" -f  "RegardsViewer_Windows.mk" PreBuild && "$(MAKE)" -f  "RegardsViewer_Windows.mk" ../include/Debug/header.h.gch && "$(MAKE)" -f  "RegardsViewer_Windows.mk" && "$(MAKE)" -f  "RegardsViewer_Windows.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ RegardsViewer_Windows - Debug ]----------"
	@cd "RegardsViewer" && "$(MAKE)" -f  "RegardsViewer_Windows.mk" clean
