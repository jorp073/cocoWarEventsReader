#ifndef DEFINE_H
#define DEFINE_H

#include <QtGlobal>

#ifdef QT_OS_MAC
#define RPATH "res/"
#else
#define RPATH "F:\\workspace\\COCClashHistoryReader\\test\\res\\dialogrecognize\\"
#endif

#define OUT

#define CLOSEBUTTON_MIN_AREA 100

// War EventBar Attack Bg Color is 194
// War EventBar Defense Bg Color is 174
#define IS_ATTACK_COLOR(c) (c>(194+174)/2)

#endif // DEFINE_H
