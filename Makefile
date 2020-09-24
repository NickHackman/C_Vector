# path #
LINUX_INCLUDE_PATH = /usr/local/include

# name #
LIB_NAME = c_vector

# src #
SRC_PATH = src
HEADER_EXTENSION = h
SRC = $(shell find $(SRC_PATH) -name '*.$(HEADER_EXTENSION)')

# uname #
UNAME = $(shell uname)

.PHONY: default_target
default_target:
	@echo "C_Vector Options:"
	@printf "\tmake install \n\t\t Install c_vector headers on a Linux machine [Requires Root]\n"
	@printf "\tmake clean \n\t\t Remove c_vector headers on a Linux machine [Requires Root]\n"
	@printf "\tmake test \\n\t\t Run the test cases for both core and extra\n"

.PHONY: install
install:
	@if [ "$(UNAME)" == "Linux" ]; then \
		echo "Installing $(LIB_NAME)"; \
		sudo mkdir -p $(LINUX_INCLUDE_PATH)/$(LIB_NAME); \
		echo "Copying $(SRC) -> $(LINUX_INCLUDE_PATH)/$(LIB_NAME)"; \
		sudo cp $(SRC) $(LINUX_INCLUDE_PATH)/$(LIB_NAME)/.; \
	fi

.PHONY: clean
clean:
	@if [ "$(UNAME)" == "Linux" ]; then \
		echo "Removing $(LIB_NAME)"; \
		sudo $(RM) -r $(LINUX_INCLUDE_PATH)/$(LIB_NAME); \
	fi

.PHONY: test
test:
	@clang ./tests/test.c -lcmocka -o test
	@./test
	@$(RM) test
