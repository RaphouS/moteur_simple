# Projet de Contrôle de Moteur Pas à Pas Simple

## Description
Ce projet permet de contrôler un moteur pas à pas avec gestion de fin de course, utilisant une carte Arduino/ESP32.

## Matériel Nécessaire
- Arduino UNO R4 WiFi ou ESP32
- Moteur pas à pas avec driver
- Interrupteur fin de course
- Alimentation
- Câbles de connexion

## Brochage
- **ENABLE_PIN (8)** : Activation du driver moteur
- **X_DIR (5)** : Contrôle de la direction
- **X_STEP (2)** : Contrôle des pas
- **X_ENDSTOP_PIN (9)** : Entrée fin de course
- **LED_BUILTIN** : LED intégrée pour indication d'état

## Bibliothèques Requises
- AccelStepper (v1.64 ou supérieure)
- ezButton (v1.0.6 ou supérieure)

## Fonctionnalités
- Contrôle du moteur pas à pas avec accélération
- Détection de fin de course
- Surveillance via port série (115200 bauds)
- Indication d'état par LED
- Suivi de position du moteur

## Installation
1. Installer l'extension PlatformIO dans Visual Studio Code
2. Cloner ce dépôt
3. Ouvrir le projet dans VSCode
4. Brancher votre carte Arduino/ESP32
5. Compiler et téléverser le projet

## Utilisation
Le système :
- Démarre avec les moteurs désactivés
- Déplace le moteur de 1000 pas à chaque cycle
- Utilise une accélération contrôlée
- Affiche la position via le moniteur série
- Fait clignoter la LED pendant le fonctionnement
- Gère la détection de fin de course

## Paramètres du Moteur
- Vitesse maximale : 400 pas/seconde
- Accélération : 300 pas/seconde²
- Déplacement par défaut : 1000 pas par cycle
- Délai de 5 secondes entre les mouvements

## Communication Série
Le système communique à 115200 bauds et affiche :
- Messages d'initialisation
- Position actuelle
- Confirmation d'atteinte de position

## Maintenance
- Vérifier régulièrement le serrage des connexions
- S'assurer que les fins de course fonctionnent correctement
- Maintenir les rails de guidage propres et lubrifiés

## Dépannage
1. Si le moteur ne bouge pas :
   - Vérifier l'alimentation
   - Contrôler les connexions
   - Vérifier l'état du signal ENABLE

2. Si le fin de course ne répond pas :
   - Vérifier le câblage
   - Contrôler la configuration des pins
   - Tester la continuité de l'interrupteur

## Licence
[Ajouter votre licence ici]