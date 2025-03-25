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
## Installation
Installer les bibliothèques :
Boost.Test : 
```
sudo apt-get install libboost-test-dev
```
SDL2_Mmixer : 
```
sudo apt install libsdl2-mixer-dev
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
Exécuter
```
./src/Game
```
