/*****************************************************************
**
** File         : tcpipfiles.lst
** Function     : Overview of files from the TCPIP stack needed in the project
**
** Generated at : 2003/09/24 10:17:53
** Generated by : stackconfmake v0.1
**
** MANUAL CHANGES WILL BE OVERWRITTEN IF THIS FILE IS RE-GENERATED
**
******************************************************************/

This file give an overview of the files needed from 
the TCPIP stack to be included in your project.

If a file is marked as 'generated from .ini' it means
the same tool which wrote this readme wrote the file mentioned.

The following paths have to be added as include files path:
  {project}
  {tcpiplib}

Always:
(generated from the .ini)
  {project}/tcpipset.h

Always:
(to be supplied by user, contains platform specific drivers settings)
  {project}/tcpipsysset.h

Always:
  {tcpiplib}/common/tcpip.c
  {tcpiplib}/common/tcpip.h
  {tcpiplib}/common/tcpip_global.h
  {tcpiplib}/common/sys.h

Only for certain platforms (see documentation):
  {tcpiplib}/common/sys_rom.c

Always needed, platform dependant:
  {tcpiplib}/{platform}/sys_{platform}.c
  {tcpiplib}/{platform}/sys_{platform}.h

Because the serial transport is used:
  {tcpiplib}/common/serial.c
  {tcpiplib}/common/serial.h
  {tcpiplib}/common/drivers/serial_driver.h
  {tcpiplib}/{platform}/serial_{platform}.c
  {tcpiplib}/{platform}/serial_{platform}.h (not all drivers)

Because the DNS server and/or client is used:
  {tcpiplib}/common/dns.c
  {tcpiplib}/common/dns.h

Because the HTTP server is used:
  {tcpiplib}/common/servers/http_server.c
  {tcpiplib}/common/servers/http_server.h

Because the HTTP server is used:
(generated from the .ini)
  {project}/httpdef.h

Because the HTTP server serves hardcoded files:
(generated from the .ini)
  {project}/httpdef_fixed.h


