# Arduino-Tap-Lock
Kod do obsługi urządzenia umożliwiającego potwierdzenie tożsamości za pomocą wystukiwania konkretnego rytmu. Stworzone urządzenie bierze pod uwagę zarówno siłę nacisku jak i rytmikę. Przygotowane zostały dwie wersje produktu - wersja “prototyp sejfu” oraz druga przenośna, wysyłająca informacje o wyszukanym rytmie do innej maszyny. Kod programów działającego na płytkach napisano używając języka C w środowisku Arduino IDE. Serwer odbierający informacje z wersji przenośnej napisano w języku Python używając środowiska Pycharm.\
Projekt zrealizowano wraz z Karolem Zarębskim, Maciejem Stefaniakiem oraz Wojciechem Mazurowskim jako część zaliczenia przedmioty OD (Ochrona Danych).

## Efekt końcowy
https://github.com/mateusz-stelmasiak/Arduino-Tap-Lock/assets/46268673/06b4e82f-1f50-4a2a-9e0e-5bfd183541b4

## Sprzęt
Do wykonania sejfu użyto:
- płytka ESP8266,
- czujnik siły nacisku FSR-406,
- serwo MG 996,
- buzzer.

Do wykonania wersji przenośnej użyto:
- płytka ESP8266,
- czujnik siły nacisku FSR-402.

## Przykład użycia
Użytkownik zarówno w wersji przenośnej jak i w wersji sejf trzykrotnie wprowadza nowe hasło.  W wersji typu sejf hasło zapisywane jest do pamięci kontrolera. Jeżeli użytkownik będzie chciał otworzyć sejf, musi w odpowiedni sposób wprowadzić hasło. Kontroler oblicza w jakim stopniu dana sekwencja odpowiada tej wprowadzonej przez użytkownika. Jeżeli stopień zgodności będzie większy lub równy 80 %, hasło uznawane jest za poprawne. W przeciwnym wypadku wprowadzone hasło jest błędne. Wprowadzenie błędnego hasła sygnalizowane jest sygnałem dźwiękowym. 

W wersji przenośnej użytkownik również trzykrotnie wpisuje hasło, którego stopień odchylenia od poprzednich wersji musi mieścić się w odpowiednim przedziale. Po wprowadzeniu hasła, jest ono zapamiętywane i wykorzystywane przy próbie logowania. Wersja przenośna komunikuje się z serwer za pomocą protokołu TCP/IP, do którego przesyłane są sekwencje. Jeżeli wprowadzone hasło jest poprawne, program pomyśle udziela autoryzacji na wykonywanie dalszych czynności.


