.PHONY: clean All

All:
	@echo "----------Building project:[ CxImage - Release ]----------"
	@cd "CxImage" && "$(MAKE)" -f  "CxImage.mk"
	@echo "----------Building project:[ RegardsRaw - Release ]----------"
	@cd "Otherlib/RegardsRaw" && "$(MAKE)" -f  "RegardsRaw.mk"
	@echo "----------Building project:[ libCompress - Release ]----------"
	@cd "libCompress" && "$(MAKE)" -f  "libCompress.mk"
	@echo "----------Building project:[ libControl - Release ]----------"
	@cd "libControl" && "$(MAKE)" -f  "libControl.mk"
	@echo "----------Building project:[ libWindowMain - Release ]----------"
	@cd "libWindowMain" && "$(MAKE)" -f  "libWindowMain.mk"
	@echo "----------Building project:[ libWindow - Release ]----------"
	@cd "libWindow" && "$(MAKE)" -f  "libWindow.mk"
	@echo "----------Building project:[ libViewer - Release ]----------"
	@cd "libViewer" && "$(MAKE)" -f  "libViewer.mk"
	@echo "----------Building project:[ libVideoThumbnail - Release ]----------"
	@cd "libVideoThumbnail" && "$(MAKE)" -f  "libVideoThumbnail.mk"
	@echo "----------Building project:[ libVideoFFMpeg - Release ]----------"
	@cd "libVideoFFMpeg" && "$(MAKE)" -f  "libVideoFFMpeg.mk"
	@echo "----------Building project:[ libUtility - Release ]----------"
	@cd "libUtility" && "$(MAKE)" -f  "libUtility.mk"
	@echo "----------Building project:[ libSqlEngine - Release ]----------"
	@cd "libSqlEngine" && "$(MAKE)" -f  "libSqlEngine.mk"
	@echo "----------Building project:[ libSqlite - Release ]----------"
	@cd "libSqlite" && "$(MAKE)" -f  "libSqlite.mk"
	@echo "----------Building project:[ libTheme - Release ]----------"
	@cd "libTheme" && "$(MAKE)" -f  "libTheme.mk"
	@echo "----------Building project:[ libSqlData - Release ]----------"
	@cd "libSqlData" && "$(MAKE)" -f  "libSqlData.mk"
	@echo "----------Building project:[ libResource - Release ]----------"
	@cd "libResource" && "$(MAKE)" -f  "libResource.mk"
	@echo "----------Building project:[ libRenderBitmap - Release ]----------"
	@cd "libRenderBitmap" && "$(MAKE)" -f  "libRenderBitmap.mk"
	@echo "----------Building project:[ libPrint - Release ]----------"
	@cd "libPrint" && "$(MAKE)" -f  "libPrint.mk"
	@echo "----------Building project:[ libPicture - Release ]----------"
	@cd "libPicture" && "$(MAKE)" -f  "libPicture.mk"
	@echo "----------Building project:[ libOpenCL - Release ]----------"
	@cd "libOpenCL" && "$(MAKE)" -f  "libOpenCL.mk"
	@echo "----------Building project:[ libOpenGL - Release ]----------"
	@cd "libOpenGL" && "$(MAKE)" -f  "libOpenGL.mk"
	@echo "----------Building project:[ libParameter - Release ]----------"
	@cd "libParameter" && "$(MAKE)" -f  "libParameter.mk"
	@echo "----------Building project:[ libIntro - Release ]----------"
	@cd "libIntro" && "$(MAKE)" -f  "libIntro.mk"
	@echo "----------Building project:[ libGeolocalisation - Release ]----------"
	@cd "libGeolocalisation" && "$(MAKE)" -f  "libGeolocalisation.mk"
	@echo "----------Building project:[ libFiltre - Release ]----------"
	@cd "libFiltre" && "$(MAKE)" -f  "libFiltre.mk"
	@echo "----------Building project:[ libExplorer - Release ]----------"
	@cd "libExplorer" && "$(MAKE)" -f  "libExplorer.mk"
	@echo "----------Building project:[ libExif - Release ]----------"
	@cd "libExif" && "$(MAKE)" -f  "libExif.mk"
	@echo "----------Building project:[ libEmail - Release ]----------"
	@cd "libEmail" && "$(MAKE)" -f  "libEmail.mk"
	@echo "----------Building project:[ libDialog - Release ]----------"
	@cd "libDialog" && "$(MAKE)" -f  "libDialog.mk"
	@echo "----------Building project:[ libDataStructure - Release ]----------"
	@cd "libDataStructure" && "$(MAKE)" -f  "libDataStructure.mk"
	@echo "----------Building project:[ RegardsViewer - Release ]----------"
	@cd "RegardsViewer" && "$(MAKE)" -f  "RegardsViewer.mk" PreBuild && "$(MAKE)" -f  "RegardsViewer.mk" && "$(MAKE)" -f  "RegardsViewer.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ CxImage - Release ]----------"
	@cd "CxImage" && "$(MAKE)" -f  "CxImage.mk"  clean
	@echo "----------Cleaning project:[ RegardsRaw - Release ]----------"
	@cd "Otherlib/RegardsRaw" && "$(MAKE)" -f  "RegardsRaw.mk"  clean
	@echo "----------Cleaning project:[ libCompress - Release ]----------"
	@cd "libCompress" && "$(MAKE)" -f  "libCompress.mk"  clean
	@echo "----------Cleaning project:[ libControl - Release ]----------"
	@cd "libControl" && "$(MAKE)" -f  "libControl.mk"  clean
	@echo "----------Cleaning project:[ libWindowMain - Release ]----------"
	@cd "libWindowMain" && "$(MAKE)" -f  "libWindowMain.mk"  clean
	@echo "----------Cleaning project:[ libWindow - Release ]----------"
	@cd "libWindow" && "$(MAKE)" -f  "libWindow.mk"  clean
	@echo "----------Cleaning project:[ libViewer - Release ]----------"
	@cd "libViewer" && "$(MAKE)" -f  "libViewer.mk"  clean
	@echo "----------Cleaning project:[ libVideoThumbnail - Release ]----------"
	@cd "libVideoThumbnail" && "$(MAKE)" -f  "libVideoThumbnail.mk"  clean
	@echo "----------Cleaning project:[ libVideoFFMpeg - Release ]----------"
	@cd "libVideoFFMpeg" && "$(MAKE)" -f  "libVideoFFMpeg.mk"  clean
	@echo "----------Cleaning project:[ libUtility - Release ]----------"
	@cd "libUtility" && "$(MAKE)" -f  "libUtility.mk"  clean
	@echo "----------Cleaning project:[ libSqlEngine - Release ]----------"
	@cd "libSqlEngine" && "$(MAKE)" -f  "libSqlEngine.mk"  clean
	@echo "----------Cleaning project:[ libSqlite - Release ]----------"
	@cd "libSqlite" && "$(MAKE)" -f  "libSqlite.mk"  clean
	@echo "----------Cleaning project:[ libTheme - Release ]----------"
	@cd "libTheme" && "$(MAKE)" -f  "libTheme.mk"  clean
	@echo "----------Cleaning project:[ libSqlData - Release ]----------"
	@cd "libSqlData" && "$(MAKE)" -f  "libSqlData.mk"  clean
	@echo "----------Cleaning project:[ libResource - Release ]----------"
	@cd "libResource" && "$(MAKE)" -f  "libResource.mk"  clean
	@echo "----------Cleaning project:[ libRenderBitmap - Release ]----------"
	@cd "libRenderBitmap" && "$(MAKE)" -f  "libRenderBitmap.mk"  clean
	@echo "----------Cleaning project:[ libPrint - Release ]----------"
	@cd "libPrint" && "$(MAKE)" -f  "libPrint.mk"  clean
	@echo "----------Cleaning project:[ libPicture - Release ]----------"
	@cd "libPicture" && "$(MAKE)" -f  "libPicture.mk"  clean
	@echo "----------Cleaning project:[ libOpenCL - Release ]----------"
	@cd "libOpenCL" && "$(MAKE)" -f  "libOpenCL.mk"  clean
	@echo "----------Cleaning project:[ libOpenGL - Release ]----------"
	@cd "libOpenGL" && "$(MAKE)" -f  "libOpenGL.mk"  clean
	@echo "----------Cleaning project:[ libParameter - Release ]----------"
	@cd "libParameter" && "$(MAKE)" -f  "libParameter.mk"  clean
	@echo "----------Cleaning project:[ libIntro - Release ]----------"
	@cd "libIntro" && "$(MAKE)" -f  "libIntro.mk"  clean
	@echo "----------Cleaning project:[ libGeolocalisation - Release ]----------"
	@cd "libGeolocalisation" && "$(MAKE)" -f  "libGeolocalisation.mk"  clean
	@echo "----------Cleaning project:[ libFiltre - Release ]----------"
	@cd "libFiltre" && "$(MAKE)" -f  "libFiltre.mk"  clean
	@echo "----------Cleaning project:[ libExplorer - Release ]----------"
	@cd "libExplorer" && "$(MAKE)" -f  "libExplorer.mk"  clean
	@echo "----------Cleaning project:[ libExif - Release ]----------"
	@cd "libExif" && "$(MAKE)" -f  "libExif.mk"  clean
	@echo "----------Cleaning project:[ libEmail - Release ]----------"
	@cd "libEmail" && "$(MAKE)" -f  "libEmail.mk"  clean
	@echo "----------Cleaning project:[ libDialog - Release ]----------"
	@cd "libDialog" && "$(MAKE)" -f  "libDialog.mk"  clean
	@echo "----------Cleaning project:[ libDataStructure - Release ]----------"
	@cd "libDataStructure" && "$(MAKE)" -f  "libDataStructure.mk"  clean
	@echo "----------Cleaning project:[ RegardsViewer - Release ]----------"
	@cd "RegardsViewer" && "$(MAKE)" -f  "RegardsViewer.mk" clean
