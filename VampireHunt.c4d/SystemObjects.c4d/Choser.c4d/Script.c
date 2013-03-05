#strict 2

local iPhase;
local aArray;

protected func Initialize()
{
	aArray = CreateArray(2);
	for (var pClonk in FindObjects(Find_ID(MEDC))) 
		SetCrewExtraData(pClonk, "VH_Role", 0);
}


public func Team(array aidDefs)
{
	iPhase = 1001;
	aArray = aidDefs;
	Vampire();
}

//öffnet die verschiedenen menüs zum Spielstart
public func Vampire()
{
	iPhase++;
	//Hostmenü zuerst
	if (iPhase == 1)
	{
		HostMenu();
		return 1;
	}
	//Host ist fertig -> Einstellungen setzen
	if (iPhase > 1 && iPhase < 4)
	{
		CloseMenu(GetCrew(GetPlayerByIndex(0)));
		if (aArray[iPhase - 2] && iPhase == 2)
		{
			CreateObject(RVHC);
			return 1;
		}
		else
			if (iPhase == 2)
			{
				var pVamp = FindObjects(Find_ID(MEDC));
				pVamp = pVamp[Random(GetLength(pVamp))];
				SetVamp(pVamp);
				return Vampire();
			}
		if (aArray[iPhase - 2] && iPhase == 3)
		{
			CreateObject(RNRC);
			return 1;
		}
		//Sicherheit
		return Vampire();
	}
	//Zum Schluss Rollen verteilen
	SetRoles();
}

public func SetVamp(pVamp)
{
	pVamp->SetRole(J_VA);
	Exit(pVamp);
	J_VA->Support(pVamp);
}

protected func MenuQueryCancel(int iSelection, object pMenuObj)
{
	return true;
}

protected func ContainedDown(object pChooser)
{
	PlayerMessage(GetOwner(pChooser), "$WaitMsg$", 0, GetPlayerName(GetOwner(Contents(0, this))));
}

//öffnet das Hostmenü
private func HostMenu()
{
	var pHost = GetCrew(GetPlayerByIndex(0));
	//eventuelle Menüs schließen
	CloseMenu(pHost);
	CreateMenu(MEDC, pHost);
	
	var idMethode;
	if (aArray[0])
		idMethode = I_CV;
	else
		idMethode = I_RV;
	AddMenuItem("$MethodeElection$", Format("Mode(0,%d)", GetPlayerByIndex(0)), idMethode, pHost);
	
	var idRoleChange;
	if (aArray[1])
		idRoleChange = IRCD;
	else
		idRoleChange = IRCE;
	AddMenuItem("$NoRoleChange$", Format("Mode(1,%d)", GetPlayerByIndex(0)), idRoleChange, pHost);
	
	AddMenuItem("$Rdy$", "Vampire()", GetID(this), pHost);
}

//Helfer für Wahrheitwerte
private func Mode(int iPos, int iPlr)
{
	aArray[iPos] = !aArray[iPos];
	Sound("Click", true, this, 110, iPlr);
	HostMenu();
}

//setzt das Rollenmenü für einen Spieler
private func SetRoles()
{
	var pPlayer = Contents(0, this);
	CreateMenu(MEDC, pPlayer);
	for (var IDdef = 0, i = 0, f = 0; IDdef = GetClass(i); i++)
	{
		if (!FindObject2(Find_ID(RNRC)) || GetCrewExtraData(pPlayer, "VH_Role") == IDdef || GetCrewExtraData(pPlayer, "VH_Role") == 0);
		if (IDdef->~IsCitizien() && IDdef->Check())
		{
			AddMenuItem("%s", "SetRole", IDdef, pPlayer);
			if (!f)
			{
				f = true;
				AddEffect("TimerMenu", pPlayer, 100, 10, this, 0, Format("SetRole(%i)", IDdef));
			}
		}
	}
	
	return 1;
}

private func GetClass(int i)
{
	if (iPhase > 1000)
	{
		return aArray[i];
	}
	else
	{
		return GetDefinition(i, C4D_StaticBack);
	}
}

//setzte eine Rolle
private func SetRole(id IDdef)
{
	if (!IDdef || !Contents(0, this))
	{
		return 0;
	}
	var pClonk = Contents(0, this);
	var iPlrNum = GetOwner(pClonk);
	//Menü schließen
	CloseMenu(pClonk);
	//Effekt entfernen um zu verhindern das dieser für den nächsten Spieler wählt
	if (GetEffect("TimerMenu", pClonk))
		RemoveEffect("TimerMenu", pClonk);
	//Alle Baupläne löschen.
	for (var IDplan, i = 0; GetDefinition(i); i++)
	{
		IDplan = GetDefinition(i);
		SetPlrKnowledge(iPlrNum, IDplan, true);
	}
	//Rolle speichern
	SetCrewExtraData(pClonk, "VH_Role", IDdef);
	pClonk->~SetRole(IDdef);
	//entsprechende baupläne adden
	IDdef->~Support(pClonk);
	//Rolle gesetzt, nächster!
	Vampire();
	return 1;
}
