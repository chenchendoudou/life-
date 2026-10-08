#ifndef CSAPP_H
#define CSAPP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>

/* Minimal stand-in for CS:APP's csapp.h.
 * Only includes the wrappers that the examples in code/ecf actually use.
 * Each wrapper aborts on failure (same semantics as the textbook). */

typedef void handler_t;

void unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

pid_t Fork(void) {
    pid_t pid = fork();
    if (pid < 0) unix_error("Fork error");
    return pid;
}

pid_t Waitpid(pid_t pid, int *statusp, int options) {
    pid_t p = waitpid(pid, statusp, options);
    if (p < 0 && errno != ECHILD) unix_error("Waitpid error");
    return p;
}

pid_t Wait(int *statusp) {
    return Waitpid(-1, statusp, 0);
}

void Sleep(unsigned int secs) {
    sleep(secs);
}

void Pause(void) {
    (void)pause();
}

char *Fgets(char *ptr, int n, FILE *stream) {
    char *r = fgets(ptr, n, stream);
    if (r == NULL && ferror(stream)) unix_error("Fgets error");
    return r;
}

/* Global environment pointer — declared via extern so the linker
 * pulls it from the libc runtime. */
extern char **environ;

void Kill(pid_t pid, int sig) {
    if (kill(pid, sig) < 0) unix_error("Kill error");
}

unsigned int Alarm(unsigned int secs) {
    return alarm(secs);
}

void Sigemptyset(sigset_t *set) {
    if (sigemptyset(set) < 0) unix_error("Sigemptyset error");
}
void Sigfillset(sigset_t *set) {
    if (sigfillset(set) < 0) unix_error("Sigfillset error");
}
void Sigaddset(sigset_t *set, int signum) {
    if (sigaddset(set, signum) < 0) unix_error("Sigaddset error");
}
void Sigdeletelist(sigset_t *set, int signum) {
    if (sigdelset(set, signum) < 0) unix_error("Sigdelset error");
}
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
    if (sigprocmask(how, set, oldset) < 0) unix_error("Sigprocmask error");
}

handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;
    action.sa_handler = handler;
    Sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return old_action.sa_handler;
}

void Execve(const char *filename, char *const argv[], char *const envp[]) {
    if (execve(filename, argv, envp) < 0) unix_error("Execve error");
}

#define MAXLINE  8192
#define MAXBUF   8192

#endif