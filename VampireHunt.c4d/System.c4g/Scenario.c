/*-- Funktionen für Scenarioinitialisierung und Relaunch --*/

#strict 2

//globale Variablen
static chooser; //Verteilobjekt
static villagers; //Anzahl der Relunches

//Initialisierung der Karte
global func InitScenario(int iType, int iRelaunches) //iType = Spieltyp der runde. Noch nicht eingebaut
{
	//Geklauter Nebel...
	for (var i = 0; i < 350; i++)
		CreateParticle("Fog", Random(LandscapeWidth()), Random(1200), 0, 0, RandomX(900, 1700));
	chooser = CreateObject(VHRG, 50, 50, NO_OWNER);
	CreateObject(VHMB, 50, 50, NO_OWNER);
	CreateObject(_ETG, 50, 50, NO_OWNER);
	CreateObject(NWLH, 50, 50, NO_OWNER);
	if (iRelaunches)
		villagers = iRelaunches;
	else
		villagers = 8;
	AddEffect("ScenPostInit", false, 1, 10);
	return 1;
}

//PostInit den Scenarien
global func FxScenPostInitStop()
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

//Relaunch

global func Relaunch(int iPlr, object pCrew, int iType)
{
	//Log("%v", iPlr);
	if (iPlr == NO_OWNER)
		return false;
	var iWealth = GetWealth(iPlr);
	//Diener
	var pClonk = GetCrew(iPlr, 0);
	if (!pClonk)
		pClonk = pCrew;
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
			Schedule("GameOver()", 500);
			Message("Das Licht siegt!");
			Sound("GoodWins", true);
		}
		else
		{
			villagers--;
			//keine Relunchs?
			if (villagers < 0)
			{
				var f = 0;
				for (var p in FindObjects(Find_ID(MEDC))) 
					if ((p->GetRole()->~IsCitizien() || !p->GetRole()) && GetOwner(p) != iPlr)
						f = 1;
				//alle menschen tod :(?
				if (!f)
				{
					//Falls noch irgentwo Dorfbewohner sein sollten diese Eliminieren
					Schedule("EliminateVillagers()", 300);
					Schedule("GameOver()", 500);
					Message("Die Dunkelheit triumphiert!");
					Sound("EvilWins", true);
				}
				return 1;
			}
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
	for (var i = GetPlayerCount(C4PT_User); i >= 0; i--)
	{
		var iPlr = GetPlayerByIndex(C4PT_User);
		var pClonk = FindObject2(Find_ID(MEDC), Find_Owner(iPlr));
		if (pClonk->~GetRole()->~IsCitizien())
			EliminatePlayer(iPlr);
	}
	return true;
}
