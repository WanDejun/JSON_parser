# Makefile
TARGET:=main
CC:=g++
PP:=g++
CFLAGS_CPP = -g -std=c++17
CFLAGS_C = -g 

# 头文件路径
INCSDIR:=./inc/
INCSDIR+=${shell ls -d ./inc/*/ 2>/dev/null}

# 源文件根目录
SRCSROOT:=./

OBJSDIR:=./obj/

# 源文件路径
SRCSDIR:=./src/
SRCSDIR+=${shell ls -d ./src/*/ 2>/dev/null}

# 文件名列表
# include(.h)
INCS:=$(foreach dir,$(INCSDIR),-I$(dir))

# sources(.c/.cpp)
SRCS_CPP:=$(foreach dir,$(SRCSDIR),$(wildcard $(dir)*.cpp))
SRCS_C:=$(foreach dir,$(SRCSDIR),$(wildcard $(dir)*.c))


# 文件名列表
# include(.h)
INCS:=$(foreach dir,$(INCSDIR),-I$(dir))

# sources(.c/.cpp)
SRCS_CPP:=$(foreach dir,$(SRCSDIR),$(wildcard $(dir)*.cpp))
SRCS_C:=$(foreach dir,$(SRCSDIR),$(wildcard $(dir)*.c))

# objects(.o)
OBJS:=$(patsubst $(SRCSROOT)%.cpp,$(OBJSDIR)%.o,$(SRCS_CPP))
OBJS+=$(patsubst $(SRCSROOT)%.c,$(OBJSDIR)%.o,$(SRCS_C))

# depends(.d)
DEPS:=$(patsubst %.o,%.d,$(OBJS))

# ALL : 
# 	@echo $(INCS)
# 	@echo SRCS_CPP: $(SRCS_CPP)
# 	@echo SRCS_C: $(SRCS_C)
# 	@echo OBJS: $(OBJS)
# 	@echo DEPS: $(DEPS)

#######################链接######################
$(TARGET) : $(OBJS)
	@echo linking...
	$(PP) $^ -o $@
	@echo comlpete!

#######################编译######################

# 编译.cpp
# -MMD -MP 用于获取.d文件(头文件引用依赖文件)
$(OBJSDIR)%.o : $(SRCSROOT)%.cpp
	@echo compiling $<
	@mkdir -p $(dir $@)
	$(PP) -MMD -MP $(CFLAGS_CPP) -c $(INCS) $< -o $@ 

# 编译.c
$(OBJSDIR)%.o : $(SRCSROOT)%.c
	@echo compiling $<
	@mkdir -p $(dir $@)
	$(CC) -MMD -MP $(CFLAGS_C) -c $(INCS) $< -o $@ 

.PHONY : clean
# 清理中间文件
clean:
	@echo try to clean...
	@rm -rf $(OBJS) $(DEPS)
	@echo complete!

.PHONY : run
run:
	./main

# 处理依赖
-include $(DEPS)
