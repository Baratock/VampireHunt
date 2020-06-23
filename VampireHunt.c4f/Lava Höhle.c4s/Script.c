/*-- Vampirjagd, allgemeiner Szenscript (übertragbar) --*/

#strict 2

static chooser; //Verteilobjekt
static villagers; //Anzahl der Relunches

protected func Initialize()
{
	//Geklauter Nebel...
	for(var i = 0;i < 350; i++) 
		CreateParticle("Fog",Random(LandscapeWidth()),Random(1200),0,0,RandomX(900,1700));
	chooser = CreateObject(VHRG,50,50,NO_OWNER);
	CreateObject(VHMB,50,50,NO_OWNER);
	CreateObject(_ETG,50,50,NO_OWNER);
	villagers = 8;
	ScriptGo(1);
	return 1;
}

protected func Script0()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		if (GetPlayerName(i))
		{
			SetFoW(true,i); //ist der FOW aus?
			var pCursor = GetCursor(i);
			if (pCursor)
				Enter(chooser,pCursor);
		}
	}
	chooser -> Vampire();
	//Wichtig: Zeit nach dem Wähler setzen
	CreateObject(TIME,100,100,-1);
	ScriptGo(0);
	return 1;
}


//------------------------------Relaunchsystem---------------------------------------

protected func RelaunchPlayer(int iPlr)
{
	var iWealth = GetWealth(iPlr);
	
	//Vampir
	if(iWealth > 1)
	{
		EliminatePlayer(iPlr);
		//Okay, Diener leben vielleicht noch: töten
		for(var iPlr = 0,i = 0; i < GetPlayerCount() ; i++)
		{
			//spieler ermitteln
			iPlr = GetPlayerByIndex(i);
			var pClonk = GetCrew(iPlr,0);
			if(pClonk)
				if(pClonk->Who() > 0)
				{
					EliminatePlayer(iPlr);
					continue;
				}
		}
		//Spiel beenden
		Schedule("GameOver()", 500);
		Message("Das Licht siegt!");
		Sound("GoodWins",true);		
	}
	//Dorf
	else
	{
		villagers--;
		//keine Relunchs?
		if(villagers < 0)
		{
			EliminatePlayer(iPlr);
			var f = 0;
			for(var p in FindObjects(Find_ID(MEDC)))
				if(p->Who() == Role_Human && GetOwner(p) != iPlr)
					f = 1;
			//alle menschen tod :(?
			if(!f)
			{
				Schedule("GameOver()", 500);
				Message("Die Dunkelheit triumphiert!");
				Sound("EvilWins",true);					
			}
			return 1;
		}
		//Relunchen
		var pClonk = CreateObject(MEDC,0,0,iPlr);
		//Provisorisch blut nachfüllen
		pClonk->Blood(100);
		MakeCrewMember(pClonk,iPlr);
		SetCursor(iPlr,GetCrew(iPlr));
		//Relunchobjekt
		var pSoul = CreateObject(SELE,LandscapeWidth()/2,50,iPlr);
		SetVisibility(VIS_Owner,pSoul);
		Enter(pSoul,pClonk);
		//leicht durchsichtig
		SetClrModulation(RGBa(255,255,255,127),pSoul);
		PlayerMessage(iPlr,"Eure Sorge lässt euch nicht ruhen.|Vielleicht könnt ihr sogar ins Leben zurückkehren...| Ihr habt noch %d Relunchs",,villagers);		
	}
	return 0;	
 }