/*-- Easteregg --*/

#strict

func Activate()
{
 Message("<c 00ff00>Du hast Twister Easteregg gefunden aber.. warte mal.. WTF! Wie bist du hier her gekommen?! Du hackst doch!</c>");
 Schedule("GameOver()", 5);
 return (1);
}