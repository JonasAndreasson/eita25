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
  
  password = getpass("Password: ");

  /* remove the newline included by getline() */
  password[strlen(password) - 1] = '\0';
  
}
char* substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}

int check_passwd(const char *username, const char *password)
{
  struct pwdb_passwd *p = pwdb_getpwnam(username);
  if (p == NULL){
    return 1;
  }
  if(p->pw_failed >= 5){
    fprint("This account is locked.");
    return 1;
  }
  const char *salt = substr(p->pw_passwd, 0, 1);
  if (strcmp(crypt(password,salt), p->pw_passwd)==0){
    return 0;
  } else {
    return 1;
  }
  
}
int failed_login(const char *username)
{
  struct pwdb_passwd *p = pwdb_getpwnam(username);
  if(p == NULL){
    return 0;
  }
  if(p->pw_failed < 5){
    p->pw_failed = p->pw_failed + 1;
    return p->pw_failed;
  }
  else {
    return 0;
  }
}

int successful_login(const char *username)
{
  struct pwdb_passwd *p = pwdb_getpwnam(username);
  p->pw_age = p->pw_age + 1;
  p->pw_failed = 0;
  return p->pw_age;
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
    read_password(password);
    var = check_passwd(username, password);
    if (var == 0){
      if(successful_login(username) > 10){
        printf("The password should be updated.");
      }
      
      break;
    }
    printf("Unknown user or incorrect password\n");
    failed_login(username);
  }
  printf("User authenticated successfully");
}
  

  
