/*-- Kopf --*/

#strict

local name; //Wer isn das?
local empty; //voller blut?

func Hit() {
   
  return(Sound("Splat*"));
}

func Activate(clonk)
{
 // Wenn wie eh schon dabei sind krankes Zeug zu machen:

 // Menschen: dummer Spruch
 if(LocalN("vampire",clonk) == 0)
 {
  Message("Ähm.. %s gehts dir gut? Du siehst irgendwie.. ääh krank aus.",clonk,name);
 }

 // Ghul: Snack
 if(LocalN("vampire",clonk) == 2)
 {
  Sound("Splatter");
  clonk -> Feed(100);
  RemoveObject();
 }

 // Vampir: Drink
 if(LocalN("vampire",clonk) == 1 && !empty)
 {
  clonk -> Feed(50,1);
  empty=1;
 }

 return(1);
}