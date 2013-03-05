/*-- Wegweiser --*/

#strict

local sID;   // Nachrichten-ID
local sText; // Text string
local szSection;
local pNPC;

protected func UpdateTransferZone()
{
	// Wegweisertext aktualisieren
	var sText2;
	sText2 = GameCall(Format("MsgSignpost%d", sID));
	szSection = GameCall(Format("MsgSignpostSect%d", sID));
	if (sText2)
		sText = sText2;
	else
		if (!sText)
			return Message("@GIMME TEXT (%s)!", this(), sID);
	Message("", this());
}

public func GetSection()
{
	// lieber sofort den Szenscript fragen anstatt auf gespeichertes zurückzugreifen (war nämlich verbuggt)
	return GameCall(Format("MsgSignpostSect%d", sID));
}

public func IsSection(string szNewSection)
{
	return szNewSection S= GameCall(Format("MsgSignpostSect%d", sID));
}

protected func ControlUp(object pPlayer)
{
	[$CtrlRead$]
	UpdateTransferZone(); //Vorher ganz sicher gehen
	if (pNPC)
	{
		var pTalker = FindObject2(Find_ID(DG69), Find_ActionTarget(pNPC));
		if (pTalker)
			return pTalker->ActivateEntrance(pPlayer);
	}
	// Spieler vorbereiten (Action halten, weil Push)
	StopClonkEx(pPlayer, true);
	MsgBox(pPlayer, Format("\"%s\"", sText));
	MsgBoxAddOption(pPlayer, NONE, "Reisen", "DoTravel");
	MsgBoxAddOption(pPlayer, NONE, "Abbrechen", "Stop");
	return 1;
}

public func Stop() { }

public func DoTravel()
{
	// Sicht ausfaden
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var iCount = GetCrewCount(GetPlayerByIndex(i));
		var pObj;
		while (iCount--)
		{
			pObj = GetCrew(GetPlayerByIndex(i), iCount);
			if (GetAction(pObj) S= "Push")
				pObj->CloseMenu();
			pObj->FadeViewOut(0, 1);
			SetCrewEnabled(0, pObj);
		}
	}
	Schedule("ChangeSection(szSection)", 48);
}

public func SetText(string sToText)
{
	sText = sToText;
	return true;
}
  
public func GetDlgMsgColor()
{
	return 10778899;
}
