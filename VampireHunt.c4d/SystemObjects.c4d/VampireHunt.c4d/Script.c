#strict 2
#include GOAL

//globale Variablen
static villagers; //Anzahl der Relunches
//locale Variablen
local chooser; //Verteilobjekt
local bStarted;
//Initialisierung der Karte
protected func Initialize()
{
	chooser = CreateObject(VHRG, 50, 50, NO_OWNER);
	CreateObject(VHMB, 50, 50, NO_OWNER);
	CreateObject(_ETG, 50, 50, NO_OWNER);
	villagers = 8;
	bStarted = false;
	AddEffect("ScenPostInit", this, 1, 10, this);
	return _inherited();
}

//PostInit den Scenarien
private func FxScenPostInitStop()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		if (GetPlayerName(i))
		{
			SetFoW(true, i); //ist der FOW aus?
			var pCursor = GetCursor(i);
			if (pCursor)
				Enter(chooser, pCursor);
		}
	}
	chooser->Vampire();
	//Wichtig: Zeit nach dem Wähler setzen
	CreateObject(TIME, 100, 100, -1);
	return true;
}

public func IsFulfilled()
{
	//wenn noch kein Vampir existierte nix unternehmen
	if (!bStarted)
	{
		if (FindVampire())
			bStarted = true;
		return false;
	}
	
	//Nur wenn die runde Schon gestartet hat
	if (bStarted)
	{
		if (!GetTrueUndeadCount() && GetTrueVillagerCount())
		{
			Message("Das Licht siegt!");
			Sound("GoodWins", true);
			return true;
		}
		
		if (!GetTrueVillagerCount() && GetTrueUndeadCount())
		{
			EliminateVillagers();
			Message("Die Dunkelheit triumphiert!");
			Sound("EvilWins", true);
			return true;
		}
	}
	return false;
}

protected func GetPublicVillagerCount()
{
	var j = 0;
	for (var i = GetPlayerCount(C4PT_User) - 1; i >= 0; i--)
	{
		var iPlr = GetPlayerByIndex(i, C4PT_User);
		var pClonk = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
		if (pClonk->~GetRole() != cRoleLich && pClonk->~GetRole() != cRoleVampire)
			j++;
	}
	j = Max(j, villagers);
	return j;
}

protected func GetPublicUndeadCount()
{
	var j = 0;
	for (var i = GetPlayerCount(C4PT_User) - 1; i >= 0; i--)
	{
		var iPlr = GetPlayerByIndex(i, C4PT_User);
		var pClonk = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
		if (pClonk->~GetRole() == cRoleLich || pClonk->~GetRole() == cRoleVampire)
			j++;
	}
	return j;
}


protected func GetTrueVillagerCount()
{
	var j = 0;
	for (var i = GetPlayerCount(C4PT_User) - 1; i >= 0; i--)
	{
		var iPlr = GetPlayerByIndex(i, C4PT_User);
		var pClonk = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
		if (pClonk->~GetRole() != cRoleGhul && pClonk->~GetRole() != cRoleLich && pClonk->~GetRole() != cRoleVampire)
			j++;
	}
	j = Max(j, villagers);
	return j;
}

protected func GetTrueUndeadCount()
{
	var j = 0;
	for (var i = GetPlayerCount(C4PT_User) - 1; i >= 0; i--)
	{
		var iPlr = GetPlayerByIndex(i, C4PT_User);
		var pClonk = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
		if (pClonk->~GetRole() == cRoleGhul || pClonk->~GetRole() == cRoleLich || pClonk->~GetRole() == cRoleVampire)
			j++;
	}
	return j;
}


//Relaunch
public func RelaunchPlayer(int iPlr, object pCrew)
{
	if (iPlr == NO_OWNER)
		return false;
	var iWealth = GetWealth(iPlr);
	//Diener
	var pClonk = GetCrew(iPlr, 0);
	if (!pClonk)
		pClonk = pCrew;
	//ToteDiener Eliminierne
	if (pClonk->~GetRole() == cRoleGhul || pClonk->~GetRole() == cRoleLich)
		EliminatePlayer(iPlr);
	else
		if (pClonk->~GetRole() == cRoleVampire)
		{
			EliminatePlayer(iPlr);
			//Okay, Diener leben vielleicht noch: töten
			for (var iPlr = 0, i = 0; i < GetPlayerCount(); i++)
			{
				//spieler ermitteln
				iPlr = GetPlayerByIndex(i);
				var pSlave = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
				if (pSlave)
					if (pSlave->~GetRole() == cRoleGhul || pSlave->~GetRole() == cRoleLich)
						EliminatePlayer(iPlr);
			}
			//Spiel beenden
		}
		else
		{
			villagers--;
			if (villagers < 0)
				return false;
			Log("!!");
			/*
		//keine Relunchs?
		if(villagers < 0)
		{
			var f = 0;
			for(var p in FindObjects(Find_ID(MEDC)))
				if((p->GetRole()->~IsCitizien() || !p->GetRole() )&& GetOwner(p) != iPlr)
					f = 1;
			//alle menschen tod :(?
			if(!f)
			{
				//Falls noch irgentwo Dorfbewohner sein sollten diese Eliminieren
				Schedule("EliminateVillagers()",300);
				Schedule("GameOver()", 500);
				Message("Die Dunkelheit triumphiert!");
				Sound("EvilWins",true);					
			}
			return 1;
		}*/
			//Relunchen
			var pClonk = CreateObject(MEDC, 0, 0, iPlr);
			MakeCrewMember(pClonk, iPlr);
			SetCursor(iPlr, GetCrew(iPlr));
			//Relunchobjekt
			var pSoul = CreateObject(SELE, LandscapeWidth() / 2, 50, iPlr);
			SetVisibility(VIS_Owner, pSoul);
			Enter(pSoul, pClonk);
			//leicht durchsichtig
			SetClrModulation(RGBa(255, 255, 255, 127), pSoul);
			PlayerMessage(iPlr, "Eure Sorge lässt euch nicht ruhen.|Vielleicht könnt ihr sogar ins Leben zurückkehren...| Ihr habt noch %d Relunchs", , villagers);
		}
	return 0;
}

global func EliminateVillagers()
{
	for (var i = GetPlayerCount(C4PT_User) - 1; i >= 0; i--)
	{
		var iPlr = GetPlayerByIndex(i, C4PT_User);
		var pClonk = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
		if (pClonk->~GetRole()->~IsCitizien())
			EliminatePlayer(iPlr);
	}
	return true;
}
//Relaunch Anzahl setzten
global func SetVillagers(int iCount)
{
	return villagers = iCount;
}
