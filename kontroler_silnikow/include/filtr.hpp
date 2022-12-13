#ifndef FITLR_H
#define FILTR_H
#include "silnik.hpp"

class Filtr : public Motor
{
public:
    Filtr();
    void setStop(bool hard);
    bool moveHome(uint32_t delayImp);
    bool movePosition(int32_t pos, uint32_t delayImp);
    void impulse();

private:
    volatile unsigned int temp1;
    volatile unsigned int temp2;
    volatile unsigned int temp3;
    volatile unsigned int temp4;
    volatile unsigned int impPerObrot;
    volatile bool err;
};

#endif /* D4D794A5_8612_420B_A935_316874503426 */
