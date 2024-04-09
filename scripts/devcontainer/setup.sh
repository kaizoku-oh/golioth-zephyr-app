# Initialize the west repository in the 'app' subdirectory
west init -l app

# Make sure the workspace contains Git repositories matching the projects in the manifest file
west update

# Export Zephyr-specific build system metadata to the 'app' directory
west zephyr-export

# Install Python dependencies required by Zephyr
pip install -r deps/zephyr/scripts/requirements.txt

# Install Python dependencies required by MCUBoot
pip install -r deps/bootloader/mcuboot/scripts/requirements.txt

# Copy nucleo_u575zi_q overlay to mcuboot
cp app/boards/nucleo_u575zi_q.overlay deps/bootloader/mcuboot/boot/zephyr/boards