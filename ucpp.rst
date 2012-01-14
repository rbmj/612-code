-----------------------------------------
UCPP for FIRST Instructions
-----------------------------------------

The ucpp_ project allows compilation of code for FRC on any platform
without proprietary software.

To download, you can clone the git repository.  Alternatively, you
can just download a .tar.gz snapshot.  Cloning the git repo is as
easy as::

   $ git clone https://github.com/nikitakit/ucpp.git

If you want to clone in the /usr hierarchy (like /usr/local/share)::

   $ sudo mkdir ucpp
   $ sudo chown 'YOUR SYSTEM USER NAME HERE' ucpp
   $ git clone https://github.com/nikitakit/ucpp.git

Yes, I know it's bad practice to have user-owned folders in the
main system hierarchy, but it's better than running git as root...
sooner or later I'll make some patches that will let one do a true
system-wide install.

Installation Dependencies (Linux/Unix)
-----------------------------------------

Installation on Unix-like OSes has a few dependencies:
 - A normal collection of command-line tools:
    - bash
    - GNU *utils, etc
    - wget
    - Your distribution most likely installs these by default.  If you
      don't have them, chances are you did that on purpose and can
      deal with the errors.
 - tclsh (you may already have this as well)
 - wine (for running the windows version of the compiler)
 - python (used to simplify scripting)
 - wput (to deploy code to the robot)

Installation Directions (Linux/Unix)
-----------------------------------------

 - Download ucpp.  This can be done through a clone of the git repo
   or through a download of a .tar.gz snapshot.
 - Add the ucpp folder to your path.  For example, I put ucpp in
   /usr/local/share, so I have this at the end of my /etc/profile::

      # This can go in either /etc/profile or ~/.profile
      # UCPP PATH - ucpp is located at /usr/local/share/ucpp in this case
      # and the scripts are located in the ucpp subdirectory of *that*
      PATH="$PATH:/usr/local/share/ucpp/ucpp"
      export PATH

 - As your user (not root) run::

      $ ucpp setup -t team_number       # NOTE: our team number is 612

 - Once ucpp has installed, you need to make a workspace folder.  Mine
   is ~/robo_wkspc
 - Put your code repository in the workspace folder - so in my case
   ~/robo_wkspc/612-code

Installation (win32)
-----------------------------------------

This section is not completed.  I can't provide a detailed walkthrough,
as I have no experience getting everything to work together on windows.
I can, however, give a little bit of information.

First, there are two options for installation currently.  One relies on
WindRiver and MSYSGit (I guess if you don't want to work through WR),
and the other relies on CYGWIN.  Once CYGWIN is set up with all of the
proper dependencies (I would highly recommend the apt-cyg script for
this), the install process should be *similar* to the *N?X install.


Running
-----------------------------------------

 - To initialize the project, run in the project's root directory::

      $ ucpp init

 - Every time you add or remove a .cpp file, you must regenerate the
   makefile with::

      $ ucpp configure

    - Note that the above command only looks in the project's root
      directory - so for now no putting .cpp files in subdirectories.
 - You can now make the project with::

      $ make                  # for debug builds
      $ make DEBUG_MODE=0     # for release builds
      $ make clean            # clean to force recompile

 - You can deploy the code with::

      $ make deploy           # deploy code - only debug builds
      $ make undeploy         # remove code from robot

 - The object files are in PPC603gnu/[Non]Debug/612-code.out
    - These can be deployed to the robot manually by renaming the file
      FRC_UserProgram.out and putting it in the /ni-rt/system directory
      of the cRIO.  The cRIO runs an anonymous FTP server for this
      purpose.      

.. _ucpp: https://github.com/nikitakit/ucpp
.. _`forked version`: https://github.com/rbmj/ucpp
