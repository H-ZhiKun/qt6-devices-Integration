# Device Integration Project

This is a C++ project for device integration, primarily used for managing and controlling various devices such as Cognex vision systems, Domino printers, PLC devices, Baumer cameras, etc. Additionally, the project includes an HTTP API manager for handling interactions with HTTP APIs and a set of tools for interacting with a MySQL database. Below, we will provide detailed descriptions of each component's functionality and usage.

## Table of Contents

1. [Main Program](#main-program)
2. [Cognex](#cognex)
3. [HTTP API Manager](#http-api-manager)
4. [Domino](#domino)
5. [Tools](#tools)
6. [Application Framework](#application-framework)
7. [PLC Devices](#plc-devices)
8. [Camera](#camera)
9. [SQL](#sql)
10. [Build and Run](#build-and-run)

## Main Program

`main.cpp` serves as the entry point of the entire project, responsible for initializing various components and managing the application's execution. It mainly handles the following functionalities:

- Initializing all devices and services.
- Handling events from devices and services.
- Managing the lifecycle of devices and services.

## Cognex

The Cognex section contains the code for interacting with Cognex vision systems. The specific implementation resides in `Cognex.h` and `Cognex.cpp`. This component offers the following functionalities:

- Connecting and disconnecting from the Cognex vision system.
- Sending and receiving data.
- Handling events from the Cognex vision system.

## HTTP API Manager

The `HttpApiManager` section's code is responsible for handling interactions with HTTP APIs. This component provides the following functionalities:

- Sending requests to the HTTP API.
- Receiving and handling responses from the HTTP API.
- Managing the state of the HTTP API.

## Domino

The Domino section's code includes the functionalities for interacting with Domino printers. The specific implementation can be found in `Domino.h` and `Domino.cpp`. This component provides the following functionalities:

- Connecting and disconnecting from the Domino printer.
- Sending and receiving data.
- Handling events from the Domino printer.

## Tools

The code in the Tools directory contains several utility functions and classes, offering the following functionalities:

- `TaskQueue`: A thread-safe task queue, allowing safe addition and removal of tasks across multiple threads.
- `LockFreeQueue`: A lock-free queue using atomic operations to ensure thread safety in multi-threaded environments.
- `ModbusClient`: A Modbus client for communicating with devices supporting the Modbus protocol.
- `TCPClient`: A TCP client for TCP communication.
- Other utilities: Including tools for logging, time management, string processing, and more.

## Application Framework

The Application Framework section's code implements the main logic and functionalities of the application. This component provides the following functionalities:

- `ImagePainter`: A tool for image drawing, supporting multiple image formats.
- `AppMetaFlash`: A tool for managing application metadata, supporting reading and writing metadata.

## PLC Devices

The PLC Devices section's code handles interactions with PLC devices. This component offers the following functionalities:

- Connecting and disconnecting from PLC devices.
- Reading and writing data to PLC devices.
- Handling events from PLC devices.

## Camera

The Camera section's code includes the functionalities for interacting with Baumer cameras and related buffer information. This component provides the following functionalities:

- Connecting and disconnecting from Baumer cameras.
- Controlling the settings of Baumer cameras.
- Receiving and handling image data from Baumer cameras.

## SQL

The code in the SQL directory handles the connection and queries to the MySQL database. This component provides the following functionalities:

- `DBConnectionPool` and `MysqlConnectionPool`: These two classes manage database connections, supporting safe acquisition and release of connections in a multi-threaded environment.
- `SqlHelper`: A tool for simplifying SQL queries, supporting various SQL operations.

## Build and Run

To build and run the project, follow these steps:

1. Ensure you have a C++ compiler and related development tools installed on your system.
2. Download or clone this project to your local machine.
3. Run the build command in the root directory of the project (specific commands may vary depending on your system and compiler).
4. If the build is successful, you will get an executable file. Running this file will start the application.