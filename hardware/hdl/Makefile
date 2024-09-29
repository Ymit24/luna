# Makefile for simulations

.PHONY = all clean show manual

BUILD_DIR = sim/build
ROOT_DIR = $(CURDIR)

GHDL = ghdl
GHDL_FLAGS = --std=08

TOP_RTL_FILES = $(wildcard rtl/*.vhd)
TOP_RTL_FILES_SUB = $(TOP_RTL_FILES:%.vhd=%)

RTL_FILES = $(wildcard rtl/**/*.vhd)
RTL_FILES_SUB = $(RTL_FILES:%.vhd=%) 

SIM_FILES = $(wildcard sim/tb/*.vhd)
SIM_FILES_SUB = $(SIM_FILES:%.vhd=%) 

all: $(RTL_FILES_SUB) $(TOP_RTL_FILES_SUB) $(SIM_FILES_SUB)

manual:
	cd $(BUILD_DIR) && ${GHDL} -a -P$(BUILD_DIR) $(GHDL_FLAGS) --work=core $(GHDL_FLAGS) \
		$(ROOT_DIR)/rtl/core/common.vhd \
		$(ROOT_DIR)/rtl/core/alu.vhd \
		$(ROOT_DIR)/rtl/core/memory_bank.vhd \
		$(ROOT_DIR)/rtl/core/memory.vhd \
		$(ROOT_DIR)/rtl/core/computer.vhd \
		$(ROOT_DIR)/rtl/core/control_unit.vhd \
		$(ROOT_DIR)/rtl/core/should_jump_unit.vhd 


$(SIM_FILES_SUB):
	cd $(BUILD_DIR) && ${GHDL} -a -P$(BUILD_DIR) $(GHDL_FLAGS) $(ROOT_DIR)/$@.vhd
	cd $(BUILD_DIR) && ${GHDL} -e -P$(BUILD_DIR) $(GHDL_FLAGS) $(word 3,$(subst /, ,$@))
	cd $(BUILD_DIR) && ${GHDL} -r -P$(BUILD_DIR) $(GHDL_FLAGS) $(word 3,$(subst /, ,$@)) --vcd=$(word 3,$(subst /, ,$@)).vcd

$(TOP_RTL_FILES_SUB):
	cd $(BUILD_DIR) && ${GHDL} -a -P$(BUILD_DIR) $(GHDL_FLAGS) --work=$(word 2,$(subst /, ,$@)) $(GHDL_FLAGS) $(ROOT_DIR)/$@.vhd

$(RTL_FILES_SUB):
	cd $(BUILD_DIR) && ${GHDL} -a -P$(BUILD_DIR) $(GHDL_FLAGS) --work=$(word 2,$(subst /, ,$@)) $(GHDL_FLAGS) $(ROOT_DIR)/$@.vhd

clean:
	@rm sim/build/*

show:
	@echo Showing... $(TB_NAME)
	@if [ "$(TB_NAME)" = "" ]; then echo "Need to set TB_NAME=..."; exit 1; fi

	gtkwave $(BUILD_DIR)/$(TB_NAME).vcd

