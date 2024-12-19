# DW3 QM33 SDK #

This readme describes how to build and run provided code and examples from QM33 SDK on the development kit. For more information about functionalities please refer to Developer Manual from SDK package.

## Required tools

The following tools are required to build and run the examples:

### Windows

- **ARM Toolchain**:
  - Download **arm-none-eabi-gcc-10.3-2021.10-win32** from: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads.
  - Install it inside: **C:\\GnuToolsArmForEmbedded\\gcc-arm-none-eabi-10.3-2021.10\\bin** to align with VS Code.
  - Add `C:\\GnuToolsArmForEmbedded\\gcc-arm-none-eabi-10.3-2021.10\\bin` to your system/user environment variables.

- **CMake**:
  - Download CMake from: https://cmake.org/download/. Make sure to use CMake version higher than 3.23.
  - Install CMake inside `<cmake_install_dir>`.
  - Add `<cmake_install_dir>/bin` to your system/user environment variables.

- **Python**:
  - Download Python from: https://www.python.org/downloads/. Recommended version is 3.10.
  - Install Python and make sure to select the option "Add Python to PATH" during the installation.

- **MinGW and make**:
  - Mingw can be downloaded from: https://sourceforge.net/projects/mingw/.
  - Install Mingw in the directory of your choice.
  - Using the MinGW Installation Manager, install `mingw32-base`.
  - Create a copy of `<install path>/bin/mingw32-make.exe` and name it `<install path>/bin/make.exe`.
  - Add `<install path>/bin` to your system/user environment variables.

- **SEGGER J-Link**:
  - Install J-Link Software and Documentation Pack from: https://www.segger.com/downloads/jlink/.

- **VS Code**:
  - Install VS Code from: https://code.visualstudio.com/.


### Linux

- **ARM Toolchain**

  - Create an install directory:

    ```bash
    mkdir /opt/gcc
    ```

  - Go to the install directory:

    ```bash
    cd /opt/gcc
    ```

  - Download arm-none-eabi gcc from: https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2

  - Extract the tarball:

    ```bash
    tar -xvf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
    ```

  - Remove the tarball:

    ```bash
    rm gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
    ```

  - open **~/.bashrc** and add this line `export PATH="/opt/gcc/gcc-arm-none-eabi-10.3-2021.10/bin:${PATH}`

- **Make**
  - Install Make:

    ```bash
    sudo apt-get install build-essential
    ```

- **CMake**

  - Download CMake from https://github.com/Kitware/CMake/releases/. Make sure to use CMake version higher than 3.23.
  - Create a cmake directory in usr/bin:

    ```bash
    sudo mkdir /usr/bin/cmake
    ```

  - Execute:

    ```bash
    sudo cmake-<cmake_version>-Linux-x86_64.sh --skip-license --prefix=/usr/bin/cmake
    ```

  - open **~/.bashrc** and add this line `export PATH="/usr/bin/cmake/bin:${PATH}"`

- **Python**
  - Install Python:

    ```bash
    sudo apt-get install -y python3 python3-pip
    ```

- **SEGGER J-Link**:
  - Install J-Link Software and Documentation Pack from: https://www.segger.com/downloads/jlink/.

- **VS Code**
  - Install VS Code from: https://code.visualstudio.com/

## Setup environment

1. Create a virtual environment:

   ```bash
   python -m venv .venv
   ```

2. Activate virtual environment:

   - On Linux:

     ```bash
     source .venv/bin/activate
     ```

   - On Windows:

     ```bash
     .\.venv\Scripts\Activate.ps1
     ```

2. Install the requirements:

   ```bash
   pip install -r requirements.txt
   ```

## Visual Studio Code

We provide support to build, flash and debug firmware directly from VS Code. Below you can find brief description of required setup and possible functionalities.

### Open workspace

To make use of all VS Code features please remember to open project as a workspace:

1. Click `File` in the top menu.
2. Pick `Open Workspace from File...`.
3. Find and open `DW3_QM33_SDK.code-workspace`.

### Extensions

For best experience, please install all recommended extensions.

1. Go to Extensions tab `Ctr+Shift+X`
2. Click **Filter extensions** button (next to search field).
3. Pick **Recommended**.
4. Click on cloud icon just below to install all recommended extensions.

List of currently recommended extensions:

- ms-vscode.cpptools (C language support)
- marus25.cortex-debug (debugging firmware on target)
- rioj7.command-variable (automated tasks for building and debugging)
- ms-python.python (Python language support)


### Tasks

> **Warning:** Make sure VS Code uses Python interpreter from virtual environment. Execute ``Ctrl+Shift+P``, write ``Python: Select interpreter``, and choose path to your local env, e.g.  `./.venv/bin/python`.

> **Warning:** On Windows, the recommended and working Terminal Default Profile is PowerShell. Git Bash and other may not be working properly because of differences in path resolution. To select the proper terminal, execute ``Ctrl+Shift+P``, write ``Terminal: Select Default Profile``, and choose **PowerShell**.

VS Code built-in tasks and launch configuration can be utilized to build, flash and debug the firmware.
You can find short description of available tasks below:

- Build:
  - **Build firmware** - rebuild the firmware (only changed files).
  - **Build clean firmware** - always performs clean build of the firmware.
  - **Flash target** - flash devkit with currently built firmware and reset the target.
  - **Build & flash target** - Build firmware + Flash target.

- Launch:
  - **Debug firmware** - runs GDB sessions with target.
  - **Build & debug firmware** - builds firmware if needed and runs GDB sessions with target.

There are two types of tasks supported: **generic** tasks and **legacy** tasks.

#### Generic tasks

Default flavor utilizes **Command Variable** extension and allows to set build/board configuration which will be used in all later executed tasks.

1. Open build dialog `Ctrl+Shift+B`.
2. Pick **Choose configuration** and VS Code will prompt about possible configurations.
3. Now you can run tasks (**Build target**, **Build target**, **Build & flash target**, **Flash target**) as well as launch firmware `Ctrl+Shift+D` (**Debug firmware** and **Build & debug firmware**), without specifying configuration again. Setting will be saved to file and will remain after VS Code restart.
4. If you want to check current configuration, choose **Check configuration** and it will be displayed in a separate terminal tab.

#### Legacy tasks

If you find the generic tasks not handy or you have issues with **Command Variable** extension, you can choose legacy tasks. They have same end-functionality but you have to choose every time a proper task for your devkit board/build type from long list of possible combinations.
To use legacy tasks, please replace `.vscode/tasks.json` and `.vscode/launch.json` with files from directory `.vscode/legacy`.

> **Note:** If you work only with one type of build and board, feel free to comment or remove tasks you are not going to use, list displayed in VS Code will be much shorter and easier to use.

## Building from command line

If you use other IDE/editor than VS Code you can also build and flash directly from your terminal.

### 1. Create a target

`CreateTarget.py` automates the process of generating a target for a project using common settings. It calls CMake with the appropriate options to configure the build environment and generate the target.

> **Warning:** Make sure your Python virtual environment is activated before running this script.

#### Usage
The project to be built needs to be created. Navigate to the project folder `<project_dir>/Projects/<project_type>/<OS_type>/<Board_name>` and execute:

- On Linux:
    ```bash
    ./CreateTarget.py [build_type] [-no-force]
    ```
- On Windows:
    ```bash
    python ./CreateTarget.py [build_type] [-no-force]
    ```

##### Options

- `build_type`: **Required** positional argument with choices `[cli, uci]`:
  - `cli`: builds the CLI only,
  - `uci`: builds the CLI only.
- `-no-force`: Optional flag to prevent the removal of the old build folder.

#### Example:

- On Linux:
    ```shell
    ./CreateTarget.py cli
    ```
- On Windows:
    ```shell
    python ./CreateTarget.py uci
    ```

#### Help Output

```
./CreateTarget.py --help
usage: CreateTarget.py [-h] [-no-force] {cli,uci}

Build specified target

positional arguments:
  {cli,uci}   Build type

options:
  -h, --help  show this help message and exit
  -no-force   Do not remove old build folder
```

### 2. Build the firmware

This step compiles the firmware, producing output files in three formats: `.hex`, `.bin`, and `.elf`.

#### Usage

Navigate to the project folder `<project_dir>/BuildOutput/<build_type>/FreeRTOS/<board_name>` and execute:

```bash
make -j
```

#### Examples

- compile CLI build for QM33120WDK1:

  ```bash
  cd BuildOutput/DW3_QM33_SDK_CLI/FreeRTOS/nRF52840DK
  make -j
  ```

- compile UCI build for DWM30001CDK:

  ```bash
  cd BuildOutput/DW3_QM33_SDK_CLI/FreeRTOS/DWM3001CDK
  make -j
  ```

> Note: to speed up building process we use ``-j`` flag so all CPU cores will be utilized to build the firmware. You can specify to use e.g. only 4 cores ``make -j4``.

### 3. Flash the firmware

There is wide variety of tools which can be used to flash the target. Below is an example demonstrating how to use the JLink command line tool.

1. Create script.jlink and paste configuration given below.

   ```txt
   si 1
   speed 4000
   <cpu_type>,
   loadfile <firmware_path>
   r
   g
   exit
   ```

   Replace **<cpu_type>** with CPU that you use:
     - **nrf52840_xxaa** for QM33120WDK1 and Murata Type2AB EVB,
     - **nrf52833_xxaa** for DWM3001CDK.

   Replace **<firmware_path>** with path to build firmware, e.g.:
   - ``BuildOutput/DW3_QM33_SDK_CLI/FreeRTOS/nRF52840DK_MAC/nRF52840DK-DW3_QM33_SDK_CLI-FreeRTOS.hex`` to flash CLI build for QM33120WDK1.

2. Launch JLink:

   - On Linux:

       ```bash
       JLinkExe -CommanderScript script.jlink
       ```

   - On Windows:

     ```bash
     JLink.exe -CommanderScript script.jlink
     ```

3. Upon successful completion of the script, the device is ready for use.
