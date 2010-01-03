/* pancake <nopcode.org> -- Copyleft 2009 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define HELP "sup [-hlv] [cmd ..]"
#define VERSION "sup 0.1 pancake <nopcode.org> copyleft 2009"

struct rule_t {
	int uid;
	int gid;
	const char *cmd;
	const char *path;
};

#include "config.h"

static int die(int ret, const char *org, const char *str) {
	fprintf (stderr, "%s%s%s\n", org, org?": ":"", str);
	return ret;
}

int main(int argc, char **argv) {
	int i, uid, gid, ret;

	if (argc < 2 || !strcmp (argv[1], "-h"))
		return die (1, NULL, HELP);

	if (!strcmp (argv[1], "-v"))
		return die (1, NULL, VERSION);

	if (!strcmp (argv[1], "-l")) {
		for (i = 0; rules[i].cmd != NULL; i++)
			printf ("%d %d %10s %s\n", rules[i].uid, rules[i].gid,
				rules[i].cmd, rules[i].path);
		return 0;
	}

	uid = getuid ();
	gid = getgid ();

	for (i = 0; rules[i].cmd != NULL; i++) {
		if (!rules[i].cmd[0] || !strcmp (argv[1], rules[i].cmd)) {
#if ENFORCE	
			struct stat st;
			lstat (rules[i].path, &st);
			if (st.st_mode & 0222)
				return die (1, "stat", "Cannot run writable binaries.");
#endif
			if (uid != SETUID && rules[i].uid != -1 && rules[i].uid != uid)
				return die (1, "urule", "User does not match");

			if (gid != SETGID && rules[i].gid != -1 && rules[i].gid != gid)
				return die (1, "grule", "Group id does not match");

			if (setuid (SETUID) == -1 || setgid (SETGID) == -1 ||
			    seteuid (SETUID) == -1 || setegid (SETGID) == -1)
				return die (1, "set[e][ug]id", strerror (errno));
#ifdef CHROOT
			if (*CHROOT)
				if (chdir (CHROOT) == -1 || chroot (".") == -1)
					return die (1, "chroot", strerror (errno));
			if (*CHRDIR)
				if (chdir (CHRDIR) == -1)
					return die (1, "chdir", strerror (errno));
#endif
			ret = execv (*rules[i].path? rules[i].path:argv[1], argv+1);
			return die (ret, "execv", strerror (errno));
		}
	}

	return die (1, NULL, "Sorry");
}
