/*-- Vampirjagd, allgemeiner Szenscript (übertragbar) --*/

#strict 2

static chooserl; //Verteilobjekt links
static chooserr; //Verteilobjekt rechts
static villagersl; //Anzahl der Relunches links
static villagersr; //Anzahl der Relunchs rechts 
static iCnt; //zähler der hilft

protected func Initialize()
{
	//Geklauter Nebel...
	for (var i = 0; i < 350; i++)
		CreateParticle("Fog", Random(LandscapeWidth()), Random(1200), 0, 0, RandomX(900, 1700));
	chooserl = CreateObject(VHRG, 50, 50, NO_OWNER);
	chooserr = CreateObject(VHRG, 50, 50, NO_OWNER);
	CreateObject(VHMB, 50, 50, NO_OWNER);
	CreateObject(_ETG, 50, 50, NO_OWNER);
	villagersl = 4;
	villagersr = 4;
	iCnt = CreateArray(1);
	DrawMatRect("Brick", 1350, 2236, 15, 44);
	DrawMatRect("Brick", 2925, 2236, 15, 44);
	return 1;
}

protected func InitializePlayer(int iPlr)
{
	if (GetPlayerName(iPlr))
	{
		SetFoW(true, iPlr); //ist der FOW aus?
		var pCursor = GetCrew(iPlr);
		if (pCursor)
		{
			if (GetPlayerTeam(iPlr) == 3)
			{
				chooserl->SetVamp(pCursor);
			}
			//rechts
			if (GetPlayerTeam(iPlr) == 2)
			{
				Enter(chooserr, pCursor);
			}
			//links
			if (GetPlayerTeam(iPlr) == 1)
			{
				Enter(chooserl, pCursor);
			}
			iCnt[0]++;
		}
	}
	if (GetPlayerCount() == iCnt[0])
		ScriptGo(true);
}

protected func Script0()
{
	chooserl->Team([J_GD, J_VG, J_SM, J_PR]);
	chooserr->Team([J_VH, J_HN, J_SM, J_FS]);
	//Wichtig: Zeit nach dem Wähler setzen
	CreateObject(TIME, 100, 100, -1)->SetTime(20);
	return 1;
}

protected func Script800()
{
	ScriptGo(false);
	AddEffect("GlobalDrain", 0, 200, 5);
	DrawMatRect("Tunnel-Rough", 1350, 2236, 15, 44);
	DrawMatRect("Tunnel-Rough", 2925, 2236, 15, 44);
}

global func FxGlobalDrainTimer(object zero, int iEffectNumber, int iEffectTime)
{
	if (iEffectTime > 3700)
	{
		return -1;
	}
	var xBase = 2204;
	var yBase = 2933;
	ExtractMaterialAmount(xBase, yBase, Material("Acid"), 100);
}


//------------------------------Relaunchsystem---------------------------------------

protected func RelaunchPlayer(int iPlr)
{
	var iWealth = GetWealth(iPlr);
	//Diener
	if (iWealth == 1)
	{
		EliminatePlayer(iPlr);
	}
	else
		if (iWealth > 1)
		{
			EliminatePlayer(iPlr);
			//Okay, Diener leben vielleicht noch: töten
			for (var iPlr = 0, i = 0; i < GetPlayerCount(); i++)
			{
				//spieler ermitteln
				iPlr = GetPlayerByIndex(i);
				var pClonk = GetCrew(iPlr, 0);
				if (pClonk)
					if (pClonk->Who() > 0)
					{
						EliminatePlayer(iPlr);
						continue;
					}
			}
			Message("Das Dunkle verlässt diese Welt!");
			Help(1);
		}
		else
		{
			if (GetPlayerTeam(iPlr) == 1)
			{
				villagersl--;
			}
			else
			{
				villagersr--;
			}
			//keine Relunchs für Döfler?
			if (villagersr < 0 && villagersl < 0)
			{
				//Vamp lebt?
				if (FindVampire() != -1)
				{
					var f = 0;
					//aber kein Mensch?
					for (var pClonk in FindObjects(Find_ID(MEDC))) 
						if (pClonk->Who() == Role_Human && GetOwner(pClonk) != iPlr)
							f = 1;
					if (!f)
					{
						//Ende
						Schedule("GameOver()", 100, 1);
						Message("Die Dunkelheit triumphiert!");
						Sound("EvilWins", true);
						return 1;
					}
				}
			}
			//eines der Dörflerteams siegen
			Help();
			//keine Relunchs?
			if (GetPlayerTeam(iPlr) == 1)
			{
				if (villagersl < 0)
				{
					EliminatePlayer(iPlr);
					return 1;
				}
			}
			else
			{
				if (villagersr < 0)
				{
					EliminatePlayer(iPlr);
					return 1;
				}
			}
			//Relunchen
			var pClonk = CreateObject(MEDC, 0, 0, iPlr);
			//Provisorisch blut nachfüllen
			pClonk->Blood(100);
			MakeCrewMember(pClonk, iPlr);
			SetCursor(iPlr, GetCrew(iPlr));
			//Relunchobjekt
			var pSoul = CreateObject(SELE, LandscapeWidth() / 2, 50, iPlr);
			SetVisibility(VIS_Owner, pSoul);
			Enter(pSoul, pClonk);
			//leicht durchsichtig
			SetClrModulation(RGBa(255, 255, 255, 127), pSoul);
			if (GetPlayerTeam(iPlr) == 1)
			{
				PlayerMessage(iPlr, "Eure Sorge lässt euch nicht ruhen.|Vielleicht könnt ihr sogar ins Leben zurückkehren...| Ihr habt noch %d Relunchs", , villagersl);
			}
			else
			{
				PlayerMessage(iPlr, "Eure Sorge lässt euch nicht ruhen.|Vielleicht könnt ihr sogar ins Leben zurückkehren...| Ihr habt noch %d Relunchs", , villagersr);
			}
		}
	return 0;
}
 
 protected func Help(bool f2)
{
	//Vamp tot?
	if (FindVampire() == -1 || f2)
	{
		//Keine Relunchs?
		if (villagersl < 0)
		{
			var f = 0;
			//noch ein Mensch im linken Team?
			for (var pClonk in FindObjects(Find_ID(MEDC))) 
				if (pClonk->Who() == Role_Human && GetPlayerTeam(GetOwner(pClonk)) == 1)
					f = 1;
			//falls ende
			if (!f)
				Schedule("GameOver()", 100, 1);
		}
		//Keine Relunchs?
		if (villagersr < 0)
		{
			var f = 0;
			//noch ein Mensch im rechten Team?
			for (var pClonk in FindObjects(Find_ID(MEDC))) 
				if (pClonk->Who() == Role_Human && GetPlayerTeam(GetOwner(pClonk)) == 2)
					f = 1;
			//falls ende
			if (!f)
				Schedule("GameOver()", 100, 1);
		}
	}
}
