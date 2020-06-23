/*-- Nebel --*/

#strict

func Initialize() {
  SetAction("Fly");
  return(1);
}

Timer: //Hin und Her wechseln
  if(Local(0)==0)  return(SetComDir(COMD_Left()),SetLocal(0,1));
  if(Local(0)==1)  return(SetComDir(COMD_Right()),SetLocal(0,0));
  return(1);