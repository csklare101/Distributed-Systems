/*-------------------------------------------------------------------------*
 *---									---*
 *---		select.c						---*
 *---									---*
 *---	    This file defines a program that takes a non-negative	---*
 *---	integer on the command line, reads lines from 'stdin', and	---*
 *---	prints only those lines to 'stdout' with the specified width.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2022 January 9		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>

const int	LINE_LEN	= 4096;

int		main		(int	argc,
				 char*	argv[]
				)
{
  char*	cPtr;
  char	line[LINE_LEN];
  int	passThruLen;
  //  YOUR CODE HERE
  //a. It should make sure there is at least one argument on the command line. 
  if (argc < 1) {
      fprintf(stderr,
          "Usage:\tselect num\n"
          "Where 'num' is a non-negative int telling"
          " the column widths to allow through.\n"
      );
      exit(EXIT_FAILURE);
  }
  if (argc > 2) {
      fprintf(stderr, "Incorrect format!");
      exit(EXIT_FAILURE);
  }
  //b. It should make sure that the argument on the command line is a non-negative integer. Use passThruLen
  passThruLen = atol(argv[1]);
  if (passThruLen <= 0) {
      fprintf(stderr, "Number format error\n");
      exit(EXIT_FAILURE);
  }

  //c. It should read a line from stdin. If that line has exactly passThruLen columns then it should print that line to stdout. See if \n appears at index of passThruLen
     while (fgets(line, LINE_LEN, stdin) != NULL) {
         if (line[passThruLen] == '\n' && line[passThruLen-2] != '\n') {
             fprintf(stdout, line);
         }
     }
  return(EXIT_SUCCESS);
}