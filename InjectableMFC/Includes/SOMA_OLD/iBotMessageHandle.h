#ifndef BOTMSG_H
#define BOTMSG_H
#include <vector>

class iBotMessageHandle
{
public:
	CString iMessageSender;

	int iMessageSenderUid;

	bool m_bUserAlreadyGreeted,
		 m_bUserIgnore,
		 m_bPendingReply;

	int	m_vTotalMessagesReceived,
		m_vTotalMessagesSent;

	int	m_vMessagesReceived,
		m_vMessagesSent;

	int m_vChatsReceived,
		m_vChatsSent;

	std::vector<CString> MessageLog,
						 Type;
public:
	iBotMessageHandle::iBotMessageHandle();
	iBotMessageHandle::~iBotMessageHandle();
};

#endif