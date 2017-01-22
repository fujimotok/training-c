#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_NUM 50 

enum {
  CMD_L_GO_AHEAD = 0,
  CMD_L_STOP,
  CMD_L_GO_BACK,
  CMD_R_GO_AHEAD,
  CMD_R_STOP,
  CMD_R_GO_BACK,
  CMD_LAUNCH,
  CMD_LAUNCH_INV,
  CMD_KILL,
};

FILE *g_fp = NULL;

int log_open (const char *filename) {
  g_fp = fopen(filename, "a");
  return g_fp != NULL ? 1 : 0;
}

void log_close (void) {
  if (g_fp) { fclose (g_fp); }
}

#define log(fmt,...) fprintf(g_fp, "%s(%d):" fmt "\n", __func__, __LINE__, ## __VA_ARGS__)

int main(int argc, char* argv[])
{
  int i, len, cmd;
  char c; 
  char *pLen = NULL;
  char buf[BUF_NUM]={0};

  pLen = getenv("CONTENT_LENGTH");
  if (pLen == NULL){return -1;}
  len = atoi(pLen);

  if (!log_open("log.txt")) { return -1; }
  atexit(log_close);

  if (len == 0 || len > BUF_NUM - 1) {
    log("len NG:%d",len);
    return -1;
  }

  //改行文字等は除外しないと後で思わぬエラーが生じる
  for (i = 0; ((c = getchar()) != EOF) && (i <= len); i++) {
    if (c != '\r' && c != '\n')
      buf[i] = c;
    else
      i--;
  }
  buf[i] = '\0';

  log("buf:%s", buf);

  if (strncmp(buf, "cmd=", 4)) {
    log("cmd ng:%s",buf);
    return -1;
  }

  cmd = buf[4] - '0';

  switch (cmd) {
  case CMD_L_GO_AHEAD:
    log("ACCEPT:CMD_L_GO_AHEAD:%s", buf);
    system("sudo echo 1 > /sys/class/gpio/gpio67/value");
    system("sudo echo 0 > /sys/class/gpio/gpio69/value");
    break;
  case CMD_L_STOP:
    log("ACCEPT:CMD_L_STOP:%s", buf);
    system("sudo echo 0 > /sys/class/gpio/gpio67/value");
    system("sudo echo 0 > /sys/class/gpio/gpio69/value");
    break;
  case CMD_L_GO_BACK:
    log("ACCEPT:CMD_L_BACK:%s", buf);
    system("sudo echo 0 > /sys/class/gpio/gpio67/value");
    system("sudo echo 1 > /sys/class/gpio/gpio69/value");
    break;
  case CMD_R_GO_AHEAD:
    log("ACCEPT:CMD_R_GO_AHEAD:%s", buf);
    system("sudo echo 1 > /sys/class/gpio/gpio71/value");
    system("sudo echo 0 > /sys/class/gpio/gpio73/value");
    break;
  case CMD_R_STOP:
    log("ACCEPT:CMD_R_STOP:%s", buf);
    system("sudo echo 0 > /sys/class/gpio/gpio71/value");
    system("sudo echo 0 > /sys/class/gpio/gpio73/value");
    break;
  case CMD_R_GO_BACK:
    log("ACCEPT:CMD_R_GO_BACK:%s", buf);
    system("sudo echo 0 > /sys/class/gpio/gpio71/value");
    system("sudo echo 1 > /sys/class/gpio/gpio73/value");
    break;
  case CMD_LAUNCH:
    log("ACCEPT:CMD_LAUNCH:%s", buf);
    system("sudo line_trace &");
    break;
  case CMD_LAUNCH_INV:
    log("ACCEPT:CMD_LAUNCH_INV:%s", buf);
    system("sudo line_trace 1 &");
    break;
  case CMD_KILL:
    log("ACCEPT:CMD_KILL:%s", buf);
    system("sudo killall line_trace");
    break;
  default:
    break;
  }

  return 0;

}
