.PHONY: clean All

All:
	@echo "----------Building project:[ libWindowMain - Debug ]----------"
	@cd "libWindowMain" && $(MAKE) -f  "libWindowMain.mk"
	@echo "----------Building project:[ libWindow - Debug ]----------"
	@cd "libWindow" && $(MAKE) -f  "libWindow.mk"
	@echo "----------Building project:[ libViewer - Debug ]----------"
	@cd "libViewer" && $(MAKE) -f  "libViewer.mk"
	@echo "----------Building project:[ libVideoThumbnail - Debug ]----------"
	@cd "libVideoThumbnail" && $(MAKE) -f  "libVideoThumbnail.mk"
	@echo "----------Building project:[ libVideoFFMpeg - Debug ]----------"
	@cd "libVideoFFMpeg" && $(MAKE) -f  "libVideoFFMpeg.mk"
	@echo "----------Building project:[ libUtility - Debug ]----------"
	@cd "libUtility" && $(MAKE) -f  "libUtility.mk"
	@echo "----------Building project:[ libTheme - Debug ]----------"
	@cd "libTheme" && $(MAKE) -f  "libTheme.mk"
	@echo "----------Building project:[ libSqlite - Debug ]----------"
	@cd "libSqlite" && $(MAKE) -f  "libSqlite.mk"
	@echo "----------Building project:[ libSqlEngine - Debug ]----------"
	@cd "libSqlEngine" && $(MAKE) -f  "libSqlEngine.mk"
	@echo "----------Building project:[ libSqlData - Debug ]----------"
	@cd "libSqlData" && $(MAKE) -f  "libSqlData.mk"
	@echo "----------Building project:[ libResource - Debug ]----------"
	@cd "libResource" && $(MAKE) -f  "libResource.mk"
	@echo "----------Building project:[ libRenderBitmap - Debug ]----------"
	@cd "libRenderBitmap" && $(MAKE) -f  "libRenderBitmap.mk"
	@echo "----------Building project:[ libPrint - Debug ]----------"
	@cd "libPrint" && $(MAKE) -f  "libPrint.mk"
	@echo "----------Building project:[ libPicture - Debug ]----------"
	@cd "libPicture" && $(MAKE) -f  "libPicture.mk"
	@echo "----------Building project:[ libParameter - Debug ]----------"
	@cd "libParameter" && $(MAKE) -f  "libParameter.mk"
	@echo "----------Building project:[ libOpenGL - Debug ]----------"
	@cd "libOpenGL" && $(MAKE) -f  "libOpenGL.mk"
	@echo "----------Building project:[ libOpenCL - Debug ]----------"
	@cd "libOpenCL" && $(MAKE) -f  "libOpenCL.mk"
	@echo "----------Building project:[ libIntro - Debug ]----------"
	@cd "libIntro" && $(MAKE) -f  "libIntro.mk"
	@echo "----------Building project:[ libGeolocalisation - Debug ]----------"
	@cd "libGeolocalisation" && $(MAKE) -f  "libGeolocalisation.mk"
	@echo "----------Building project:[ libFiltre - Debug ]----------"
	@cd "libFiltre" && $(MAKE) -f  "libFiltre.mk"
	@echo "----------Building project:[ libExplorer - Debug ]----------"
	@cd "libExplorer" && $(MAKE) -f  "libExplorer.mk"
	@echo "----------Building project:[ libExif - Debug ]----------"
	@cd "libExif" && $(MAKE) -f  "libExif.mk"
	@echo "----------Building project:[ libEmail - Debug ]----------"
	@cd "libEmail" && $(MAKE) -f  "libEmail.mk"
	@echo "----------Building project:[ libDialog - Debug ]----------"
	@cd "libDialog" && $(MAKE) -f  "libDialog.mk"
	@echo "----------Building project:[ libDataStructure - Debug ]----------"
	@cd "libDataStructure" && $(MAKE) -f  "libDataStructure.mk"
	@echo "----------Building project:[ libCompress - Debug ]----------"
	@cd "libCompress" && $(MAKE) -f  "libCompress.mk"
	@echo "----------Building project:[ RegardsRaw - Debug ]----------"
	@cd "Otherlib/RegardsRaw" && $(MAKE) -f  "RegardsRaw.mk"
	@echo "----------Building project:[ CxImage - Debug ]----------"
	@cd "CxImage" && $(MAKE) -f  "CxImage.mk"
	@echo "----------Building project:[ libControl - Debug ]----------"
	@cd "libControl" && $(MAKE) -f  "libControl.mk"
	@echo "----------Building project:[ RegardsViewer - Debug ]----------"
	@cd "RegardsViewer" && $(MAKE) -f  "RegardsViewer.mk" PreBuild && $(MAKE) -f  "RegardsViewer.mk" && $(MAKE) -f  "RegardsViewer.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ libWindowMain - Debug ]----------"
	@cd "libWindowMain" && $(MAKE) -f  "libWindowMain.mk"  clean
	@echo "----------Cleaning project:[ libWindow - Debug ]----------"
	@cd "libWindow" && $(MAKE) -f  "libWindow.mk"  clean
	@echo "----------Cleaning project:[ libViewer - Debug ]----------"
	@cd "libViewer" && $(MAKE) -f  "libViewer.mk"  clean
	@echo "----------Cleaning project:[ libVideoThumbnail - Debug ]----------"
	@cd "libVideoThumbnail" && $(MAKE) -f  "libVideoThumbnail.mk"  clean
	@echo "----------Cleaning project:[ libVideoFFMpeg - Debug ]----------"
	@cd "libVideoFFMpeg" && $(MAKE) -f  "libVideoFFMpeg.mk"  clean
	@echo "----------Cleaning project:[ libUtility - Debug ]----------"
	@cd "libUtility" && $(MAKE) -f  "libUtility.mk"  clean
	@echo "----------Cleaning project:[ libTheme - Debug ]----------"
	@cd "libTheme" && $(MAKE) -f  "libTheme.mk"  clean
	@echo "----------Cleaning project:[ libSqlite - Debug ]----------"
	@cd "libSqlite" && $(MAKE) -f  "libSqlite.mk"  clean
	@echo "----------Cleaning project:[ libSqlEngine - Debug ]----------"
	@cd "libSqlEngine" && $(MAKE) -f  "libSqlEngine.mk"  clean
	@echo "----------Cleaning project:[ libSqlData - Debug ]----------"
	@cd "libSqlData" && $(MAKE) -f  "libSqlData.mk"  clean
	@echo "----------Cleaning project:[ libResource - Debug ]----------"
	@cd "libResource" && $(MAKE) -f  "libResource.mk"  clean
	@echo "----------Cleaning project:[ libRenderBitmap - Debug ]----------"
	@cd "libRenderBitmap" && $(MAKE) -f  "libRenderBitmap.mk"  clean
	@echo "----------Cleaning project:[ libPrint - Debug ]----------"
	@cd "libPrint" && $(MAKE) -f  "libPrint.mk"  clean
	@echo "----------Cleaning project:[ libPicture - Debug ]----------"
	@cd "libPicture" && $(MAKE) -f  "libPicture.mk"  clean
	@echo "----------Cleaning project:[ libParameter - Debug ]----------"
	@cd "libParameter" && $(MAKE) -f  "libParameter.mk"  clean
	@echo "----------Cleaning project:[ libOpenGL - Debug ]----------"
	@cd "libOpenGL" && $(MAKE) -f  "libOpenGL.mk"  clean
	@echo "----------Cleaning project:[ libOpenCL - Debug ]----------"
	@cd "libOpenCL" && $(MAKE) -f  "libOpenCL.mk"  clean
	@echo "----------Cleaning project:[ libIntro - Debug ]----------"
	@cd "libIntro" && $(MAKE) -f  "libIntro.mk"  clean
	@echo "----------Cleaning project:[ libGeolocalisation - Debug ]----------"
	@cd "libGeolocalisation" && $(MAKE) -f  "libGeolocalisation.mk"  clean
	@echo "----------Cleaning project:[ libFiltre - Debug ]----------"
	@cd "libFiltre" && $(MAKE) -f  "libFiltre.mk"  clean
	@echo "----------Cleaning project:[ libExplorer - Debug ]----------"
	@cd "libExplorer" && $(MAKE) -f  "libExplorer.mk"  clean
	@echo "----------Cleaning project:[ libExif - Debug ]----------"
	@cd "libExif" && $(MAKE) -f  "libExif.mk"  clean
	@echo "----------Cleaning project:[ libEmail - Debug ]----------"
	@cd "libEmail" && $(MAKE) -f  "libEmail.mk"  clean
	@echo "----------Cleaning project:[ libDialog - Debug ]----------"
	@cd "libDialog" && $(MAKE) -f  "libDialog.mk"  clean
	@echo "----------Cleaning project:[ libDataStructure - Debug ]----------"
	@cd "libDataStructure" && $(MAKE) -f  "libDataStructure.mk"  clean
	@echo "----------Cleaning project:[ libCompress - Debug ]----------"
	@cd "libCompress" && $(MAKE) -f  "libCompress.mk"  clean
	@echo "----------Cleaning project:[ RegardsRaw - Debug ]----------"
	@cd "Otherlib/RegardsRaw" && $(MAKE) -f  "RegardsRaw.mk"  clean
	@echo "----------Cleaning project:[ CxImage - Debug ]----------"
	@cd "CxImage" && $(MAKE) -f  "CxImage.mk"  clean
	@echo "----------Cleaning project:[ libControl - Debug ]----------"
	@cd "libControl" && $(MAKE) -f  "libControl.mk"  clean
	@echo "----------Cleaning project:[ RegardsViewer - Debug ]----------"
	@cd "RegardsViewer" && $(MAKE) -f  "RegardsViewer.mk" clean
