#support variables
$(MODULE)_SRC_FILES := $(wildcard $($(MODULE)_SRC_DIR)/*.cpp)
$(MODULE)_OBJ_FILES := $(patsubst $($(MODULE)_SRC_DIR)/%.cpp,$($(MODULE)_OBJ_DIR)/%.o,$($(MODULE)_SRC_FILES))
$(MODULE)_DEP_FILES := $($(MODULE)_OBJ_FILES:.o=.d)

#compile rules
$($(MODULE)_OBJ_DIR)/%.o: private MODULE := $(MODULE)
$($(MODULE)_OBJ_DIR)/%.o: $($(MODULE)_SRC_DIR)/%.cpp
	@echo "[CC]  $@"
	@mkdir -p $(dir $@)
	@$(CXX) -c $< $(CXXFLAGS) $($(MODULE)_CXXFLAGS) -o $@

#link rules
$($(MODULE)_TARGET): private MODULE := $(MODULE)
$($(MODULE)_TARGET): $($(MODULE)_OBJ_FILES)  
	@echo "[LD]  $@"
	@mkdir -p $(dir $@)
	@$(CXX) $^ $(LDFLAGS) $($(MODULE)_LDFLAGS) -o $@

#dependencies for incremental build
-include $($(MODULE)_DEP_FILES)

#build target using module name $(MODULE)
.PHONY: $(MODULE)
$(MODULE): $($(MODULE)_DEPS) $($(MODULE)_TARGET)

#clean up intermediate files and target
.PHONY: $(MODULE)-clean
$(MODULE)-clean: private MODULE := $(MODULE)
$(MODULE)-clean:
	@for f in $($(MODULE)_OBJ_FILES) $($(MODULE)_DEP_FILES) $($(MODULE)_TARGET) ; do \
		if [ -f "$$f" ] ; then \
			echo "[RM]  $$f" ; \
			rm $$f ; \
		else \
			echo "[ERR] $$f not found!"; \
		fi \
	done

#run target
.PHONY: $(MODULE)-run
$(MODULE)-run: $(MODULE)
	@echo "[RUN] $($<_TARGET)"
	@$($<_TARGET)