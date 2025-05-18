# MCPackTool
Goal: to allow user-friendly management, creation, and merging of minecraft resourcepacks

Curently under development, so barely anything actually works. Use at your own risk.

latest edit: experimenting with OpenGL to render Minecraft assets in 3d from resourcepacks. A lot of work to go...


# Setting up the environment
This is really finniky so buckle up...

**ONLY TESTED ON WINDOWS 64 bit - Any other OS's are not guaranteed to work.**

1) Installing / Setting up QT

    - please note that this will take up approximately a ton of storage space depending on the components you install. (the source code is massive)

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
    
        a) 

to be finished...
