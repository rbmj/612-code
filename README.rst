----------------------------------------------
Chantilly Robotics Code for 2012 FRC Challenge
----------------------------------------------

This project contains all of `FIRST Team 612 Chantilly Robotics`_' code for the
2012 FRC Challenge.

Usage
++++++++++++++++

For now, this project assumes that you already have ucpp_ installed and
configured for your system.  The build process is as follows::

  $ ucpp init               #run once
  $ ucpp configure          #run each time the files in the project have changed
  $ make                    #compile and link all code (debug)
  $ make DEBUG_MODE=0       #compile and link all code (release)
  $ make deploy             #deploy code to robot

For more information regarding ucpp, see 'ucpp.rst'.

Configuring
++++++++++++++++

 - Some configuration is required on the cRIO to get everything to work.  First,
   when imaging it is necessary to select the option for WindRiver C++, and please
   enable NetConsole (except for in perhaps release builds).
 - Remember that the cRIO runs an ftp server that allows anonymous ftp access.
 - Second, for NetConsole to work ni-rt.ini needs to have the DNS and default
   gateway set to the router.  The default is 10.6.12.1, and, though this works on
   the field, it will not allow NetConsole to work for debugging.  The subnet mask
   of computer running NetConsole must also be set to 255.0.0.0 (not the more
   intuitive 255.255.255.0).
    - Again, without these changes, the robot will still DRIVE fine.
 - FOR NOW, the robot REQUIRES version 30 of the firmware.  This WILL change come
   build season, though!
 - Be sure to undo any/all changes to ni-rt.ini before going onto the field!!!

License
++++++++++++++++

All code in this project is licensed under the ISC License (see 'LICENSE.rst')
and is Copyright |c| 2012 Chantilly Robotics unless otherwise noted.

Contributors
++++++++++++++++

A full list of contributors is in 'AUTHORS.rst'.

Thanks
++++++++++++++++
Thanks go out to nikitakit and all the other developers of the ucpp_ project.
Thanks to github_ for providing hosting of our code.
And finally, a big thanks to the entire organization of FIRST_.



.. _`FIRST Team 612 Chantilly Robotics`: http://www.chantillyrobotics.org/
.. _ucpp: https://github.com/nikitakit/ucpp
.. _github: https://github.com/
.. _FIRST: http://usfirst.org/
.. |c| unicode:: 0xA9 .. (Copyright (c) Sign)
