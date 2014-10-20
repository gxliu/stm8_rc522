#include"iostm8s103f2.h"
#include"iwdg.h"
void iwdg_enable(void)
{
    IWDG_KR=KEY_ENABLE;
    IWDG_KR=KEY_ACCESS;
    IWDG_PR=0x06;//div by 256
    IWDG_RLR=0xFF;//1.02s reset interval
}

void iwdg_disable(void)
{
}

void iwdg_refresh(void)
{
    IWDG_KR=KEY_REFRESH;
}
