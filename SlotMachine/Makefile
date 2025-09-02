include config.mk
include $(wildcard $(SRC_DIR)/*/config.mk)

$(foreach MODULE,$(MODULES),$(eval include rules.mk))

.PHONY: all
.DEFAULT_GOAL := all
all: $(MODULES)

.PHONY: clean
clean:
	@echo "[RM]  $(OBJ_DIR)/* $(BIN_DIR)/*"
	@rm -rf $(OBJ_DIR) $(BIN_DIR)