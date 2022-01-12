#This make file is for 4sources : genere-mots.c genere-textes.c ac-matrice , ac-achage
$(CC)=gcc
final: genere-mots genere-texte ac-matrice ac-hashage
	$(cc) genere-mots genere-texte ac-matrice ac-hashage -o final 

genere-mots: genere-mots.h genere-mots.c
	$(cc)  genere-mots.h genere-mots.c -o genere-mots
	
genere-texte: genere-texte.h genere-texte.c
	$(CC) genere-texte.h genere-texte.c -o genere-texte

ac-hashage: hash.h hash.c file.h file.c ac-hashage.c
	$(CC)  hash.h hash.c file.h file.c ac-hashage.c -o ac-hashage

ac-matrice: fAcMatrice.h fAcMatrice.c file.h file.c ac-matrice.c
	$(CC) fAcMatrice.h fAcMatrice.h file.h file.c ac-matrice.c -o ac-matrice

clean:
	rm *.o final