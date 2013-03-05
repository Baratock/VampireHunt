/*-- Überprüfen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "315";
}

public func CanUse(object pCaller)
{
	if (pCaller->SleepingClonk())
		return true;
}

public func Activate(object pCaller)
{
	var pVictim = pCaller->SleepingClonk();
	if (!pVictim)
		return 0;
	var szClass = "";
	var szBlood = "";
	var szPain = "";
	//Info sammeln
	if (pVictim->GetVal(cBlood) < 40)
		szBlood = "$VHCheckBlood1$";
	if (pVictim->GetVal(cBlood) < 20)
		szBlood = "$VHCheckBlood2$";
	if (pVictim->GetVal(cBlood) > 40)
		szPain = "$VHCheckPains1$";
	if (pVictim->GetVal(cBlood) > 60)
		szPain = "$VHCheckPains2$";
	//Klasse finden
	szClass = Format("$VHCheckClass$", GetName(0, pVictim->GetRole()));
	var aObjs = [];
	//IDs speichern
	for (var i = 0; i < ContentsCount(0, pVictim); i++)
	{
		aObjs[i] = Contents(i, pVictim);
	}
	//Anzahl finden
	var szItems = "";
	var i = 0;
	for (var dwClr = 0, i3 = 0; i3 < GetLength(aObjs); i3++)
	{
		if (aObjs[i3] == 0)
			continue;
		for (var i2 = 0; i2 < GetLength(aObjs); i2++)
		{
			if (GetID(aObjs[i3]) == GetID(aObjs[i2]) && GetClrModulation(aObjs[i3]) == GetClrModulation(aObjs[i2]))
			{
				if (i2 != i3)
					aObjs[i2] = 0;
				i++;
			}
		}
		dwClr = GetClrModulation(aObjs[i3]);
		if (dwClr == 0)
			dwClr = RGB(255, 255, 255);
		if (szItems == "")
			szItems = Format("%d <c %x>{{%i}}</c>", i, dwClr, GetID(aObjs[i3]));
		else
			szItems = Format("%s, %d <c %x>{{%i}}</c>", szItems, i, dwClr, GetID(aObjs[i3]));
		i = 0;
	}
	szItems = Format("$VhCheckItems$", szItems);
	//nachricht
	PlayerMessage(GetOwner(pCaller), "$VHCheckName$", pCaller, szBlood, szPain, szClass, szItems);
}




