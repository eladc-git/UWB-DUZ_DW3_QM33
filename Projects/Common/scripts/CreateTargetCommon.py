#!/usr/bin/env python3

# SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
# SPDX-License-Identifier: LicenseRef-QORVO-2

import os
import sys
import subprocess
import argparse
import shutil


class CreateTargetCommon:
    """Class to handle the creation of a target using common settings."""

    def __init__(self, project_base, project, target_os, board):
        """
        Initialize the CreateTargetCommon instance.

        Args:
            project_base (str): The base directory of the project.
            project (str): The project name.
            target_os (str): The target operating system.
            board (str): The target board.
        """
        self.project_base = project_base
        self.project = project
        self.target_os = target_os
        self.board = board
        self.libs_path = "Libs"

    def __convert_path(self, path):
        """
        Converts the path separators to forward slashes if the operating system is Windows.

        Args:
            path (str): The input path.

        Returns:
            str: The converted path.
        """
        if os.name == 'nt':
            return path.replace("\\", "/")
        else:
            return path

    def parse_arguments(self):
        """
        Parse command-line arguments for building specified target.

        Returns:
            argparse.Namespace: Parsed arguments.
        """
        parser = argparse.ArgumentParser(description='Build specified target')
        parser.add_argument('build_type', choices=['cli', 'uci'], type=str.lower, help='Build type')
        parser.add_argument('-no-force', action='store_true', help='Do not remove old build folder')

        # Make arguments case-insensitive
        args = vars(parser.parse_args())
        return argparse.Namespace(**args)

    def run(self):
        """
        Run the target creation process.

        Warning: Only FreeRTOS target OS is supported.
        """
        args = self.parse_arguments()

        # Set environment variables based on command line options
        self.custom_cmake = f'_{args.build_type.upper()}'

        # Variables based on class attributes and default values
        target_script = f"project{self.custom_cmake}.cmake" if self.custom_cmake else ""
        build_path = os.path.join(self.project_base, 'BuildOutput', f"{self.project}{self.custom_cmake}", self.target_os, self.board)
        source_path = os.path.join(self.project_base, 'Projects', self.project, self.target_os, self.board)

        print(f"Build Output is {build_path}")
        print(f"Target script is {target_script}")

        if os.path.exists(build_path):
            if args.no_force:
                print("Directory exists and no-force was specified, exiting...")
                sys.exit()

            print(f"Removing existing build output {build_path}")
            shutil.rmtree(build_path, ignore_errors=True)

        # Build for FreeRTOS
        if self.target_os == "FreeRTOS":
            generator = "MinGW Makefiles" if os.name == 'nt' else "Unix Makefiles"
            subprocess.run(['cmake', '-S', source_path,
                            '-B', build_path,
                            '-G', generator,
                            '-DCMAKE_BUILD_TYPE=Debug',
                            '-DCMAKE_TOOLCHAIN_FILE=' + self.__convert_path(os.path.join(self.project_base, 'Projects', 'Common', 'cmakefiles', 'arm-none-eabi-gcc.cmake')),
                            '-DMY_TARGET_SCRIPT=' + self.__convert_path(os.path.join(source_path, target_script)),
                            '-DPROJECT_BASE=' + self.__convert_path(self.project_base),
                            '-DCOMMON_PATH=' + self.__convert_path(os.path.join(self.project_base, 'Projects', 'Common', 'cmakefiles')),
                            '-DPROJECT_COMMON=' + self.__convert_path(os.path.join(self.project_base, 'Projects', self.project, self.target_os, f"{self.project}-{self.target_os}-Common")),
                            f'-DLIBS_PATH={self.__convert_path(self.libs_path)}'], check=True)
