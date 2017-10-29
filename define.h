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


// WarEvent界面 列表项的主背景色（去除顶部），有为进攻方和防御方的背景色
#define EVENTBAR_ATTACK_BGCOLOR 193
#define EVENTBAR_DEFENSE_BGCOLOR 173
// War EventBar Attack Bg Color is 194
// War EventBar Defense Bg Color is 174

#define IS_ATTACK_COLOR(c) (c>(EVENTBAR_ATTACK_BGCOLOR+EVENTBAR_DEFENSE_BGCOLOR)/2)



#endif // DEFINE_H
