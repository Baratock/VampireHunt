#strict 2

#appendto ARRW

local poision;

public func JoinPack(object pClonk, bool fForce)
{
	// Nicht wenn unterdrückt
	if (GetEffect("IntNoPacking", pClonk))
		return 0;
	// Pfeilpakete im Clonk suchen
	for (var obj in FindObjects(Find_Container(pClonk), Find_ID(PackTo()))) 
	{
		// Noch Platz?
		if (obj->MaxPackCount() > obj->PackCount() && obj->CheckPoision() == CheckPoision())
		{
			// Integrieren
			obj->DoPackCount(1);
			return RemoveObject(this);
		}
	}
	// Nur bei passendem Flag Aufnahme erzwingen
	if (!fForce)
		return 0;
	// Neues Pack anfangen
	var new = CreateObject(PackTo(), -GetX(this), -GetY(this), GetOwner(pClonk));
	new->DoPackCount(1 - new->MaxPackCount());
	new->Impregnate(poision);
	var fBow = Contents(0, pClonk)->~IsBow();
	Enter(pClonk, new);
	//wenn bogen, dann bogen nach vorne
	if (fBow)
	{
		ShiftContents(pClonk, true, BOW1, 0);
	}
	return RemoveObject(this);
}

public func Pack(object pClonk)
{
	// Andere Einzelpfeine suchen und packen
	for (var obj in FindObjects(Find_Exclude(this), Find_Container(pClonk), Find_ID(GetID(this)))) 
	{
		obj->~JoinPack(pClonk, 1);
	}
	// und selber zu Pack hinzufügen
	JoinPack(pClonk, 1);
	return 1;
}

public func Impregnate(id IDdef)
{
	if (!IDdef || !IDdef->IsPoision())
	{
		return 0;
	}
	var dwClr = IDdef->~Color();
	SetClrModulation(dwClr);
	var name = Format("%s-%s", GetName(0, IDdef), GetName());
	SetName(name);
	poision = IDdef;
}

public func HitEffect(object pTarget) //Direkte Wirkung
{
	if (GetID(pTarget) == WIPF)
	{
		Kill(pTarget);
		RemoveObject(this);
		return 0;
	}
	UsePoision(this, poision, pTarget);
	if (!Random(4))
	{
		AddEffect("FadeOut", this, 10, 2, this, 0);
	}
	return 1;
}

//überladung
public func HitTarget(object pObject)
{
	Sound("ArrowHit");
	// Ruestungsritter z.B. wollen nicht immer getroffen werden
	if (pObject->~QueryCatchBlow(this))
		return 0;
	if (GetAlive(pObject))
	{
		var iDmg = (ArrowStrength() * (Max(Abs(GetXDir()), Abs(GetYDir())) + 1)) / 50;
		DoEnergy(-iDmg, pObject);
		
		// Objekt wurde getroffen. Damit Schild des Ritters noch vor Pfeilen schuetzt
		if (pObject && GetAlive(pObject))
			pObject->~CatchBlow(-iDmg, this());
		// Ziel koennte irgendwas mit dem Pfeil gemacht haben
		if(!this)
			return 0;
		// Schleudern nur einmal / Sekunde erlaubt
		var fTumble = true;
		var iEffect;
		if (pObject && pObject->~IsClonk())
		{ 
			for (var i = 0; iEffect = GetEffect("IntNoArrowTumble", pObject, i); i++)
			{
				if (EffectVar(0, pObject, GetEffect("IntNoArrowTumble", pObject, i)) != this)
					continue;
				fTumble = false;
				break;
			}
		}
		var iTumblevamp = 1;
		if (pObject->~GetVal(cForm) == cRoleVampire)
			iTumblevamp = 0;
		// Zielobjekt auch sichtbar treffen
		if (pObject && fTumble && !Random(ArrowTumbleRandom()) && iTumblevamp)
		{
			var xDir = GetXDir(this) / 3;
			var yDir = GetYDir(this) / 3;
			
			if (GetAlive(pObject))
			{
				pObject->SetAction("Tumble");
				AddEffect("IntNoArrowTumble", pObject, 1, 30, 0, ARRW, this());
			}
			pObject->SetSpeed(GetXDir(pObject) + xDir, GetYDir(pObject) + yDir - 1);
		}
		// Wenn sich das Objekt nicht entfernt hat noch festhaengen (Zielscheibe zB)
		if (pObject && pObject->~IsArrowTarget(this))
		{
			SetXDir(0);
			SetYDir(0);
			SetRDir(0);
			Local(2) = pObject;
			Local(3) = GetX() - GetX(pObject);
			Local(4) = GetY() - GetY(pObject);
			SetAction("Stick");
		}
		// Pfeil stoppen?
		if (ArrowStopsOnHit())
		{
			SetSpeed(GetXDir(pObject), GetYDir(pObject));
			if (GetAction() == "Shot")
				SetAction("Idle");
			// Nicht sofort einsammeln
			AddEffect("CannotBeCollected", this(), 1, 36 * 2, 0, ARRW);
		}
	}
}

public func CheckPoision()
{
	return poision;
}
