cmake_minimum_required(VERSION 3.22)
project(Server)

# 第三方库
find_library(SQLITE3_LIB sqlite3)

# 文件名和库名
set(PRJ_EXE ${PROJECT_NAME})
set(PRJ_LIBS ${SQLITE3_LIB})

# 头文件路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# 源代码路径
aux_source_directory(${PROJECT_SOURCE_DIR}/src/json JSONCPPS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Server SERVERCPPS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Database DBCPPS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/Tools TOOLSCPPS)

set(CPP_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp ${JSONCPPS} ${SERVERCPPS} ${DBCPPS} ${TOOLSCPPS})

# 添加可执行文件
add_executable(${PRJ_EXE} ${CPP_FILES})
target_link_libraries(${PRJ_EXE} ${PRJ_LIBS})

# 安装
install(TARGETS ${PRJ_EXE} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
