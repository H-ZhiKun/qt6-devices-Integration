cmake_minimum_required(VERSION 3.16)
# Set a default build type if none was specified
# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#   message("Setting build type to 'RelWithDebInfo' as none was specified.")
#   set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
#   # Set the possible values of build type for cmake-gui
#   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
#     "MinSizeRel" "RelWithDebInfo")
# endif()

# 设置默认构建类型为 Debug
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build.")
# 输出当前的编译类型
message("build type: ${CMAKE_BUILD_TYPE}")

set(PROJECT_NAME DeviceIntegration)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
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
        qml/PowerPage.qml
        qml/ProduceData.qml
        qml/SensorPage.qml
        qml/UserPage.qml
        qml/ValvePage.qml      
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # debug 版本使用
    message("target link Debug")
    target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Multimedia
    ${3RD_ROOT_DIR}/lib/vld.lib
    ${3RD_ROOT_DIR}/lib/zlibd.lib
    ${3RD_ROOT_DIR}/lib/jsoncppd.lib
    ${3RD_ROOT_DIR}/lib/fmtd.lib
    ${3RD_ROOT_DIR}/lib/spdlogd.lib
    ${3RD_ROOT_DIR}/lib/modbusd.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_ext_sc.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_genicam.lib
    ${3RD_ROOT_DIR}/lib/xlntd.lib
    ${3RD_ROOT_DIR}/lib/opencv_world453d.lib
    ${3RD_ROOT_DIR}/lib/cryptlibd.lib
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${3RD_ROOT_DIR}/bin/debug $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${QML_DIR}/ico $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
    )
else()
    # release 版本使用
    message("target link Release")
    target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Multimedia
    ${3RD_ROOT_DIR}/lib/zlib.lib
    ${3RD_ROOT_DIR}/lib/jsoncpp.lib
    ${3RD_ROOT_DIR}/lib/fmt.lib
    ${3RD_ROOT_DIR}/lib/spdlog.lib
    ${3RD_ROOT_DIR}/lib/modbus.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_ext_sc.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_genicam.lib
    ${3RD_ROOT_DIR}/lib/xlnt.lib
    ${3RD_ROOT_DIR}/lib/opencv_world453.lib
    ${3RD_ROOT_DIR}/lib/cryptlib.lib
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${3RD_ROOT_DIR}/bin/release $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${QML_DIR}/ico $<TARGET_FILE_DIR:${PROJECT_NAME}>/ico
    )
endif()
