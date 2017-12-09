#ifndef DEFINE_H
#define DEFINE_H

#include <QtGlobal>

#ifdef Q_OS_MACX
#define RPATH "test/res/dialogrecognize/"
#define SPATH "res/sample/"
#else
#define RPATH "F:\\workspace\\COCClashHistoryReader\\test\\res\\dialogrecognize\\"
#define SPATH "F:\\workspace\\COCClashHistoryReader\\res\\sample\\"
#endif

#define OUT

#define CLOSEBUTTON_MIN_AREA 100


// WarEvent界面 列表项的主背景色（去除顶部），有为进攻方和防御方的背景色
#define EVENTBAR_ATTACK_BGCOLOR 193
#define EVENTBAR_DEFENSE_BGCOLOR 173
// War EventBar Attack Bg Color is 194
// War EventBar Defense Bg Color is 174

#define IS_ATTACK_COLOR(c) (c>(EVENTBAR_ATTACK_BGCOLOR+EVENTBAR_DEFENSE_BGCOLOR)/2)

enum STAR_TYPE {
    HIGHTLIGHT,
    STAR,
    BLANK
};

#define STAR_COLORS std::map<STAR_TYPE, uchar>{{HIGHTLIGHT, 208},{STAR, 204},{BLANK, 178}}

#endif // DEFINE_H
