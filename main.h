/*
 * main.h
 *
 *   Author: Alexander Stepanov
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <istream>
#include <iostream>
#include <fstream>
#include <stdlib.h>

// Print the terms of usage (help)
void printUsage(void);

// Convert all chars in string to UPPERCASE
void strToUpperCase(std::string &);

// Duplicate char to string N times
void dupCharToStr(std::string & /*source string*/, char /*char to duplicating*/, unsigned int /*Num times*/);


#endif /* MAIN_H_ */
