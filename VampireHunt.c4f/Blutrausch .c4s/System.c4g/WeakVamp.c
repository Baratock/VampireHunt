#strict 2

#appendto MEDC

//alt 1. Entf. 2. 160% 3. 130% 4. 110% 5. 90% 6. 80% 7. 75% 8. 70% 9. 65% 10. 60% >11. 55%
//neu 1. Entf. 2. 165% 3. 154% 4. 143% 5. 132%6. 121%7. 110%8. 99% 9. 88% 10. 77% >11. 65%
private func CalcPlayerValue(int iValue)
{
    //spielerzahl
    var x = 0;
    x = 176 - 11*(iPlayers-1);
    if(iPlayers > 11)
        x = 65;
    //nun zurückgeben
    return x;
}
