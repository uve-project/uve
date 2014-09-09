[Unified Verification Environment (UVE)](http://www.systemverilog.ch)
========================================================================

The aim of the UVE project is to create software that automatically generates
a verification testbench (TB) written in [SystemVerilog (SV)]
(http://www.systemverilog.org/) and integrating the [UVM]
(http://www.uvmworld.org/) methodology. 

UVE makes the rapid development of a verification environment a simple process.
The generated TB is directly able to perform random actions on the DUV (design 
under verification). For this UVE provides a graphical user interface, a code
generator, compilation scripts and a library of verification IPs (VIP). One of 
the main innovations of UVE is a list of TODOs in the TB code which help in 
finalizing the TB. This is especially useful for developers not familiar with 
SV and/or UVE, but also experienced developers profit from that easy to use 
task list.

Moreover, the graphical interface lets the user observe the structure of the 
generated testbench. Files can be accessed easily by double clicking on the 
graphical view. Simulation can be launched directly from the tool.

For more information visit our homepage: __www.systemverilog.ch__

Windows
-------

You can get the [latest UVE installer for windows form our homepage]
(http://www.systemverilog.ch/downloads/) our you can build the source yourself
with the instructions below.

### QScintilla v2.6

This is the component used to build the code editing feature. It's already 
present in the repository, but has to be installed
manually. This is done by opening the Qt Console. You should find it in your
START menu, it might be called something like _Qt 5.3 for Desktop (MinGW 4.8 32 bit)_.

Inside the console you have to navigate to the folder 
_dev\qscintilla\qscintilla_uve\Qt4Qt5_ in your repository. Then you execute the
following commands in this order in the command prompt:
```
>qmake qscintilla.pro
>make
>make install 
```
   
or in the MinGW version
```
>qmake qscintilla.pro
>mingw32-make
>mingw32-make install
```

If an error like this _"Makefile.Debug:60: *** missing separator.  Stop."_
appears, MinGW is not installed. On Qt Creator 2.4.1 or older
( _check Help->About Qt Creator..._ ) you can check if you have MinGW installed
by going to _Help->Start Updater_ in the Qt Creator Menu. Then select Package
Manager on the _Maintain Qt SDK_ dialog that opens. Upon clicking _Next_ you
get to the _Select components_ screen where you should find
_Miscellaneous-> MinGW x.x._ Put a tick in the box and continue the installation
with _Next_. If you have a newer version of Qt Creator you can install minGW 
with the [latest Windows installer from their website]
(http://sourceforge.net/projects/mingw/files/Installer/mingw-get-inst/).

### Flex and Bison 

Install Bison with the installer from [their website]
(https://www.gnu.org/software/bison/) (recommended: v2.4.1) or [Bison for Windows](http://gnuwin32.sourceforge.net/packages/bison.htm)
(recommended: v2.4.1) in a folder without
spaces in it's name and location (recommended: _C:\GnuWin32_), else it will not
work. Then add the location of the bin folder in the installation directory to
the environment variable PATH (e.g. _C:\GnuWin32\bin_).
For flex download a zip file from [their website](http://flex.sourceforge.net).
Extract the files (without any folders) in zip file to the bin folder of Bison.

### Load and Compile

Open QtCreator and select _Open Project..._. Open the file _/dev/gui/uve.pro_.
Then on the _Project Setup_ dialog you get the possibility to choose between
different _Create Build Configurations_. If you select _None_ everything should
be fine. When you choose another option, you have to copy the
_/dev/uveapp/templates_ folder to the parent folder of the generated executable,
otherwise the generation process won't work. Finally to be able to see the help
documentation, copy the folder _\doc\UVEUserManualenglish_ into the parent 
folder of the executable.
