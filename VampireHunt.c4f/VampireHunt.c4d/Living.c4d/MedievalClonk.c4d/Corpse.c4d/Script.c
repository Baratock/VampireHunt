/*-- Körper --*/

#strict

local name; //Wer isn das?

func Hit() {
  return(Sound("Splat*"));
}

func Head()
{
 SetAction("Headless");
 Sound("Splatter");
 var head = CreateObject(CP_1,-6,0,-1);
 LocalN("name",head) = name;
}