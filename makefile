
include config.mk

.PHONY: all clean

all:
	# Build each subdirectory listed in BUILD_DIR.
	# Use $$ for shell variables inside make recipes.
	@for dir in $(BUILD_DIR); \
	do \
		make -C $$dir; \
	done


clean:
	# Remove generated artifacts from all known build locations.
	rm -rf app/link_obj app/dep nginx
	rm -rf signal/*.gch app/*.gch

