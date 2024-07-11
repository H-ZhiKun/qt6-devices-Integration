cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME DeviceIntegration)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)
# 定义全局预编译宏命令
add_definitions(-DNOMINMAX)
add_definitions(-DWIN32_LEAN_AND_MEAN)
# 全局预配置项
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
# 根据 CMAKE_BUILD_TYPE 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../${CMAKE_BUILD_TYPE}")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(3RD_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(RES_DIR ${CMAKE_SOURCE_DIR}/res)

message("build type: ${CMAKE_BUILD_TYPE}")
# 使用环境变量指定vcpkg.cmake路径
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Please set CMAKE_TOOLCHAIN_FILE environment variable to the location of vcpkg.cmake")
endif()

include_directories(${3RD_ROOT_DIR}/include/
                    ${CMAKE_SOURCE_DIR}/src/app
                    ${CMAKE_SOURCE_DIR}/src/app/interface
                    ${CMAKE_SOURCE_DIR}/src/utils
                    ${CMAKE_SOURCE_DIR}/src/sql
                    ${CMAKE_SOURCE_DIR}/src/network
                    ${CMAKE_SOURCE_DIR}/src/plc
                    ${CMAKE_SOURCE_DIR}/src/camera
                    ${CMAKE_SOURCE_DIR}/src/http
                    ${CMAKE_SOURCE_DIR}/src/product
                    ${CMAKE_SOURCE_DIR}/src/statistics
                    ${CMAKE_SOURCE_DIR}/src/gpu
                    ${CMAKE_SOURCE_DIR}/src/test
                    )

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Multimedia Network Sql QuickControls2 Widgets WebSockets)

find_package(jsoncpp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(Xlnt CONFIG REQUIRED)
find_package(libmodbus CONFIG REQUIRED)


qt_standard_project_setup(REQUIRES 6.5)

#使用aux_source_directory把当前目录下的源文件存列表存放到变量SRC_LIST里
aux_source_directory (./src/utils SRC_UTILS)
aux_source_directory (./src/sql SRC_SQL)
aux_source_directory (./src/network SRC_NETWORK)
aux_source_directory (./src/plc SRC_PLC)
aux_source_directory (./src/camera SRC_CAMERA)
aux_source_directory (./src/http SRC_HTTP)
aux_source_directory (./src/product SRC_PRODUCT)
aux_source_directory (./src/gpu SRC_GPU)
aux_source_directory (./src/statistics SRC_STATISTICS)
aux_source_directory (./src/test SRC_TEST)
aux_source_directory (./src/app SRC_APP)
aux_source_directory (./src/app/interface SRC_APP_INTERFACE)

qt_add_executable(${PROJECT_NAME}
        src/main.cpp
        ${SRC_APP}
        ${SRC_APP_INTERFACE}
        ${SRC_STATISTICS}
        ${SRC_SQL}
        ${SRC_UTILS}
        ${SRC_NETWORK}
        ${SRC_PLC}
        ${SRC_CAMERA}
        ${SRC_HTTP}
        ${SRC_PRODUCT}
        ${SRC_GPU}
        ${SRC_TEST}
        res/logo.rc
)

qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

########    获取 qml 文件    ########
file(GLOB_RECURSE QML_FILES_ELEMENT qml/element/*.qml)
file(GLOB_RECURSE QML_FILES_ROOT qml/*.qml)

# 将两个文件夹的文件合并到 QML_PATHS
set(QML_PATHS ${QML_FILES_ELEMENT} ${QML_FILES_ROOT})

foreach(filepath ${QML_PATHS})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    message("qml file: ${filename}")
    list(APPEND QML_FILES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES QML_FILES) #表示删除非第一个重名值
# 添加QML模块
qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QML_FILES}
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# 设置库文件夹路径
set(THIRD_PARTY_LIB_DIR "${3RD_ROOT_DIR}/lib")

# 使用 file(GLOB ...) 获取库文件列表
file(GLOB THIRD_PARTY_LIBS "${THIRD_PARTY_LIB_DIR}/*.lib")  # 根据实际情况修改文件扩展名

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::WebSockets
    JsonCpp::JsonCpp
    ZLIB::ZLIB
    fmt::fmt
    spdlog::spdlog
    cryptopp::cryptopp
    xlnt::xlnt
    yaml-cpp::yaml-cpp
    modbus
    ${OpenCV_LIBS}
    ${THIRD_PARTY_LIBS}  # 添加所有第三方库文件
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${3RD_ROOT_DIR}/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${RES_DIR}/ico $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
)