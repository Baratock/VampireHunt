/*-- Neues Script --*/

#strict 2
#appendto XBOW

public func Fire(bool fAuto,object pClonk)
{
  // Noch am Nachladen? Nix da.
  if(GetEffect("IntReload", this()) ) return(true);

  // Pfeilpakete zerlegen, sofern nötig
  SplitPacks();

  // Pfeil suchen
  var pArrow=Contents();
  if (!pArrow)
  {
  	Sound("Click");
  	return 0;
  }

  // Austritt berechnen
  var iX = GetPhase()*(GetDir()*2-1);
  var iY = GetPhase()-15;
  var iR = iX*5;
  var iAngle = BoundBy(GetPhase()*5,0,90);
  var iXDir = Sin(iAngle,13*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-13);
  var iRDir = GetDir()*6-3;

  // Pfeil abfeuern
  Exit(pArrow,iX,iY,iR,iXDir,iYDir,iRDir);
  pArrow->~Launch(this);
  pArrow->~SetShooter(pClonk);
  
  // Pfeil trifft Schuetzen fuer 10 Frames nicht
  pArrow->~NoArrowHit(pClonk,10);
  
  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pArrow);
  // Sound
  Sound("Arrow");
  // Nachladen
  AddEffect("IntReload", this, 1, ReloadDelay(), this );
  // Fertig
  return 1;
}
