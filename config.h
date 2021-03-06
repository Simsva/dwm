/* See LICENSE file for copyright and license details. */

/* constants */
#define TERMINAL "st"
#define TERMCLASS "St"

#define GUI_EDITOR

/* appearance */
static unsigned int gappih       = 10;       /* horiz inner gap between windows */
static unsigned int gappiv       = 10;       /* vert inner gap between windows */
static unsigned int gappoh       = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov       = 10;       /* vert outer gap between windows and screen edge */
static int smartgaps             = 0;        /* 1 means no outer gap when there is only one window */

static unsigned int borderpx     = 1;        /* border pixel of windows */
static unsigned int snap         = 32;       /* snap pixel */
static const int swallowfloating = 0;        /* 1 means swallow floating windows by default */
static int showbar               = 1;        /* 0 means no bar */
static int topbar                = 1;        /* 0 means bottom bar */
static const char *fonts[] = {
	"monospace:size=10",
	"emoji:pixelsize=10:antialias=true:autohint=true",
};
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

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class        instance    title                                  tags mask     isfloating   isterminal     noswallow       monitor */
	{ "Gimp",       NULL,       NULL,                                  1 << 8,       1,           0,             0,              -1 },
	{ "Firefox",    NULL,       NULL,                                  1 << 2,       0,           0,             0,               1 },
	/* { "KeePassXC",  NULL,       NULL,                                  1 << 0,       0,           0,             0,               1 }, */
	{ "discord",    NULL,       NULL,                                  1 << 3,       0,           0,             0,               1 },
	{ "Anki",       NULL,       NULL,                                  1 << 5,       0,           0,             0,              -1 },
	{ TERMCLASS,    NULL,       NULL,                                  0,            0,           1,             0,              -1 },
	{ NULL,         NULL,       "Event Tester",                        0,            0,           0,             1,              -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },

	{ "[@]",      spiral },
	{ "[\\]",     dwindle },

	{ "H[]",      deck },
	{ "[M]",      monocle },

	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },

	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },

	/* unused layouts, TODO: comment out the functions in dwm.c */
	/*{ "===",      bstackhoriz },*/
	/*{ "HHH",      grid },*/
	/*{ "###",      nrowgrid },*/
	/*{ "---",      horizgrid },*/
	/*{ ":::",      gaplessgrid },*/
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,                          XK_j,     ACTION,         {.i = INC(+1)} }, \
	{ MOD,                          XK_k,     ACTION,         {.i = INC(-1)} }, \
	{ MOD,                          XK_v,     ACTION,         {.i = 0} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "color0",         STRING,  &normbgcolor },
	{ "color0",         STRING,  &normbordercolor },
	{ "color4",         STRING,  &normfgcolor },
	{ "color4",         STRING,  &selbgcolor },
	{ "color8",         STRING,  &selbordercolor },
	{ "color0",         STRING,  &selfgcolor },
	{ "borderpx",       INTEGER, &borderpx },
	{ "snap",           INTEGER, &snap },
	{ "showbar",        INTEGER, &showbar },
	{ "topbar",         INTEGER, &topbar },
	{ "nmaster",        INTEGER, &nmaster },
	{ "resizehints",    INTEGER, &resizehints },
	{ "mfact",          FLOAT,   &mfact },
	{ "gappih",         INTEGER, &gappih },
	{ "gappiv",         INTEGER, &gappiv },
	{ "gappoh",         INTEGER, &gappoh },
	{ "gappov",         INTEGER, &gappov },
	{ "smartgaps",      INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* navigation */
	{ MODKEY,                       XK_space,  spawn,          SHCMD("dmenu_run") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focusstack)
	STACKKEYS(MODKEY|ShiftMask,                pushstack)
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },

	/* layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[5]} }, /* monocle */
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_s,      togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[8]} }, /* floating */
	/*{ MODKEY|Mod1Mask,              XK_f,      toggleswallow,  {0} },*/

	/* tags */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* programs */
	{ MODKEY,                       XK_b,      spawn,          SHCMD("$BROWSER") },
#ifdef GUI_EDITOR
	{ MODKEY,                       XK_e,      spawn,          SHCMD("$EDITOR") },
#else
	{ MODKEY,                       XK_e,      spawn,          SHCMD(TERMINAL " -e \"$EDITOR\"") },
#endif
	{ MODKEY,                       XK_p,      spawn,          SHCMD(TERMINAL " -e pulsemixer") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("maimpick") },
	{ 0,                            XK_Print,  spawn,          SHCMD("maimpick sc") },

	/* system actions */
	{ MODKEY,                       XK_Escape, spawn,          SHCMD("sysact") },                           /* run sysact */
	{ MODKEY|ShiftMask,             XK_q,      spawn,          SHCMD("sysact -p \"Quit?\" dwmq") },         /* quit dwm */
	{ MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD("sysact -p \"Reload?\" dwmr") },       /* reload dwm (SIGHUP) */
	{ MODKEY|ShiftMask|ControlMask, XK_q,      spawn,          SHCMD("sysact -p \"Shutdown?\" shutdown") }, /* shutdown */
	{ MODKEY|ShiftMask|ControlMask, XK_r,      spawn,          SHCMD("sysact -p \"Reboot?\" reboot") },     /* reboot */

	/* multimedia */
	{ 0, XF86XK_AudioMute,          spawn,     SHCMD("pamixer -t; pkill -RTMIN+6 dwmblocks") },
	{ 0, XF86XK_AudioRaiseVolume,   spawn,     SHCMD("pamixer --allow-boost -i 3; pkill -RTMIN+6 dwmblocks") },
	{ 0, XF86XK_AudioLowerVolume,   spawn,     SHCMD("pamixer --allow-boost -d 3; pkill -RTMIN+6 dwmblocks") },
	{ 0, XF86XK_AudioPrev,          spawn,     SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,          spawn,     SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,         spawn,     SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,          spawn,     SHCMD("mpc toggle") },
	{ 0, XF86XK_AudioStop,          spawn,     SHCMD("mpc toggle") },
	{ 0, XF86XK_AudioRewind,        spawn,     SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,       spawn,     SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,         spawn,     SHCMD(TERMINAL " -e ncmpcpp") },
	{ 0, XF86XK_AudioMicMute,       spawn,     SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_PowerOff,           spawn,     SHCMD("sysact") },
	{ 0, XF86XK_Calculator,         spawn,     SHCMD(TERMINAL " -e python") },
	{ 0, XF86XK_WWW,                spawn,     SHCMD("$BROWSER") },
	{ 0, XF86XK_DOS,                spawn,     SHCMD(TERMINAL) },
	{ 0, XF86XK_TaskPane,           spawn,     SHCMD(TERMINAL " -e htop") },
	{ 0, XF86XK_MyComputer,         spawn,     SHCMD(TERMINAL " -e lf /") },
	{ 0, XF86XK_MonBrightnessUp,    spawn,     SHCMD("light -A 2; pkill -RTMIN+25 dwmblocks") },
	{ 0, XF86XK_MonBrightnessDown,  spawn,     SHCMD("light -U 2; pkill -RTMIN+25 dwmblocks") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif /* __OpenBSD */
#ifdef GUI_EDITOR
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD("\"$EDITOR\" ~/.local/src/dwmblocks/blocks.h") },
	{ ClkWinTitle,          ShiftMask,      Button3,        spawn,          SHCMD("\"$EDITOR\" ~/.local/src/dwm/config.h") },
#else
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e \"$EDITOR\" ~/.local/src/dwmblocks/blocks.h") },
	{ ClkWinTitle,          ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e \"$EDITOR\" ~/.local/src/dwm/config.h") },
#endif
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
	{ ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
