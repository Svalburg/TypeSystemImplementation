.PHONY: clean All

All:
	@echo "----------Building project:[ Internship - Debug ]----------"
	$(MAKE) -C "." -f "Internship.mk"
clean:
	@echo "----------Cleaning project:[ Internship - Debug ]----------"
	$(MAKE) -C "." -f "Internship.mk" clean
