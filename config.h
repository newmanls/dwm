/* See LICENSE file for copyright and license details. */
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
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

static const char *const autostart[] = {
        "setbg", NULL,
        "sxhkd", NULL,
        "dwmsb", "cmus", "volume", "battery", "wifi", "date", NULL,
        NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V" };

static const Rule rules[] = {
        /* xprop(1):
         *      WM_CLASS(STRING) = instance, class
         *      WM_NAME(STRING) = title
         */
        /* class            instance    title           tags mask   float   isterm    noswallow monitor */
        { "St",             NULL,       NULL,           1 << 0,     0,      1,        0,        -1 },
        { "firefox",        NULL,       NULL,           1 << 1,     0,      0,        0,        -1 },
        { "Soffice",        NULL,       NULL,           1 << 2,     0,      0,        0,        -1 },
        { NULL,             NULL,       "LibreOffice",  1 << 2,     0,      0,        0,        -1 },
        { "Gimp",           NULL,       NULL,           1 << 2,     0,      0,        0,        -1 },
        { "Inkscape",       NULL,       NULL,           1 << 2,     0,      0,        0,        -1 },
        { "discord",        NULL,       NULL,           1 << 3,     0,      0,        0,        -1 },
        { "firefox",        "Toolkit",  NULL,           ~0,         1,      0,        0,        -1 },
        { "firefox",        NULL,       "Library",      0,          1,      0,        0,        -1 },
        { "mpv",            NULL,       NULL,           0,          1,      0,        0,        -1 },
        { "Nsxiv",          NULL,       NULL,           0,          1,      0,        0,        -1 },
        { "Qalculate-gtk",  NULL,       NULL,           0,          1,      0,        0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

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
static const char *dmenucmd[]      = { "dmenu_run", NULL };
static const char *termcmd[]       = { "st", NULL };

static Key keys[] = {
        /* modifier                     key        function        argument */
        { MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
        { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
        { MODKEY,                       XK_b,      togglebar,      {0} },
        { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
        { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
        { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
        { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
        { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
        { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
        { MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
        { MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
        { MODKEY|ShiftMask,             XK_h,      zoom,           {0} },
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

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
        { "font",         STRING,  &font },
        { "color0",       STRING,  &normbgcolor },
        { "color0",       STRING,  &normbordercolor },
        { "foreground",   STRING,  &normfgcolor },
        { "background",   STRING,  &selbgcolor },
        { "accentColor",  STRING,  &selbordercolor },
        { "foreground",   STRING,  &selfgcolor },
};
