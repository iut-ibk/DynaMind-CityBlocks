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
	DM::View view_footprint;
public:
	Buildings();
	void run();
	void createRoof(DM::System *sys, DM::Face *f, double alpha);

};

#endif // BUILDINGS_H
