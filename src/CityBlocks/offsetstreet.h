#ifndef OFFSETSTREET_H
#define OFFSETSTREET_H

#include <dm.h>

class DM_HELPER_DLL_EXPORT OffsetStreet : public DM::Module
{
    DM_DECLARE_NODE(OffsetStreet)
private:
    DM::View view_streets;
    DM::View view_street_faces;
    DM::View view_cityblocks;
public:
    OffsetStreet();
    void run();
};

#endif // OFFSETSTREET_H
