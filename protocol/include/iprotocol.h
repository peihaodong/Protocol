#pragma once
#include <QByteArray>
#include <memory>
#include <cstdint>

namespace protocol
{
#pragma pack(push, 1)
	struct Q_DECL_EXPORT SMsgHead
	{
		uint16_t m_type = 0;		//消息类型
		size_t m_len = 0;			//消息大小（消息头+消息体）
		uint64_t m_id = 0;			//消息id
	};
#pragma pack(pop)

	class Q_DECL_EXPORT IProtocol
	{
	public:
		virtual ~IProtocol();
		//获取数据
		virtual std::shared_ptr<QByteArray> GetData() = 0;
		//解析数据
		virtual void ParseData(const std::shared_ptr<QByteArray>& datas) = 0;

		//获取唯一ID
		uint64_t GetUniqueID();

	public:
		uint16_t m_type = 0;		//消息类型
		size_t m_len = 0;			//消息大小（消息头+消息体）
		uint64_t m_id = 0;			//消息id
	};
	using PProtocol = std::shared_ptr<IProtocol>;

}
