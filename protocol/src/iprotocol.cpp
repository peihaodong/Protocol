#include "iprotocol.h"
#include <atomic>

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