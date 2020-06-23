/*-- Schalter --*/

#strict 2

local fSpecial;
local fMiddle;

protected func Initialize()
  {
  SetAction("Normal");
  SetPhase(Random(1));
  }

protected func ControlLeft(object pClonk)
  {
  [DescSwitch]
  if(fSpecial) return;	  
  if (GetPhase()==0) return;
  Sound("Click"); SetPhase();
  // Angeschlossene Objekte benachrichtigen
  var i,obj; while (obj = Local(i++))
  {	  
    if (Local(i)==1)
      obj->~ControlLeft(this);
    else if (Local(i)==0)
      obj->~ControlRight(this);
    else if (Local(i)==3)
      obj->~ControlUp(this);
    else if(Local(i)==2)
      obj->~ControlDown(this);
    else if(Local(i)==4)
      obj->~Activate(this);
    else if(Local(i)==5)
      obj->~Special(this);
    i++;
  }
  if (Local()) SetPlrView(GetController(pClonk), Local());
  return true;
  }

protected func ControlRight(object pClonk)
  {
  [DescSwitch]
  if (GetPhase()==1) return;
  Sound("Click"); SetPhase(1);
  // Angeschlossene Objekte benachrichtigen
  var i,obj; while (obj = Local(i++))
  {	  
    if(fSpecial)
      obj->~ControlSpecial(this);
    else if (Local(i)==0)
      obj->~ControlLeft(this);
    else if(Local(i)==1)
      obj->~ControlRight(this);
    else if (Local(i)==2)
      obj->~ControlUp(this);
    else if(Local(i)==3)
      obj->~ControlDown(this);
    else if(Local(i)==4)
      obj->~Activate(this);
    else if(Local(i)==5)
      obj->~Special(this);
    i++;
  }
  if (Local()) SetPlrView(GetController(pClonk), Local());
  return true;
  }

public func Special()
{
  // Angeschlossene Objekte benachrichtigen
  var i,obj; while (obj = Local(i++))
  {	  
    if (Local(i)==0)
      Local(i)=2;
    else if(Local(i)==1)
      Local(i)=3;
    else if (Local(i)==2)
      Local(i)=0;
    else if(Local(i)==3)
      Local(i)=1;
    else if(Local(i)==4)
      Local(i)=5;
    else if(Local(i)==5)
      Local(i)=4;
    SetDir(!GetDir());
    i++;
  }
  return true;
}

public func AttachBottom()
  {
  var obj = FindObject(0, 0,13,0,0);
  if (!obj) return;
  // Vertex ankleben
  SetVertex(0, 0, obj->GetX()-GetX()+GetVertex(0, 0, obj));
  SetVertex(0, 1, obj->GetY()-GetY()+GetVertex(0, 1, obj));
  SetAction("Attach", obj);
  return true;
  }

public func AddTarget(object pTarget, int iDir)
  {
//  if (!iDir) iDir=1;
  var i; while (Local(i)) ++++i;
  Local(i++) = pTarget; Local(i) = iDir;
  }

public func IsSwitch() { return true; }
