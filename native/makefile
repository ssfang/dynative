# http://stackoverflow.com/questions/7751640/undefined-reference-to-gxx-personality-sj0
# gcc is just a driver progam that runs the correct compiler, and for a .cpp or .cc 
# file it will run the C++ compiler. But unlike the g++ driver it doesn't automatically 
# link to the C++ Standard Library. This is documented in the manual, of course. 
# https://gcc.gnu.org/onlinedocs/gcc/Invoking-G_002b_002b.html
GCC:=x86_64-w64-mingw32-g++ -ggdb -std=c++11 -Wno-invalid-offsetof
TARGET_DIR:=../lib/
OBJ_DIR:=../obj/

LOCAL_SRC_FILES:=jn.cpp dynative.c resolve_x64.S
INC_DIR := $(JAVA_HOME)/include $(JAVA_HOME)/include/win32
LDFLAGS:=-Wl,--add-stdcall-alias

#inc_dir := $(foreach d,$(sort $(INC_DIR) $(SRC_DIR)),-I$d)
inc_dir := $(addprefix -I,$(INC_DIR))

#CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
#CXXFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

# CFLAGS  += -fpic -shared
# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
LDFLAGS += -shared

TARGET:=$(addprefix $(TARGET_DIR),jn.dll)
#C_OBJS:=$(filter %.c %.s, $(LOCAL_SRC_FILES))
## $(basename $(LOCAL_SRC_FILES)) leads LOCAL_SRC_FILES=x.c x.cpp to x.o
OBJS:=$(addprefix $(OBJ_DIR),$(addsuffix .o, $(LOCAL_SRC_FILES)))

MKDIRS:=mkdir -p

## http://www.gnu.org/software/make/manual/html_node/Phony-Targets.html#Phony-Targets
PHONY = all clean

all : $(TARGET)
	$(info [^-^] JAVA_HOME = "$(JAVA_HOME)")
	
# $@ matches the target, $< matches the first dependancy, $^ matches all dependencies
$(TARGET): $(OBJS)
	$(MKDIRS) $(TARGET_DIR)
	$(GCC) $(LDFLAGS) -o $@ $^

## http://www.gnu.org/software/make/manual/make.html#Eval-Function
#define GENERATE_OBJS_template 
#$(OBJ_DIR)$(1).o: $(1)
#	$(GCC) $(inc_dir) $(CFLAGS) -c $(1) -o $(OBJ_DIR)$(1).o
#endef
#$(foreach srcfile,$(LOCAL_SRC_FILES),$(eval $(call GENERATE_OBJS_template,$(srcfile))))

# http://www.gnu.org/software/make/manual/make.html#Pattern-Rule-Examples
$(OBJ_DIR)%.cpp.o: %.cpp 
	# gcc -c: Compile or assemble the source files without linking
	$(GCC) $(inc_dir) $(CFLAGS) -c -o $@ $<
$(OBJ_DIR)%.c.o: %.c 
	# gcc -c: Compile or assemble the source files without linking
	$(GCC) $(inc_dir) $(CFLAGS) -c -o $@ $<
$(OBJ_DIR)%.S.o: %.S 
	# gcc -c: Compile or assemble the source files without linking
	$(GCC) $(inc_dir) $(CFLAGS) -c -o $@ $<
	
## http://www.gnu.org/savannah-checkouts/gnu/make/manual/html_node/Prerequisite-Types.html#Prerequisite-Types
$(OBJS): | $(OBJ_DIR)
$(TARGET): | $(TARGET_DIR)

${TARGET_DIR}:
	$(MKDIRS) $@
${OBJ_DIR}:
	$(MKDIRS) $@

clean :
	rm $(TARGET) $(OBJS) $(TARGET)