[CCode (lower_case_cprefix = "", cheader_filename = "config.h")]
namespace Config
{
	/* Gettext package */
	public const string GETTEXT_PACKAGE;

	/* Configured paths - these variables are not present in config.h, they are
	 * passed to underlying C code as cmd line macros. */
	public const string LUNAR_DATE_LOCALEDIR;  /* /usr/share/locale  */
}
