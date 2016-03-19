#ifndef __INLINE_H__
#define __INLINE_H__
#include <iostream>

class CPeople {
public:
    CPeople(int age, bool bHouse, int* p);
    ~CPeople();

    void SetHouse(bool bStatus) { m_hasHouse = bStatus; }

    void Show() const;
private:
    int m_age;
    //bool m_hasCar;
    bool m_hasHouse;
    int* m_pNull;
};

#endif
