/*-------------------------------------------------------------------------*
 *---									---*
 *---		lsRevsortSelect.c					---*
 *---									---*
 *---	    This file defines a program that chains together child	---*
 *---	processes running 'ls', 'sort -r' and 'select int' such that	---*
 *---	the output of one becomes the input for the next using pipes.	---*
 *---	The output of the 'select' process comes back to this process	---*
 *---	using another pipe.  Then this process prints that output.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2022 January 9		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<dirent.h>
#include	<unistd.h>

const int	LINE_LEN	= 4096;

int		main		(int		argc,
				 char*		argv[]
				)
{
  char*	cPtr;
  int	passThruLen;

  //  YOUR CODE HERE
 //a.  It should make sure there is at least one argument on the command line.
  if (argc < 1) {
      fprintf(stderr,
          "Usage:\tselect num\n"
          "Where 'num' is a non-negative int telling"
          " the column widths to allow through.\n"
      );
      exit(EXIT_FAILURE);
  }
  // if more than 1 arguments
  if (argc > 2) {
      fprintf(stderr, "Incorrect format!");
      exit(EXIT_FAILURE);
  }

  //b. It should make sure that the argument on the command line is a non-negative integer. 
  //Call this integer passThruLen. If passThruLen is not a non-negative integer, the program should do:
  passThruLen = atol(argv[1]);
  if (passThruLen <= 0) {
      fprintf(stderr, "Number format error\n");
      exit(EXIT_FAILURE);
  }

  //  ls
  //
  int	lsToSort[2];
  pid_t	lsChild;

  //  YOUR CODE HERE
    //b. It should make a pipe in lsToSort[2] and then make a child process to run ls.
    //The child process should dup2() to send the pipe output to STDOUT_FILENO.Then it should close() both ends of the pipe and run ls.
    //The parent process should close() the output end of the pipe.
  if (pipe(lsToSort) < 0) {
      fprintf(stderr, "Cannot create pipe\n");
      exit(EXIT_FAILURE);
  }
  lsChild = fork();
  if (lsChild < 0)
  {
      fprintf(stderr, "fork() failed\n");
      exit(EXIT_FAILURE);
  }

  if (lsChild == 0) {
      dup2(lsToSort[STDOUT_FILENO], STDOUT_FILENO);
      close(lsToSort[STDIN_FILENO]);
      close(lsToSort[STDOUT_FILENO]);
      execl("/usr/bin/ls", "ls", NULL);
      fprintf(stderr, "Cannot find ls\n");
      exit(EXIT_FAILURE);
  }

  close(lsToSort[STDOUT_FILENO]);

  //  sort
  //
  int	sortToSelect[2];
  pid_t	sortChild;

  //  select
  int	selectToParent[2];
  pid_t	selectChild;

  //  YOUR CODE HERE
  // 
  //d. It should make a pipe in sortToSelect[2] and then make a child process to run sort - r.
      //The child process should dup2() to send the sortToSelect output to STDOUT_FILENO.Then it should close() both ends of that pipe.
      //It should also dup2() to get its input from lsToSort, and then close() the input end of that pipe.
      //Lastly, it shoul run sort with - r as a command line argument to reverse the sort order.
      //The parent process should close() the input end of lsToSort and the output end of sortToSelect.
  if (pipe(sortToSelect) < 0) {
      fprintf(stderr, "Cannot create pipe\n");
      exit(EXIT_FAILURE);
  }
  sortChild = fork();
  if (sortChild < 0)
  {
      fprintf(stderr, "fork() failed\n");
      exit(EXIT_FAILURE);
  }

  if (sortChild == 0) {
      dup2(sortToSelect[STDOUT_FILENO], STDOUT_FILENO);
      close(sortToSelect[STDIN_FILENO]);
      close(sortToSelect[STDOUT_FILENO]);
      dup2(lsToSort[STDIN_FILENO], STDIN_FILENO);
      close(lsToSort[STDIN_FILENO]);

      execlp("sort", "sort", "-r", NULL);
  }
  close(lsToSort[STDIN_FILENO]);
  close(sortToSelect[STDOUT_FILENO]);

  //e. It should make a pipe in selectToParent[2] and then make a child process to run select with arg[1] as the command line argument.
  //  This is very similar to the one above. Be sure to close() pipe file descriptors when they are no longer needed.

  if(pipe(selectToParent) < 0) {
      fprintf(stderr, "Cannot create pipe\n");
      exit(EXIT_FAILURE);
  }

  selectChild = fork();
  if (selectChild < 0) {
      fprintf(stderr, "fork() failed\n");
      exit(EXIT_FAILURE);
  }

  if (selectChild == 0) {
      dup2(selectToParent[STDOUT_FILENO], STDOUT_FILENO);
      close(selectToParent[STDIN_FILENO]);
      close(selectToParent[STDOUT_FILENO]);
      dup2(sortToSelect[STDIN_FILENO], STDIN_FILENO);
      close(sortToSelect[STDIN_FILENO]);
      execl("./select", "select", argv[1], NULL);
  }
  close(sortToSelect[STDIN_FILENO]);
  close(selectToParent[STDOUT_FILENO]);


  //  Receive and print output:
  char		line[LINE_LEN];
  int		numBytes;

  while  ( (numBytes=read(selectToParent[STDIN_FILENO],line,LINE_LEN-1)) > 0 )
  {
    line[numBytes]	= '\0';
    printf("%s",line);
  }

  close(selectToParent[STDIN_FILENO]);

  //  YOUR CODE HERE
  wait(NULL);
  wait(NULL);
  wait(NULL);
  return(EXIT_SUCCESS);
}