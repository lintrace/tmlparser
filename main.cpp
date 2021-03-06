/*
 * main.cpp
 * Copyright (C) 2015 Alexander Stepanov
 *
 *         The T-Mail.log parser (TMLParser)
 *
 * This program is designed to select the required data
 * such as IP address, date, name of subscriber from
 * the log file of the program T-Mail.
 * T-Mail is a FIDO mailer written by Russian programmer
 * Andy Elkin (2:5030/15) in 1997 (last version is 2600).
 * T-Mail sometimes used by the Banks for exchange
 * data with Customers in old Client-Bank system.
 * Please check the PrintUsage() for the parameters.
 *
 *
 * TMLParser is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TMLParser is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"

// Length of the buffer for the input line from T-Mail log file
const unsigned int INPUT_LINE_BUFFER_LEN = 1024;


int main(int argc, char* argv[]) {

	// List of the arguments for command line
	struct CommandLineArg {
		unsigned bVerboseMode:   1;
		unsigned bAppendMode:    1;
		char chDelimiter;
		std::string sInputFile;
		std::string sOutputFile;
		std::string sMaskSID;
		std::string sExcludeIP;
		CommandLineArg() {
			bVerboseMode = bAppendMode = 0;
			sInputFile = sOutputFile = sMaskSID = sExcludeIP = "";
			chDelimiter = '#';
		}
	} cmdLineKey;

	std::string sArgument = "";

    // If command-line arguments are not passed then exit with help output
    if (argc == 1) {
        printUsage();
        return (1);
    }

    for (int argN = 1; argN < argc; argN++) {
        sArgument = argv[argN];
        // Search keys (when argument begins with '-') and parameters.
        if (sArgument.size() > 1 && sArgument.at(0) == '-') {
            switch (tolower(sArgument.at(1))) {
            case 'i': // input log file
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sInputFile = argv[++argN];
                }
                break;
            case 'o': // output file (report)
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sOutputFile = argv[++argN];
                }
                break;
            case 's': // SID for search
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sMaskSID = argv[++argN];
                }
                break;
            case 'e': // Exclude IP from log
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sExcludeIP = argv[++argN];
                }
            case 'd': // Delimiter for fields in output file
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.chDelimiter = argv[++argN][0];
                }
                break;
            case 'v': // Verbose mode
                cmdLineKey.bVerboseMode = 1;
                break;
            case 'a': // Append to output file (overwrite by default)
                cmdLineKey.bAppendMode = 1;
                break;
            default:  // For the -h (help) and any unknown options
                printUsage();
                return (1);
                break;
            }
        }
    }

    // Checking parameters
    if (cmdLineKey.sInputFile.empty())  {
        std::cout << "Error: You must specify the correct log file from T-MAIL with key -i"
                  << std::endl;
        return (2);
    }

    if (cmdLineKey.sOutputFile.empty()) {
        std::cout << "Error: You must specify the output file with key -o" << std::endl;
        return (2);
    }

    if (cmdLineKey.sMaskSID.empty() && cmdLineKey.bVerboseMode) {
        std::cout << "Do you really want to get a list of ALL customers without filtering by SID? (y/n)"
                  << std::endl;
        // Waiting and check the answer
        char chUserIn;
        std::cin.get(chUserIn);
        if (std::tolower(chUserIn) != 'y') return (1);
    }

    //-----------[ OPEN INPUT AND OUTPUT FILES ]------------

    // Open input file
    std::ifstream f_infile;
    f_infile.open(cmdLineKey.sInputFile.data(), std::ios_base::in);
    if (!f_infile.is_open()) {
        std::cout << "Error: Input t-mail.log file is not found!"
                  << "Need a correct filename: -i "
                  << cmdLineKey.sInputFile << std::endl;
        return (2);
    }

    // Create output file (truncate or append to existing)
    std::ofstream f_outfile;
    if (cmdLineKey.bAppendMode) {
        f_outfile.open(cmdLineKey.sOutputFile.data(),
                       std::ios_base::out | std::ios_base::app);
    } else {
        f_outfile.open(cmdLineKey.sOutputFile.data(),
                       std::ios_base::out | std::ios_base::trunc);
    }

    if (!f_outfile.is_open()) {
        std::cout << "Error: Could not create output file: "
                  << cmdLineKey.sOutputFile << std::endl;
        f_infile.close();
        return (2);
    }
    //-----------[ PRINT LIST ALL PARAMETERS ]-------------
    // Print table with all parameters if selected verbose mode
    if (cmdLineKey.bVerboseMode) {
        std::cout << dupCharToStr('=', 33) << "[ PARAMETERS ]" << dupCharToStr('=', 33);
        std::string sTmp = '+' + dupCharToStr('-', 17) + '+' + dupCharToStr('-', 61);

        printf("\n| T-MAIL log file | %-61s\n"
               "%s\n| Output filename | %-61s\n"
               "%s\n| Filter by SID   | %s\n"
               "%s\n| Exclude IP by   | %s\n"
               "%s\n| Delimiter is    | symbol '%c'\n"
               "%s\n| %s\n"
               "%s\n\n",
               cmdLineKey.sInputFile.data(), sTmp.data(),
               cmdLineKey.sOutputFile.data(), sTmp.data(),
               cmdLineKey.sMaskSID.empty() ?
                         "none - will be shown all connections!" :
                         cmdLineKey.sMaskSID.data(), sTmp.data(),
               cmdLineKey.sExcludeIP.empty() ?
                         "none (result will not filtered by IP addresses)" :
                         cmdLineKey.sExcludeIP.data(), sTmp.data(),
               cmdLineKey.chDelimiter,
               dupCharToStr('-', 80).data(),
               cmdLineKey.bAppendMode ?
				         "Output data will append to existing output file" :
				         "Output file will be overwritten if exist",
               dupCharToStr('=', 80).data());
    }

    //--[MAIN LOGIC FOR THE GENERATING OUTPUT] --

    // ALGORITHM
    // ~~~~~~~~~
    /*
     1) In first stage we will search the substring "Incoming call: CONNECT"
        and remembering IP-address. Keep in mind that connection may be closed
        by error (handshake failure e.g.)!

     2) If was used the mask for exclude IP-addresses from log file and current
        IP was found in this list then return to Stage 1.

     3) Collecting all interested us fields such as IP, SysOp, Date, System
        by moving down by lines in input log file.

     4) If was found all fields from the Stage 3 then they pass to a output file
        with use of char '#' as delimiter and execution start again from Stage 1
        for the next connection.

     5) If in a searching fields on a Stage 3 was unexpectedly found new statement
        for established connection then all parameters need be reset, stage 5
        will be interrupted  and execution will return to a Stage 1

        PS: Local date and time we receive from the 'CONNECT' line
     */

    struct {
        std::string sLocalDateTime;
        std::string sIncomingIP;
        std::string sSysOp;
        std::string sSID;
        std::string sRemoteDateTime;
        unsigned bConnectIP:    1;
        unsigned bSysOp:        1;
        unsigned bRemoteDate:   1;
        unsigned bSystem:       1;
        void reset()
        {
            bConnectIP = bSysOp = bRemoteDate = bSystem = 0;
        }
    } logFields;

    // Create buffer for reading strings from input file
    char * lpLineBuffer = (char *) calloc(INPUT_LINE_BUFFER_LEN, sizeof(char));
    std::string  sInputString = "";  // String from input file (from buffer)
    unsigned int iStatTotalConn = 0; // Counter total connections of Customer
                                     // (equals the lines in output file)

    // Reading input file by lines from begin to the end
    while (!f_infile.eof()) {
        f_infile.getline(lpLineBuffer, INPUT_LINE_BUFFER_LEN);
        sInputString = lpLineBuffer;
        // Searching for CONNECT in log to begin a work
        if (sInputString.find("Incoming call: CONNECT", 0) != std::string::npos) {
            logFields.reset();
            // Set the flag what connection is been found
            logFields.bConnectIP = 1;
            // Set local date and time field from line 'CONNECTION'
            logFields.sLocalDateTime = sInputString.substr(0, 14);
            // Getting IP from 'CONNECTION'
            logFields.sIncomingIP = sInputString.substr(sInputString.find("TCP/IP/") + 7,
            		                                sInputString.find_last_of('/') - (sInputString.find("TCP/IP/") + 7 )
							);

            if (!cmdLineKey.sExcludeIP.empty() && sInputString.find(cmdLineKey.sExcludeIP)!=std::string::npos) {
                logFields.reset();
            }
            continue;
        } else {
            if (logFields.bConnectIP) {
            	// If connection was found we looking in log for the other fields
                if (!logFields.bSysOp && sInputString.find("SysOp:") != std::string::npos) {
                    logFields.sSysOp = sInputString.substr(
                            sInputString.find("SysOp:")+7,
                            sInputString.find_last_of(',')-(sInputString.find("SysOp:")+7));
                    logFields.bSysOp = 1;
                    continue;
                }

                if (!logFields.bRemoteDate && sInputString.find("Remote date") != std::string::npos) {
                    logFields.sRemoteDateTime = sInputString.substr(sInputString.find("Remote date") + 20);
                    logFields.bRemoteDate = 1;
                    continue;
                }

                if (!logFields.bSystem && sInputString.find("System:") != std::string::npos) {
                    logFields.sSID = sInputString.substr(
                                        sInputString.find("System:") + 8,
                                        sInputString.find(',')-(sInputString.find("System:") + 8)
                                        );
                    strToUpperCase(logFields.sSID);
                    logFields.bSystem = 1;

                    if (!cmdLineKey.sMaskSID.empty() && logFields.sSID.compare(cmdLineKey.sMaskSID)) {
                        logFields.reset();
                        continue;
                    }

                    // Go to the new line in the log if not all fields been found
                    if (!logFields.bConnectIP && !logFields.bRemoteDate &&
                            !logFields.bSysOp && !logFields.bSystem) continue;

                    // All fields been found. Write line in output file.
                    f_outfile << logFields.sLocalDateTime << cmdLineKey.chDelimiter
                              << logFields.sSID << cmdLineKey.chDelimiter
                              << logFields.sIncomingIP << cmdLineKey.chDelimiter
                              << logFields.sSysOp << cmdLineKey.chDelimiter
                              << logFields.sRemoteDateTime << std::endl;
                    logFields.reset();
                    iStatTotalConn++;
                    if (cmdLineKey.bVerboseMode) {
                        printf("\rCounter: [%10i]  Client: [%9s] %-80s",
                                iStatTotalConn,
                                logFields.sSID.data(),
                                logFields.sSysOp.data()
															);
                    }
                }
            }
        }
    }
    f_infile.close();
    f_outfile.close();
    free(lpLineBuffer);

    if (cmdLineKey.bVerboseMode) {
        printf("\n\n%s\n",
                (iStatTotalConn) ?
                        "Work complete!" :
                        "Sorry, but nothing found by specified SID! :-(");
    }
    return (0);
}

// Print the terms of usage (help)
void printUsage(void) {
    std::cout << "T-Mail.log parser (TMLParser)     (c) 2015 Alexander Stepanov" << std::endl
         << std::endl
         << "TMLParser usage:" << std::endl
         << "   -h or --help   print out this message" << std::endl
         << "   -i filename    specify filename for the input log file from T-MAIL" << std::endl
         << "                  (usually t-mail.log)" << std::endl
         << "   -o filename    specify filename for the output report (out.txt, eg)" << std::endl
         << "   -a             append mode (do not overwrite output file if exist)" << std::endl
         << "   -s SID         specify filter by SID for the input log file" << std::endl
         << "   -e IP          specify filter to EXCLUDE IP from the input log file" << std::endl
         << "                  to example: -e 10.0.0. will exclude all IP in range 10.0.0.xxx" << std::endl
         << "                  is useful to exclude local connections to server" << std::endl
         << "   -d             delimiter of the fields in output file ('#' by default)" << std::endl
         << std::endl
         << "   -v             verbose mode (give more information about each stage)" << std::endl
         << std::endl
         << "tmlparser -i d:\\t-mail.log -o d:\\report.txt -s 00000001" << std::endl
         << "tmlparser -i d:\\t-mail.log -o d:\\report.txt -s 7P000001 -e 10.0." << std::endl
         << std::endl
         << "Return codes:" << std::endl
         << "   0 - successful completion of job" << std::endl
         << "   1 - non-critical error" << std::endl
         << "   2 - critical error" << std::endl;
    return;
}

// Convert all chars in string to UPPERCASE
void strToUpperCase(std::string & sStr) {
    for (unsigned int i = 0; i < sStr.size(); i++) {
        sStr[i] = toupper((char) sStr[i]);
    }
    return;
}

// Duplicate char ch in string sStr uiDup times
//void dupCharToStr(std::string & sStr, char ch, unsigned int uiDup) {
//    while (uiDup--) {
//        sStr += ch;
//    }
//    return;
//}

std::string dupCharToStr(char ch, unsigned int uiDup) {
    std::string _sTmp;
    while (uiDup--) { _sTmp += ch; }
    return _sTmp;
}
