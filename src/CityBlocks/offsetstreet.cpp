#include "offsetstreet.h"
#include "cgalgeometry.h"
#include "tbvectordata.h"

#include <math.h>

DM_DECLARE_NODE_NAME(OffsetStreet,BlockCity)

OffsetStreet::OffsetStreet()
{
    view_streets = DM::View("STREET", DM::EDGE,DM::READ);
    view_streets.addAttribute("width");

    view_street_faces = DM::View("STREET_FACE", DM::FACE, DM::WRITE);

    view_cityblocks = DM::View("CITYBLOCK", DM::FACE, DM::READ);

    std::vector<DM::View> datastream;
    datastream.push_back(view_streets);
    datastream.push_back(view_street_faces);

    this->addData("city", datastream);
}

void OffsetStreet::run() {
    DM::System * city = this->getData("city");

    DM::System genSys;

    std::vector<std::string> street_uuids = city->getUUIDs(view_streets);
    foreach (std::string street_uuid,street_uuids) {
        DM::Edge * e = city->getEdge(street_uuid);
        double offset = e->getAttribute("width")->getDouble() / 2;
        DM::Node * n1 = city->getNode(e->getStartpointName());
        DM::Node * n2 = city->getNode(e->getEndpointName());

        DM::Node dn = *(n1) - *(n2);

        double alpha = atan(dn.getY()/dn.getX());

        double dx = - sin(alpha) * offset;
        double dy = cos(alpha) * offset;

        DM::Node n_offest(dx, dy, 0);

        DM::Node * n1_new = genSys.addNode(*n1+n_offest);
        DM::Node * n2_new = genSys.addNode(*n2+n_offest);

        DM::Node * n3_new = genSys.addNode(*n2-n_offest);
        DM::Node * n4_new = genSys.addNode(*n1-n_offest);

        genSys.addEdge(n1_new, n2_new, view_streets);
        //city->addEdge(n2_new, n3_new, view_street_faces);
        genSys.addEdge(n3_new, n4_new, view_streets);
        //city->addEdge(n4_new, n1_new, view_street_faces);


    }

    DM::System return_sys = DM::CGALGeometry::ShapeFinder(&genSys, view_streets, view_street_faces, true, 0.01, true);


    std::vector<std::string> face_uuids = return_sys.getUUIDs(view_street_faces);

    std::vector<std::string> cityblock_uuids = city->getUUIDs(view_cityblocks);
    std::vector<QPointF> centroids;
    foreach (std::string cityblock_uuid, cityblock_uuids) {
        DM::Face * f = city->getFace(cityblock_uuid);
        DM::Node n_c = TBVectorData::CaclulateCentroid(city, f);
        QPointF p(n_c.getX(), n_c.getY());
        centroids.push_back(p);
    }

    foreach (std::string face_uuid, face_uuids) {
        DM::Face * f = return_sys.getFace(face_uuid);
        std::vector<DM::Node*> node_l = TBVectorData::getNodeListFromFace(&return_sys, f);

        std::vector<DM::Node*> nodes_new;
        foreach (DM::Node * n, node_l) {
            nodes_new.push_back(city->addNode(*n));
        }
        QPolygonF qf = TBVectorData::FaceAsQPolgonF(&return_sys, f);

        bool exists = false;
        foreach (QPointF n_c, centroids) {
            if(qf.containsPoint(n_c, Qt::OddEvenFill) )
                exists = true;
        }
        if  (exists)
            continue;

        city->addFace(nodes_new, view_street_faces);

    }

    DM::Logger(DM::Debug) << "Number of Faces " << face_uuids.size();


}
