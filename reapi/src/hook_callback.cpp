#include "precompiled.h"

hookctx_t* g_hookCtx;

/*
* ReHLDS functions
*/
void SV_StartSound(IRehldsHook_SV_StartSound *chain, int recipients, edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch)
{
	auto original = [chain](int _recipients, int _entity, int _channel, const char *_sample, int _volume, float _attenuation, int _fFlags, int _pitch)
	{
		chain->callNext(_recipients, INDEXENT(_entity), _channel, _sample, _volume, _attenuation, _fFlags, _pitch);
	};

	callVoidForward(RH_SV_StartSound, original, recipients, indexOfEdict(entity), channel, sample, volume, attenuation, fFlags, pitch);
}

void SV_DropClient(IRehldsHook_SV_DropClient *chain, IGameClient *cl, bool crash, const char *fmt)
{
	auto original = [chain](int _cl, bool _crash, const char *_fmt)
	{
		chain->callNext(g_RehldsSvs->GetClient(_cl), _crash, _fmt);
	};

	callVoidForward(RH_SV_DropClient, original, cl->GetId(), crash, fmt);
}

void SV_ActivateServer(IRehldsHook_SV_ActivateServer *chain, int runPhysics)
{
	auto original = [chain](int _runPhysics)
	{
		chain->callNext(_runPhysics);
	};

	callVoidForward(RH_SV_ActivateServer, original, runPhysics);
}

void Cvar_DirectSet(IRehldsHook_Cvar_DirectSet *chain, cvar_t *var, const char *value)
{
	auto original = [chain](cvar_t *_var, const char *_value)
	{
		chain->callNext(_var, _value);
	};

	callVoidForward(RH_Cvar_DirectSet, original, var, value);
}

/*
* ReGameDLL functions
*/
void CBasePlayer_Spawn(IReGameHook_CBasePlayer_Spawn *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};
	
	callVoidForward(RG_CBasePlayer_Spawn, original, pthis->entindex());
}

void CBasePlayer_Precache(IReGameHook_CBasePlayer_Precache *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_Precache, original, pthis->entindex());
}

int CBasePlayer_ObjectCaps(IReGameHook_CBasePlayer_ObjectCaps *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext();
	};

	return callForward<int>(RG_CBasePlayer_ObjectCaps, original, pthis->entindex());
}

int CBasePlayer_Classify(IReGameHook_CBasePlayer_Classify *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		return chain->callNext();
	};

	return callForward<int>(RG_CBasePlayer_Classify, original, pthis->entindex());
}

void CBasePlayer_TraceAttack(IReGameHook_CBasePlayer_TraceAttack *chain, CBasePlayer *pthis, entvars_t *pevAttacker, float flDamage, Vector& vecDir, TraceResult *ptr, int bitsDamageType)
{
	Vector vecDirCopy(vecDir);

	auto original = [chain, &vecDirCopy](int _pthis, int _pevAttacker, float _flDamage, cell _vecDir, int _ptr, int _bitsDamageType)
	{
		chain->callNext(PEV(_pevAttacker), _flDamage, vecDirCopy, (TraceResult *)_ptr, _bitsDamageType);
	};

	callVoidForward(RG_CBasePlayer_TraceAttack, original, pthis->entindex(), indexOfEdict(pevAttacker), flDamage, g_amxxapi.PrepareCellArrayA(reinterpret_cast<cell *>(&vecDirCopy), 3, true), int(ptr), bitsDamageType);
}

int CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage *chain, CBasePlayer *pthis, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	auto original = [chain](int _pthis, int _pevInflictor, int _pevAttacker, float _flDamage, int _bitsDamageType)
	{
		return chain->callNext(PEV(_pevInflictor), PEV(_pevAttacker), _flDamage, _bitsDamageType);
	};

	return callForward<int>(RG_CBasePlayer_TakeDamage, original, pthis->entindex(), indexOfEdict(pevInflictor), indexOfEdict(pevAttacker), flDamage, bitsDamageType);
}

int CBasePlayer_TakeHealth(IReGameHook_CBasePlayer_TakeHealth *chain, CBasePlayer *pthis, float flHealth, int bitsDamageType)
{
	auto original = [chain](int _pthis, float _flHealth, int _bitsDamageType)
	{
		return chain->callNext(_flHealth, _bitsDamageType);
	};

	return callForward<int>(RG_CBasePlayer_TakeHealth, original, pthis->entindex(), flHealth, bitsDamageType);
}

void CBasePlayer_Killed(IReGameHook_CBasePlayer_Killed *chain, CBasePlayer *pthis, entvars_t *pevAttacker, int iGib)
{
	auto original = [chain](int _pthis, int _pevAttacker, int _iGib)
	{
		chain->callNext(PEV(_pevAttacker), _iGib);
	};

	callVoidForward(RG_CBasePlayer_Killed, original, pthis->entindex(), indexOfEdict(pevAttacker), iGib);
}

void CBasePlayer_AddPoints(IReGameHook_CBasePlayer_AddPoints *chain, CBasePlayer *pthis, int score, BOOL bAllowNegativeScore)
{
	auto original = [chain](int _pthis, int _score, BOOL _bAllowNegativeScore)
	{
		chain->callNext(_score, _bAllowNegativeScore);
	};

	callVoidForward(RG_CBasePlayer_AddPoints, original, pthis->entindex(), score, bAllowNegativeScore);
}

void CBasePlayer_AddPointsToTeam(IReGameHook_CBasePlayer_AddPointsToTeam *chain, CBasePlayer *pthis, int score, BOOL bAllowNegativeScore)
{
	auto original = [chain](int _pthis, int _score, BOOL _bAllowNegativeScore)
	{
		chain->callNext(_score, _bAllowNegativeScore);
	};

	callVoidForward(RG_CBasePlayer_AddPointsToTeam, original, pthis->entindex(), score, bAllowNegativeScore);
}

BOOL CBasePlayer_AddPlayerItem(IReGameHook_CBasePlayer_AddPlayerItem *chain, CBasePlayer *pthis, CBasePlayerItem *pItem)
{
	auto original = [chain](int _pthis, int _pItem)
	{
		return chain->callNext(getPrivate<CBasePlayerItem>(_pItem));
	};

	return callForward<BOOL>(RG_CBasePlayer_AddPlayerItem, original, pthis->entindex(), pItem->entindex());
}

BOOL CBasePlayer_RemovePlayerItem(IReGameHook_CBasePlayer_RemovePlayerItem *chain, CBasePlayer *pthis, CBasePlayerItem *pItem)
{
	auto original = [chain](int _pthis, int _pItem)
	{
		return chain->callNext(getPrivate<CBasePlayerItem>(_pItem));
	};

	return callForward<BOOL>(RG_CBasePlayer_RemovePlayerItem, original, pthis->entindex(), pItem->entindex());
}

int CBasePlayer_GiveAmmo(IReGameHook_CBasePlayer_GiveAmmo *chain, CBasePlayer *pthis, int iAmount, char *szName, int iMax)
{
	auto original = [chain](int _pthis, int _iAmount, char *_szName, int _iMax)
	{
		return chain->callNext(_iAmount, _szName, _iMax);
	};

	return callForward<int>(RG_CBasePlayer_GiveAmmo, original, pthis->entindex(), iAmount, szName, iMax);
}

void CBasePlayer_ResetMaxSpeed(IReGameHook_CBasePlayer_ResetMaxSpeed *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_ResetMaxSpeed, original, pthis->entindex());
}

void CBasePlayer_Jump(IReGameHook_CBasePlayer_Jump *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_Jump, original, pthis->entindex());
}

void CBasePlayer_Duck(IReGameHook_CBasePlayer_Duck *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_Duck, original, pthis->entindex());
}

void CBasePlayer_PreThink(IReGameHook_CBasePlayer_PreThink *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_PreThink, original, pthis->entindex());
}

void CBasePlayer_PostThink(IReGameHook_CBasePlayer_PostThink *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_PostThink, original, pthis->entindex());
}

void CBasePlayer_UpdateClientData(IReGameHook_CBasePlayer_UpdateClientData *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_UpdateClientData, original, pthis->entindex());
}

void CBasePlayer_ImpulseCommands(IReGameHook_CBasePlayer_ImpulseCommands *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_ImpulseCommands, original, pthis->entindex());
}

void CBasePlayer_RoundRespawn(IReGameHook_CBasePlayer_RoundRespawn *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_RoundRespawn, original, pthis->entindex());
}

void CBasePlayer_Blind(IReGameHook_CBasePlayer_Blind *chain, CBasePlayer *pthis, float flUntilTime, float flHoldTime, float flFadeTime, int iAlpha)
{
	auto original = [chain](int _pthis, float _flUntilTime, float _flHoldTime, float _flFadeTime, int _iAlpha)
	{
		chain->callNext(_flUntilTime, _flHoldTime, _flFadeTime, _iAlpha);
	};

	callVoidForward(RG_CBasePlayer_Blind, original, pthis->entindex(), flUntilTime, flHoldTime, flFadeTime, iAlpha);
}

CBaseEntity *CBasePlayer_Observer_IsValidTarget(IReGameHook_CBasePlayer_Observer_IsValidTarget *chain, CBasePlayer *pthis, int iPlayerIndex, bool bSameTeam)
{
	auto original = [chain](int _pthis, int _iPlayerIndex, bool _bSameTeam)
	{
		return chain->callNext(_iPlayerIndex, _bSameTeam);
	};

	return callForward<CBaseEntity *>(RG_CBasePlayer_Observer_IsValidTarget, original, pthis->entindex(), iPlayerIndex, bSameTeam);
}

void CBasePlayer_SetAnimation(IReGameHook_CBasePlayer_SetAnimation *chain, CBasePlayer *pthis, PLAYER_ANIM playerAnim)
{
	auto original = [chain](int _pthis, PLAYER_ANIM _playerAnim)
	{
		chain->callNext(_playerAnim);
	};

	callVoidForward(RG_CBasePlayer_SetAnimation, original, pthis->entindex(), playerAnim);
}

void CBasePlayer_GiveDefaultItems(IReGameHook_CBasePlayer_GiveDefaultItems *chain, CBasePlayer *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBasePlayer_GiveDefaultItems, original, pthis->entindex());
}

void CBasePlayer_GiveNamedItem(IReGameHook_CBasePlayer_GiveNamedItem *chain, CBasePlayer *pthis, const char *pszName)
{
	auto original = [chain](int _pthis, const char *_pszName)
	{
		chain->callNext(_pszName);
	};

	callVoidForward(RG_CBasePlayer_GiveNamedItem, original, pthis->entindex(), pszName);
}

void CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount *chain, CBasePlayer *pthis, int amount, bool bTrackChange)
{
	auto original = [chain](int _pthis, int _amount, bool _bTrackChange)
	{
		chain->callNext(_amount, _bTrackChange);
	};

	callVoidForward(RG_CBasePlayer_AddAccount, original, pthis->entindex(), amount, bTrackChange);
}

void CBasePlayer_GiveShield(IReGameHook_CBasePlayer_GiveShield *chain, CBasePlayer *pthis, bool bDeploy)
{
	auto original = [chain](int _pthis, bool _bDeploy)
	{
		chain->callNext(_bDeploy);
	};

	callVoidForward(RG_CBasePlayer_GiveShield, original, pthis->entindex(), bDeploy);
}

void CBaseAnimating_ResetSequenceInfo(IReGameHook_CBaseAnimating_ResetSequenceInfo *chain, CBaseAnimating *pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext();
	};

	callVoidForward(RG_CBaseAnimating_ResetSequenceInfo, original, pthis->entindex());
}

int GetForceCamera(IReGameHook_GetForceCamera *chain, CBasePlayer *pObserver)
{
	auto original = [chain](int _pObserver)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pObserver));
	};

	return callForward<int>(RG_GetForceCamera, original, pObserver->entindex());
}

void PlayerBlind(IReGameHook_PlayerBlind *chain, CBasePlayer *pPlayer, entvars_t *pevInflictor, entvars_t *pevAttacker, float fadeTime, float fadeHold, int alpha, Vector& color)
{
	Vector colorCopy(color);

	auto original = [chain, &colorCopy](int _pPlayer, int _pevInflictor, int _pevAttacker, float _fadeTime, float _fadeHold, int _alpha, cell _color)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), PEV(_pevInflictor), PEV(_pevAttacker), _fadeTime, _fadeHold, _alpha, colorCopy);
	};

	callVoidForward(RG_PlayerBlind, original, pPlayer->entindex(), indexOfEdict(pevInflictor), indexOfEdict(pevAttacker), fadeTime, fadeHold, alpha, g_amxxapi.PrepareCellArrayA(reinterpret_cast<cell *>(&colorCopy), 3, true));
}

void RadiusFlash_TraceLine(IReGameHook_RadiusFlash_TraceLine *chain, CBasePlayer *pPlayer, entvars_t *pevInflictor, entvars_t *pevAttacker, Vector& vecSrc, Vector& vecSpot, TraceResult *ptr)
{
	Vector vecSrcCopy(vecSrc), vecSpotCopy(vecSpot);

	auto original = [chain, &vecSrcCopy, &vecSpotCopy](int _pPlayer, int _pevInflictor, int _pevAttacker, cell _vecSrc, cell _vecSpot, int _ptr)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pPlayer), PEV(_pevInflictor), PEV(_pevAttacker), vecSrcCopy, vecSpotCopy, (TraceResult *)_ptr);
	};

	callVoidForward(RG_RadiusFlash_TraceLine, original, pPlayer->entindex(), indexOfEdict(pevInflictor), indexOfEdict(pevAttacker), g_amxxapi.PrepareCellArrayA(reinterpret_cast<cell *>(&vecSrcCopy), 3, true), g_amxxapi.PrepareCellArrayA(reinterpret_cast<cell *>(&vecSpotCopy), 3, true), int(ptr));
}

bool RoundEnd(IReGameHook_RoundEnd *chain, int winStatus, ScenarionEventEndRound event, float tmDelay)
{
	auto original = [chain](int _winStatus, ScenarionEventEndRound _event, float _tmDelay)
	{
		return chain->callNext(_winStatus, _event, _tmDelay);
	};
	
	return callForward<bool>(RG_RoundEnd, original, winStatus, event, tmDelay);
}

int g_iClientStartSpeak, g_iClientStopSpeak;

void ClientStartSpeak(size_t clientIndex)
{
	g_amxxapi.ExecuteForward(g_iClientStartSpeak, clientIndex);
}

void ClientStopSpeak(size_t clientIndex)
{
	g_amxxapi.ExecuteForward(g_iClientStopSpeak, clientIndex);
}
