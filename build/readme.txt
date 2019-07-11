### Adding new directories ####################################################
1. Create new folder under PROJECT_ROOT/app
2. Create a local makefile in the folder and copy any of the following lines:

	-include common_makefiles/if.mk       # for interface class declarations
	-include common_makefiles/src_inc.mk  # for source and include files
	-include common_makefiles/test.mk     # for unit tests

3. Add the path to the local makefile to the file modules.mk

### Building ##################################################################

To build the binary to flash:

	make bin

To flash the binary to the atmega328:

	make flash

note: assumes atmega is connected to com port 5.

To build tests:

	make test

To run tests:

	test.exe

