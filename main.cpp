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

// List of the arguments for command line
struct CommandLineArg {
    unsigned bQuietMode:     1;
    unsigned bAppendMode:    1;
    std::string sInputFile;
    std::string sOutputFile;
    std::string sMaskSID;
    std::string sExcludeIP;
    CommandLineArg() {
        bQuietMode = bAppendMode = 0;
        sInputFile = sOutputFile = sMaskSID = sExcludeIP = "";
    }
};

int main(int argc, char* argv[]) {
    char chKey;

    char * lpLineBuffer = NULL;
    CommandLineArg cmdLineKey;

    unsigned int iStatTotalConn = 0;

    std::string sArgument = "";

    // If the command-line arguments are not passed then exit with the help output
    if (argc == 1) {
        printUsage();
        return (1);
    }

    for (int argN = 1; argN < argc; argN++) {
        sArgument = argv[argN];
        // Search keys (when argument begins with '-') and parameters. Each key may have only one parameter!
        if (sArgument.at(0) == '-') {
            chKey = tolower(sArgument.at(1));
            switch (chKey) {
            case 'i':
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sInputFile = argv[++argN];
                }
                break;
            case 'o':
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sOutputFile = argv[++argN];
                }
                break;
            case 's':
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sMaskSID = argv[++argN];
                }
                break;
            case 'e':
                if ((argN+1)<argc && argv[argN+1][0]!='-'){
                    cmdLineKey.sExcludeIP = argv[++argN];
                }
                break;
            case 'q':
                cmdLineKey.bQuietMode = 1;
                break;
            case 'a':
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

    if (cmdLineKey.sMaskSID.empty() && cmdLineKey.bQuietMode == 0) {
        std::cout << "Do you really want to get a list of ALL customers without filtering by SID? (y/n)"
             << std::endl;
        // Waiting and check of the answer
        char chUserIn;
        std::cin.get(chUserIn);
        if (std::tolower(chUserIn) != 'y') {
            std::cout << "OK, i'm exiting...\n";
            return (1);
        }

    }
    //-----------[ OPEN INPUT AND OUTPUT FILES ]------------
    std::ifstream f_infile;
    std::ofstream f_outfile;

    // Open input file
    f_infile.open(cmdLineKey.sInputFile.data(), std::ios_base::in);
    if (!f_infile.is_open()) {
        std::cout << "Error: Input t-mail.log file is not found!"
             << "Need a correct filename: -i "
             << cmdLineKey.sInputFile << std::endl;
        return (2);
    }

    // Create output file (truncate or append to existing)
    if (cmdLineKey.bAppendMode == 1) {
        f_outfile.open(cmdLineKey.sOutputFile.data(), std::ios_base::out | std::ios_base::app);
    } else {
        f_outfile.open(cmdLineKey.sOutputFile.data(), std::ios_base::out | std::ios_base::trunc);
    }

    if (!f_outfile.is_open()) {
        std::cout << "Error: Could not create output file: " << cmdLineKey.sOutputFile << std::endl;
        f_infile.close();
        return (2);
    }
    //-----------[ PRINT LIST ALL PARAMETERS ]-------------
    // Print table with all used parameters if not selected quiet mode
    if (cmdLineKey.bQuietMode == 0) {
        std::string sTmp("");
        dupCharToStr(sTmp, '=', 33);
        sTmp += "[ PARAMETERS ]";
        dupCharToStr(sTmp, '=', 23);
        sTmp += "[_][V][X]=";
        std::cout << sTmp;

        sTmp = '+';
        dupCharToStr(sTmp, '-', 17);
        sTmp += '+';
        dupCharToStr(sTmp, '-', 61);

        printf("\n| T-MAIL log file | %-61s\n"
                "%s\n| Output filename | %-61s\n"
                "%s\n| Filter by SID   | %s\n"
                "%s\n| Exclude IP by   | %s\n", cmdLineKey.sInputFile.data(), sTmp.data(),
                cmdLineKey.sOutputFile.data(), sTmp.data(),
                (cmdLineKey.sMaskSID.empty()) ?
                        "none - will be shown all connections!" :
                        cmdLineKey.sMaskSID.data(), sTmp.data(),
                (cmdLineKey.sExcludeIP.empty()) ?
                        "none (result will not filtered by IP addresses)" :
                        cmdLineKey.sExcludeIP.data());

        sTmp = "";
        dupCharToStr(sTmp, '-', 80);

        printf("%s\n| %s\n%s\n", sTmp.data(),
                (cmdLineKey.bAppendMode == 1) ?
                        "Output data will append to existing output file" :
                        "Output file will be overwritten if exist",
                sTmp.data());

        std::cout
                << "| (*) Job will be done more faster if you use quiet mode (-q)!\n";

        sTmp = "";
        dupCharToStr(sTmp, '=', 80);
        std::cout << sTmp << std::endl << std::endl;
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
        by moving down  by lines in input log file.

     4) If was found all fields from the Stage 3 then they pass to a output file
        with use of char '#' as delimiter and execution start again from Stage 1
        for the next connection.

     5) If in a searching fields on a Stage 3 was unexpectedly found new statement
        for established connection then all parameters need be reset, stage 5
        will be interrupted  and execution will return to a Stage 1

        PS: Local date and time given from the 'CONNECT' line
     */


    // Create buffer for reading strings from input file
    lpLineBuffer = (char *) calloc(INPUT_LINE_BUFFER_LEN, sizeof(char));

     std::string  sInputString = "";

    struct {
        std::string sLocalDateTime;
        std::string sIncomingIP;
        std::string sSysOp;
        std::string sSID;
        std::string sRemoteDateTime;
    } logFields;

    struct {
        unsigned ConnectIP:    1;
        unsigned SysOp:        1;
        unsigned RemoteDate:   1;
        unsigned System:       1;
        void reset()
        {
            ConnectIP = SysOp = RemoteDate = System = 0;
            return;
        }
    } validLogFields;

    // Reading input file by lines from begin to the end
    while (!f_infile.eof()) {
        f_infile.getline(lpLineBuffer, INPUT_LINE_BUFFER_LEN);
        sInputString = lpLineBuffer;

        if (sInputString.find("Incoming call: CONNECT ", 0) != std::string::npos) {
            validLogFields.reset();
            validLogFields.ConnectIP = 1;
            logFields.sLocalDateTime = sInputString.substr(0, 14);
            logFields.sIncomingIP = sInputString.substr(sInputString.find("TCP/IP/") + 7, sInputString.find_last_of('/') - (sInputString.find("TCP/IP/") + 7 ));

            if (!cmdLineKey.sExcludeIP.empty() && sInputString.find(cmdLineKey.sExcludeIP)!=std::string::npos) {
                validLogFields.reset();
            }
            continue;
        } else {
            if (validLogFields.ConnectIP == true) {
                if (validLogFields.SysOp == 0 && sInputString.find("SysOp:") != std::string::npos) {
                    logFields.sSysOp = sInputString.substr(
                            sInputString.find("SysOp:")+7,
                            sInputString.find_last_of(',')-(sInputString.find("SysOp:")+7));
                    validLogFields.SysOp = 1;
                    continue;
                }

                if (validLogFields.RemoteDate == 0 &&
                        sInputString.find("Remote date") != std::string::npos) {
                    logFields.sRemoteDateTime = sInputString.substr(
                            sInputString.find("Remote date") + 20);
                    validLogFields.RemoteDate = 1;
                    continue;
                }

                if (validLogFields.System == 0 && sInputString.find("System:") != std::string::npos) {
                    logFields.sSID = sInputString.substr(sInputString.find("System:") + 8,
                            sInputString.find(',')-(sInputString.find("System:") + 8));
                    strToUpperCase(logFields.sSID);

                    if (!cmdLineKey.sMaskSID.empty() && logFields.sSID.compare(cmdLineKey.sMaskSID)) {
                        validLogFields.reset();
                        continue;
                    }

                    // Go to new line in the log file if not all fields been found
                    if (validLogFields.ConnectIP == 0 && validLogFields.RemoteDate == 0 &&
                            validLogFields.SysOp == 0 && validLogFields.System == 0) continue;

                    f_outfile << logFields.sLocalDateTime << '#' << logFields.sSID << '#' << logFields.sIncomingIP
                            << '#' << logFields.sSysOp << '#' << logFields.sRemoteDateTime << std::endl;
                    validLogFields.reset();
                    iStatTotalConn++;
                    if (cmdLineKey.bQuietMode == 0) {
                        printf("\rCounter: [%10i]  Client: [%9s] %-80s",
                                iStatTotalConn, logFields.sSID.data(), logFields.sSysOp.data());
                    }

                    continue;
                }
            }
        }
    }
    f_infile.close();
    f_outfile.close();
    free(lpLineBuffer);

    if (cmdLineKey.bQuietMode == 0)
        printf("\n\n%s\n",
                (iStatTotalConn) ?
                        "Work complete!" :
                        "Sorry, but nothing found by specified SID! :-(");
    return (0);
}

// Print the terms of usage (help)
void printUsage(void) {
    std::cout << "T-Mail.log parser (TMLParser)     (c) 2015 Alexander Stepanov" << std::endl
         << std::endl
         << "TMLParser usage:" << std::endl
         << "   -h or --help   print out this message" << std::endl
         << "   -i filename    specify filename for the input log file from T-MAIL (usually t-mail.log)" << std::endl
         << "   -o filename    specify filename for the output report" << std::endl
         << "   -a             append mode (do not overwrite output file if exist)" << std::endl
         << "   -s SID         specify filter by SID for the input log file" << std::endl
         << "   -e IP          specify filter to EXCLUDE IP from the input log file" << std::endl
         << "                  to example: -e 10.0.0. will exclude all IP in range 10.0.0.xxx" << std::endl
         << std::endl
         << "   -q             quiet mode (silent mode with suppressing messages)" << std::endl
         << "                  With this option job will be done is very fast!" << std::endl
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
void dupCharToStr(std::string & sStr, char ch, unsigned int uiDup) {
    while (uiDup) {
        sStr += ch;
        uiDup--;
    }
    return;
}
