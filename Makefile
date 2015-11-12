.PHONY: clean All

All:
	@echo "----------Building project:[ Internship - Debug ]----------"
	@"$(MAKE)" -f  "Internship.mk"
clean:
	@echo "----------Cleaning project:[ Internship - Debug ]----------"
	@"$(MAKE)" -f  "Internship.mk" clean
