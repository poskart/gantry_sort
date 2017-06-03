Piotr Poskart, 244117

Treść problemu:

Kazdy egzemplarz pewnego urzadzenia sklada sie z k róznych czesci. Sa one zgromadzone, jako nieuporzadkowana sekwencja, w dlugim magazynie. Uporzadkowac je tak, aby utworzyly
k - elementowe, wewnetrznie uporzadkowane komplety, potrzebne do zbudowania kolejnych egzemplarzy urzadzenia. Jesli liczby poszczególnych czesci sa nierówne, czesci nadmiarowe
moga pozostac nieuporzadkowane. Czesci sa przenoszone za pomoca suwnicy, zawsze grupami po k sasiadujacych ze soba czesci, z dowolnego miejsca w sekwencji na jej koniec, bez
zmiany ich uporzadkowania, a nastepnie koncowa czesc sekwencji jest dosuwana do przodu, aby wypelnic powstala luke. Ulozyc plan pracy suwnicy, dyktujacy w kolejnych ruchach, 
która grupe k czesci przeniesc na koniec sekwencji. Wykonac jak najmniejsza liczbe ruchów. Porównac czas obliczen i wyniki róznych metod.

nazwa programu: AAL

parametry wykonania:



Metody rozwiązania problemu:
W programmie zaimplementowano 3 algorytmy sortowania za pomocą suwnicy. Odpowiadają im klasy: Sorter1, Sorter2, SystematicFinder.
1. Pierwszy algorytm Sorter1 układa elementy od lewej strony, wyszukując aktualnie brakujący w ciągu
element y a następnie usuwając wszystkie możliwe elementy pomiędzy znalezionym y a ostatnim z ułożonych po lewej stronie - x, formując 
podciąg ...xy....

2. Drugi algorytm Sorter2 układa k kolejnych elementów po prawej stronie ciągu na pozycjach od n - 2k do n - k. Wyszukuje kolejny brakujący
element w ciągu, przenosi go do k ostatnich elementów i "iteruje cyklicznie" do takiej pozycji aby dołączył do tworzonego ciągu. Jako "cykliczne
iterowanie" rozumie się przesuwanie suwnicą k elementów na pozycjach n - k - 1 do n - 2 włącznie, na koniec kolejki, ustawiając tym samym element
n-1 na pozycji n - k - 1. W ten sposób na pozycji n - k - 1 można ustawić dowolny z ostatnich k+1 elementów i dołączyć go do już ułożonej sekwencji.
Po ułożeniu kompletu jest on przesuwany maksymalnie do lewej strony.

3. Trzeci algorytm to algorytm przeszukiwania systematycznego. Algorytm ten dla n elementów, o k częściach w zestawie tworzy
drzewo wszyskich możliwych sekwencji ruchów suwnicy. Pierwszy poziom drzewa zawiera n - k możliwych wektórów części, każdy po przesunięciu
suwnicą elementów z pod indeksu od 0 do n - k - 1. Drugi poziom drzewa dla każdego wektora z pierwszego poziomu wykonuje tą samą ilość (n - k)
możliwych przesunięć. W efekcie powstaje (n-k)^d możliwych wektorów ułożeń elementów, które mogą być przeszukiwane w poszukiwaniu
wektora z ułożonymi kompletami części. d jest głębokością utworzonego drzewa.
Algorytm ten ma katastrofalną, potęgową złożoność czasową i nie powinien być używany. Rozsądny czas obliczeń uzyskuje się dla n i k < 10.