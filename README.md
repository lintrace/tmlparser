The T-Mail.log parser (TMLParser)
Copyright (C) 2015 Alexander Stepanov
 
 This program is designed to select the required data such as
IP-address, date, name of subscriber from the log file of the
program T-Mail. T-Mail is a FIDO mailer written by Russian
programmer Andy Elkin (2:5030/15) in 1997 (last version is 2600).
 T-Mail sometimes used by the Banks for exchange data with
Customers in old Client-Bank system.
Please check the PrintUsage() for the parameters.

TMLParser is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

TMLParser is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>

-------------------------------------------------------------
Sample of T-Mail.log file for understanding situation:

  15/03 09:52:23 Rescanning All netmail..
  15/03 09:53:39 ■ Incoming call: CONNECT TCP/IP/10.0.1.1/IFC
  15/03 09:53:50 Handshake failure.
  15/03 09:54:52 ■ Incoming call: CONNECT TCP/IP/10.0.1.2/IFC
  15/03 09:55:04 ? Unlisted node 2:1111/11
  15/03 09:55:04 2:1111/11, using T-Mail/2608.NT/TCP/IP/
  15/03 09:55:04 Remote date/time is 15 Mar 11 9:55
  15/03 09:55:04 Flags: MO,V34
  15/03 09:55:04 SysOp: Client "Number 1", "-Unpublished-"
  15/03 09:55:04 System: CL0001, Moscow, Russia
  15/03 09:55:05 Handshake: EMSI, protocol: Hydra, chat: not enabled
  15/03 09:55:05 Traffic prognosis: Mail: 0  bytes, Files: 735  bytes.
  15/03 09:55:06 Hydra link options: XON,TLN,ASC,UUE,NFI,ECP
  15/03 09:55:06 ? cbfile.pgp renamed to cbfile.pgq
  15/03 09:55:06 HR-16 cbfile.pgp 735 OK: 0:01, 735 cps, 12%
  15/03 09:55:06 ! Running ..\server.bat 2:1111/11.0
  15/03 09:55:07 Returned to T-Mail with errorlevel 0
  15/03 09:55:10 HS-16 error.txt 250 OK: 0:01, 250 cps, 4%
  15/03 09:55:11 ■ 2:1111/11, bytes 735/250, time 0:00:19

  15/03 09:55:12 ! Running ..\PGP\result.bat
  15/03 09:55:12 Returned to T-Mail with errorlevel 1
  15/03 09:56:10 ■ Incoming call: CONNECT TCP/IP/10.0.1.2/IFC
  15/03 09:56:22 ? Unlisted node 2:1111/11
  15/03 09:56:22 2:1111/11, using T-Mail/2608.NT/TCP/IP/
  15/03 09:56:22 Remote date/time is 15 Mar 11 9:56
  15/03 09:56:22 Flags: MO,V34
  15/03 09:56:22 SysOp: Client "Number 1", "-Unpublished-"
  15/03 09:56:22 System: CL0001, Moscow, Russia
  15/03 09:56:23 Handshake: EMSI, protocol: Hydra, chat: not enabled
  15/03 09:56:23 Traffic prognosis: Mail: 0  bytes, Files: 809  bytes.
  15/03 09:56:24 Hydra link options: XON,TLN,ASC,UUE,NFI,ECP
  15/03 09:56:24 HR-16 cbfile.pgp 809 OK: 0:01, 809 cps, 14%
  15/03 09:56:24 ! Running ..\server.bat 2:1111/11.0
  15/03 09:56:25 Returned to T-Mail with errorlevel 0
  15/03 09:56:28 HS-16 error.txt 250 OK: 0:01, 250 cps, 4%
  15/03 09:56:29 ■ 2:1111/11, bytes 809/250, time 0:00:19
  15/03 09:56:30 ! Running ..\PGP\result.bat
  15/03 09:56:30 Returned to T-Mail with errorlevel 1
  15/03 10:09:58 ■ Incoming call: CONNECT TCP/IP/10.0.1.2/IFC
  15/03 10:10:10 ? Unlisted node 2:1111/11
  15/03 10:10:10 2:1111/11, using T-Mail/2608.NT/TCP/IP/
  15/03 10:10:10 Remote date/time is 15 Mar 11 10:10
  15/03 10:10:10 Flags: MO,V34
  15/03 10:10:10 SysOp: Just Another One Client, "-Unpublished-"
  15/03 10:10:10 System: CL0002, Moscow, Russia
  15/03 10:10:11 Handshake: EMSI, protocol: Hydra, chat: not enabled
  15/03 10:10:11 Traffic prognosis: Mail: 0  bytes, Files: 734  bytes.
  15/03 10:10:12 Hydra link options: XON,TLN,ASC,UUE,NFI,ECP
  15/03 10:10:13 HR-16 cbfile.pgp 734 OK: 0:01, 734 cps, 12%
  15/03 10:10:13 ! Running ..\server.bat 2:1111/11.0
  15/03 10:10:14 Returned to T-Mail with errorlevel 0
  15/03 10:10:16 HS-16 error.txt 203 OK: 0:01, 203 cps, 3%
  15/03 10:10:18 ■ 2:1111/11, bytes 734/203, time 0:00:19
 
  27/02 13:11:19 ! Running ..\PGP\result.bat
  27/02 13:11:19 Returned to T-Mail with errorlevel 0
  27/02 13:18:34 ■ Incoming call: CONNECT TCP/IP/118.118.118.119/IFC
  27/02 13:18:46 ? Unlisted node 2:1111/11
  27/02 13:18:46 2:1111/11, using T-Mail/2608.NT/TCP/IP/
  27/02 13:18:46 Remote date/time is 27 Feb 15 12:19
  27/02 13:18:46 Flags: MO,V34
  27/02 13:18:46 SysOp: John Carmack, "-Unpublished-"
  27/02 13:18:46 System: 80000001, Moscow, Russia
  27/02 13:18:48 Handshake: EMSI, protocol: Hydra, chat: not enabled
  27/02 13:18:48 Traffic prognosis: Mail: 0  bytes, Files: 2k bytes.
  27/02 13:18:48 Hydra link options: XON,TLN,ASC,UUE,NFI,ECP
  27/02 13:18:48 HR-16 cbfile.pgp 2267 OK: 0:01, 2267 cps, 39%
  27/02 13:18:48 ! Running ..\server.bat 2:1111/11.0
  27/02 13:18:50 Returned to T-Mail with errorlevel 0
  27/02 13:18:53 HS-16 bcfile.pgp 1098 OK: 0:01, 1098 cps, 19%
  27/02 13:18:54 ■ 2:1111/11, bytes 2267/1098, time 0:00:20
