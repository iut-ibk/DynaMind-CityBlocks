#ifndef RETROFITCITYBLOCK_H
#define RETROFITCITYBLOCK_H

#include <dm.h>

class  DM_HELPER_DLL_EXPORT RetrofitBlock : public DM::Module
{
	DM_DECLARE_NODE(RetrofitBlock)
	private:
		DM::View view_remove;
	std::string remove_name;


public:
	RetrofitBlock();
	void init();
	void run();
};

#endif // RETROFITCITYBLOCK_H
