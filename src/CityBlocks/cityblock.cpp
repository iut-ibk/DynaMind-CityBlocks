/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2011-2012  Christian Urich
 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "cityblock.h"
#include <dm.h>
#include <math.h>
#include <tbvectordata.h>
#include <dmgeometry.h>
#include <cgalgeometry.h>

DM_DECLARE_NODE_NAME(CityBlock,BlockCity)

CityBlock::CityBlock()
{

    std::vector<DM::View> views;
    superblock = DM::View("SUPERBLOCK", DM::FACE, DM::READ);
    superblock.addAttribute("CityBlock_Width");
    superblock.addAttribute("CityBlock_Height");
    cityblock = DM::View("CITYBLOCK", DM::FACE, DM::WRITE);
    cityblock.addAttribute("Area");
    cityblock.addAttribute("grid_id");
    cityblock.addAttribute("relative_x");
    cityblock.addAttribute("relative_y");

    streets = DM::View("STREET", DM::EDGE, DM::WRITE);
    intersections = DM::View("INTERSECTION", DM::NODE, DM::WRITE);

    centercityblock = DM::View("CENTERCITYBLOCK", DM::NODE, DM::WRITE);
    centercityblock.addAttribute("ID_CATCHMENT");

    views.push_back(superblock);
    views.push_back(cityblock);
    views.push_back(streets);
    views.push_back(intersections);
    views.push_back(centercityblock);


    this->width = 100;
    this->height = 100;
    this->createStreets = true;
    this->addParameter("Width", DM::DOUBLE, &this->width);
    this->addParameter("Height", DM::DOUBLE, &this->height);
    devider = 100;
    this->addParameter("CreateStreets", DM::BOOL, &this->createStreets);
    offset = 7.5;
    this->addParameter("Offset", DM::DOUBLE, &this->offset);
    this->addData("City", views);
}



void CityBlock::run() {

    DM::System * city = this->getData("City");

    DM::SpatialNodeHashMap nodeList(city, devider);

    std::vector<std::string> blockids = city->getUUIDsOfComponentsInView(superblock);

    foreach (std::string blockid, blockids) {
        //calulculate height;

        DM::Face * fblock = city->getFace(blockid);
        double minX = 0;
        double maxX = 0;
        double minY = 0;
        double maxY = 0;
        for (int i = 0; i < fblock->getNodes().size(); i++){

            DM::Node * n1 = city->getNode(fblock->getNodes()[i]);
            DM::Node * n2 = city->getNode(fblock->getNodes()[i]);

            if (i == 0) {
                minX = n1->getX();
                maxX = n1->getX();
                minY = n1->getY();
                maxY = n1->getY();
            }

            if(minX > n1->getX())
                minX = n1->getX();
            if(minX > n2->getX())
                minX = n2->getX();
            if(maxX < n1->getX())
                maxX = n1->getX();
            if(maxX < n2->getX())
                maxX = n2->getX();

            if(minY > n1->getY())
                minY = n1->getY();
            if(minY > n2->getY())
                minY = n2->getY();
            if(maxY < n1->getY())
                maxY = n1->getY();
            if(maxY < n2->getY())
                maxY = n2->getY();

        }
        double blockWidth = maxX - minX;
        double blockHeight = maxY - minY;


        DM::Logger(DM::Debug) << blockHeight;
        DM::Logger(DM::Debug) << blockWidth;


        //Create Parcels

        int elements_x = blockWidth/this->width;
        int elements_y = blockHeight/this->height;
        double realwidth = blockWidth / elements_x;
        double realheight = blockHeight / elements_y;
        fblock->addAttribute("CityBlock_Width",realwidth);
        fblock->addAttribute("CityBlock_Height",realheight);
        StartAndEndNodeList.clear();


        int counter = 0;
        for (int x = 0; x < elements_x; x++) {
            for (int y = 0; y < elements_y; y++) {
                counter++;
                DM::Node * n1 = nodeList.addNode(minX + realwidth*x,minY + realheight*y,0,
                                        .001, intersections);
                DM::Node * n2 = nodeList.addNode( minX + realwidth*(x+1),minY + realheight*y,0,
                                        .001, intersections);
                DM::Node * n3 = nodeList.addNode(minX + realwidth*(x+1),minY + realheight*(y+1),0,
                                        .001, intersections);
                DM::Node * n4 = nodeList.addNode(minX + realwidth*x,minY + realheight*(y+1),0,
                                        .001, intersections);

                DM::Edge * e1 = getAlreadyCreateEdge(n1, n2);
                DM::Edge * e2 = getAlreadyCreateEdge(n2, n3);
                DM::Edge * e3 = getAlreadyCreateEdge(n3, n4);
                DM::Edge * e4 = getAlreadyCreateEdge(n4, n1);

                //Every Edge is also a Street
                if (e1 == 0) {
                    e1 = city->addEdge(n1, n2);
                    if (createStreets)
                        city->addComponentToView(e1, streets);
                    this->addEdge(e1, n1, n2);
                }
                if (e2 == 0) {
                    e2 = city->addEdge(n2, n3);
                    if (createStreets)
                        city->addComponentToView(e2, streets);
                    this->addEdge(e2, n2, n3);
                }
                if (e3 == 0) {
                    e3 = city->addEdge(n3, n4);
                    if (createStreets)
                        city->addComponentToView(e3, streets);
                    this->addEdge(e3, n3, n4);
                }
                if (e4 == 0) {
                    e4 = city->addEdge(n4, n1);
                    if (createStreets)
                        city->addComponentToView(e4, streets);
                    this->addEdge(e4, n4, n1);
                }

                //Create Face
                std::vector<DM::Node*> ve;
                ve.push_back(n1);
                ve.push_back(n2);
                ve.push_back(n3);
                ve.push_back(n4);
                //ve.push_back(n1);

                std::vector<DM::Node> offest_nodes = DM::CGALGeometry::offsetPolygon(ve, 7.5);

                std::vector<DM::Node*> face_nodes;
                foreach (DM::Node n, offest_nodes) {
                    face_nodes.push_back(city->addNode(n));
                }
                if (face_nodes.size() < 3) {
                    DM::Logger(DM::Warning) << "Couldn't create face";
                    continue;
                }
                face_nodes.push_back(face_nodes[0]);
                
                DM::Face * f = city->addFace(face_nodes, cityblock);
                f->addAttribute("Area", realwidth*realheight);
                f->addAttribute("grid_id", counter);
                f->addAttribute("relative_x", x+1);
                f->addAttribute("relative_y", y+1);
                DM::Node * n =city->addNode(minX + realwidth*(x+0.5),minY + realheight*(y+0.5),0, centercityblock);
                DM::Attribute attr("ID_CATCHMENT");
                attr.setString(f->getUUID());
                n->addAttribute(attr);  
            }
        }
    }
    DM::Logger(DM::Debug) << "Number of CityBlocks " << city->getAllComponentsInView(cityblock).size();
    DM::Logger(DM::Debug) << "Number of Streets " << city->getAllComponentsInView(streets).size();

}

DM::Edge * CityBlock::getAlreadyCreateEdge(DM::Node * n1, DM::Node* n2) {
    if (StartAndEndNodeList.find(n1) == StartAndEndNodeList.end())
        return 0;
    std::map<DM::Node*, DM::Edge* > submap = StartAndEndNodeList[n1];
    if (submap.find(n2) == submap.end())
        return 0;
    return submap[n2];
}

void CityBlock::addEdge(DM::Edge *e, DM::Node * n1, DM::Node * n2) {
    if (StartAndEndNodeList.find(n1) == StartAndEndNodeList.end()) {
        StartAndEndNodeList[n1] = std::map<DM::Node*, DM::Edge* >();
    }
    std::map<DM::Node*, DM::Edge* > submap = StartAndEndNodeList[n1];
    submap[n2] = e;
    StartAndEndNodeList[n1] = submap;
    if (StartAndEndNodeList.find(n2) == StartAndEndNodeList.end()) {
        StartAndEndNodeList[n2] = std::map<DM::Node*, DM::Edge* >();
    }
    submap = StartAndEndNodeList[n2];
    submap[n1] = e;
    StartAndEndNodeList[n2] = submap;
}


