#include "buildings.h"
#include "cgalskeletonisation.h"
#include <tbvectordata.h>
#include <dmgeometry.h>
//#include <cgaltriangulation.h>

DM_DECLARE_NODE_NAME(Buildings, BlockCity)

Buildings::Buildings()
{
    view_buildings_nodes =  DM::View("Building_nodes", DM::NODE, DM::WRITE);
    view_buildings_faces =  DM::View("Building_faces", DM::FACE, DM::WRITE);
    view_footprint = DM::View("Footprint", DM::FACE, DM::WRITE);

    std::vector<DM::View> datastream;
    datastream.push_back(view_buildings_nodes);
    datastream.push_back(view_buildings_faces);
    datastream.push_back(view_footprint);

    this->addData("city", datastream);
}

void Buildings::run()
{

    DM::System * sys = this->getData("city");

    DM::Node * n1_1 = sys->addNode(DM::Node(0,1,0));
    DM::Node * n1_2 = sys->addNode(DM::Node(0,3,0));
    DM::Node * n1_3 = sys->addNode(DM::Node(4,2,0));
    DM::Node * n1_4 = sys->addNode(DM::Node(4,0,0));
    DM::Node * n1_5 = sys->addNode(DM::Node(2,0,0));
    DM::Node * n1_6 = sys->addNode(DM::Node(2,1,0));

    //L
    std::vector<DM::Node * > nodes1;
    nodes1.push_back(n1_1);
    nodes1.push_back(n1_2);
    nodes1.push_back(n1_3);
    nodes1.push_back(n1_4);
    nodes1.push_back(n1_5);
    nodes1.push_back(n1_6);
    nodes1.push_back(n1_1);

    DM::Face * f = sys->addFace(nodes1, this->view_footprint);
    createRoof(sys, f, 30);

    nodes1.clear();

    n1_1 = sys->addNode(DM::Node(10,10,0));
    n1_2 = sys->addNode(DM::Node(10,14,0));
    n1_3 = sys->addNode(DM::Node(14,14,0));
    n1_4 = sys->addNode(DM::Node(14,10,0));

    nodes1.push_back(n1_1);
    nodes1.push_back(n1_2);
    nodes1.push_back(n1_3);
    nodes1.push_back(n1_4);
    nodes1.push_back(n1_1);

    f = sys->addFace(nodes1, this->view_footprint);
    createRoof(sys, f,30);

    nodes1.clear();

    n1_1 = sys->addNode(DM::Node(20,20,0));
    n1_2 = sys->addNode(DM::Node(20,24,0));
    n1_3 = sys->addNode(DM::Node(24,24,0));


    nodes1.push_back(n1_1);
    nodes1.push_back(n1_2);
    nodes1.push_back(n1_3);
    nodes1.push_back(n1_1);

    f = sys->addFace(nodes1, this->view_footprint);
    createRoof(sys, f,30);


}

void Buildings::createRoof(DM::System * sys, DM::Face * f, double alpha)
{
    DM::System roof = DM::CGALSkeletonisation::StraightSkeletonisation(sys, f, alpha);

    DM::View view_roof_faces("Roof", DM::EDGE, DM::WRITE);
    std::vector<std::string> roof_uuids = roof.getUUIDs(view_roof_faces);

    /*DM::SpatialNodeHashMap sphm(sys, 100, false);
    foreach (std::string uuid, roof_uuids) {
        DM::Face * f = roof.getFace(uuid);

        std::vector<DM::Node> triangles;
        CGALTriangulation::Triangulation(&roof, f, triangles);

        int numberOfTrinalges = triangles.size()/3;
        for (int i = 0; i < numberOfTrinalges; i++) {
            std::vector<DM::Node*> fnodes;
            for (int j = 0; j < 3; j++) {
                DM::Node n(triangles[i*3+j]);
                fnodes.push_back(sphm.addNode( n.getX(), n.getY(), n.getZ(),0.0001 ));
            }
            fnodes.push_back(fnodes[0]);
            sys->addFace(fnodes, view_buildings_faces);
        }
    }*/
}
