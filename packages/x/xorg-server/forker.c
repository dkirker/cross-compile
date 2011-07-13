/*
 * ===========================================================================
 *
 *       Filename:  forker.c
 *
 *    Description:  Sole purpose is to fork off the requested process
 *
 *        Version:  1.0
 *        Created:  04/28/2011 01:04:01 PM
 *
 *         Author:  Will Dietz (WD), webos@wdtz.org
 *        Company:  dtzTech
 *
 * ===========================================================================
 */

int main(int argc, char ** argv, char ** envp)
{
  if (fork())
  {
    // Parent
    return 0;
  }

  // Execute the requested command
  execve(argv[1], argv+1, envp);
}
