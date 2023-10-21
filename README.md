# Kings
## How to build
Note: this build needs internet connection to install SFML
### - Windows
#### Using Ninja
if you run ```build/build-windows-ninja.bat``` file it will automatically build the project.
But this bat file requires cmake and ninja, if you do not want to build to ninja you can change the CMakeLists.txt file
> build/build-windows-ninja.bat debug

This will build the project in debug mode.
If you do not specify the mode it will automatically be release mode
#### Using Visual Studio
if you run ```build/build-windows-vs.bat``` file it will automatically build the solution file.
But this bat file requires cmake and visual studio 2022.
> build/build-windows-vs.bat debug

This command will automatically open visual studio, and you have to follow some instructions
- Right-click the Kings.sln file
- Choose ``Set as startup project``

This will build the project in debug mode.
If you do not specify the mode it will automatically be release mode

### - Linux
if you run ```build/build-linux-ninja.sh``` file it will automatically build the project.
But this bat file requires cmake and ninja, if you do not want to build to ninja you can change the CMakeLists.txt file
> build/build-linux-ninja.sh debug

This will build the project in debug mode.
If you do not specify the mode it will automatically be release mode

## FAQ
### Can not access to assest folder
You probably did not set working directory in your IDE