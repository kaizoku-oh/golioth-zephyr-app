# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app)

target_include_directories(app PRIVATE include)
target_include_directories(app PUBLIC ${ZEPHYR_BASE}/../modules/lib/golioth-firmware-sdk/examples/zephyr/common/include)

target_sources(app PRIVATE
  src/main.cpp
  # src/Button.cpp
  # src/Led.cpp
  # src/Temperature.cpp
  # src/Serial.cpp
  # src/Network.cpp
  # src/Storage.cpp
  # src/HttpClient.cpp
  # src/EventManager.cpp
  # src/Updater.cpp
  # src/AppAcquisition.cpp
  # src/AppTelemetry.cpp
  # src/AppEventDrivenFsm.cpp
  # src/app_golioth.c
)
