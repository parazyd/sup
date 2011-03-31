#define USER 1000
#define GROUP -1

#define SETUID 0
#define SETGID 0

#define CHROOT ""
#define CHRDIR ""

#define ENFORCE 1

static struct rule_t rules[] = {
	{ USER, GROUP, "whoami", "/usr/bin/whoami" },
	{ USER, GROUP, "ifconfig", "/sbin/ifconfig" },
	{ USER, GROUP, "ls", "/bin/ls" },
	{ USER, GROUP, "wifi", "/root/wifi.sh" },
	{ USER, GROUP, "cp", "*"}, // allow to run this program in PATH
	{ USER, GROUP, "*", "*"}, // allow to run any program in PATH
	{ 0 },
};
