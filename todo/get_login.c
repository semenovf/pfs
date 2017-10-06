#ifdef _WIN32
#	include <windows.h>
/*#	include <lmcons.h>*/
#else
#	include <sys/types.h>
#	include <unistd.h>
#	include <pwd.h>
#endif

char_type* jq_c_getlogin( void  )
{
	char_type *login;
#ifdef _WIN32
	/* http://msdn.microsoft.com/en-us/library/ms724432(v=vs.85).aspx */
	DWORD sz = 1;     /* size of user name */
	char_type buf[1]; /* buffer for expanded string */

	login = buf;
	if( !GetUserName(login, &sz) ) {
		JQ_ASSERT(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		login = JQ_MALLOC(char_type,sz);
		JQ_ASSERT(GetUserName(login, &sz));
	}
#else /* POSIX */
	uid_t uid;
	struct passwd *pw;
	uid = getuid();

	JQ_LOCK;
	pw = getpwuid(uid);
	JQ_ASSERT(pw);
	login = JQ_STRDUP(pw->pw_name);
	JQ_UNLOCK;
#endif

	return login;
}
