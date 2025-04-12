# 编译器设置
CXX := g++
CXXFLAGS := -I./include -Wall -Wextra -g

# 定义模块目录
MODULES := tree sort graph

# 自动获取所有目标文件（各模块bin目录下的可执行文件）
TARGETS := $(foreach mod,$(MODULES),\
            $(patsubst $(mod)/src/%.cpp,$(mod)/bin/%,$(wildcard $(mod)/src/*.cpp)))

# 默认目标
all: $(TARGETS)

# 定义生成规则模板
define MODULE_RULE
$(1)/bin/%: $(1)/src/%.cpp
	@mkdir -p $$(@D)
	$(CXX) $(CXXFLAGS) $$< -o $$@
endef

# 为每个模块应用规则模板
$(foreach mod,$(MODULES),$(eval $(call MODULE_RULE,$(mod))))

# 清理目标
clean:
	rm -rf $(foreach mod,$(MODULES),$(mod)/bin/*)

.PHONY: all clean