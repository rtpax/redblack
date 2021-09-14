CXX := g++

RELEASE_DIR := release
RELEASE_CXX_FLAGS := -std=c++17 -O2 -g -Wall -Wno-sign-compare -I.
RELEASE_CPP_FLAGS := -DNDEBUG
DEBUG_DIR := debug
DEBUG_CXX_FLAGS := -std=c++17 -O0 -fno-inline -g3 -Wall -Wno-sign-compare -I.
DEBUG_CPP_FLAGS := -DDEBUG
PROFILE_DIR := profile
PROFILE_CXX_FLAGS := -std=c++17 -O2 -pg -Wall -Wno-sign-compare -I.
PROFILE_CPP_FLAGS := -DNDEBUG

BUILD_DIR := $(RELEASE_DIR)
CXXFLAGS := $(RELEASE_CXX_FLAGS)
CPPFLAGS := $(RELEASE_CPP_FLAGS)

ifeq ($(config), release)
BUILD_DIR := $(RELEASE_DIR)
CXXFLAGS := $(RELEASE_CXX_FLAGS)
CPPFLAGS := $(RELEASE_CPP_FLAGS)
endif
ifeq ($(config), debug)
BUILD_DIR := $(DEBUG_DIR)
CXXFLAGS := $(DEBUG_CXX_FLAGS)
CPPFLAGS := $(DEBUG_CPP_FLAGS)
endif
ifeq ($(config), profile)
BUILD_DIR := $(PROFILE_DIR)
CXXFLAGS := $(PROFILE_CXX_FLAGS)
CPPFLAGS := $(PROFILE_CPP_FLAGS)
endif

INSTALL_DIR := ~/bin

LD_LIBS :=

SHARED_SRCS :=

T_SRCS := test/test.cc test/custom_allocator.cc $(SHARED_SRCS)
T_OBJS := $(T_SRCS:%.cc=$(BUILD_DIR)/%.o)
T_DEPS := $(T_SRCS:%.cc=$(BUILD_DIR)/%.d)

SRCS := main.cc $(SHARED_SRCS)
OBJS := $(SRCS:%.cc=$(BUILD_DIR)/%.o)
DEPS := $(SRCS:%.cc=$(BUILD_DIR)/%.d)

.PHONY: clean clean-objs all main test install

main: $(BUILD_DIR) $(BUILD_DIR)/test $(BUILD_DIR)/rb_tree

test: $(BUILD_DIR) $(BUILD_DIR)/runtest

all: main test

# install: $(INSTALL_DIR)/main 

$(INSTALL_DIR)/main: $(BUILD_DIR) $(BUILD_DIR)/dif
	cp $(BUILD_DIR)/dif $(INSTALL_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/test: $(BUILD_DIR)
	mkdir $(BUILD_DIR)/test

$(BUILD_DIR)/rb_tree: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LD_LIBS) -o $(BUILD_DIR)/rb_tree

$(BUILD_DIR)/runtest: $(T_OBJS)
	$(CXX) $(T_OBJS) $(CXXFLAGS) $(LD_LIBS) -o $(BUILD_DIR)/runtest

$(BUILD_DIR)/%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MT $@ > $(BUILD_DIR)/$*.d

-include $(DEPS) $(T_DEPS)

clean-objs:
	rm -rf $(OBJS) $(T_OBJS)
	rm -rf $(DEPS) $(T_DEPS)

clean:
	rm -rf $(BUILD_DIR)
