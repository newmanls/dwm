/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static char font[]                  = "monospace:size=10";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-c", "spterm", "-g", "160x34", NULL };
const char *spcmd2[] = {"st", "-c", "spmusic", "-g", "160x34", "-e", "cmus", NULL };
const char *spcmd3[] = {"st", "-c", "spcalc", "-g", "56x8", "-f", "monospace:size=12", "-e", "bc", "-lq", NULL};
static Sp scratchpads[] = {
	/* name         cmd  */
	{"spterm",      spcmd1},
	{"spmusic",     spcmd2},
        {"spcalc",      spcmd3},
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance   title           tags mask  isfloating  monitor */
	{ "St",	           NULL,      NULL,           1 << 0,    0,          -1 },
	{ "firefox",       NULL,      NULL,           1 << 1,    0,          -1 },
	{ "Soffice",       NULL,      NULL,           1 << 2,    0,          -1 },
	{ NULL,	           NULL,      "LibreOffice",  1 << 2,    0,          -1 },
	{ "Gimp",          NULL,      NULL,           1 << 2,    0,          -1 },
	{ "Inkscape",      NULL,      NULL,           1 << 2,    0,          -1 },
	{ "mpv",           NULL,      NULL,           0,         1,          -1 },
	{ "Nsxiv",         NULL,      NULL,           0,         1,          -1 },
	{ "st-floating",   NULL,      NULL,           0,         1,          -1 },
	{ "spterm",        NULL,      NULL,           SPTAG(0),  1,          -1 },
	{ "spmusic",       NULL,      NULL,           SPTAG(1),  1,          -1 },
	{ "spcalc",        NULL,      NULL,           SPTAG(2),  1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu-run-i", NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier                     key         function        argument */
	{ MODKEY|ShiftMask,             XK_Escape,  spawn,          SHCMD("dmenu-session") },
	TAGKEYS(                        XK_1,                       0)
	TAGKEYS(                        XK_2,                       1)
	TAGKEYS(                        XK_3,                       2)
	TAGKEYS(                        XK_4,                       3)
	TAGKEYS(                        XK_5,                       4)
	{ MODKEY,                       XK_9,       view,           {.ui = ~0 } },
	{ MODKEY,                       XK_0,       spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle && kill -37 $(pidof dwmblocks)") },
	// { MODKEY|ShiftMask,             XK_0,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_minus,   spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10% && kill -37 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_minus,   spawn,          SHCMD("xbacklight -dec 10") },
	{ MODKEY,                       XK_equal,   spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10% && kill -37 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_equal,   spawn,          SHCMD("xbacklight -inc 10") },

	{ MODKEY,                       XK_Tab,     view,           {0} },
	{ MODKEY,                       XK_q,       killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,       spawn,          SHCMD("xdotool getwindowfocus windowkill") },
	{ MODKEY,                       XK_w,       spawn,          SHCMD("firefox") },
	// { MODKEY|ShiftMask,             XK_w,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_e,       spawn,          SHCMD("st -e lfub") },
	{ MODKEY|ShiftMask,             XK_e,       spawn,          SHCMD("dmenu-emoji") },
	// { MODKEY,                       XK_r,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_r,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_t,       setlayout,      {.v = &layouts[0]} },
	// { MODKEY|ShiftMask,             XK_t,       spawn,          SHCMD("") },
	// { MODKEY,                       XK_y,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_y,       spawn,          SHCMD("") },
	// { MODKEY,                       XK_u,       spawn,          SHCMD("") },
	{ MODKEY|ShiftMask,             XK_u,       spawn,          SHCMD("dmenu-disks unmount") },
	{ MODKEY,                       XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,       setmfact,       {.f = +0.05} },
	// { MODKEY,                       XK_o,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_o,       spawn,          SHCMD("") },
        { MODKEY,                       XK_p,       togglescratch,  {.ui = 1} },
	// { MODKEY|ShiftMask,             XK_p,       spawn,          SHCMD("") },
        { MODKEY,                       XK_bracketleft,  spawn,     SHCMD("cmus-remote --prev") },
        { MODKEY,                       XK_bracketright, spawn,     SHCMD("cmus-remote --next") },
        { MODKEY,                       XK_backslash,    spawn,     SHCMD("cmus-remote --pause") },

	// { MODKEY,                       XK_a,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_a,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_s,       tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_s,       spawn,          SHCMD("dmenu-screenshot") },
	{ MODKEY,                       XK_d,       incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_d,       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_f,       togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,       setlayout,      {.v = &layouts[1]} },
	// { MODKEY,                       XK_g,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_g,       spawn,          SHCMD("") },
	// { MODKEY,                       XK_h,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_h,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_j,       focusstack,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_j,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_k,       focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_k,       zoom,           {0} },
	// { MODKEY,                       XK_l,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_l,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_Return,  spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return,  togglescratch,  {.ui = 0} },

	// { MODKEY,                       XK_z,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_z,       spawn,          SHCMD("") },
	// { MODKEY,                       XK_x,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_x,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_c,       togglescratch,  {.ui = 2} },
	{ MODKEY|ShiftMask,             XK_c,       spawn,          SHCMD("st -c st-floating -e nmtui-connect") },
	// { MODKEY,                       XK_v,       spawn,          SHCMD("") },
	// { MODKEY|ShiftMask,             XK_v,       spawn,          SHCMD("") },
	{ MODKEY,                       XK_b,       togglebar,      {0} },
	// { MODKEY|ShiftMask,             XK_b,       spawn,          SHCMD("") },
        { MODKEY,                       XK_n,       spawn,          SHCMD("st -e newsboat") },
	{ MODKEY|ShiftMask,             XK_n,       spawn,          SHCMD("st -e podboat") },
	{ MODKEY,                       XK_m,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_m,       spawn,          SHCMD("dmenu-disks mount") },
	{ MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_space,   spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_space,   spawn,          SHCMD("rofi -show drun") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = dmenucmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{ "font",               STRING,  &font },
	{ "color0",		STRING,  &normbgcolor },
	{ "color8",		STRING,  &normbordercolor },
	{ "foreground",		STRING,  &normfgcolor },
	{ "color8",             STRING,  &selbgcolor },
	{ "color7",		STRING,  &selbordercolor },
	{ "foreground",         STRING,  &selfgcolor },
};
