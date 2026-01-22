#include "chatprotocol.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

namespace protocol
{

	ProtocolHeartbeatAsk::ProtocolHeartbeatAsk()
	{
		m_type = EMsgType::EMT_Heartbeat_Ask;
	}

	std::shared_ptr<ProtocolHeartbeatAsk> ProtocolHeartbeatAsk::New()
	{
		return std::make_shared<ProtocolHeartbeatAsk>();
	}

	std::shared_ptr<QByteArray> ProtocolHeartbeatAsk::GetData()
	{
		QByteArray vecDescribe = m_strDescribe.toUtf8();

		std::shared_ptr<QByteArray> datas = std::make_shared<QByteArray>();

		//计算总数据长度
		m_len = sizeof(SMsgHead);
		m_len += sizeof(int);		//Describe数量
		m_len += vecDescribe.size();//Describe数据
		datas->resize(m_len);

		//拷贝数据
		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(p, &m_type, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(p, &m_len, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(p, &m_id, len);
		p += len;

		//Describe数量
		int nDescribeNum = vecDescribe.size();
		len = sizeof(int);
		memcpy(p, &nDescribeNum, len);
		p += len;
		//Describe数据
		len = vecDescribe.size();
		memcpy(p, vecDescribe.data(), len);
		p += len;

		return datas;
	}

	void ProtocolHeartbeatAsk::ParseData(const std::shared_ptr<QByteArray>& datas)
	{
		QByteArray vecDescribe;

		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(&m_type, p, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(&m_len, p, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(&m_id, p, len);
		p += len;

		//Describe数量
		int nDescribeNum = 0;
		len = sizeof(int);
		memcpy(&nDescribeNum, p, len);
		p += len;
		//Describe数据
		vecDescribe.resize(nDescribeNum);
		len = nDescribeNum;
		memcpy(vecDescribe.data(), p, len);
		p += len;

		m_strDescribe = QString::fromUtf8(vecDescribe);
	}

	ProtocolHeartbeatReply::ProtocolHeartbeatReply()
	{
		m_type = EMsgType::EMT_Heartbeat_Reply;
	}

	std::shared_ptr<ProtocolHeartbeatReply> ProtocolHeartbeatReply::New()
	{
		return std::make_shared<ProtocolHeartbeatReply>();
	}

	std::shared_ptr<QByteArray> ProtocolHeartbeatReply::GetData()
	{
		QByteArray vecDescribe = m_strDescribe.toUtf8();

		std::shared_ptr<QByteArray> datas = std::make_shared<QByteArray>();

		//计算总数据长度
		m_len = sizeof(SMsgHead);
		m_len += sizeof(int);		//Describe数量
		m_len += vecDescribe.size();//Describe数据
		datas->resize(m_len);

		//拷贝数据
		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(p, &m_type, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(p, &m_len, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(p, &m_id, len);
		p += len;

		//Describe数量
		int nDescribeNum = vecDescribe.size();
		len = sizeof(int);
		memcpy(p, &nDescribeNum, len);
		p += len;
		//Describe数据
		len = vecDescribe.size();
		memcpy(p, vecDescribe.data(), len);
		p += len;

		return datas;
	}

	void ProtocolHeartbeatReply::ParseData(const std::shared_ptr<QByteArray>& datas)
	{
		QByteArray vecDescribe;
		
		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(&m_type, p, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(&m_len, p, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(&m_id, p, len);
		p += len;

		//Describe数量
		int nDescribeNum = 0;
		len = sizeof(int);
		memcpy(&nDescribeNum, p, len);
		p += len;
		//Describe数据
		vecDescribe.resize(nDescribeNum);
		len = nDescribeNum;
		memcpy(vecDescribe.data(), p, len);
		p += len;

		m_strDescribe = QString::fromUtf8(vecDescribe);
	}

	ProtocolChatAsk::ProtocolChatAsk()
	{
		m_type = EMsgType::EMT_Chat_Ask;
	}

	std::shared_ptr<ProtocolChatAsk> ProtocolChatAsk::New()
	{
		return std::make_shared<ProtocolChatAsk>();
	}

	std::shared_ptr<QByteArray> ProtocolChatAsk::GetData()
	{
		QByteArray vecString = m_string.toUtf8();

		std::shared_ptr<QByteArray> datas = std::make_shared<QByteArray>();

		//计算总数据长度
		m_len = sizeof(SMsgHead);
		m_len += sizeof(int);		//字符串数量
		m_len += vecString.size();	//字符串数据
		datas->resize(m_len);

		//拷贝数据
		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(p, &m_type, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(p, &m_len, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(p, &m_id, len);
		p += len;

		//字符串数量
		int nStringNum = vecString.size();
		len = sizeof(int);
		memcpy(p, &nStringNum, len);
		p += len;
		//字符串数据
		len = vecString.size();
		memcpy(p, vecString.data(), len);
		p += len;

		return datas;
	}

	void ProtocolChatAsk::ParseData(const std::shared_ptr<QByteArray>& datas)
	{
		QByteArray vecString;

		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(&m_type, p, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(&m_len, p, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(&m_id, p, len);
		p += len;

		//字符串数量
		int nStringNum = 0;
		len = sizeof(int);
		memcpy(&nStringNum, p, len);
		p += len;
		//字符串数据
		vecString.resize(nStringNum);
		len = nStringNum;
		memcpy(vecString.data(), p, len);
		p += len;

		m_string = QString::fromUtf8(vecString);
	}

	ProtocolChatReply::ProtocolChatReply()
	{
		m_type = EMsgType::EMT_Chat_Reply;
	}

	std::shared_ptr<ProtocolChatReply> ProtocolChatReply::New()
	{
		return std::make_shared<ProtocolChatReply>();
	}

	std::shared_ptr<QByteArray> ProtocolChatReply::GetData()
	{
		QByteArray vecString = m_string.toUtf8();
		QByteArray vecIP = m_strClientIP.toUtf8();

		std::shared_ptr<QByteArray> datas = std::make_shared<QByteArray>();

		//计算总数据长度
		m_len = sizeof(SMsgHead);
		m_len += sizeof(int);		//字符串长度
		m_len += sizeof(int);		//IP字符串长度
		m_len += sizeof(quint16);	//端口号数据
		m_len += vecString.size();	//字符串数据
		m_len += vecIP.size();		//IP字符串数据
		datas->resize(m_len);

		//拷贝数据
		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(p, &m_type, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(p, &m_len, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(p, &m_id, len);
		p += len;

		//字符串长度
		int nStringNum = vecString.size();
		len = sizeof(int);
		memcpy(p, &nStringNum, len);
		p += len;
		//IP字符串长度
		int nIpStringNum = vecIP.size();
		len = sizeof(int);
		memcpy(p, &nIpStringNum, len);
		p += len;
		//端口号数据
		len = sizeof(quint16);
		memcpy(p, &m_nClientPort, len);
		p += len;
		//字符串数据
		len = vecString.size();
		memcpy(p, vecString.data(), len);
		p += len;
		//IP字符串数据
		len = vecIP.size();
		memcpy(p, vecIP.data(), len);
		p += len;

		return datas;
	}

	void ProtocolChatReply::ParseData(const std::shared_ptr<QByteArray>& datas)
	{
		QByteArray vecString;
		QByteArray vecIP;

		auto p = datas->data();
		//type
		size_t len = sizeof(uint16_t);
		memcpy(&m_type, p, len);
		p += len;
		//len
		len = sizeof(size_t);
		memcpy(&m_len, p, len);
		p += len;
		//id
		len = sizeof(uint64_t);
		memcpy(&m_id, p, len);
		p += len;

		//字符串数量
		int nStringNum = 0;
		len = sizeof(int);
		memcpy(&nStringNum, p, len);
		p += len;
		//IP字符串数量
		int nIpStringNum = 0;
		len = sizeof(int);
		memcpy(&nIpStringNum, p, len);
		p += len;
		//端口号数据
		len = sizeof(quint16);
		memcpy(&m_nClientPort, p, len);
		p += len;
		//字符串数据
		vecString.resize(nStringNum);
		len = nStringNum;
		memcpy(vecString.data(), p, len);
		p += len;
		//IP字符串数据
		vecIP.resize(nIpStringNum);
		len = nIpStringNum;
		memcpy(vecIP.data(), p, len);
		p += len;

		m_string = QString::fromUtf8(vecString);
		m_strClientIP = QString::fromUtf8(vecIP);
	}

}