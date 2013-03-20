#include "buildings.h"
#include "cgalskeletonisation.h"
#include <tbvectordata.h>
#include <dmgeometry.h>

DM_DECLARE_NODE_NAME(Buildings, BlockCity)

Buildings::Buildings()
{
    view_buildings_nodes =  DM::View("Building_nodes", DM::NODE, DM::WRITE);
    view_buildings_faces =  DM::View("Building_faces", DM::FACE, DM::WRITE);

    std::vector<DM::View> datastream;
    datastream.push_back(view_buildings_nodes);
    datastream.push_back(view_buildings_faces);

    this->addData("city", datastream);
}

void Buildings::run()
{

    DM::System * sys = this->getData("city");

    DM::Node * n1_1 = sys->addNode(DM::Node(0,1,0));
    DM::Node * n1_2 = sys->addNode(DM::Node(0,3,0));
    DM::Node * n1_3 = sys->addNode(DM::Node(8,2,0));
    DM::Node * n1_4 = sys->addNode(DM::Node(4,0,0));
    DM::Node * n1_5 = sys->addNode(DM::Node(2,0,0));
    DM::Node * n1_6 = sys->addNode(DM::Node(2,1,0));

    std::vector<DM::Node * > nodes1;
    nodes1.push_back(n1_1);
    nodes1.push_back(n1_2);
    nodes1.push_back(n1_3);
    nodes1.push_back(n1_4);
    nodes1.push_back(n1_5);
    nodes1.push_back(n1_6);
    nodes1.push_back(n1_1);

    DM::Face * f = sys->addFace(nodes1);


    DM::System roof = DM::CGALSkeletonisation::StraightSkeletonisation(sys, f, 30);
    DM::View view_roof_faces("Roof", DM::EDGE, DM::WRITE);

    std::vector<std::string> roof_uuids = roof.getUUIDs(view_roof_faces);
    DM::SpatialNodeHashMap sphm(sys, 100, false);
    foreach (std::string uuid, roof_uuids) {
        DM::Face * f = roof.getFace(uuid);
        std::vector<DM::Node*> fnodes;
        std::vector<DM::Node*> nodes_old = TBVectorData::getNodeListFromFace(&roof, f);

        foreach (DM::Node * n, nodes_old) {
            DM::Logger(DM::Debug) << n->getZ();
            fnodes.push_back(sphm.addNode(n->getX(), n->getY(), n->getZ(), 0.001,view_buildings_nodes ));
        }
        sys->addFace(fnodes, view_buildings_faces);
    }


}
