#pragma once

#include "StdAfx.h"
#include "../../include/DataDefine.h"

void SoapSetLoginParams(const VidyoClientInEventLogIn* pLoginParams);
char* GetSoapEndpoint(const char* pPortalUrl, char* pEndpoint);
void SoapSearch(char* EntityID, VidyoClientAccountList* ptAccountList);
void SoapInvite(char* EntityID);
void SoapMyAccount(VidyoClientAccountData* ptAccountData);
void SoapGetContactList(VidyoClientAccountList* ptAccountList);
int	SoapJoinConference(char* EntityID, char* RoomPin = NULL);
BOOL SoapAddToMyContacts(char* EntityID);
BOOL SoapRemoveFromMyContacts(char* EntityID);

BOOL SoapGetConferenceID(char* ConferenceID);
BOOL SoapGetEntityByEntityID(char* EntityID, VidyoClientAccountData* ptAccountData);
BOOL SoapGetParticipants(char* ConferenceID, VidyoClientAccountList* ptAccountList, char* ModeratorPin = NULL);
BOOL SoapGetLectureModeParticipants(char* ConferenceID, VidyoClientParticipantList* ptAccountList, char* ModeratorPin = NULL);
BOOL SoapStartVideo(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapStopVideo(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapStartAudio(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapStopAudio(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);

BOOL SoapLockRoom(char* EntityID, char* ModeratorPin = NULL);
BOOL SoapUnLockRoom(char* EntityID, char* ModeratorPin = NULL);
BOOL SoapGetRoomProfile(char* EntityID, char* RoomProfile);
BOOL SoapSetRoomProfile(char* EntityID, char* RoomProfile);
BOOL SoapRemoveRoomProfile(char* EntityID);
BOOL SoapCreateRoomURL(char* EntityID);
BOOL SoapRemoveRoomURL(char* EntityID);
BOOL SoapGetRoomAccessOptions(char* EntityID, char* RoomURL);
BOOL SoapCreateRoomPin(char* EntityID, char* RoomPin);
BOOL SoapRemoveRoomPin(char* EntityID);
BOOL SoapCreateModeratorPin(char* EntityID, char* ModeratorPin);
BOOL SoapRemoveModeratorPin(char* EntityID);

BOOL SoapGetVidyoReplayLibrary(tRecordLibrary* pRelayLibrary);

BOOL SoapGetUserName(char* UserName);
// Conference Control
BOOL SoapStartLectureMode(char* ConferenceID, char* ModeratorPin = NULL);
BOOL SoapStopLectureMode(char* ConferenceID, char* ModeratorPin = NULL);
BOOL SoapMuteVidyoServerAll(char* ConferenceID, bool bMute, char* ModeratorPin = NULL);
BOOL SoapMuteAudioServerAll(char* ConferenceID, bool bMute, char* ModeratorPin = NULL);
BOOL SoapStartRecording(char* ConferenceID, char* ModeratorPin = NULL);
BOOL SoapPauseRecording(char* ConferenceID, int RecordID, char* ModeratorPin = NULL);
BOOL SoapResumeRecording(char* ConferenceID, int RecordID, char* ModeratorPin = NULL);
BOOL SoapStopRecording(char* ConferenceID, int RecordID, char* ModeratorPin = NULL);
BOOL SoapLeaveConference(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapDisconnectConferenceAll(char* ConferenceID, char* ModeratorPin = NULL);

BOOL SoapSetPresenter(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapRemovePresenter(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapRaisehand();
BOOL SoapUnRaisehand();
BOOL SoapDismissRaisedHand(char* ConferenceID, char* ParticipantID, char* ModeratorPin = NULL);
BOOL SoapDismissAllRaisedHand(char* ConferenceID, char* ModeratorPin = NULL);

BOOL SoapUpdatePassword(char* Password);
BOOL SoapChangePsw();