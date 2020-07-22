# Programmation d'un bot discord à visé pédagogique en langage C : Cbotpedago

## Analyse du besoin du projet

### État de l'art
Dans le cadre du confinement, les élèves et les professeurs se retrouvent en cours. Ils se retrouvent sur des plateformes de discussions et essaye de faire classe dans les meilleurs conditions que possible. Cependant, il est flagrant de voir à quel point il est compliqué au professeur de faire son cours. Entre les élèves en retard, les problèmes de connexions et le manque d'outils, il n'est pas rare que des enseignants perdent du temps a cause du manque d'outil numérique. Le but de se projet est donc de donner plus d'outils surtout pour les professeurs mais aussi pour l'élève pour qu'ils puissent gagner du temps par l'intermédiaire d'un outil simple d'utilisation.

### Qu'est ce que discord ?
Discord est un logiciel propriétaire gratuit de VoIP conçu initialement pour les communautés de joueurs. Il fonctionne sur les systèmes d’exploitations Windows, macOS, Linux, Android, iOS ainsi que sur les navigateurs web. L'application propose diverses fonctionnalités, améliorant l'expérience utilisateur, comme le support de la syntaxe Markdown, la recherche globale de messages, la double authentification, un correcteur orthographique, ou le support de robots sur les serveurs. Les robots sont déployés grâce à des scripts (en Python, en Java, ou d'autres langages de programmation) et sont capables d'interagir avec les autres utilisateurs du serveur. Ils peuvent être utilisés pour la modération, la surveillance, l'animation du serveur, l'affichage automatique de données... Chaque utilisateur peut développer son propre robot.

###  Qu'est ce qu'un bot ?
Un bot informatique est un agent logiciel automatique ou semi-automatique qui interagit avec des serveurs informatiques. Un bot se connecte et interagit avec le serveur comme un programme client utilisé par un humain. Le bot est un programme informatique autonome supposé intelligent, doué de personnalité, et qui habituellement, mais pas toujours, rend un service.

### A quels attente devra répondre le bot ?
Le robot informatique ou bot devra permettre de faire l'appel ainsi que d'effectuer un sondage.

### Pourquoi le réaliser en langage C est pas dans un autre langage tel que le Nodes.js, langage utilisé de base par discord ?
Le langage C est un langage de programmation impératif généraliste, de bas niveau. Inventé au début des années 1970 pour réécrire UNIX, C est devenu un des langages les plus utilisés, encore de nos jours. De nombreux langages plus modernes comme C++, C#, Java et PHP ou Javascript ont repris une syntaxe similaire au C et reprennent en partie sa logique. C offre au développeur une marge de contrôle importante sur la machine (notamment sur la gestion de la mémoire) et est de ce fait utilisé pour réaliser les « fondations » (compilateurs, interpréteurs…) de ces langages plus modernes. Il est extrêmement performant et rapide par son approchement direct au langage assembleur, language d'instructions du processeur. 

### Pourquoi utiliser la librairie libdiscord ?
libdiscord est une librairie qui permet de faire le lien entre le langage C et l'application discord. Son utilisation on permet donc de on faciliter enormement le passage entre discord et le langage C.

## Réalisation du projet

Comme précisé précédemment, on utiliseras le projet https://github.com/dxing97/libdiscord.git afin de pouvoir réaliser ce projet.

### Mise en place des outils de développement

#### Créer un serveur discord

#### Création et Installation d'un bot discord
Après s'être connecté sur le site https://discord.com/developers/applications, on clique sur "New Application", on choisit un nom pour votre application de bot et on clique sur "Create".

Sur le bandeau de gauche, allez dans la catégorie "Bot" et on clique sur Add Bot et on clique sur "Yes"

Le bot est apparu ! Il possède un nom d'utilisateur et aussi un token qui permettra de le gérer.

Maintenant il faut revenir sur "General Information" du bandeau de gauche et copiez l'identifiant du bot.

On va désormais sur https://discordapi.com/permissions.html, on met les droits "Administrator", copiez l'id de l'application dans "Client ID" et on clique sur le lien généré en dessous

Il suffit maintenant simplement de sélectionner un serveur voulu de cliquer sur "Continuer" et en suite sur "Autoriser". Valider le captcha et voilà !


#### Installation des dépendances de libdiscord
Afin d'interconnecter le bot avec notre code, on utilisera la librairie écrite en C libdiscord

Certaines dépendances sont à rajouter pour initialiser la connexion du bot :
- libwebsockets v2.4.1 minimum
- Jansson v2.7 minimum
- libcurl
- libdiscord 
<!-- - OpenSSL just for exemple-bot-hash -->

On commence par installer les prérequis cmake et build-essential
```bash
sudo apt install build-essential cmake
```
On installe ensuite les dépendances lié au paquet en verifiant tout d'abord les bonnes versions des paquets :
```bash
sudo apt show libwebsockets-dev libjansson-dev
```
Après avoir regarder que ces paquets sont a jour, on lance la commande suivante pour installer toutes les dépendances :
```bash 
sudo apt install libwebsockets-dev libjansson-dev libcurl4-openssl-dev
```
#### Installer libdiscord et les bots tests
```bash
git clone https://github.com/dxing97/libdiscord.git
cd libdiscord
mkdir build && cd build
cmake ..
make
```

Il suffit maintenant de récuperer le TOKEN du bot et de taper les commandes :
```
cd build
./example-bot-ayylmao -t $TOKEN
```

A l'aide de mon professeur, nous avons modifié le fichier CMakeList.txt présent sur le projet d'origine pour pouvoir compiler et déployer le bot plus facilement.

L'environnement de base que j'ai donc utilisé est disponible ici -> https://github.com/SebDruon/discord_bot

### Utilisation du bot

Le bot répond donc à deux commandes : "!appel" et "!sondage". Il répond aussi à "!présent", afin de faire l'appel. En effet quand il voit que le message commence par ces mots, il va traiter l'action demandé :

> jerem: !appel

> Bot for test: Usage : ![commande] [action] option <br>
!appel start -> commence l'appel <br>
!present -> permet de dire que l'on est présent <br>
!appel list -> liste les personnes présentes <br>
!appel stop -> arrête l'appel<br>

> jerem: !sondage

> Bot for test: Usage : ![commande] [action] (option) <br>
!sondage start (question) -> commence le sondage  <br>
!sondage add (choix) -> rajoute un choix au sondage <br>
!sondage choose (n°choix) -> permet de choisir une option du sondage <br>
!sondage rm (n°choix) -> rajoute un choix au sondage <br>
!sondage show -> montre l'etat du sondage <br>
!appel stop -> arrête le sondage <br>

Sans aucun autre argument ou avec des argument qu'il ne comprend pas, le bot répond en précisant ce qu'il est possible de faire.

Le bot est donc capable de répondre à plusieurs requêtes.

En effet pour démarrer un appel, on peut envoyer le message :
> jerem: !appel start

>  Bot for test: Début de l'appel

Pour dire que l'on est présent :
> jerem: !present

> Bot for test: Coucou jerem

> Bot for test: jerem est présent

Pour voir la liste des présents :

> jerem : !appel list

> Bot for test: Les présents sont : jerem

Pour terminer l'appel :
> jerem : !appel stop

> Bot for test:  Fin de l'appel

Pour commencer un sondage :
> jerem: !sondage start Question à poser ?

> Bot for test: Début du sondage

Ajouter une proposition au sondage :
> jerem: !sondage add proposition 1

> Bot for test: Choix n°1 : "proposition 1" ajouté

Supprimer une proposition :
> jerem: !sondage rm 1

> Bot for test: Le choix n°1 a ete supprimé


Choisir une proposition :
> jerem: !sondage choose 1

> Bot for test: jerem a voté 1 : "proposition 1"

Voir l'état du sondage :
> jerem: !sondage show

> Bot for test: Question à poser ?
1 : "proposition 1", 1 vote(s)
2 : "proposition 2", 0 vote(s)

Arreter le sondage

> jerem: !sondage stop

> Bot for test: Fin du sondage

## Conclusion

### Cahier des charges
Je pense donc pouvoir dire que le rendu final répond au besoin auquel ce projet répond. En effet, le bot est capable de faire l'appel ainsi que d'organiser un sondage.
### Améliorations possibles
Le projet, assez ambitieux, peut-être encore amélioré. En effet, on pourrait par exemple corriger certains bugs dépendant de la libdiscord qui est en version de test ainsi que quelques bugs aussi crée a l'issue du développement du bot. On pourrait ensuite relié le bot au compte github de chaque étudiant pour que le professeur puisse trouver le github de chaque élève facilement.

### Retour sur expériences
Je suis très satisfait du résultat malgré les améliorations encore possible. En effet, j'ai commencé ce projet avec une approche très basique et très débutante du langage C, et j'ai pu découvrir pas mal de notion autour de ce langage tel que les librairies ou encore la gestion des chaines de caractères. J'ai apprécié passé du temps sur ce projet.
