/* See LICENSE file for copyright and license details. */

/* User and group to run as */
#define SETUID 0
#define SETGID 0

/* sup authorizations
 *
 * The format is as follows:
 * - UID allowed to run the command (-1 means anyone)
 * - Alias/command used to call the executable
 * - Path to the executable to run
 * - SHA256 checksum of the executable
 */
static struct rule_t rules[] = {
	{ 1000, "ls", "/bin/ls",
		"87e8fd7d813c135875aca43a4da43d3ced41d325ed2931906444471b4e93e017" },

	{ -1, "grep", "/bin/grep",
		"fedeb344d1be24f4a340591cd25ed81f7e46ea12772f563c9c9f43773028e23a" },

	{ 1000, "tar", "/bin/tar",
		"fedeb344d1be24f4a340591cd25ed81f7e46ea12772f563c9c9f43773028e23a" },

	{ 1001, "test", "./a.out",
		"254260b676a44f1529f7c855f0126a57a3fbd7ec8a74de08835f08e8e6ed21be" },
};
