#include "iprotocol.h"
#include <atomic>

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

namespace protocol
{

	IProtocol::~IProtocol()
	{

	}

	uint64_t IProtocol::GetUniqueID()
	{
		static std::atomic<uint64_t> g_ID = 0;
		g_ID++;
		return g_ID;
	}

}