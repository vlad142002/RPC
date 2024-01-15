RPC readme

Ce este RPC?
O aplicație RPC clasică va consta dintr-un client și un server. Aplicația client va executa 
prin intermediul serverului o procedură, serverul fiind cel care o execută și cel care trimite 
înapoi către client rezultatul.


Cum foloseste clientul aplicatia?

Pas1: Pentru a folosii functiile oferite de aplicatie, clientul trebuie sa se foloseasca de biblioteca dinamica so_client.h.
Pentru asta trebuie sa ruleze Makefile-ul si sa introduca in terminal comanda "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.".

Pas2: Biblioteca ii pune la dispozitie anumite functii. Pentru a le folosii, clientul trebuie sa creeze intai o conexiune cu 
serverul. Se va folosii functia de connect pentru a crea conexiunea. Functia primeste ca parametru adresa ip al serverului si
stocheaza intr-o structura informatiile despre conexiune.

Pas3: Clientul poate apela orice functie din lista de functii.

Pas4: La final trebuie apelata functia de inchidere a conexiunii.


Ce functii poate apela utilizatorul?


1. base64()
   Funcția base64 primește un șir de caractere și îl convertește într-o reprezentare Base64. Codificarea Base64 este utilizată 
pentru a transforma octeții binari în caractere ASCII, oferind astfel o modalitate eficientă de reprezentare a datelor într-un 
format ușor de gestionat.

2. unbase64()
   Funcția unbase64 primește un șir de caractere codificate în Base64 și îl decodează, returnând șirul de octeți original. 
Decodificarea Base64 este utilizată pentru a reveresa operația de codificare, transformând caracterele ASCII codificate înapoi 
în octeți binari.

3. hash_djb2()
   Funcția hash_djb2 implementează algoritmul de dispersie DJB2 (Dan Bernstein) și este utilizată pentru a calcula un hash 
unic pe baza unui șir de caractere. Algoritmii de dispersie sunt adesea utilizați pentru a asigna rapid o cheie sau un identificator 
unui set de date, facilitând astfel căutarea sau indexarea acestora.

4. ls()
   Funcția ls implementează o comandă simplă de listare a fișierelor și directoarelor într-un director specificat. Această funcție 
primește ca parametri un descriptor de socket și un șir de caractere care conține parametrii pentru comanda ls. Funcția utilizează 
fork pentru a crea un proces copil care va executa comanda ls și va redirecționa ieșirea standard și erorile standard către descriptorul 
de socket al clientului.

5. pwd()
   Funcția pwd implementează o comandă pentru a obține directorul de lucru curent al procesului. Această funcție utilizează fork pentru 
a crea un proces copil care va executa comanda sh -c pwd și va redirecționa ieșirea standard și erorile standard către descriptorul de socket 
al clientului.

6. sum()
   Funcția sum implementează o funcționalitate de adunare a două numere. Funcția primește doua float-uri separate prin virgulă și întoarce un float 
care reprezintă rezultatul adunării acestor două numere.

7. shell()
   Shell începe prin a citi linii de comandă de la utilizator și le parsează în argumente. Apoi, determină tipul comenzii (simplă, cu 
redirecționare sau de atribuire a variabilelor de mediu) și le execută în consecință, utilizând procese separate acolo unde este necesar. 
Variabilele de mediu sunt gestionate pentru a înlocui corespunzător valorile în linia de comandă. Shell-ul continuă să aștepte noi comenzi 
după fiecare execuție și se ocupă de procesarea lor în mod similar.

8. download()
   Functie pentru descarcarea fisierelor de pe server.

9. upload()
    Functie pentru incarcarea fisierelor pe server.