WIN_USER ?=
BOARD ?=
EXAMPLE ?=

$(info WIN_USER : $(WIN_USER))
$(info BOARD : $(BOARD))

ifeq ($(BOARD), XMC4700_Relax)
DEVICE := XMC4700-F144x2048
TARGET := APP_KIT_XMC47_RELAX_V1
else ifeq ($(BOARD), XMC1100_Boot)
DEVICE := XMC1100-T038x0064
TARGET := APP_KIT_XMC11_BOOT_001
else ifeq ($(BOARD), XMC4800_Relax)
DEVICE := XMC4800-F144x2048
TARGET := APP_KIT_XMC48_RELAX_ECAT_V1
endif

$(info DEVICE : $(DEVICE))
$(info TARGET : $(TARGET))


TESTS_NEEDS_SENSOR=TEST_TLx493D_A1B6_NEEDS_SENSOR \
                   TEST_TLx493D_A2B6_NEEDS_SENSOR \
				   TEST_TLx493D_P2B6_NEEDS_SENSOR \
				   TEST_TLx493D_W2B6_NEEDS_SENSOR \
	               TEST_TLx493D_W2BW_NEEDS_SENSOR \
				   TEST_TLx493D_P3B6_NEEDS_SENSOR \
				   TEST_TLx493D_P3I8_NEEDS_SENSOR


TESTS_NO_SENSOR=TEST_TLx493D_A1B6 \
                TEST_TLx493D_A2B6 \
				TEST_TLx493D_P2B6 \
				TEST_TLx493D_W2B6 \
			 	TEST_TLx493D_W2BW \
				TEST_TLx493D_P3B6 \
				TEST_TLx493D_P3I8

				

A1B6_needsSensor: TESTS=TEST_TLx493D_A1B6 TEST_TLx493D_A1B6_NEEDS_SENSOR
A1B6: TESTS=TEST_TLx493D_A1B6

A2B6_needsSensor: TESTS=TEST_TLx493D_A2B6 TEST_TLx493D_A2B6_NEEDS_SENSOR
A2B6: TESTS=TEST_TLx493D_A2B6

P2B6_needsSensor: TESTS=TEST_TLx493D_P2B6 TEST_TLx493D_P2B6_NEEDS_SENSOR
P2B6: TESTS=TEST_TLx493D_P2B6

W2B6_needsSensor: TESTS=TEST_TLx493D_W2B6 TEST_TLx493D_W2B6_NEEDS_SENSOR
W2B6: TESTS=TEST_TLx493D_W2B6

W2BW_needsSensor: TESTS=TEST_TLx493D_W2BW TEST_TLx493D_W2BW_NEEDS_SENSOR
W2BW: TESTS=TEST_TLx493D_W2BW

P3B6_needsSensor: TESTS=TEST_TLx493D_P3B6 TEST_TLx493D_P3B6_NEEDS_SENSOR
P3B6: TESTS=TEST_TLx493D_P3B6

P3I8_needsSensor: TESTS=TEST_TLx493D_P3I8 TEST_TLx493D_P3I8_NEEDS_SENSOR
P3I8: TESTS=TEST_TLx493D_P3I8

A1B6_needsSensor A1B6 \
A2B6_needsSensor A2B6 \
P2B6_needsSensor P2B6 \
W2B6_needsSensor W2B6 \
W2BW_needsSensor W2BW \
P3B6_needsSensor P3B6 \
P3I8_needsSensor P3I8: flash


test_all: TESTS=$(TESTS_NEEDS_SENSOR) $(TESTS_NO_SENSOR)
test_needsSensor: TESTS=$(TESTS_NEEDS_SENSOR) $(TEST_COMMON_NEEDS_SENSOR)
test: TESTS=$(TESTS_NO_SENSOR)

test_all \
test_needsSensor \
test: unity flash


# additional manifest list to retrive developmental project
# then the developmental project appears under XMC BSPs->XMC_[1100, 4500, 47/4800]_boards->Sensing->MTB Example TLx493D 3D Magnetic Sensors
set_manifest:
	cp-f manifest.loc C:\Users\$(WIN_USER)\.modustoolbox\

clean:
	rm -rf build/*
	-rm -rf ~/mtb3DMagneticSensors


mtb: clean
ifeq ($(WIN_USER),)
	$(error "Must set variable WIN_USER (Windows user account name) in order to be able to copy files to MTB directories !")
endif
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to Arduino or MTB directories !")
else
	-rm -rf ~/mtb3DMagneticSensors
	mkdir -p ~/mtb3DMagneticSensors/src
	C:/Users/$(WIN_USER)/ModusToolbox/tools_3.1/modus-shell/bin/find src -name '*.[hc]' -a \( \! -path '*arduino*' \) -print -exec cp {} ~/mtb3DMagneticSensors/src \;
	rm -rf C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/IFX3DMagneticSensors
	mkdir C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/IFX3DMagneticSensors
	rm -rf C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/examples
	mkdir C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/examples
endif

examples: mtb
ifeq ($(WIN_USER),)
	$(error "Must set variable WIN_USER (Windows user account name) in order to be able to copy files to MTB directories !")
endif
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to MTB directories !")
else
	cp -r examples/framework/mtb/xmc/* C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/examples/
	cp -r ~/mtb3DMagneticSensors/* C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/IFX3DMagneticSensors
	cp -f main.c C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors
endif

unity: mtb
ifeq ($(WIN_USER),)
	$(error "Must set variable WIN_USER (Windows user account name) in order to be able to copy files to MTB directories !")
endif
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to MTB directories !")
else
	cp test/unit/src/framework/mtb/xmc/*.c ~/mtb3DMagneticSensors/src
	cp test/unit/src/Test_*.h ~/mtb3DMagneticSensors/src
	cp test/unit/src/tlx493d/Test_*.h ~/mtb3DMagneticSensors/src
	cp -r test/unit/Unity ~/mtb3DMagneticSensors/src
	cp -r ~/mtb3DMagneticSensors/* C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/IFX3DMagneticSensors
	rm -rf C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/main.c
endif


compile_tests: unity
ifeq ($(WIN_USER),)
	$(error "Must set variable WIN_USER (Windows user account name) in order to be able to copy files to MTB directories !")
endif
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to Arduino or MTB directories !")
else
	$(SHELL) -c "$(MAKE) -C C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors clean; $(MAKE) -C C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors app DEFINES+='$(TESTS)'"
	cp -f C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/build\$(TARGET)\Debug\mtb-example-xmc-tlx493d-sensors.hex build/
endif

compile_examples: examples
ifeq ($(WIN_USER),)
	$(error "Must set variable WIN_USER (Windows user account name) in order to be able to copy files to MTB directories !")
endif
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to Arduino or MTB directories !")
else
	$(SHELL) -c "$(MAKE) -C C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors clean; $(MAKE) -C C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors app DEFINES+='EXAMPLE=$(EXAMPLE)'"
	cp -f C:/Users/$(WIN_USER)/mtw/$(BOARD)_MTB_Example_for_TLx493D_3D_Magnetic_Sensors/build\$(TARGET)\Debug\mtb-example-xmc-tlx493d-sensors.hex build/
endif

upload_tests: compile_tests
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to Arduino or MTB directories !")
else
	jlink.exe -AutoConnect 1 -ExitOnError 1 -NoGui 1 -Device $(DEVICE) -If SWD -Speed 4000 -CommandFile program.jlink
endif

upload_examples: compile_examples
ifeq ($(BOARD),)
	$(error "Must set variable BOARD (board name prefix, eg XMC4700_Relax or XMC1100_Boot) in order to be able to copy files to Arduino or MTB directories !")
else
	jlink.exe -AutoConnect 1 -ExitOnError 1 -NoGui 1 -Device $(DEVICE) -If SWD -Speed 4000 -CommandFile program.jlink
endif


flash: compile_tests upload_tests
ci_cd_examples: compile_examples upload_examples

.DEFAULT: test_all