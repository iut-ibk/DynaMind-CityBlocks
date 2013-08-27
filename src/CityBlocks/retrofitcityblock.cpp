#include "retrofitcityblock.h"

DM_DECLARE_NODE_NAME(RetrofitBlock,BlockCity)

RetrofitBlock::RetrofitBlock()
{
	this->remove_name = "";
	this->addParameter("Retrofit", DM::STRING, &this->remove_name);


}

void RetrofitBlock::init() {
	if (this->remove_name.empty())
		return;

	this->view_remove = DM::View(this->remove_name, DM::COMPONENT, DM::MODIFY);
	this->view_remove.getAttribute("retrofit");
	std::vector<DM::View> datastream;
	datastream.push_back(view_remove);
	this->addData("city", datastream);
}

void RetrofitBlock::run() {
	DM::System * city = this->getData("city");

	std::vector<std::string> uuids = city->getUUIDs(this->view_remove);
	foreach (std::string uuid, uuids) {
		DM::Component * cmp = city->getComponent(uuid);
		if (cmp->getAttribute("retrofit")->getDouble() < 0.01)
			continue;
		city->removeComponentFromView(cmp, this->view_remove);
	}


}


