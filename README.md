# Standalone Zephyr application template

[![GitHub Build workflow status](https://github.com/kaizoku-oh/zephyr-app-workspace/workflows/Build/badge.svg)](https://github.com/kaizoku-oh/zephyr-app-workspace/actions/workflows/build.yaml)
[![GitHub release](https://img.shields.io/github/v/release/kaizoku-oh/zephyr-app-workspace)](https://github.com/kaizoku-oh/zephyr-app-workspace/releases)
[![GitHub issues](https://img.shields.io/github/issues/kaizoku-oh/zephyr-app-workspace)](https://github.com/kaizoku-oh/zephyr-app-workspace/issues)
![GitHub top language](https://img.shields.io/github/languages/top/kaizoku-oh/zephyr-app-workspace)
[![License](https://img.shields.io/github/license/kaizoku-oh/zephyr-app-workspace)](https://github.com/kaizoku-oh/zephyr-app-workspace/blob/main/LICENSE)

This repo can be used as a template and a playground for experimenting with a standalone Zephyr application.

## 🚀 Getting started

```bash
# Create a directory for zephyr projects and switch to it
$ mkdir zephyr-projects
$ cd zephyr-projects

# Create python virtual environment there
$ python3 -m venv zephyr-venv

# Activate it
$ source zephyr-venv/bin/activate

# Install west
(zephyr-venv) $ pip install west

# Initialize workspace for the app (main branch)
(zephyr-venv) $ west init -m https://github.com/kaizoku-oh/zephyr-app-workspace --mr main app-workspace
(zephyr-venv) $ cd app-workspace

# Update Zephyr modules
(zephyr-venv) $ west update

# Install python dependencies for zephyr and mcuboot
(zephyr-venv) $ python -m pip install -r deps/zephyr/scripts/requirements.txt
(zephyr-venv) $ python -m pip install -r deps/bootloader/mcuboot/scripts/requirements.txt

# Build the bootloader (mcuboot)
(zephyr-venv) $ west build deps/bootloader/mcuboot/boot/zephyr -d deps/bootloader/mcuboot/boot/zephyr/build -b nucleo_f767zi

# Flash the bootloader (mcuboot)
(zephyr-venv) $ west flash -d deps/bootloader/mcuboot/boot/zephyr/build

# Build the app
(zephyr-venv) $ west build app -d app/build -b nucleo_f767zi

# Flash the app
(zephyr-venv) $ west flash -d app/build

# Copy vscode workspace file from the app to the outer workspace directory
(zephyr-venv) $ cp app/linux.code-workspace .

# Open vscode workspace from within the python venv
(zephyr-venv) $ code linux.code-workspace
```
Once vscode is open you can run your workspace tasks.

## 🔨 Application footprint for NUCLEO-F767ZI

| Memory region | Used Size   | Region Size | %age Used   |
| ------------- | ----------- | ----------- | ----------- |
| FLASH         | 191046 B    | 768 KB      | 24.29%      |
| RAM           | 60768  B    | 384 KB      | 15.45%      |
| QSPI          | 0     GB    | 256 MB      | 0.00%       |
| DTCM          | 12544  B    | 128 KB      | 9.57%       |
| IDT_LIST      | 0     GB    | 2   KB      | 0.00%       |

## 🔨 Bootloader footprint for NUCLEO-F767ZI

| Memory region | Used Size   | Region Size | %age Used   |
| ------------- | ----------- | ----------- | ----------- |
| FLASH         | 32706  B    | 64  KB      | 49.91%      |
| RAM           | 24320  B    | 384 KB      | 6.18%       |
| QSPI          | 0     GB    | 256 MB      | 0.00%       |
| DTCM          | 0      B    | 128 KB      | 0.00%       |
| IDT_LIST      | 0     GB    | 2   KB      | 0.00%       |

## ✅ ToDo

- [ ] Use an event driven state machine using the SMF OS service and the message queue kernel object

- [ ] Add trinamic driver as a west module

- [ ] Add separate build status badges for each board

- [ ] Fix flashing in devcontainer

- [ ] Fix menuconfig vscode task inside devcontainer

- [ ] Add USB support to Serial class

- [ ] Solve the Shared Callback Registration problem in the Network class (broadcast an event)

- [ ] Add a script to download and setup the project (intellisense, structure, venv...)

- [ ] Automate updating the README.md Build footprint table automatically by running a post build

- [ ] Add stm32f4_disco LEDs and button to repl file

- [ ] Control actuators (servos, dc motors...) with PWM

- [ ] Add TLS to HttpClient class

- [ ] Add thread safety to user classes

- [ ] Run app in Renode

- [ ] Integrate Renode + robot in the CI workflow

- [ ] Add native simulator to vscode build task

- [ ] Add static code analysis with CodeChecker

- [ ] Add coverage testing with gcov

- [ ] Add unit tests using Ztest and run them using Twister

- [ ] Add integration tests

- [ ] Add HIL tests using GitHub local runner

- [ ] Add debouncing mechanism to Button class

- [ ] Add lambda callback to Button class

- [ ] Use the active object design pattern

- [ ] Add Local class for managing local time and geo-location

- [ ] Add MQTT client class

- [ ] Add TCP class

- [ ] Add an out-of-tree driver

- [x] Blink board LED

- [x] Shell over UART

- [x] Fix vscode launch with cortex-debug

- [x] Add multithreading code

- [x] Add die temp sensor class

- [x] Add Github CI workflow

- [x] Add GitHub badges to readme

- [x] Enable C++20

- [x] Add lambda callback to Serial class

- [x] Add a simple Network manager class

- [x] Add NVS Storage class

- [x] Add HTTP client class

- [x] Add an IoT app that reads die temperature, store it in NVS, retrieve it, format it then send it

- [x] Add vscode dev container environment

- [x] Build project with MCUBoot

- [x] Download a binary file with HTTP client

- [x] Boot on the new image after downloading the binary file

- [x] Add Updater class/service to manage firmware upgrades (on top of HttpClient, DFU subsystem)

- [x] Add console and shell over USB on stm32f4_disco

- [x] Add application events manager using zbus

- [x] Add asserts on arguments for all user class methods

- [x] Add configurable vscode tasks

## 💳 Credits
This project is generated from the [zephyr-vscode-example](https://github.com/beriberikix/zephyr-vscode-example) template by [Jonathan Beri](https://github.com/beriberikix).

It is following the [Golioth](https://github.com/golioth) recommended way to structure a zephyr app using manifest files, more on this in this [blog post](https://blog.golioth.io/improving-zephyr-project-structure-with-manifest-files/).
