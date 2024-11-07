# 设置 CMake 最低版本要求  
cmake_minimum_required(VERSION 3.10)  
  
# 设置项目名称和版本  
project(LittleRGS)  
  
# 指定 C++ 标准（例如 C++11, C++14, C++17 等）  
set(CMAKE_CXX_STANDARD 17)  
  
# 添加包含目录（如果有头文件目录需要添加）  
include_directories("libs/eigen3")  
  
# 添加源文件  
set(SOURCE_FILES  
    "src/Application.cpp"
  "src/Window.cpp" "src/WindowsWindow.cpp")  

# 添加头文件
set(HEADER_FILES  
    "src/DrawLine.h"
 "src/Application.h" "src/Window.h" "src/WindowsWindow.h" "src/Base.h")  

# 将源文件编译成可执行文件  
add_executable(MyExecutable ${SOURCE_FILES} ${HEADER_FILES})  
  
# 链接库（如果有需要链接的库）  
# target_link_libraries(MyExecutable PRIVATE SomeLibrary)  
  
# 设置编译选项（可选）  
# set_target_properties(MyExecutable PROPERTIES  
#     CXX_FLAGS "-Wall -Wextra -pedantic"  
# )  
  
# 安装目标（可选，如果需要安装生成的可执行文件或库）  
# install(TARGETS MyExecutable DESTINATION /usr/local/bin)  
  
# 添加测试（可选，如果需要使用 CTest 进行测试）  
# enable_testing()  
# add_test(NAME MyTest COMMAND MyExecutable)