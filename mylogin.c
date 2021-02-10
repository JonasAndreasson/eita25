/*
 * Shows user info from local pwfile.
 *  
 * Usage: userinfo username
 */

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pwdblib.h"   /* include header declarations for pwdblib.c */

/* Define some constants. */
#define USERNAME_SIZE (32)
#define NOUSER (-1)


int print_info(const char *username)
{
  struct pwdb_passwd *p = pwdb_getpwnam(username);
  if (p != NULL) {
    printf("Name: %s\n", p->pw_name);
    printf("Passwd: %s\n", p->pw_passwd);
    printf("Uid: %u\n", p->pw_uid);
    printf("Gid: %u\n", p->pw_gid);
    printf("Real name: %s\n", p->pw_gecos);
    printf("Home dir: %s\n",p->pw_dir);
    printf("Shell: %s\n", p->pw_shell);
	return 0;
  } else {
    return NOUSER;
  }
}

void read_username(char *username)
{
  printf("login: ");
  fgets(username, USERNAME_SIZE, stdin);

  /* remove the newline included by getline() */
  username[strlen(username) - 1] = '\0';
}
void read_password(char *password)
{
  printf("Password before password: %s\n", password);
  password = getpass("password: ");
  printf("Length of password: %ld\n",strlen(*password));
  printf("Password: %s\n", password);
  printf("*password: %d\n", *password);
  
}

int check_passwd(const char *username, const char *password)
{
  struct pwdb_passwd *p = pwdb_getpwnam(username);
  if (p == NULL){
    return 1;
  }
  char *salt = p->pw_passwd;
   printf("Prior to hash: %s\n", password);
  char *deshash = crypt(password,salt);
  printf("Post hash: %s\n", password);
  printf("%s\n",deshash);
  printf("%s\n", salt);
  if (strcmp(deshash, p->pw_passwd)==0){
    return 0;
  } else {
    return 1;
  }
  
}



int main(int argc, char **argv)
{
  char username[USERNAME_SIZE];
  char password[USERNAME_SIZE];
  /* 
   * Write "login: " and read user input. Copies the username to the
   * username variable.
   */
  int var = 1;
  while (var == 1)
  {
    read_username(username);
    //delete read_password and just do getpass here???
    read_password(password);
    var = check_passwd(username, password);
    if (var == 1){
      printf("Unknown user or incorrect password\n");
    }

  }
  printf("User authenticated successfully");
}
  

  
