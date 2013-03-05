#appendto CANN
#strict 2

public func Fire(bool fAuto)
{
  // Schieﬂpulver und Projektil suchen
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  
  // Schieﬂpulver oder Projektil fehlt
  if (!pGunpowder || !pProjectile) {
    Sound("Click");
    return false;
  }
  

  // Austritt berechnen
  var iX = (GetPhase()+3)*(GetDir()*2-1);
  var iY = GetPhase()-14;
  var iAngle = BoundBy(GetPhase()*5-5,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-18);
  
  //Neuer Code: Abfrage f¸r AntiWallhack
  if(!PathFree(this->GetX(), this->GetY(), this->GetX()+iX, this->GetY()+iY) && FindObject(NWLH)) 
  {
  	CustomMessage("No Wallhack", this, this->GetController());
  	return true;
  }
  
  // Schieﬂpulver verbrauchen
  RemoveObject(pGunpowder);
  
  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch
  for (var i = 0; i < 6; ++i) 
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
  // Erfolgreich geschossen
  return true;
}