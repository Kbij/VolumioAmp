/*
 * Raspberry Pi Ultimate Alarm Clock
 */
#include <string>
#include "glog/logging.h"
#include "gflags/gflags.h"

#include <chrono>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <pthread.h>
#include <iostream>
#include <fstream>

DEFINE_bool(daemon, false, "Run rgbclock as Daemon");
DEFINE_string(pidfile,"","Pid file when running as Daemon");
DEFINE_bool(i2cstatistics, false, "Print I2C statistics");


void signal_handler(int sig);
void daemonize();
bool runMain = true;

int pidFilehandle;

void signal_handler(int sig)
{
	switch(sig)
    {
		case SIGHUP:
			LOG(WARNING) << "Received SIGHUP signal.";
			break;
		case SIGINT:
		case SIGTERM:
		case SIGKILL:
			LOG(INFO) << "Daemon exiting";
			runMain = false;
			break;
		default:
			LOG(WARNING) << "Unhandled signal " << strsignal(sig);
			break;
    }
}

void registerSignals()
{
    struct sigaction newSigAction;
    sigset_t newSigSet;

    /* Set signal mask - signals we want to block */
    sigemptyset(&newSigSet);
    sigaddset(&newSigSet, SIGCHLD);  /* ignore child - i.e. we don't need to wait for it */
    sigaddset(&newSigSet, SIGTSTP);  /* ignore Tty stop signals */
    sigaddset(&newSigSet, SIGTTOU);  /* ignore Tty background writes */
    sigaddset(&newSigSet, SIGTTIN);  /* ignore Tty background reads */
    sigaddset(&newSigSet, SIGKILL);  /* Netbeans terminate process */
    sigprocmask(SIG_BLOCK, &newSigSet, NULL);   /* Block the above specified signals */

    /* Set up a signal handler */
    newSigAction.sa_handler = signal_handler;
    sigemptyset(&newSigAction.sa_mask);
    newSigAction.sa_flags = 0;

    /* Signals to handle */
    sigaction(SIGHUP, &newSigAction, NULL);     /* catch hangup signal */
    sigaction(SIGTERM, &newSigAction, NULL);    /* catch term signal */
    sigaction(SIGINT, &newSigAction, NULL);     /* catch interrupt signal */
}

void daemonize()
{
    int pid, sid, i;
    char str[10];

    /* Check if parent process id is set */
 //   if (getppid() == 1)
 //   {
 //   	std::cout << "Already daemon" << std::endl;
 //       /* PPID exists, therefore we are already a daemon */
 //       return;
 //   }

    /* Fork*/
    pid = fork();
    std::cout << "Forked !!" << std::endl;
    if (pid < 0)
    {
        /* Could not fork */
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
    	std::cout << "Exit parent" << std::endl;
        /* Child created ok, so exit parent process */
        exit(EXIT_SUCCESS);
    }

    /* Get a new process group */
    sid = setsid();
    std::cout << "This is the forked process" << std::endl;
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* close all descriptors */
    for (i = getdtablesize(); i >= 0; --i)
    {
        close(i);
    }

    /* Route I/O connections */

    /* Open STDIN */
    i = open("/dev/null", O_RDWR);

    /* STDOUT */
    dup(i);

    /* STDERR */
    dup(i);

//    chdir(rundir); /* change running directory */

    /* Ensure only one copy */
    pidFilehandle = open(FLAGS_pidfile.c_str(), O_RDWR|O_CREAT, 0600);

    if (pidFilehandle == -1 )
    {
        /* Couldn't open lock file */
    	LOG(ERROR) << "Could not open PID lock file " << FLAGS_pidfile << ", exiting";
        exit(EXIT_FAILURE);
    }

    /* Try to lock file */
    if (lockf(pidFilehandle,F_TLOCK,0) == -1)
    {
        /* Couldn't get lock on lock file */
    	LOG(WARNING) << "Could not lock PID lock file " << FLAGS_pidfile << ", exiting";
        exit(EXIT_FAILURE);
    }

    /* Get and format PID */
    sprintf(str,"%d\n",getpid());

    /* write pid to lockfile */
    write(pidFilehandle, str, strlen(str));
    std::cout << "pid file wirtten" << std::endl;
}

int main (int argc, char* argv[])
{
	std::string usage("Raspberry Pi Ultimate Alarm Clock. Sample usage:\n");
	usage += argv[0];
	gflags::SetUsageMessage(usage);
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	std::cout << "Starting, registering signals..." << std::endl;

	registerSignals();

	if (FLAGS_daemon)
	{
		std::cout << "Daemonize..." << std::endl;
		daemonize();
	}

    try
    {
		// Start the RTC clock first; need to have a valid date/time for logging
		// Don't use any glog functions @constructor time of RTC
		google::InitGoogleLogging("RGBClock");


		LOG(INFO) << "Raspberry Pi Ultimate Alarm Clock";
		LOG(INFO) << "=================================";




		close(pidFilehandle);
		LOG(INFO) << "Exit application";
	}
	catch (std::string &ex)
	{
		std::cerr << "Exception: " << ex << std::endl;
	}

	return EXIT_SUCCESS;
}

