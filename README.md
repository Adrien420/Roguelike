# Roguelike

## Auteurs
Adrien MILLE  
Bastien COUQUE  
Clarence PFISTER  

## Présentation du projet

Ce jeu s’inspire du genre roguelike. L’univers de jeu est structuré en rounds successifs, où deux personnages s'affrontent en combat dans une arène. Le joueur peut choisir de jouer contre un autre joueur local ou bien affronter une intelligence artificielle.

Objectif du jeu :

Le but est de réduire les points de vie de l’adversaire à zéro en l’attaquant. Chaque round remporté rapproche le joueur de la victoire finale. La partie est gagnée par le premier joueur remportant quatre rounds.
Déroulement d’une partie

## Comment jouer ?

Au lancement du jeu, le joueur choisit entre :

- Un mode Joueur vs Joueur (en local, sur un même clavier) ;
- Un mode Joueur vs IA, où l'adversaire est contrôlé par une intelligence artificielle.

Chaque round se joue en temps réel. À la fin de chaque round, les deux joueurs se voient proposer des cartes d’amélioration aléatoires. Ces cartes permettent de renforcer les caractéristiques de leur personnage pour les prochains rounds (par exemple : vitesse, puissance d’attaque, projectiles, ...).

Commandes :

Joueur contre Joueur :

    Joueur 1 :

        Déplacement : Z (haut), S (bas), Q (gauche), D (droite)

        Attaque : Espace

    Joueur 2 :

        Déplacement : ↑ (haut), ↓ (bas), ← (gauche), → (droite)

        Attaque : M

Joueur contre IA :

    Joueur 1 uniquement :

        Déplacement : Z / Q / S / D

        Attaque : Espace

Choix des cartes d'amélioration :

Lors de la phase d'amélioration entre deux rounds :

- La navigation dans les cartes se fait avec les touches de déplacement (QD ou ←→ selon le joueur).
- La validation du choix se fait avec la même touche que pour attaquer (Espace pour le joueur 1, M pour le joueur 2).

## Installation
Cloner le dossier :
```
git clone https://github.com/Adrien420/Roguelike.git
```

Installer les bibliothèques :
SDL2 :
```
sudo apt update
sudo apt install libsdl2-dev
```

SDL2_Mmixer : 
```
sudo apt install libsdl2-mixer-dev
```

Boost.Test : 
```
sudo apt-get install libboost-test-dev
```

## Compiler & Lancer le jeu
Compilation (à la racine du dossier cloné)
```
cd Roguelike
mkdir -p build
cd build
cmake ..
make install
```

Lancer le jeu
```
./../bin/Game
```
