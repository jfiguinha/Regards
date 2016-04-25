.PHONY: clean All

All:
	@echo "----------Building project:[ include - Debug ]----------"
	@cd "include" && "$(MAKE)" -f  "include.mk"
clean:
	@echo "----------Cleaning project:[ include - Debug ]----------"
	@cd "include" && "$(MAKE)" -f  "include.mk" clean
