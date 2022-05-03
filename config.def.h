/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Iosevka Nerd Font-9" };
static const char col_gray1[]       = "#1a1b26";
static const char col_gray2[]       = "#24283b";
static const char col_gray3[]       = "#414868";
static const char col_gray4[]       = "#c0caf5";
static const char col_accent[]      = "#bb9af7";
static const char *colors[][3]      = {
        /*               fg          bg          border   */
        [SchemeNorm] = { col_gray4,  col_gray1,  col_gray2 },
        [SchemeSel]  = { col_accent, col_gray2,  col_accent  },
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V" };

static const Rule rules[] = {
        /* xprop(1):
         *      WM_CLASS(STRING) = instance, class
         *      WM_NAME(STRING) = title
         */
        /* class            instance    title           tags mask   float   monitor */
        { "st",             NULL,       NULL,           1 << 0,     0,      -1 },
        { "firefox",        NULL,       NULL,           1 << 1,     0,      -1 },
        { "Soffice",        NULL,       NULL,           1 << 2,     0,      -1 },
        { NULL,             NULL,       "LibreOffice",  1 << 2,     0,      -1 },
        { "Gimp",           NULL,       NULL,           1 << 2,     0,      -1 },
        { "Inkscape",       NULL,       NULL,           1 << 2,     0,      -1 },
        { "discord",        NULL,       NULL,           1 << 3,     0,      -1 },

        { "firefox",        "Toolkit",  NULL,           ~0,         1,      -1 },
        { "mpv",            NULL,       NULL,           ~0,         1,      -1 },
        { "Nsxiv",          NULL,       NULL,           ~0,         1,      -1 },
        { "Qalculate-gtk",  NULL,       NULL,           0,          1,      -1 },
        { "mGBA",           NULL,       NULL,           0,          1,      -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
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
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
        { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
        { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* component of dmenucmd, manipulated in spawn() */
static char dmenumon[2] = "0";

/* commands */
static const char *dmenucmd[]      = { "dmenu_run_i", NULL };
static const char *termcmd[]       = { "st", NULL };
static const char *firefox[]       = { "firefox", NULL };
static const char *qalculate[]     = { "qalculate-gtk", NULL };
static const char *lightdown[]     = { "notify-backlight", "down", NULL };
static const char *lightup[]       = { "notify-backlight", "up", NULL };
static const char *volmute[]       = { "notify-pamixer", "mute", NULL };
static const char *voldown[]       = { "notify-pamixer", "down", NULL };
static const char *volup[]         = { "notify-pamixer", "up", NULL };
static const char *dmenu_maim[]    = { "dmenu_maim", NULL };
static const char *dmenu_session[] = { "dmenu_session", NULL };
static const char *dmenu_emoji[]   = { "emojipick", NULL };
static const char *dmenu_gba[]     = { "dmenu_gba", NULL };
static const char *dmenu_mount[]   = { "dmenu_mount", NULL };
static const char *dmenu_unmount[] = { "dmenu_unmount", NULL };

static Key keys[] = {
        /* modifier                     key        function        argument */
        { MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
        { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
        { MODKEY,                       XK_w,      spawn,          {.v = firefox } },
        { MODKEY,                       XK_c,      spawn,          {.v = qalculate } },
        { MODKEY|ShiftMask,             XK_minus,  spawn,          {.v = lightdown } },
        { MODKEY,                       XK_equal,  spawn,          {.v = lightup } },
        { MODKEY,                       XK_0,      spawn,          {.v = volmute } },
        { MODKEY,                       XK_minus,  spawn,          {.v = voldown } },
        { MODKEY,                       XK_equal,  spawn,          {.v = volup } },
        { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = dmenu_maim } },
        { MODKEY,                       XK_Escape, spawn,          {.v = dmenu_session } },
        { MODKEY|ALTKEY,                XK_space,  spawn,          {.v = dmenu_emoji } },
        { MODKEY|ALTKEY,                XK_g,      spawn,          {.v = dmenu_gba } },
        { MODKEY|ALTKEY,                XK_m,      spawn,          {.v = dmenu_mount } },
        { MODKEY|ALTKEY,                XK_u,      spawn,          {.v = dmenu_unmount } },
        { MODKEY,                       XK_b,      togglebar,      {0} },
        { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
        { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
        { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
        { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
        { MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
        { MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
        { MODKEY,                       XK_g,      zoom,           {0} },
        { MODKEY,                       XK_Tab,    view,           {0} },
        { MODKEY,                       XK_q,      killclient,     {0} },
        { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
        { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
        { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
        { MODKEY,                       XK_f,      togglefloating, {0} },
        { MODKEY,                       XK_9,      view,           {.ui = ~0 } },
        { MODKEY,                       XK_s,      tag,            {.ui = ~0 } },
        { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
        { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
        { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
        { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
        TAGKEYS(                        XK_1,                      0)
        TAGKEYS(                        XK_2,                      1)
        TAGKEYS(                        XK_3,                      2)
        TAGKEYS(                        XK_4,                      3)
        TAGKEYS(                        XK_5,                      4)
        { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
        { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
        { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
        { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
        { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
        { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
        { ClkTagBar,            0,              Button1,        view,           {0} },
        { ClkTagBar,            0,              Button3,        toggleview,     {0} },
        { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
        { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

