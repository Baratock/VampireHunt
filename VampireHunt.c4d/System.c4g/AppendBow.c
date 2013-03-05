#strict 2

#appendto BOW1

local pMunition;
local iIndex;

// Aktivieren (zum Zielen per Maussteuerung)
public func Activate(object pClonk)
{
	[$CtrlActivateDesc$|Condition=ShowInContextMenu]
	if (inherited(pClonk))
	{
		// Munitionsid suchen
		iIndex = 0;
		pMunition = FindMunition(iIndex);
		if (pMunition)
			PlayerMessage(GetOwner(pClonk), "%s", this, GetName(pMunition));
	}
	// Erfolg	
	return 1;
}

public func ControlDig(object pClonk)
{
	if (IsAiming())
	{
		// Nächste Munition
		iIndex++;
		pMunition = FindMunition(iIndex);
		if (pMunition)
			PlayerMessage(GetOwner(pClonk), "%s", this, GetName(pMunition));
		return 1;
	}
	return 0;
}

// Munitionssuche
private func FindMunition(int Index)
{
	if (!Contained())
		return 0;
	var aidAmmo = [], Found = 0;
	//Eine Liste mit allen Munitionsids machen
	for (var pObj in FindObjects(Find_Container(Contained()), Find_Or(Find_Func("IsArrow"), Find_Func("IsArrowPack")))) 
	{
		Found = ImpGetID(pObj);
		for (var idDef in aidAmmo) 
		{
			if (Found == idDef)
				Found = 0;
		}
		if (Found)
			aidAmmo[GetLength(aidAmmo)] = Found;
	}
	//Eintrag auslesen
	Found = aidAmmo[Index % GetLength(aidAmmo)];
	var idPoision = ImpGetImp(Found);
	var idMunition = ImpGetObj(Found);
	//nichts geunden und index 0?
	if (!idMunition && !Index)
		return 0;
	//nichts gefunden :(?
	if (!idMunition)
	{
		iIndex = 0;
		return FindMunition(iIndex);
	}
	//Bevorzugt schon entpackte Pfeile, des gesuchten Typs
	Found = 0;
	for (var pObj in FindObjects(Find_Container(Contained()), Find_ID(idMunition))) 
	{
		if (pObj->CheckPoision() == idPoision)
			Found = pObj;
	}
	//falls aber keine entpackten gefunden werden, wird ein Pfeilpacket gesucht
	if (!Found)
	{
		for (var pObj in FindObjects(Find_Container(Contained()), Find_ID(idMunition->PackTo()))) 
		{
			if (pObj->CheckPoision() == idPoision)
				Found = pObj;
		}
		//immernoch nichts gefunden :(
		if (!Found)
			return 0;
	}
	//falls doch zurückgeben
	return Found;
}

private func GetMunition()
{
	var pObj = FindMunition(iIndex);
	if (!pObj)
		return false;
	if (pObj->~IsArrow())
		return pObj;
	if (pObj->~IsArrowPack())
		return pObj->GetItem();
}

// Pfeil abfeuern
public func Fire()
{
	var pClonk = Contained();
	// Kein Träger?
	if (!pClonk)
		return 0;
	// Im Gebäude?
	if (pClonk->Contained())
	{
		pClonk->~StopAiming();
		return 0;
	}
	// Unvollständig?
	if (~GetOCF() & OCF_Fullcon)
		return 0;
	// Noch beim Nachladen
	if (GetAction() == "Reload")
		return 0;
	// Pfeil vom Träger holen
	var pArrow = GetMunition();
	//Keine geeignete Munition
	if (!pArrow)
		return 0;
	// Beim Träger ggf. Bogen wieder auswählen
	ShiftContents(pClonk, 0, BOW1);
	// Austrittsgeschwindigkeit ermitteln
	var iAngle = pClonk->~GetBowAimAngle();
	var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
	pClonk->~GetBowOut(this, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
	
	//Neuer Code(AntiWallhack): Abfrage ob Wand im Weg
  if(!PathFree(pClonk->GetX(), pClonk->GetY(), pClonk->GetX()+iOutX, pClonk->GetY()+iOutY) && FindObject(NWLH)) 
  {
  	Sound("CommandFailure1", false, pClonk, 60);
  	CustomMessage("No Wallhack", pClonk, pClonk->GetController());
  	return true;
  }
	
	// Besitzer des Projektils setzen
	pArrow->SetOwner(Contained()->GetOwner());
	// Pfeil abfeuern
	Exit(pArrow, iOutX, iOutY, iOutR, 0, 0, iOutRDir);
	pArrow->SetXDir(iOutXDir);
	pArrow->SetYDir(iOutYDir);
	pArrow->~Launch();
	// Sicht verfolgen
	if (GetPlrView(GetController(pClonk)) != 2)
		SetPlrView(GetController(pClonk), pArrow);
	// Sound
	Sound("Bow");
	// Nachladen
	SetAction("Reload");
	pClonk->~LoadBow();
	// Fertig!
	return 1;
}
