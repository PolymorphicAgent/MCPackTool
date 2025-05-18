# MCPackTool

Goal: to allow user-friendly management, creation, and merging of minecraft resourcepacks

*Note: I created most of this project as a 12-year-old, so I'm in the process of cleaning up messy code. The GUI is terrible. I know. I'll get to fixing it soon*

Curently under development, so barely anything actually works. Use at your own risk.

**Currently working functions:**
- Quickmerge - all except "prompt at each conflict"


latest edit: experimenting with OpenGL to render Minecraft assets in 3d from resourcepacks for the conflict prompter. A lot of work to go...


# Setting up the environment

**ONLY TESTED ON WINDOWS 64 bit - Any other OS's are not guaranteed to work.**

1) Installing / Setting up QT

    - please note that this will take up a ton of storage space depending on the components you install. (the source code is massive)

        a) Download the QT installer from [this site](https://www.qt.io/download-qt-installer-oss).
        
        b) You may have to create a free account with QT.
        
        c) I reccomend keeping the default installation directory (at C:/Qt)
        
        d) Select "Qt 6.9 for desktop development"
        
        e) Finish the installation process. It will take a while!

2) Cloning this project via git

    - Once QT installed, you are ready to clone this project and set up the enivronment!

        a) Open command prompt to your Documents folder, making sure it's NOT your OneDrive Documents (onedrive will attempt to upload everything...) or wherever you keep your projects

        - Note: you must have [git for windows](https://git-scm.com/downloads/win) installed for this next step if you wish to be able to contribute to the project. 
        - You may skip this and directly download and unzip the source code, but you will be unable to contribute to the project.
        
        b) Run `git clone https://github.com/PolymorphicAgent/MCPackTool.git` after git has been installed
        - You should see a new folder named `MCPackTool' appear in your Documents folder (or wherever you decided to put it) 
        
3) Configuring the environment in QT creator
    - In file explorer, you should be able to right-click open the project file `MCPackTool.pro` with QT creator, the IDE that integrates best with the QT Framework.
    
        a) Once QT Creator loads, you should be on the "configure project screen". Deselect whatever default kit pops up, and select `Desktop Qt 6.9.0 MSVC2022 64bit`

        - **This project NEEDS MSVC to work!**

        b) Hit `Configure Project" in the bottom corner.
        
4) Test Build!
    - Once you've got this environment set up, make sure "Debug" is selected as the build configuration in the bottom left corner, and the selected kit is `Desktop Qt 6.9.0 MSVC2022 64bit`.
    
        a) Hit the green play button to run the build process!
        
        b) If it compiles correctly, **you should get an error about failed dependencies**. This is a bug that I will get around to fixing soon. Any other errors signify a misconfigured environment.
            
        - To Supress this error, simply copy the two files `copy.bat` and `copyA.bat` to the `build/Desktop_blahblah_MSVC_blahblah` folder

5) Contributing to the project
    - Before making changes, make sure you have the latest changes (in case I've already fixed the bug you see)
    
        a) `git fetch`
    
        b) `git pull`
        
    - You're good to make changes! 
    - To submit changes to the project, a Pull Request (PR) is needed.
    
        a) First, create the PR

to be finished...
