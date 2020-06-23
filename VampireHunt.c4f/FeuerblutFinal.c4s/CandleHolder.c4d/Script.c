/* Kerzenhalter */

#strict

local aOMGCheck;

protected func Initialize()
  {
  SetAction("On");
  return(true);
  }

public func SetOMG()
  {
  RemoveEffect("IntOMG", this());
  AddEffect("IntOMG", this(), 1, 51, this());
  aOMGCheck = Find_And(Find_InRect(-35,100,70,70), Find_OCF(OCF_Alive), Find_Not(Find_Owner(NO_OWNER)));
  }

protected func FxIntOMGTimer()
  {
  // Runterfallcheck
  if (FindObject2(aOMGCheck))
    {
    Sound("TreeDown2");
    //SetAction("Off");
    SetCategory(2);
    return(-1);
    }
  }

protected func Hit()
  {
  // Rumms!
  var pBrick;
  if (pBrick = FindObject2(Find_ID(_BRK), Find_InRect(-35,0,70,50)))
    {
    Split2Components(pBrick);
    }
  Sound("Crystal1");
  Sound("Crystal2");
  Sound("Crystal3");
  Explode(40, CreateObject(CNDL,0,0,-1));
  Split2Components();
  }
