# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

import sys
import os
import subprocess
import platform
import argparse


def flash_firmware(firmware_path, cpu):
    """
    A function to flash firmware to a device using JLink.

    Args:
        firmware_path (str): The path to the firmware file.
        cpu (str): The CPU identifier.
    Returns:
        None
    """

    # Determine the JLink executable based on the operating system
    jlink_exe = 'JLinkExe' if platform.system() == 'Linux' else 'JLink.exe'

    # Construct the JLink command
    commands = [
        'si 1',                         # Select SWD interface
        'speed 4000',                   # Set speed to 4000 kHz
        f'device {cpu}_xxaa',           # Specify the device
        f'loadfile {firmware_path}',    # Load the firmware file
        'r',                            # Reset the device
        'g',                            # Start the device
        'exit',                         # Exit JLinkExe
    ]
    command_str = '\n'.join(commands)

    # Create a temporary file to store the commands
    script_file_path = "script.jlink"
    with open(script_file_path, 'w') as script_file:
        script_file.write(command_str)

    cmd =  (jlink_exe, "-ExitOnError", "1", "-CommanderScript", script_file_path)

    # Run the JLink command
    try:
        result = subprocess.run(cmd, text=True, capture_output=True, check=True)
        # Print the JLink output
        print(result.stdout)
        if result.returncode != 0:
            print("Error flashing firmware:", result.stderr, file=sys.stderr)
            sys.exit(result.returncode)
        else:
            print("Firmware successfully flashed.")
    except FileNotFoundError:
        print(f"Error: {jlink_exe} not found. Please make sure it is installed and in your PATH.", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"An error occurred: {e}", file=sys.stderr)
        sys.exit(1)
    finally:
        os.remove(script_file_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Flash firmware to a device using JLink.")
    parser.add_argument('firmware_path', type=str, help="Path to the firmware file.")
    parser.add_argument('cpu', type=str, help="CPU identifier (e.g., nrf52833).")
    args = parser.parse_args()

    # Check if the firmware file exists
    if not os.path.isfile(args.firmware_path):
        print(f"Error: The firmware file {args.firmware_path} does not exist.", file=sys.stderr)
        sys.exit(1)

    # Flash the firmware
    flash_firmware(args.firmware_path, args.cpu)
