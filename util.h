#pragma once
//=============================================================
//    ## namespace ## MY_UTIL
//=============================================================

#define PI 3.1415926f
#define PI2 (PI * 2)

//나중에 이미지 회전시 사용할 값
#define PI_2 (PI / 2) //90도
#define PI_4 (PI / 4) //45도
#define PI_8 (PI / 8) //22.5도

namespace MY_UTIL
{
    //두점 사이의 거리
    float getDistance(float startX, float startY, float endX, float endY);

    //두점 사이의 각도
    float getAngle(float startX, float startY, float endX, float endY);


    float lerp(float v0, float v1, float t);

};

enum class MONSTERKIND {
    GOLEM,
    KNIGHT,
    SUMMONER,
    GHOUL,
    GHOULLARGE
};

enum STAGEKIND {
    STARTSTAGE,
    MIDDLESTAGE,
    FINALSTAGE,
    STAGEMAX
};

enum class PLAYERSKILL {
    BLAZE,
    METEOR,
    DASHFIRE,
    INFERNO,
    DRAGONARC,
    ICESPEAR,

};