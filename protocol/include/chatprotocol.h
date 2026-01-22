#pragma once
#include "iprotocol.h"
#include <QString>

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

namespace protocol
{
	enum EMsgType
	{
		EMT_NULL,
		EMT_Heartbeat_Ask,		//心跳_请求
		EMT_Heartbeat_Reply,	//心跳_响应
		EMT_Chat_Ask,			//聊天_请求
		EMT_Chat_Reply,			//聊天_响应
	};

	//心跳_请求
	class Q_DECL_EXPORT ProtocolHeartbeatAsk : public IProtocol
	{
	public:
		ProtocolHeartbeatAsk();
		static std::shared_ptr<ProtocolHeartbeatAsk> New();

		//获取数据
		virtual std::shared_ptr<QByteArray> GetData() override;
		//解析数据
		virtual void ParseData(const std::shared_ptr<QByteArray>& datas) override;

	public:
		QString m_strDescribe = "ping";
	};
	using PProtocolHeartbeatAsk = std::shared_ptr<ProtocolHeartbeatAsk>;
	//心跳_响应
	class Q_DECL_EXPORT ProtocolHeartbeatReply : public IProtocol
	{
	public:
		ProtocolHeartbeatReply();
		static std::shared_ptr<ProtocolHeartbeatReply> New();

		//获取数据
		virtual std::shared_ptr<QByteArray> GetData() override;
		//解析数据
		virtual void ParseData(const std::shared_ptr<QByteArray>& datas) override;

	public:
		QString m_strDescribe = "pang";
	};
	using PProtocolHeartbeatReply = std::shared_ptr<ProtocolHeartbeatReply>;

	//聊天_请求
	class Q_DECL_EXPORT ProtocolChatAsk : public IProtocol
	{
	public:
		ProtocolChatAsk();
		static std::shared_ptr<ProtocolChatAsk> New();

		//获取数据
		virtual std::shared_ptr<QByteArray> GetData() override;
		//解析数据
		virtual void ParseData(const std::shared_ptr<QByteArray>& datas) override;

	public:
		QString m_string;		//聊天字符串
	};
	using PProtocolChatAsk = std::shared_ptr<ProtocolChatAsk>;
	//聊天_响应
	class Q_DECL_EXPORT ProtocolChatReply : public IProtocol
	{
	public:
		ProtocolChatReply();
		static std::shared_ptr<ProtocolChatReply> New();

		//获取数据
		virtual std::shared_ptr<QByteArray> GetData() override;
		//解析数据
		virtual void ParseData(const std::shared_ptr<QByteArray>& datas) override;

	public:
		QString m_string;			//聊天字符串
		QString m_strClientIP;		//ip地址
		quint16 m_nClientPort = 0;	//端口号
	};
	using PProtocolChatReply = std::shared_ptr<ProtocolChatReply>;
}