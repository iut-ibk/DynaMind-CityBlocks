#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <dm.h>

class DM_HELPER_DLL_EXPORT Buildings : public DM::Module
{
    DM_DECLARE_NODE(Buildings)
    private:
        DM::View view_buildings_nodes;
        DM::View view_buildings_edges;
        DM::View view_buildings_faces;
public:
    Buildings();
    void run();

};

#endif // BUILDINGS_H
