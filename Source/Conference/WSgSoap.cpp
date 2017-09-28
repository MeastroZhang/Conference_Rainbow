#pragma once

#include "StdAfx.h"
#include "WSgSoap.h"
#include <ShellAPI.h>
#include "../../include/Util/Util.h"

VidyoClientInEventLogIn tLoginParams = {0};
char chEndpoint[1024] = {0};

void SoapSetLoginParams(const VidyoClientInEventLogIn* pLoginParams)
{
	strcpy(tLoginParams.portalUri, pLoginParams->portalUri);
	strcpy(tLoginParams.userName, pLoginParams->userName);
	strcpy(tLoginParams.userPass, pLoginParams->userPass);

	GetSoapEndpoint(pLoginParams->portalUri, chEndpoint);
}

char* GetSoapEndpoint(const char* pPortalUrl, char* pEndpoint)
{
	char chEndpoint[1024] = {0};
	char chHttp[] = "http://";

	if (strncmp(pPortalUrl, chHttp, strlen(chHttp)) == 0)
	{
		strcat(chEndpoint, pPortalUrl);
	}
	else
	{
		memcpy(chEndpoint, chHttp, strlen(chHttp));
		strcat(chEndpoint, pPortalUrl);
	}
		
	strcat(chEndpoint, "/services/v1_1/VidyoPortalUserService/");
	memcpy(pEndpoint, chEndpoint, strlen(chEndpoint));

	return pEndpoint;
}

void SoapSearch(char* EntityID, VidyoClientAccountList* ptAccountList)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__SearchRequest* request = new _ns1__SearchRequest;
	_ns1__SearchResponse response;
	request->soap_alloc();
	request->Filter = new _ns1__Filter;
	request->Filter->query = EntityID;
	_ns1__EntityType iType = _ns1__EntityType__Room;
	request->Filter->EntityType = &iType;

	if (soap_call___ns1__search(&soap, chEndpoint, NULL, request, response) == SOAP_OK)
	{
		ptAccountList->accountData = new VidyoClientAccountData[response.__sizeEntity];
		ptAccountList->total = response.__sizeEntity;
		for (int i = 0; i < response.__sizeEntity; i++)
		{
			memset(ptAccountList->accountData[i].entityID, 0, sizeof(ptAccountList->accountData[i].entityID));
			memcpy(ptAccountList->accountData[i].entityID, response.Entity[i]->entityID, strlen(response.Entity[i]->entityID));
			memset(ptAccountList->accountData[i].ownerID, 0, sizeof(ptAccountList->accountData[i].ownerID));
			memcpy(ptAccountList->accountData[i].ownerID, response.Entity[i]->ownerID, strlen(response.Entity[i]->ownerID));
			memset(ptAccountList->accountData[i].displayName, 0, sizeof(ptAccountList->accountData[i].displayName));
			memcpy(ptAccountList->accountData[i].displayName, response.Entity[i]->displayName, strlen(response.Entity[i]->displayName));
			memset(ptAccountList->accountData[i].extension, 0, sizeof(ptAccountList->accountData[i].extension));
			memcpy(ptAccountList->accountData[i].extension, response.Entity[i]->extension, strlen(response.Entity[i]->extension));
			memset(ptAccountList->accountData[i].tenant, 0, sizeof(ptAccountList->accountData[i].tenant));
			memcpy(ptAccountList->accountData[i].tenant, response.Entity[i]->tenant, strlen(response.Entity[i]->tenant));

			ptAccountList->accountData[i].entityType = (VidyoClientPortalServiceEntityType)(response.Entity[i]->EntityType+1);
			ptAccountList->accountData[i].memberStatus = *(VidyoClientMemberStatus*)response.Entity[i]->MemberStatus;
			ptAccountList->accountData[i].roomStatus = (VidyoClientRoomStatus)(*(VidyoClientRoomStatus*)response.Entity[i]->RoomStatus+1);
			ptAccountList->accountData[i].isInMyContacts = *(VidyoBool*)response.Entity[i]->isInMyContacts;
			ptAccountList->accountData[i].canCallDirect = *(VidyoBool*)response.Entity[i]->canCallDirect;
			ptAccountList->accountData[i].canJoinMeeting = *(VidyoBool*)response.Entity[i]->canJoinMeeting;
			ptAccountList->accountData[i].canControl = *(VidyoBool*)response.Entity[i]->canControl;
			ptAccountList->accountData[i].roomMode.isRoomLocked = response.Entity[i]->RoomMode->isLocked;
			ptAccountList->accountData[i].roomMode.hasPIN = response.Entity[i]->RoomMode->hasPIN;
			ptAccountList->accountData[i].roomMode.hasModeratorPIN = *(VidyoBool*)response.Entity[i]->RoomMode->hasModeratorPIN;

			memset(ptAccountList->accountData[i].roomMode.roomPIN, 0, sizeof(ptAccountList->accountData[i].roomMode.roomPIN));
			if (ptAccountList->accountData[i].roomMode.hasPIN)
				memcpy(ptAccountList->accountData[i].roomMode.roomPIN, response.Entity[i]->RoomMode->roomPIN, strlen(response.Entity[i]->RoomMode->roomPIN));

			memset(ptAccountList->accountData[i].roomMode.moderatorPIN, 0, sizeof(ptAccountList->accountData[i].roomMode.moderatorPIN));		
			memset(ptAccountList->accountData[i].roomMode.roomURL, 0, sizeof(ptAccountList->accountData[i].roomMode.roomURL));
			if (ptAccountList->accountData[i].canControl)
			{
				if (ptAccountList->accountData[i].roomMode.hasModeratorPIN)
					memcpy(ptAccountList->accountData[i].roomMode.moderatorPIN, response.Entity[i]->RoomMode->moderatorPIN, strlen(response.Entity[i]->RoomMode->moderatorPIN));					
				memcpy(ptAccountList->accountData[i].roomMode.roomURL, response.Entity[i]->RoomMode->roomURL, strlen(response.Entity[i]->RoomMode->roomURL));
			}	
		}
	}
	delete request->Filter;
	delete request;
	request = NULL;
	soap_end(&soap);
	soap_done(&soap);
}

void SoapInvite(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__GetInviteContentRequest* reqest = new _ns1__GetInviteContentRequest;
	_ns1__GetInviteContentResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	if (soap_call___ns1__getInviteContent(&soap, chEndpoint, NULL, reqest, response) == SOAP_OK)
	{
		TCHAR czTempSub[1024] = {0}, czTempCon[1024] = {0};
		ChangUtf8ToText(response.content, czTempCon);
		ChangUtf8ToText(response.subject, czTempSub);

		int iLen = wcslen(czTempCon);
		TCHAR szContent[1024+20] = {0};

		for (int i=0,j=0; i < iLen; i++)
		{
			if (czTempCon[i] == 0x26)
			{
				szContent[j++] = '%';
				szContent[j++] = '2';
				szContent[j++] = '6';
			}
			else if(czTempCon[i] == 0x0A)
			{
				szContent[j++] = '%';
				szContent[j++] = '0';
				szContent[j++] = 'A';
			}
			else
				szContent[j++] = czTempCon[i];
		}

		CDuiString dStrContent;
		dStrContent.Format(_T("mailto:?subject=%s&body=%s"), czTempSub, szContent);

		ShellExecute(0, _T("open"), dStrContent, NULL, NULL, SW_SHOW);
	}
	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);
}

void SoapMyAccount(VidyoClientAccountData* ptAccountData)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__MyAccountRequest* reqest = new _ns1__MyAccountRequest;
	_ns1__MyAccountResponse response;
	reqest->soap_alloc();

	if (soap_call___ns1__myAccount(&soap, chEndpoint, NULL, reqest, response) == SOAP_OK){
		memset(ptAccountData->entityID, 0, sizeof(ptAccountData->entityID));
		strcpy(ptAccountData->entityID, response.Entity->entityID);
		memset(ptAccountData->displayName, 0, sizeof(ptAccountData->displayName));
		strcpy(ptAccountData->displayName, response.Entity->displayName);
		memset(ptAccountData->emailAddress, 0, sizeof(ptAccountData->emailAddress));
		strcpy(ptAccountData->emailAddress, response.Entity->emailAddress);
		memset(ptAccountData->ownerID, 0, sizeof(ptAccountData->ownerID));
		strcpy(ptAccountData->ownerID, response.Entity->ownerID);
		memset(ptAccountData->extension, 0, sizeof(ptAccountData->extension));
		strcpy(ptAccountData->extension, response.Entity->extension);
		memset(ptAccountData->tenant, 0, sizeof(ptAccountData->tenant));
		strcpy(ptAccountData->tenant, response.Entity->tenant);
		//ptAccountData->language = *(VidyoClientLanguageCode*)response.Entity->Language;
		ptAccountData->memberStatus = *(VidyoClientMemberStatus*)response.Entity->MemberStatus;
		ptAccountData->roomStatus = *(VidyoClientRoomStatus*)response.Entity->RoomStatus;
		ptAccountData->isInMyContacts = *(VidyoBool*)response.Entity->isInMyContacts;
		ptAccountData->canCallDirect = *(VidyoBool*)response.Entity->canCallDirect;
		ptAccountData->canJoinMeeting = *(VidyoBool*)response.Entity->canJoinMeeting;
		ptAccountData->canControl = *(VidyoBool*)response.Entity->canControl;
		ptAccountData->roomMode.hasPIN = response.Entity->RoomMode->hasPIN;
		ptAccountData->roomMode.isRoomLocked = response.Entity->RoomMode->isLocked;
	}
	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);
}

void SoapGetContactList(VidyoClientAccountList* ptAccountList)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__SearchMyContactsRequest* request = new _ns1__SearchMyContactsRequest;
	_ns1__SearchMyContactsResponse response;
	request->soap_alloc();
	_ns1__EntityType type = _ns1__EntityType__Room;
	request->Filter = new _ns1__Filter;
	request->Filter->EntityType = new enum _ns1__EntityType;
	request->Filter->EntityType = &type;
	if (soap_call___ns1__searchMyContacts(&soap, chEndpoint, NULL, request, response) == SOAP_OK)
	{
		ptAccountList->accountData = new VidyoClientAccountData[response.total+1];
		ptAccountList->total = response.total;
		// add room to list
		for (int i = 0; i < response.total; i++)
		{
			memset(ptAccountList->accountData[i].entityID, 0, sizeof(ptAccountList->accountData[i].entityID));
			memcpy(ptAccountList->accountData[i].entityID, response.Entity[i]->entityID, strlen(response.Entity[i]->entityID));
			memset(ptAccountList->accountData[i].ownerID, 0, sizeof(ptAccountList->accountData[i].ownerID));
			memcpy(ptAccountList->accountData[i].ownerID, response.Entity[i]->ownerID, strlen(response.Entity[i]->ownerID));
			memset(ptAccountList->accountData[i].displayName, 0, sizeof(ptAccountList->accountData[i].displayName));
			memcpy(ptAccountList->accountData[i].displayName, response.Entity[i]->displayName, strlen(response.Entity[i]->displayName));
			memset(ptAccountList->accountData[i].extension, 0, sizeof(ptAccountList->accountData[i].extension));
			memcpy(ptAccountList->accountData[i].extension, response.Entity[i]->extension, strlen(response.Entity[i]->extension));
			memset(ptAccountList->accountData[i].tenant, 0, sizeof(ptAccountList->accountData[i].tenant));
			memcpy(ptAccountList->accountData[i].tenant, response.Entity[i]->tenant, strlen(response.Entity[i]->tenant));
			ptAccountList->accountData[i].entityType = (VidyoClientPortalServiceEntityType)(response.Entity[i]->EntityType+1);
			ptAccountList->accountData[i].memberStatus = *(VidyoClientMemberStatus*)response.Entity[i]->MemberStatus;
			ptAccountList->accountData[i].roomStatus = (VidyoClientRoomStatus)(*(VidyoClientRoomStatus*)response.Entity[i]->RoomStatus+1);
			ptAccountList->accountData[i].isInMyContacts = *(VidyoBool*)response.Entity[i]->isInMyContacts;
			ptAccountList->accountData[i].canCallDirect = *(VidyoBool*)response.Entity[i]->canCallDirect;
			ptAccountList->accountData[i].canJoinMeeting = *(VidyoBool*)response.Entity[i]->canJoinMeeting;
			ptAccountList->accountData[i].canControl = *(VidyoBool*)response.Entity[i]->canControl;
			ptAccountList->accountData[i].roomMode.isRoomLocked = response.Entity[i]->RoomMode->isLocked;
			ptAccountList->accountData[i].roomMode.hasPIN = response.Entity[i]->RoomMode->hasPIN;
			ptAccountList->accountData[i].roomMode.hasModeratorPIN = *(VidyoBool*)response.Entity[i]->RoomMode->hasModeratorPIN;

			memset(ptAccountList->accountData[i].roomMode.roomPIN, 0, sizeof(ptAccountList->accountData[i].roomMode.roomPIN));
			if (ptAccountList->accountData[i].roomMode.hasPIN)
				memcpy(ptAccountList->accountData[i].roomMode.roomPIN, response.Entity[i]->RoomMode->roomPIN, strlen(response.Entity[i]->RoomMode->roomPIN));

			memset(ptAccountList->accountData[i].roomMode.moderatorPIN, 0, sizeof(ptAccountList->accountData[i].roomMode.moderatorPIN));		
			memset(ptAccountList->accountData[i].roomMode.roomURL, 0, sizeof(ptAccountList->accountData[i].roomMode.roomURL));
			if (ptAccountList->accountData[i].canControl)
			{
				if (ptAccountList->accountData[i].roomMode.hasModeratorPIN)
					memcpy(ptAccountList->accountData[i].roomMode.moderatorPIN, response.Entity[i]->RoomMode->moderatorPIN, strlen(response.Entity[i]->RoomMode->moderatorPIN));					
				memcpy(ptAccountList->accountData[i].roomMode.roomURL, response.Entity[i]->RoomMode->roomURL, strlen(response.Entity[i]->RoomMode->roomURL));
			}	
		}
	}
	else
	{
		ptAccountList->accountData = new VidyoClientAccountData;
		ptAccountList->total = 0;
	}
	delete request;
	request = NULL;
	soap_end(&soap);
	soap_done(&soap);
}

int SoapJoinConference(char* EntityID, char* RoomPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__JoinConferenceRequest* reqest = new _ns1__JoinConferenceRequest;
	_ns1__JoinConferenceResponse response;
	reqest->soap_alloc();
	reqest->__union_JoinConferenceRequest = SOAP_UNION__ns1__union_JoinConferenceRequest_conferenceID;
	reqest->union_JoinConferenceRequest.conferenceID = EntityID;
	reqest->PIN = RoomPin;
	int iResult = soap_call___ns1__joinConference(&soap, chEndpoint, NULL, reqest, response);

	if (iResult)
	{
		if (!strcmp(soap.fault->faultstring, "Conference is Locked"))
			iResult = 2;
		else if (!strcmp(soap.fault->faultstring, "Wrong PIN"))
			iResult = 1;
	}

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return iResult;
}

BOOL SoapAddToMyContacts(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__AddToMyContactsRequest* request = new _ns1__AddToMyContactsRequest;
	_ns1__AddToMyContactsResponse response;
	request->soap_alloc();
	request->entityID = EntityID;
	
	int iResult = soap_call___ns1__addToMyContacts(&soap, chEndpoint, NULL, request, response);
	
	delete request;
	request = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRemoveFromMyContacts(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RemoveFromMyContactsRequest* request = new _ns1__RemoveFromMyContactsRequest;
	_ns1__RemoveFromMyContactsResponse response;
	request->soap_alloc();
	request->entityID = EntityID;
	
	int iResult = soap_call___ns1__removeFromMyContacts(&soap, chEndpoint, NULL, request, response);

	delete request;
	request = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetConferenceID(char* ConferenceID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__GetConferenceIDRequest* reqest = new _ns1__GetConferenceIDRequest;
	_ns1__GetConferenceIDResponse response;
	reqest->soap_alloc();

	int iResult = soap_call___ns1__getConferenceID(&soap, chEndpoint, NULL, reqest, response);

	if (iResult == SOAP_OK)
		memcpy(ConferenceID, response.conferenceID, sizeof(ConferenceID));

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetEntityByEntityID(char* EntityID, VidyoClientAccountData* ptAccountData)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__GetEntityByEntityIDRequest* reqest = new _ns1__GetEntityByEntityIDRequest;
	_ns1__GetEntityByEntityIDResponse response;
	reqest->soap_alloc();
	reqest->__sizeentityID = 1;
	reqest->entityID = &EntityID;

	int iResult = soap_call___ns1__getEntityByEntityID(&soap, chEndpoint, NULL, reqest, response);
	if (iResult == SOAP_OK)
	{
		memset(ptAccountData->displayName, 0, sizeof(ptAccountData->displayName));
		memcpy(ptAccountData->displayName, response.Entity[0]->displayName, sizeof(ptAccountData->displayName));
		ptAccountData->canControl = *(VidyoBool*)response.Entity[0]->canControl;
		ptAccountData->roomMode.isRoomLocked = response.Entity[0]->RoomMode->isLocked;
		ptAccountData->roomMode.hasModeratorPIN = *(VidyoBool*)response.Entity[0]->RoomMode->hasModeratorPIN;
	}
	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetParticipants(char* ConferenceID, VidyoClientAccountList* ptAccountList, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__GetParticipantsRequest* reqest = new _ns1__GetParticipantsRequest;
	_ns1__GetParticipantsResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__getParticipants(&soap, chEndpoint, NULL, reqest, response);
	if(iResult == SOAP_OK)
	{
		ptAccountList->accountData = new VidyoClientAccountData[response.total+1];
		ptAccountList->total = response.total;
		for (int i = 0; i < response.total; i++){
			memset(ptAccountList->accountData[i].entityID, 0, sizeof(ptAccountList->accountData[i].entityID));
			memcpy(ptAccountList->accountData[i].entityID, response.Entity[i]->entityID, strlen(response.Entity[i]->entityID));
			memset(ptAccountList->accountData[i].participantID, 0, sizeof(ptAccountList->accountData[i].participantID));
			memcpy(ptAccountList->accountData[i].participantID, response.Entity[i]->participantID, strlen(response.Entity[i]->participantID));
			memset(ptAccountList->accountData[i].displayName, 0, sizeof(ptAccountList->accountData[i].displayName));
			memcpy(ptAccountList->accountData[i].displayName, response.Entity[i]->displayName, strlen(response.Entity[i]->displayName));
			ptAccountList->accountData[i].audio = *(VidyoBool*)response.Entity[i]->audio;
			ptAccountList->accountData[i].video = *(VidyoBool*)response.Entity[i]->video;
		}
	}
	else
	{
		if (!strcmp(soap.fault->faultstring, "Control of conference not allowed"))
		{
			iResult = 1;
		}
	}

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetLectureModeParticipants(char* ConferenceID, VidyoClientParticipantList* ptAccountList, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;
	soap_set_mode(&soap, SOAP_C_UTFSTRING);

	_ns1__GetLectureModeParticipantsRequest* reqest = new _ns1__GetLectureModeParticipantsRequest;
	_ns1__GetLectureModeParticipantsResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__getLectureModeParticipants(&soap, chEndpoint, NULL, reqest, response);
	if(iResult == SOAP_OK)
	{
		if (ptAccountList)
		{
			ptAccountList->participantData = new VidyoClientParticipantData[response.total];
			ptAccountList->total = response.total;
			ptAccountList->lectureMode = *(VidyoBool*)response.lectureMode;
			ptAccountList->RecordID = *(int*)response.recorderID;
			for (int i = 0; i < response.total; i++){
				memset(ptAccountList->participantData[i].entityID, 0, sizeof(ptAccountList->participantData[i].entityID));
				memcpy(ptAccountList->participantData[i].entityID, response.LectureModeParticipant[i]->entityID, strlen(response.LectureModeParticipant[i]->entityID));
				memset(ptAccountList->participantData[i].participantID, 0, sizeof(ptAccountList->participantData[i].participantID));
				memcpy(ptAccountList->participantData[i].participantID, response.LectureModeParticipant[i]->participantID, strlen(response.LectureModeParticipant[i]->participantID));
				memset(ptAccountList->participantData[i].displayName, 0, sizeof(ptAccountList->participantData[i].displayName));
				memcpy(ptAccountList->participantData[i].displayName, response.LectureModeParticipant[i]->displayName, strlen(response.LectureModeParticipant[i]->displayName));
				ptAccountList->participantData[i].audio = *(VidyoBool*)response.LectureModeParticipant[i]->audio;
				ptAccountList->participantData[i].video = *(VidyoBool*)response.LectureModeParticipant[i]->video;
				ptAccountList->participantData[i].handRaised = *(VidyoBool*)response.LectureModeParticipant[i]->handRaised;
				ptAccountList->participantData[i].presenter = *(VidyoBool*)response.LectureModeParticipant[i]->presenter;
			}
		}
	}

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStartVideo(char* ConferenceID, char* ParticipantID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__StartVideoRequest* reqest = new _ns1__StartVideoRequest;
	_ns1__StartVideoResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__startVideo(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStopVideo(char* ConferenceID, char* ParticipantID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__StopVideoRequest* reqest = new _ns1__StopVideoRequest;
	_ns1__StopVideoResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__stopVideo(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStartAudio(char* ConferenceID, char* ParticipantID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__UnmuteAudioRequest* reqest = new _ns1__UnmuteAudioRequest;
	_ns1__UnmuteAudioResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__unmuteAudio(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStopAudio(char* ConferenceID, char* ParticipantID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__MuteAudioRequest* reqest = new _ns1__MuteAudioRequest;
	_ns1__MuteAudioResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__muteAudio(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapMuteVidyoServerAll(char* ConferenceID, bool bMute, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__muteVideoServerAllRequest* reqest = new _ns1__muteVideoServerAllRequest;
	_ns1__muteVideoServerAllResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->muteState = bMute;
	reqest->moderatorPIN = ModeratorPin;
	int iResult = soap_call___ns1__muteVideoServerAll(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapMuteAudioServerAll(char* ConferenceID, bool bMute, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__muteAudioServerAllRequest* reqest = new _ns1__muteAudioServerAllRequest;
	_ns1__muteAudioServerAllResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->muteState = bMute;
	reqest->moderatorPIN = ModeratorPin;
	int iResult = soap_call___ns1__muteAudioServerAll(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapLockRoom(char* EntityID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__LockRoomRequest* reqest = new _ns1__LockRoomRequest;
	_ns1__LockRoomResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__lockRoom(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapUnLockRoom(char* EntityID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__UnlockRoomRequest* reqest = new _ns1__UnlockRoomRequest;
	_ns1__UnlockRoomResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__unlockRoom(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetRoomProfile(char* EntityID, char* RoomProfile)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__GetRoomProfileRequest* reqest = new _ns1__GetRoomProfileRequest;
	_ns1__GetRoomProfileResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__getRoomProfile(&soap, chEndpoint, NULL, reqest, response);
	if (!iResult && response.roomProfile)
		memcpy(RoomProfile, response.roomProfile->roomProfileName, strlen(response.roomProfile->roomProfileName));

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapSetRoomProfile(char* EntityID, char* RoomProfile)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__SetRoomProfileRequest* reqest = new _ns1__SetRoomProfileRequest;
	_ns1__SetRoomProfileResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;
	reqest->roomProfileName = RoomProfile;

	int iResult = soap_call___ns1__setRoomProfile(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRemoveRoomProfile(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RemoveRoomProfileRequest* reqest = new _ns1__RemoveRoomProfileRequest;
	_ns1__RemoveRoomProfileResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__removeRoomProfile(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapCreateRoomURL(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__CreateRoomURLRequest* reqest = new _ns1__CreateRoomURLRequest;
	_ns1__CreateRoomURLResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__createRoomURL(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRemoveRoomURL(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RemoveRoomURLRequest* reqest = new _ns1__RemoveRoomURLRequest;
	_ns1__RemoveRoomURLResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__removeRoomURL(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetRoomAccessOptions(char* EntityID, char* RoomURL)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RoomAccessOptionsRequest* reqest = new _ns1__RoomAccessOptionsRequest;
	_ns1__RoomAccessOptionsResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__getRoomAccessOptions(&soap, chEndpoint, NULL, reqest, response);
	if (!iResult)
	{
		memcpy(RoomURL, (response.RoomAccessOption[0])->value, strlen((response.RoomAccessOption[0])->value));
	}

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapCreateRoomPin(char* EntityID, char* RoomPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__CreateRoomPINRequest* reqest = new _ns1__CreateRoomPINRequest;
	_ns1__CreateRoomPINResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;
	reqest->PIN = RoomPin;

	int iResult = soap_call___ns1__createRoomPIN(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRemoveRoomPin(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RemoveRoomPINRequest* reqest = new _ns1__RemoveRoomPINRequest;
	_ns1__RemoveRoomPINResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__removeRoomPIN(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapCreateModeratorPin(char* EntityID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__CreateModeratorPINRequest* reqest = new _ns1__CreateModeratorPINRequest;
	_ns1__CreateModeratorPINResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;
	reqest->PIN = ModeratorPin;

	int iResult = soap_call___ns1__createModeratorPIN(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRemoveModeratorPin(char* EntityID)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RemoveModeratorPINRequest* reqest = new _ns1__RemoveModeratorPINRequest;
	_ns1__RemoveModeratorPINResponse response;
	reqest->soap_alloc();
	reqest->roomID = EntityID;

	int iResult = soap_call___ns1__removeModeratorPIN(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetVidyoReplayLibrary(tRecordLibrary* pRelayLibrary)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__GetVidyoReplayLibraryRequest* reqest = new _ns1__GetVidyoReplayLibraryRequest;
	_ns1__GetVidyoReplayLibraryResponse response;
	reqest->soap_alloc();

	int iResult = soap_call___ns1__getVidyoReplayLibrary(&soap, chEndpoint, NULL, reqest, response);
	if (iResult == SOAP_OK)
	{
		memcpy(pRelayLibrary->token, response.authToken, sizeof(pRelayLibrary->token));
		memcpy(pRelayLibrary->URL, response.vidyoReplayLibraryUrl, sizeof(pRelayLibrary->URL));
	}

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapGetUserName(char* UserName)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__GetUserNameRequest* reqest = new _ns1__GetUserNameRequest;
	_ns1__GetUserNameResponse response;
	reqest->soap_alloc();

	int iResult = soap_call___ns1__getUserName(&soap, chEndpoint, NULL, reqest, response);
	if (iResult == SOAP_OK)
	{
		memcpy(UserName, response.realUserName, strlen(response.realUserName));
	}

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStartLectureMode(char* ConferenceID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__StartLectureModeRequest* reqest = new _ns1__StartLectureModeRequest;
	_ns1__StartLectureModeResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__startLectureMode(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStopLectureMode(char* ConferenceID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__StopLectureModeRequest* reqest = new _ns1__StopLectureModeRequest;
	_ns1__StopLectureModeResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__stopLectureMode(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStartRecording(char* ConferenceID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__StartRecordingRequest* reqest = new _ns1__StartRecordingRequest;
	_ns1__StartRecordingResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;
	reqest->recorderPrefix = "01";

	int iResult = soap_call___ns1__startRecording(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapPauseRecording(char* ConferenceID, int RecordID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__PauseRecordingRequest* reqest = new _ns1__PauseRecordingRequest;
	_ns1__PauseRecordingResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->recorderID = RecordID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__pauseRecording(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapResumeRecording(char* ConferenceID, int RecordID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__ResumeRecordingRequest* reqest = new _ns1__ResumeRecordingRequest;
	_ns1__ResumeRecordingResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->recorderID = RecordID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__resumeRecording(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapStopRecording(char* ConferenceID, int RecordID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__StopRecordingRequest* reqest = new _ns1__StopRecordingRequest;
	_ns1__StopRecordingResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->recorderID = RecordID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__stopRecording(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapLeaveConference(char* ConferenceID, char* ParticipantID, char* ModeratorPin /* = NULL */)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__LeaveConferenceRequest* reqest = new _ns1__LeaveConferenceRequest;
	_ns1__LeaveConferenceResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__leaveConference(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapDisconnectConferenceAll(char* ConferenceID, char* ModeratorPin)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__disconnectConferenceAllRequest* reqest = new _ns1__disconnectConferenceAllRequest;
	_ns1__disconnectConferenceAllResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__disconnectConferenceAll(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapSetPresenter(char* ConferenceID, char* ParticipantID, char* ModeratorPin /* = NULL */)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__SetPresenterRequest* reqest = new _ns1__SetPresenterRequest;
	_ns1__SetPresenterResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__setPresenter(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRemovePresenter(char* ConferenceID, char* ParticipantID, char* ModeratorPin /* = NULL */)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RemovePresenterRequest* reqest = new _ns1__RemovePresenterRequest;
	_ns1__RemovePresenterResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__removePresenter(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapRaisehand()
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__RaiseHandResponse response;
	int iResult = soap_call___ns1__raiseHand(&soap, chEndpoint, NULL, NULL, response);

	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapUnRaisehand()
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__UnraiseHandResponse response;
	int iResult = soap_call___ns1__unraiseHand(&soap, chEndpoint, NULL, NULL, response);

	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapDismissRaisedHand(char* ConferenceID, char* ParticipantID, char* ModeratorPin /* = NULL */)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__DismissRaisedHandRequest* reqest = new _ns1__DismissRaisedHandRequest;
	_ns1__DismissRaisedHandResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->participantID = ParticipantID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__dismissRaisedHand(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapDismissAllRaisedHand(char* ConferenceID, char* ModeratorPin /* = NULL */)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__DismissAllRaisedHandRequest* reqest = new _ns1__DismissAllRaisedHandRequest;
	_ns1__DismissAllRaisedHandResponse response;
	reqest->soap_alloc();
	reqest->conferenceID = ConferenceID;
	reqest->moderatorPIN = ModeratorPin;

	int iResult = soap_call___ns1__dismissAllRaisedHand(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapUpdatePassword(char* Password)
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__UpdatePasswordRequest* reqest = new _ns1__UpdatePasswordRequest;
	_ns1__UpdatePasswordResponse response;
	reqest->soap_alloc();
	reqest->password = Password;

	int iResult = soap_call___ns1__updatePassword(&soap, chEndpoint, NULL, reqest, response);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}

BOOL SoapChangePsw()
{
	struct soap soap;
	soap_init(&soap);
	soap.userid = tLoginParams.userName;
	soap.passwd = tLoginParams.userPass;
	soap.keep_alive = 1;
	soap.action = SOAP_NAMESPACE_OF_ns1;

	_ns1__GetChangePasswordHtmlUrlWithTokenRequest* reqest = new _ns1__GetChangePasswordHtmlUrlWithTokenRequest;
	_ns1__GetChangePasswordHtmlUrlWithTokenResponse response;
	reqest->soap_alloc();

	int iResult = soap_call___ns1__getChangePasswordHtmlUrlWithToken(&soap, chEndpoint, NULL, reqest, response);

	TCHAR czTempSub[1024] = {0};
	ChangUtf8ToText(response.changePasswordHtmlUrlWithToken, czTempSub);

	ShellExecute(0, _T("open"), czTempSub, NULL, NULL, SW_SHOW);

	delete reqest;
	reqest = NULL;
	soap_end(&soap);
	soap_done(&soap);

	return (iResult==SOAP_OK)?TRUE:FALSE;
}