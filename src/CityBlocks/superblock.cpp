/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2011  Christian Urich
 
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

#include "superblock.h"
#include <dm.h>
#include <dmview.h>
//Creates a Block with 1 per 1 meter


DM_DECLARE_NODE_NAME( SuperBlock,BlockCity )
SuperBlock::SuperBlock()
{
    std::vector<DM::View> views;
    block = DM::View("SUPERBLOCK", DM::FACE, DM::WRITE);
    block.addAttribute("height");
    block.addAttribute("width");
    views.push_back(block);
    height = 1000;
    width = 1000;
    offsetx = 0;
    offsety = 0;
    this->addParameter("Height", DM::LONG, &height);
    this->addParameter("Width", DM::LONG, &width);
    this->addParameter("offsetx", DM::DOUBLE, &offsetx);
    this->addParameter("offsety", DM::DOUBLE, &offsety);
    this->addData("City", views);

}
void SuperBlock::run() {

    DM::System * blocks = this->getData("City");

    DM::Node * n1 = blocks->addNode(offsetx,offsety,0);
    DM::Node * n2 = blocks->addNode(offsetx+width,offsety,0);
    DM::Node * n3 = blocks->addNode(offsetx+width,offsety+height,0);
    DM::Node * n4 = blocks->addNode(offsetx,offsety+height,0);

    std::vector<DM::Node*> ve;
    ve.push_back(n1);
    ve.push_back(n2);
    ve.push_back(n3);
    ve.push_back(n4);
    ve.push_back(n1);

    DM::Face * f = blocks->addFace(ve, block);
    f->addAttribute("height", height);
    f->addAttribute("width", width);


}
