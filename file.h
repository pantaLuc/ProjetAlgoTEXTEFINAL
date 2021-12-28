#ifndef FILE_H
#define FILE_H

/*
Creation de la Structure ElementFile 

*/
typedef struct ElementFile ElementFile;
struct ElementFile
{
    int nombre;         
    ElementFile *suivant;   
};

/*
  
Creation de la structure de controle File
*/
typedef struct File File;
struct File
{
    ElementFile *premier;
};


/*
 Nos Functions a utiliser pour gerer nos file
 */
File *init();                       
void enfiler(File *file, int nouveauNBR);    
int defiler(File *file);            
int estVideFile(File file);           

#endif