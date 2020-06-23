/*-- Schmerzen Teilen --*/

#strict 2

//Wird als Gebet erkannt
public func IsPrayer() { return true;}
public func GetPrayCooldown() { return 30;} //Abklingzeit


//Voraussetzungen
public func Requirements(object pClonk)
{
 	//Nur Menschen mit Level 2 oder besser
	if(pClonk ->~ Who() != Role_Human || GetAction(pClonk) != "Walk" || GetMagicEnergy(pClonk) < 28 || pClonk->~GetLevel() < 2)
	{ 
		return false;
	}
	return true;
}

public func Apply(object pClonk)
{
	AddEffect("PraySharePain",pClonk,150,11,0,GetID(this));
	RemoveObject(this);
	return true;
}
//Effekt für das Schmerzen teilen
protected func FxPraySharePainStart(object pTarget, int iEffectNumber, int iTemp)
{
	ObjectSetAction(pTarget,"Pray");
	return true;
}

protected func FxPraySharePainTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(GetAction(pTarget) != "Pray")
		return -1;
	else
	{
		if(iEffectTime < 11*7)
		{
			//Mana abziehen: in diesem Fall Glauben
			DoMagicEnergy(-3, pTarget);
		}
		else
		{
			//Wenn der Priester zu ende Beten konnte andere Clonks heilen und selbst Schmerz beziehen
			var aClonks = FindObjects(Find_ID(MEDC), Find_Distance(50, GetX(pTarget), GetY(pTarget)), Find_Allied(GetOwner(pTarget)), Find_Exclude(pTarget));
			for(var clonk in aClonks)
			{
				//Schmerz errechnen
				var pain;
				pain = clonk->~Pain()*35/100;
				//Abziehen und dem Priester hinzufügen
				clonk->~Pain(-pain);
				pTarget->~Pain(pain*40/100);
			}
			ObjectSetAction(pTarget, "FlatUp");
			return -1;
		}
	}
	return true;
}
