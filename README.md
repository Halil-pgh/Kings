# Kings
## How to build
Note: this build needs internet connection to install SFML
### - Windows
if you run ```build/build-windows-ninja.bat``` file it will automatically build the project.
But that bat file requires cmake and ninja, if you do not want to build to ninja you can change the CMakeLists.txt file
> build/build-windows-ninja.bat debug

This will build the project in debug mode.
If you do not specify the mode it will automatically be release mode

### - Linux
if you run ```build/build-linux-ninja.sh``` file it will automatically build the project.
But that bat file requires cmake and ninja, if you do not want to build to ninja you can change the CMakeLists.txt file
> build/build-linux-ninja.sh debug

This will build the project in debug mode.
If you do not specify the mode it will automatically be release mode
