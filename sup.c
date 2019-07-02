/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "arg.h"
#include "sha256.h"

#define nelem(x) (sizeof (x) / sizeof *(x))

struct rule_t {
	const int uid;
	const char *cmd;
	const char *path;
	const char *hash;
};

#include "config.h"

char *argv0;

void die(char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

#define CHUNK 1048576 /* 1MiB */
static uint32 getsha(const char *path, unsigned char *dest) {
	static sha256_context sha;

	unsigned char buf[CHUNK];
	uint32 len, tot = 0;
	FILE *fd;

	fd = fopen(path, "r");
	if (!fd)
		die("Can not read binary file.");

	sha256_starts(&sha);

	while ((len = fread(buf, 1, CHUNK, fd))) {
		sha256_update(&sha, buf, len);
		tot += len;
	}
	fclose(fd);

	sha256_finish(&sha, dest);
	return tot;
}

int main(int argc, char *argv[]) {
	unsigned int c, i, lflag = 0;
	unsigned char digest[32];
	char output[65];
	struct stat st;

	ARGBEGIN {
		case 'l':
			lflag = 1;
			break;
		default:
			die("Usage: sup [-l] command [ args ... ]");
	} ARGEND;

	if (lflag) {
		printf("List of compiled authorizations:\n");
		for (i = 0; i < nelem(rules); i++)
			printf("\nuser: %d\ncmd: %s\nbinary: %s\nsha256: %s\n",
					rules[i].uid, rules[i].cmd, rules[i].path, rules[i].hash);

		return 0;
	}

	if (argc < 1)
		die("Usage: sup [-l] command [ args ... ]");

	for (i = 0; i < nelem(rules); i++) {
		if (!strcmp(argv[0], rules[i].cmd)) {

			if (rules[i].uid != getuid() && rules[i].uid != -1)
				die("Unauthorized user.");

			if (stat(rules[i].path, &st) == -1)
				die("Can not stat program.");

			if (st.st_mode & 0022)
				die("Can not run binaries others can write.");

			if (getsha(rules[i].path, digest) != st.st_size)
				die("Binary file differs from size read.");

			for (c = 0; c < 32; c++)
				sprintf(output + (c*2), "%02x", digest[c]);
			output[64] = '\0';

			if (strncmp(rules[i].hash, output, 64))
				die("Checksums do not match.");

			if (setgid(SETGID) < 0) die("setgid failed");
			if (setuid(SETUID) < 0) die("setuid failed");

			if (execv(rules[i].path, argv) < 0)
				die("execv failed.");
		}
	}

	die("Unauthorized command.");
}
