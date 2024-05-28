cmake_minimum_required (VERSION 3.5)                # cmake版本最低要求
project (Paillier)  # 设置工程名称
 
# 打印信息
message(STATUS "CMAKE_CURRENT_SOURCE_DIR PATH IS : " ${CMAKE_CURRENT_SOURCE_DIR})

# 将src下面的所有头文件路径保存至 all_head_files 数组变量中
# 将src下面的所有源文件路径保存至 all_source_files 数组变量中
FILE(GLOB_RECURSE all_source_files "src/*.cpp" "src/*.c")
FILE(GLOB_RECURSE all_head_files "src/*.hpp" "src*.h")


# 添加可执行文件
ADD_EXECUTABLE(
            paillier
            WIN32 # Windows 非命令行程序
            ${all_head_files}
            ${all_source_files}
             )
 
# 链接库文件
target_link_libraries(paillier PRIVATE gmpxx gmp)