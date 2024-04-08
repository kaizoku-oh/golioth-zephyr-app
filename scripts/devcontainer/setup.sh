west init -l app
west update
west zephyr-export
pip install -r deps/zephyr/scripts/requirements.txt
pip install -r deps/bootloader/mcuboot/scripts/requirements.txt
