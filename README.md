# CUITExamClient

## Deploy

### Prepare

1. Install Qt 6.4.2 (with MSVC and Qt Installer Framework)
2. Add environment variable `Qt` like value `C:\Qt`
3. Add value `F:\Qt\Tools\QtInstallerFramework\4.5\bin` to `Path` environment variable
4. Use Visual Studio Code to open this project
5. `Ctrl+Shift+P -> CMake: Configure`

### Build

Please always notice the error which is shown with paartial cpps,\
and `Ctrl+Shift+P -> CMake: Configure` may solve the porblem.

There are two configurations:

1. MSVC_Debug: Will build and debug program
2. Pack: Use QtIFW to pack the installer (will build before packing)
