/*-- Wandnachricht --*/

#strict 2

local sID; // Text-ID
local sText; // Text-string oder Array für längere Texte

protected func Initialize()
{
	SetAction("Walling");
}

protected func ControlUp(object pPlayer)
{
	[$CtrlRead$]
	if (!CreateMenu(GetID(), pPlayer, this, 0, "", 0, C4MN_Style_Dialog | C4MN_Style_EqualItemHeight))
		return;
	AddMsgText(pPlayer, sText);
	SetMenuDecoration(MD70, pPlayer);
	return true;
}

private func AddMsgText(object pPlayer, vTxt)
{
	// Mögliche Formate für vTxt sind:
	// "Text"
	// [ID, "Text"]
	// ["Text1", [ID, "Text2"], ...]
	var sString, idIcon = NONE;
	if (GetType(vTxt) == C4V_Array)
	{
		var vItem = vTxt[0];
		if (GetType(vItem) == C4V_C4ID)
		{
			// Array vom Typ [C4ID, "Text"]?
			idIcon = vItem;
			sString = vTxt[1];
			// [C4ID, [...]]: Nur Icon eintragen
			if (GetType(sString) == C4V_Array)
			{
				AddMenuItem(Format("%i", idIcon), "", NONE, pPlayer, 0, 0, "", C4MN_Add_ImgTextSpec, 0, 0);
				return AddMsgText(pPlayer, sString);
			}
		}
		else
		{
			// Ansonsten Array, das weitere Texte enthält
			for (vItem in vTxt) 
				AddMsgText(pPlayer, vItem);
			return true;
		}
	}
	else
		sString = vTxt;
	return AddMenuItem(sString, "", idIcon, pPlayer, 0, 0, "", C4MN_Add_ForceNoDesc, 0, 0);
}

public func SetText(sToText)
{
	sText = sToText;
	return true;
}

global func AssignWScriptText(sScriptName, sToText)
{
	var obj;
	while (obj = FindObject(_WSC, 0, 0, 0, 0, 0, 0, 0, 0, obj))
		if (LocalN("sID", obj) == sScriptName)
		{
			obj->SetText(sToText);
			return true;
		}
}
