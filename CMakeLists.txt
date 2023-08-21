cmake_minimum_required(VERSION 3.16)
# Set a default build type if none was specified
# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#   message("Setting build type to 'RelWithDebInfo' as none was specified.")
#   set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
#   # Set the possible values of build type for cmake-gui
#   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
#     "MinSizeRel" "RelWithDebInfo")
# endif()
set(CMAKE_BUILD_TYPE Debug)
include(F:/workspace/vcpkg/scripts/buildsystems/vcpkg.cmake)
# 设置默认构建类型为 Debug
# set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build.")
# 输出当前的编译类型
message("build type: ${CMAKE_BUILD_TYPE}")
add_definitions(-DNOMINMAX)
# 根据 CMAKE_BUILD_TYPE 设置输出目录
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Release)
endif()

set(PROJECT_NAME DeviceIntegration)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(3RD_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(QML_DIR ${CMAKE_SOURCE_DIR}/qml)

include_directories(${3RD_ROOT_DIR}/include/
                    ${CMAKE_SOURCE_DIR}/src/utils
                    ${CMAKE_SOURCE_DIR}/src/sql
                    ${CMAKE_SOURCE_DIR}/src/domino
                    ${CMAKE_SOURCE_DIR}/src/plc
                    ${CMAKE_SOURCE_DIR}/src/app
                    ${CMAKE_SOURCE_DIR}/src/camera
                    ${CMAKE_SOURCE_DIR}/src/cognex
                    ${CMAKE_SOURCE_DIR}/src/http)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Multimedia Network Sql QuickControls2 Widgets)

find_package(jsoncpp CONFIG REQUIRED)
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
aux_source_directory (./src/app SRC_APP)
aux_source_directory (./src/domino SRC_DOMINO)
aux_source_directory (./src/plc SRC_PLC)
aux_source_directory (./src/camera SRC_CAMERA)
aux_source_directory (./src/cognex SRC_COGNEX)
aux_source_directory (./src/http SRC_HTTP)


qt_add_executable(${PROJECT_NAME}
        src/main.cpp
        ${SRC_SQL}
        ${SRC_UTILS}
        ${SRC_DOMINO}
        ${SRC_PLC}
        ${SRC_CAMERA}
        ${SRC_APP}
        ${SRC_COGNEX}
        ${SRC_HTTP}
        logo.rc
)

qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)



qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES 
        qml/AddUser.qml
        qml/Alarm.qml
        qml/Camera.qml
        qml/CameraParam.qml
        qml/ComponentList.qml
        qml/FormulaPage.qml
        qml/Login.qml
        qml/MainWindow.qml
        qml/Master.qml 
        qml/ModifyUser.qml
        qml/ModifyUserInfo.qml
        qml/MySwitch.qml
        qml/PageButton.qml
        qml/PageComponent.qml
        qml/PowerPage.qml
        qml/ProduceData.qml
        qml/SensorPage.qml
        qml/UserPage.qml
        qml/ValvePage.qml      
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Multimedia
    JsonCpp::JsonCpp
    ZLIB::ZLIB
    fmt::fmt
    spdlog::spdlog
    cryptopp::cryptopp
    xlnt::xlnt
    modbus
    ${OpenCV_LIBS}
    ${3RD_ROOT_DIR}/lib/vld.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_ext_sc.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_genicam.lib
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${3RD_ROOT_DIR}/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${QML_DIR}/ico $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
)