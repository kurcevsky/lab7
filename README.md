<h2 align="center"><center>Sprawozdanie</center>
<br>Programowanie równoległe i rozproszone</h2>
Zacząłem od skopiowania programu z wykładu, następnie przerobieniu go na Statki/Przystan. Gotowy plik przerzuciłem na linuxa, skompilowałem i uruchomiłem.
Dodałem zmienną pasażerów i obsługę przeładowania statku(większe spalanie).

```md
mpicc main.c -o main
mpirun -np 5 main
```
Otrzymałem:
<img src="https://github.com/kurcevsky/lab7/blob/main/Zrzut%20ekranu%20(61).png?raw=true" width="500" height="300">

