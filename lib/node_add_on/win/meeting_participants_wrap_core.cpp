
#include "../meeting_participants_wrap_core.h"
#include "wrap/sdk_wrap.h"
#include "zoom_native_to_wrap.h"
#include "sdk_events_wrap_class.h"
extern ZOOM_SDK_NAMESPACE::IMeetingServiceWrap& g_meeting_service_wrap;

ZUserInfoWrap::ZUserInfoWrap()
{
	m_pUserInfo = NULL;
}
ZUserInfoWrap::~ZUserInfoWrap()
{
	m_pUserInfo = NULL;
}

ZMeetingParticipantsWrap::ZMeetingParticipantsWrap()
{
	SDKEventWrapMgr::GetInst().m_meetingParticipantsCtrlEvent.SetOwner(this);
	m_pSink = 0;
}
ZMeetingParticipantsWrap::~ZMeetingParticipantsWrap()
{
	Uninit();
	m_pSink = 0;
	SDKEventWrapMgr::GetInst().m_meetingParticipantsCtrlEvent.SetOwner(NULL);
}
void ZMeetingParticipantsWrap::Init()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().Init_Wrap(&g_meeting_service_wrap);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().SetEvent(&SDKEventWrapMgr::GetInst().m_meetingParticipantsCtrlEvent);
}
void ZMeetingParticipantsWrap::Uninit()
{
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().SetEvent(NULL);
	ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().Uninit_Wrap();
}
void ZMeetingParticipantsWrap::SetSink(ZNativeSDKMeetingParticipantsWrapSink* pSink)
{
	m_pSink = pSink;
}

void ZMeetingParticipantsWrap::onUserJoin(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList)
{
	if (m_pSink)
		m_pSink->onUserJoin(lstUserID, strUserList);
}
void ZMeetingParticipantsWrap::onUserLeft(ZNList<unsigned int> lstUserID, ZoomSTRING strUserList)
{
	if (m_pSink)
		m_pSink->onUserLeft(lstUserID, strUserList);
}
void ZMeetingParticipantsWrap::onHostChangeNotification(unsigned int userId)
{
	if (m_pSink)
		m_pSink->onHostChangeNotification(userId);
}
void ZMeetingParticipantsWrap::onLowOrRaiseHandStatusChange(unsigned int userId, bool isRaisedHand)
{
	if (m_pSink)
		m_pSink->onLowOrRaiseHandStatusChange(userId, isRaisedHand);
}
ZNSDKError ZMeetingParticipantsWrap::LowerHand(unsigned int userid)
{
	ZOOM_SDK_NAMESPACE::SDKError sdk_err = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().LowerHand(userid);
	ZNSDKError err = Map2WrapDefine(sdk_err);
	return err;
}
ZNSDKError ZMeetingParticipantsWrap::LowerAllHands()
{
	ZOOM_SDK_NAMESPACE::SDKError sdk_err = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().LowerAllHands();
	ZNSDKError err = Map2WrapDefine(sdk_err);
	return err;

}
ZNList<unsigned int> ZMeetingParticipantsWrap::GetParticipantsList()
{
	ZNList<unsigned int> userId_list;
	ZOOM_SDK_NAMESPACE::IList<unsigned int >* lstUserID = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().GetParticipantsList();
	if (NULL == lstUserID)
		return userId_list;
	for (int i = 0; i < lstUserID->GetCount(); ++i)
	{

		unsigned int zn_userid;
		
		zn_userid = lstUserID->GetItem(i);

		userId_list.push_back(zn_userid);
	}
	return userId_list;

}
ZNUserInfomation ZMeetingParticipantsWrap::GetUserInfomationByUserID(unsigned int userid)
{
	ZNUserInfomation zn_UserInfomation;

	if (!m_user_info.IsValidUserID(userid))
	{
		zn_UserInfomation.userInfoType = ZN_FAKE_USERINFO;
	}
	else
	{
		zn_UserInfomation.userName = m_user_info.GetUserNamme(userid);
		//zn_UserInfomation.email = m_user_info.GetEmail(userid);
		zn_UserInfomation.isHost = m_user_info.IsHost(userid);
		zn_UserInfomation.userID = m_user_info.GetUserID(userid);
		zn_UserInfomation.isVideoOn = m_user_info.IsVideoOn(userid);
		zn_UserInfomation.isAudioMuted = m_user_info.IsAudioMuted(userid);
		zn_UserInfomation.audioJoinType = m_user_info.GetAudioJoinType(userid);
		zn_UserInfomation.isMySelf = m_user_info.IsMySelf(userid);
		zn_UserInfomation.isInWaitingRoom = m_user_info.IsInWaitingRoom(userid);
		zn_UserInfomation.isRaiseHand = m_user_info.IsRaiseHand(userid);
		zn_UserInfomation.userRole = m_user_info.GetUserRole(userid);
		zn_UserInfomation.isPurePhoneUser = m_user_info.IsPurePhoneUser(userid);
		zn_UserInfomation.AudioVoiceLevel = m_user_info.GetAudioVoiceLevel(userid);
		zn_UserInfomation.isClosedCaptionSender = m_user_info.IsClosedCaptionSender(userid);
		zn_UserInfomation.webinarAttendeeStatus = m_user_info.GetWebinarAttendeeStauts(userid);
		zn_UserInfomation.audioStatus = m_user_info.GetAudioStatus(userid);
		zn_UserInfomation.userInfoType = ZN_REAL_USERINFO;
	}

	return zn_UserInfomation;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ZUserInfoWrap::IsValidUserID(unsigned int userid)
{

	bool bIsValid = false;
	ZOOM_SDK_NAMESPACE::IUserInfo* userinfo = ZOOM_SDK_NAMESPACE::CSDKWrap::GetInst().GetMeetingServiceWrap().T_GetMeetingParticipantsController().
		GetUserByUserID(userid);
	if (userinfo)
	{
		m_pUserInfo = userinfo;
		bIsValid = true;
	}
	else
	{
		m_pUserInfo = NULL;
	}
	return bIsValid;
}

ZoomSTRING ZUserInfoWrap::GetUserNamme(unsigned int userid)
{
	
	ZoomSTRING zn_user_name;
	if (m_pUserInfo)
	{
		zn_user_name = m_pUserInfo->GetUserNameA();
	}
	return zn_user_name;
}
//ZoomSTRING ZUserInfoWrap::GetEmail(unsigned int userid)
//{
//	
//	ZoomSTRING zn_user_email;
//	if (m_pUserInfo)
//	{
//		zn_user_email = m_pUserInfo->GetEmail();
//	}
//	return zn_user_email;
//}
bool ZUserInfoWrap::IsHost(unsigned int userid)
{
	
	bool zn_is_host = false;
	if (m_pUserInfo)
	{
		zn_is_host = m_pUserInfo->IsHost();
	}
	return zn_is_host;
}
unsigned int ZUserInfoWrap::GetUserID(unsigned int userid)
{
	
	unsigned int user_id = 0;

	if (m_pUserInfo)
	{
		user_id = m_pUserInfo->GetUserID();
	}
	return user_id;
}
bool ZUserInfoWrap::IsVideoOn(unsigned int userid)
{
	
	bool zn_is_on = false;
	
	if (m_pUserInfo)
	{
		zn_is_on = m_pUserInfo->IsVideoOn();
	}
	return zn_is_on;
}
bool ZUserInfoWrap::IsAudioMuted(unsigned int userid)
{
	
	bool zn_is_muted = true;

	if (m_pUserInfo)
	{
		zn_is_muted = m_pUserInfo->IsAudioMuted();
	}
	return zn_is_muted;
}
ZNAudioType ZUserInfoWrap::GetAudioJoinType(unsigned int userid)
{
	return Map2WrapDefine(m_pUserInfo ? m_pUserInfo->GetAudioJoinType() : ZOOM_SDK_NAMESPACE::AUDIOTYPE_NONE);
}
bool ZUserInfoWrap::IsMySelf(unsigned int userid)
{
	
	bool zn_is_myself = false;
	if (m_pUserInfo)
	{
		zn_is_myself = m_pUserInfo->IsMySelf();
	}
	return zn_is_myself;
}
bool ZUserInfoWrap::IsInWaitingRoom(unsigned int userid)
{
	
	bool zn_is_in = false;
	
	if (m_pUserInfo)
	{
		zn_is_in = m_pUserInfo->IsInWaitingRoom();
	}
	return zn_is_in;
}
bool ZUserInfoWrap::IsRaiseHand(unsigned int userid)
{
	
	bool zn_is_raise = false;

	if (m_pUserInfo)
	{
		zn_is_raise = m_pUserInfo->IsRaiseHand();
	}
	return zn_is_raise;
}
ZNUserRole ZUserInfoWrap::GetUserRole(unsigned int userid)
{
	return Map2WrapDefine(m_pUserInfo ? m_pUserInfo->GetUserRole() : ZOOM_SDK_NAMESPACE::USERROLE_NONE);
}
ZNAudioStatus ZUserInfoWrap::GetAudioStatus(unsigned int userid)
{
	return Map2WrapDefine(m_pUserInfo ? m_pUserInfo->GetAudioStatus() : ZOOMSDK::Audio_None);
}
bool ZUserInfoWrap::IsPurePhoneUser(unsigned int userid)
{
	
	bool zn_is_pure_phone = false;
	
	if (m_pUserInfo)
	{
		zn_is_pure_phone = m_pUserInfo->IsPurePhoneUser();
	}
	return zn_is_pure_phone;
}
bool ZUserInfoWrap::IsClosedCaptionSender(unsigned int userid)
{
	
	bool zn_is_ = false;
	
	if (m_pUserInfo)
	{
		zn_is_ = m_pUserInfo->IsClosedCaptionSender();
	}
	return zn_is_;
}
bool ZUserInfoWrap::GetWebinarAttendeeStauts(unsigned int userid)
{
	
	if (NULL == m_pUserInfo)
		return false;
	ZOOM_SDK_NAMESPACE::WebinarAttendeeStatus* p_webinarAttendeeStatus = m_pUserInfo->GetWebinarAttendeeStauts();
	if (p_webinarAttendeeStatus)
	{
		bool zn_is_allow_talk = p_webinarAttendeeStatus->allow_talk;
		return zn_is_allow_talk;
	}
	else
	{
		return false;
	}
}
ZoomSTRING ZUserInfoWrap::GetAudioVoiceLevel(unsigned int userid)
{
	int audio_voice_level = 0;

	if (m_pUserInfo)
		audio_voice_level = m_pUserInfo->GetAudioVoiceLevel();
	wchar_t temp[1024];
	int radix = 10;
	_itow(audio_voice_level, temp, radix);
	ZoomSTRING zn_audio_voice_level = temp;
	return zn_audio_voice_level;
}