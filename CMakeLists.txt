cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME DeviceIntegration)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(3RD_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party)

include_directories(${3RD_ROOT_DIR}/include/
                    ${3RD_ROOT_DIR}/include/Siso
                    ${3RD_ROOT_DIR}/include/Baumer
                    ${CMAKE_SOURCE_DIR}/src/utils
                    ${CMAKE_SOURCE_DIR}/src/sql
                    ${CMAKE_SOURCE_DIR}/src/domino
                    ${CMAKE_SOURCE_DIR}/src/plc
                    ${CMAKE_SOURCE_DIR}/src/app
                    ${CMAKE_SOURCE_DIR}/src/camera
                    ${CMAKE_SOURCE_DIR}/src/camera/Baumer)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Network Sql QuickControls2 Widgets)

qt_standard_project_setup(REQUIRES 6.5)

#使用aux_source_directory把当前目录下的源文件存列表存放到变量SRC_LIST里
aux_source_directory (./src/utils SRC_UTILS)
aux_source_directory (./src/sql SRC_SQL)
aux_source_directory (./src/app SRC_APP)
aux_source_directory (./src/domino SRC_DOMINO)
aux_source_directory (./src/plc SRC_PLC)
aux_source_directory (./src/camera SRC_CAMERA)
aux_source_directory (./src/camera/Baumer SRC_BAUMER)

qt_add_executable(${PROJECT_NAME}
        src/main.cpp
        ${SRC_SQL}
        ${SRC_UTILS}
        ${SRC_DOMINO}
        ${SRC_PLC}
        ${SRC_APP}
        ${SRC_CAMERA}
        ${SRC_BAUMER}
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
        qml/Main.qml
        qml/App.qml
        qml/Alarm.qml
        qml/CameraParam.qml
        qml/FormulaPage.qml
        qml/PowerPage.qml
        qml/ProduceData.qml
        qml/SensorPage.qml
        qml/UserPage.qml
        qml/ValvePage.qml 
        qml/MySwitch.qml  
        qml/ComponentList.qml   
        qml/Master.qml 
        qml/MainWindow.qml 
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# debug 版本使用

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::QuickControls2
    Qt6::Widgets
    ${3RD_ROOT_DIR}/lib/vld.lib
    ${3RD_ROOT_DIR}/lib/jsoncppd.lib
    ${3RD_ROOT_DIR}/lib/fmtd.lib
    ${3RD_ROOT_DIR}/lib/spdlogd.lib
    ${3RD_ROOT_DIR}/lib/modbusd.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_ext_sc.lib
    ${3RD_ROOT_DIR}/lib/bgapi2_genicam.lib
    ${3RD_ROOT_DIR}/lib/iolibrt.lib

    ${3RD_ROOT_DIR}/lib/xlntd.lib
)

# release 版本使用
#[[
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::QuickControls2
    ${3RD_ROOT_DIR}/lib/zlib.lib
    ${3RD_ROOT_DIR}/lib/jsoncpp.lib
    ${3RD_ROOT_DIR}/lib/fmt.lib
    ${3RD_ROOT_DIR}/lib/spdlog.lib
    ${3RD_ROOT_DIR}/lib/modbus.lib
    ${3RD_ROOT_DIR}/lib/xlnt.lib
)
]]
# debug 版本使用

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${3RD_ROOT_DIR}/bin/debug $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# release 版本使用
#[[
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${3RD_ROOT_DIR}/bin/release $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
]]