#include "inline.h"

CPeople::CPeople(int age, bool bHouse, int* p)
: m_age(age)
//, m_hasCar(false)
, m_hasHouse(bHouse)
, m_pNull(p)
{
}


CPeople::~CPeople()
{
}


void CPeople::Show() const
{
    // just for debug
    std::cout << m_age << std::endl;
}
