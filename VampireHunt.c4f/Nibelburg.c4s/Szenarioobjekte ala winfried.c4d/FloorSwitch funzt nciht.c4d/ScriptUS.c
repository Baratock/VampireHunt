/*-- Bodenschalter --*/

#strict

static const _FLS_NeededWeight = 20;
static const _FLS_DownPos      = 4;

local iYPos;

protected func Timer10()
  {
  // Zielposition nach draufliegendem Gewicht
  var iWgt, obj;
  while (obj = FindObject(0, -25,-30,50,30, 0, 0,0, NoContainer(), obj))
    if (obj->GetContact(0, -1, 8))
      if (~obj->GetCategory() & C4D_StaticBack)
        iWgt += obj->GetMass();
  var iDesiredY;
  if (iWgt >= _FLS_NeededWeight) iDesiredY = _FLS_DownPos;
  // Zielposition erreicht?
  if (iDesiredY == iYPos) return();
  // Kein Ziel? ("It�s locked!")
  if (!Local())
    {
    iYPos = iDesiredY;
    Sound("ArrowHit");
    Message("$MsgStuck$", this());
    return(1);
    }
  // Sound am Anfang
  if (!iYPos || iYPos == _FLS_DownPos) Sound("Elevator");
  // Nach oben/unten bewegen
  iDesiredY = BoundBy(iDesiredY-iYPos,-1,1);
  while (obj = FindObject(0, -25,-30,50,30, 0, 0,0, NoContainer(), obj))
    if (obj->GetContact(0, -1, 8))
      if (~obj->GetCategory() & C4D_StaticBack)
        obj->SetPosition(obj->GetX(), obj->GetY()+iDesiredY);
  SetPosition(GetX(), GetY()+iDesiredY);
  iYPos += iDesiredY;
  // Sound am Ende
  if (!iYPos || iYPos == _FLS_DownPos) Sound("Chuff");
  Message("damaged :(", this());
  }


public func IsSwitch() { return(true); }
