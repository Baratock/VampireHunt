/*-- Beistand --*/

#strict 2

local user;
local phase;

//Wird als Gebet erkannt
public func IsPrayer() { return true; }

//Voraussetzungen
public func Requirements(caller)
{
	//Nur Menschen
	if(caller ->~ Who() != Role_Human || GetAction(caller) != "Walk" || GetMagicEnergy(caller) < 10)
	{ 
		return false;
	}
 	return true;
}

public func Apply()
{
	if(Contained(this))
	{
		user = Contained(this);
		user -> Exit(this);
	}
}

private func Phase()
{
	if(!phase)
		Message("Scriptus magnus!",user);
	 if(phase == 1)
	 	Message("Servos tuas in poenas clamunt",user);
	 if(phase == 2)
	 	 Message("Poenas a corpi expelle!",user);
	 if(phase == 3)
	 	Message("Corpus hostilis a ea tormenti eras!",user);
	user->DoMagicEnergy(-2);
	phase++;	 
}

//Ablauf
public func Check()
{
	//Wenn er stirbt zb
	if(!user) 
		return RemoveObject(this);
	//Klappe zu? Treffer, Abtauchen, Kämpfe...
	if(GetAction(user) != ("Walk" || "Climb" || "Hangle" || "Jump"))
		return RemoveObject(this);

	SetPosition(GetX(user),GetY(user));

	if(phase == 4)
	{
		//Schmerzen tilgen
		var pain;
		for(var pClonk in FindObjects(Find_ID(MEDC),Find_Distance(120)))
		{
			pain = pClonk -> RestorePain();
			if(pClonk -> Who() == Role_Human)
			{
				pClonk -> RestorePain(pain/2);
				if(GetAction(pClonk) == "Dead") pClonk -> SetAction("FlatUp");
				Message("AMEN",pClonk);
			}
		} 
		return RemoveObject(this);
	}
	
	Phase();
}
