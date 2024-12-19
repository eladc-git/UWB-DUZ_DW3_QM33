# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

"""
This script serves as a helper for managing build and flash tasks in VSCode.
It allows tasks to store build type, board type, and CPU type in the project_config.json file.
The script is designed with specific functionality and is intended to be utilized solely with
the provided tasks.json file.

Usage:
    python script.py <build> <board>

Arguments:
    build: Build variant, currently CLI or UCI
    board: Board to be used, should be one of the keys in cpu_mapping dict

This script generates a json file named project_config.json with the following structure:
{
    "build": <build>,
    "board": <board>,
    "cpu": <cpu>
}

Where <cpu> is a value from cpu_mapping dict based on the <board> value.
"""

import json
import sys
import argparse

cpu_mapping = {
    "nRF52840DK": "nrf52840",
    "DWM3001CDK": "nrf52833",
    "Type2AB_EVB": "nrf52840"
}

builds = ["CLI", "UCI"]


def create_json_file(build, board):
    """
    Creates a json file with project configuration.

    Args:
        build: Build variant, currently CLI or UCI
        board: Board to be used, should be one of the keys in cpu_mapping dict
    """
    data = {
        "build": build,
        "board": board,
        "cpu": cpu_mapping.get(board)
    }
    with open('../project_config.json', 'w', encoding='utf-8') as json_file:
        json.dump(data, json_file, indent=4)
        json_file.write('\n')

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="This script manages build and flash tasks in VSCode.")
    parser.add_argument('build', choices=builds, help="Build variant, currently CLI or UCI")
    parser.add_argument('board', choices=cpu_mapping.keys(), help="Board to be used, should be one of the keys in cpu_mapping dict")

    args = parser.parse_args()

    create_json_file(args.build, args.board)
