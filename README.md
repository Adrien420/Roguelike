# Roguelike
## Auteurs
Adrien MILLE  
Bastien COUQUE  
Clarence PFISTER  

## Commandes Git
```
cd Roguelike
git pull # Synchroniser

git add <nom_de_fichier> # Préparer
git commit -m "Message" # Commit
git push # Envoyer
```
## Lancer le jeu
Créer un dossier de build
```
mkdir -p build
cd build
```
Compiler
```
cmake ..
make
```
Compiler en mode DEBUG
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
Exécuter
```
./src/Game
```
