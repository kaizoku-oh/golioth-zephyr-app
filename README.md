# Standalone STM32 Zephyr application

This repo can be used as a template and a playground for experimenting with a standalone Zephyr application for STM32.

## 🚀 Getting started

```bash
# Initialize workspace for the app (main branch)
$ west init -m https://github.dev/kaizoku-oh/stm32-zephyr-app --mr main workspace
$ cd workspace

# Update Zephyr modules
$ west update

# Copy vscode workspace file from the app to the outer workspace directory
$ Copy-Item –Path app/zephyr-windows.code-workspace -Destination .

# Build the app
$ west build app -d app/build -b nucleo_f767zi

# Turn on compilation database
$ west config build.cmake-args -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Retrieve `ZEPHYR_SDK_INSTALL_DIR`
$ cmake -P deps/zephyr/cmake/verify-toolchain.cmake

# Open vscode workspace
$ code zephyr-windows.code-workspace

# Open prj.conf and set your Golioth PSK credentials

# Build the app
$ west build app -d app/build -b nucleo_f767zi

# Flash the app on the board
$ STM32_Programmer_CLI -c port=swd mode=UR -w app/build/zephyr/zephyr.bin 0x08000000
```

Once the app is flashed open a serial monitor like PuTTy and reset your board.

```bash
uart:~$ 
```

## 🔨 Build footprint for NUCLEO-F767

| Memory region | Used Size   | Region Size | %age Used   |
| -----------   | ----------- | ----------- | ----------- |
| FLASH         | 232428 B    | 2   MB      | 11.08%      |
| RAM           | 68956  B    | 384 KB      | 17.54%      |
| QSPI          | 0      GB   | 256 MB      | 0.00%       |
| DTCM          | 12544  B    | 128 KB      | 9.57%       |
| IDT_LIST      | 0      GB   | 2   KB      | 0.00%       |

## ✅ ToDo

- [x] Blink board LED

- [x] Shell over UART

- [ ] Add Github CI workflow

- [ ] Update the README.md Build footprint table automatically when running CI workflow

- [ ] Add GitHub badges to readme

- [ ] Add OTA update example

- [ ] Enable C++20

## 💳 Credits
This project is generated from the [zephyr-vscode-example](https://github.com/beriberikix/zephyr-vscode-example) template by [Jonathan Beri](https://github.com/beriberikix).

It is following the [Golioth](https://github.com/golioth) recommended way to structure a zephyr app using manifest files, more on this in this [blog post](https://blog.golioth.io/improving-zephyr-project-structure-with-manifest-files/).