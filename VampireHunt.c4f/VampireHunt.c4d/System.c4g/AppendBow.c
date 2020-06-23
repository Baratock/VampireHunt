#strict 2

#appendto BOW1

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
	[$CtrlActivateDesc$|Condition=ShowInContextMenu]
	if(inherited(pClonk))
	{
		// Munitionsid suchen
		Local(9) = GetMunition();
		Local(10) = 0;
		if(Local(9)) 
			PlayerMessage(GetOwner(pClonk), "%s", this, GetName(Local(9)));
	}
	// Erfolg	
	return 1;
}

public func ControlDig(pClonk)
{
	if (IsAiming())
	{
		// Nächste Munition
		Local(10)++;
		Local(9) = GetMunition(Local(10));
		if(Local(9)) 
			PlayerMessage(GetOwner(pClonk), "%s", this, GetName(Local(9)));
		return 1;
	}
	return 0;
}

/* Munitionssuche */
private func GetMunition(iIndex)
{
	if(!Contained()) 
		return 0;
	var aAmmo = [], idPack, idFound, fFound;
	// Liste mit Munitionsid's machen
	for(var pObj in FindObjects(Find_Container(Contained()), Find_Or(Find_Func("IsArrow"), Find_Func("IsArrowPack"))))
	{
		var idObj,idObjPack,idArrow;
		// Haben wir die ID schon?
	    fFound = 0;
		for(pTest in aAmmo)
		{
			if(pTest->~IsArrowPack())
			{
				idPack = GetID(pTest);
				idArrow  = pTest->UnpackTo();
			}
			else
				idArrow = GetID(pTest);
			if(pObj->~IsArrowPack())
			{
				idObjPack = GetID(pTest);
				idObj = pObj->UnpackTo();
			}
			else
				idObj = GetID(pObj);
			if(idObjPack == idPack || idObj == idArrow)
				if(LocalN("poision",pTest) == LocalN("poision",pObj))
					fFound = 1;
		}
		if(!fFound)
			aAmmo[GetLength(aAmmo)] = pObj;
	}
	// den Eintrag mit passendem Index ausgeben
	return aAmmo[ iIndex % GetLength(aAmmo) ];
} 

// Haben wir noch was von der Munitionssorte?
private func CheckMunitionID(object pMuni)
{
	// Liste mit Munitionsid's machen
	var idFound;
	for(var pObj in FindObjects(Find_Container(Contained()), Find_Or(Find_Func("IsArrow"), Find_Func("IsArrowPack"))))
	{
		// ID suchen
		idFound = GetID(pObj);
		if(pObj->~IsArrowPack()) idFound = pObj->~UnpackTo();
		// Ist die ID die gesuchte?
		if(idFound == GetID(pMuni) && LocalN("poision",pMuni) == LocalN("poision",pObj)) return 1;
	}
}

private func GetMunition2()
{
	var pObj = GetMunition(Local(10));
	if(!pObj)
		return false;
	if(pObj->~IsArrow())
		return pObj;
	if(pObj->~IsArrowPack())
		return pObj->GetItem();
}

/* Pfeil abfeuern */
public func Fire()
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return 0;
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return 0;
  // Noch beim Nachladen
  if (GetAction() == "Reload") return 0;
  // Pfeil vom Träger holen
  var pArrow = GetMunition2();
  if (!pArrow) 
  {
    // andere Munition?
    ControlDig(Contained());
    pArrow = GetMunition2();
    // wohl doch nicht :-(
    if (!pArrow) 
      return 0;
  }
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(pClonk, 0, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iAngle = pClonk->~GetBowAimAngle();
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutOld(pClonk, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
  pArrow->~SetShooter(Contained(this));
  // Sicht verfolgen
  if (GetPlrView(GetController(pClonk)) != 2)
    SetPlrView(GetController(pClonk), pArrow);
  // Sound
  Sound("Bow");
  // Nachladen
  SetAction("Reload");
  pClonk->~LoadBow();
  // Munitionscheck
  if(!CheckMunitionID(Local(9)))
  {
    // Leer? Dann andere Munition
    ControlDig(Contained());
  }
  // Fertig!
  return 1;
}
