Voici une analyse structurée des bottlenecks majeurs possibles et de la documentation essentielle à lire avant de commencer, basée sur les exigences du projet.


1. Bottlenecks Majeurs Potentiels
A. Gestion des I/O Non-Bloquantes


Problème :

Le serveur doit être non-bloquant et utiliser un seul poll() (ou équivalent) pour toutes les opérations d’I/O (lecture/écriture, y compris le listen).
Une mauvaise gestion des file descriptors (FD) peut entraîner :

Des blocages si read/write sont appelés sans vérifier l’état du FD via poll().
Une consommation excessive de ressources si poll() n’est pas utilisé correctement.


Risque :

Le serveur peut devenir lent ou instable sous charge élevée.





Solutions :

Toujours vérifier l’état des FD avec poll()/epoll()/kqueue avant toute opération read/write.
Gérer les événements de manière asynchrone et éviter les boucles bloquantes.




B. Gestion de la Configuration


Problème :

Le fichier de configuration doit permettre de :

Définir plusieurs ports/serveurs virtuels.
Configurer des routes, méthodes HTTP, CGI, etc.


Une mauvaise implémentation peut rendre le serveur inflexible ou instable.



Solutions :

Parser le fichier de configuration avant de démarrer le serveur.
Valider les paramètres (ports valides, chemins existants, etc.).
Prévoir des valeurs par défaut pour les champs manquants.




C. Gestion des Requêtes HTTP


Problème :

Le serveur doit supporter au moins GET, POST, DELETE.
Les requêtes doivent être correctement parsées (en-têtes, corps, méthodes).
Les réponses doivent inclure des codes d’erreur précis et des pages d’erreur par défaut.
Les requêtes malformées ou trop grandes doivent être rejetées.



Solutions :

Implémenter un parseur HTTP robuste (respecter le RFC 7230/7231 pour HTTP/1.1).
Limiter la taille des requêtes (client_max_body_size).
Gérer les timeouts pour éviter les connexions pendantes.




D. Gestion des CGI


Problème :

Les CGI doivent être exécutés dans le bon répertoire, avec les bons arguments.
La communication avec le CGI doit être gérée (stdin/stdout).
Les requêtes "chunked" doivent être déchunkées avant d’être passées au CGI.



Solutions :

Utiliser fork() + execve() pour lancer le CGI.
Gérer les entrées/sorties du CGI via des pipes.
Respecter les conventions (ex: PATH_INFO, EOF pour la fin du corps).




E. Gestion de la Concurrence


Problème :

Le serveur doit gérer plusieurs clients simultanément sans bloquer.
Une mauvaise gestion des FD peut entraîner des fuites de ressources.



Solutions :

Utiliser poll()/epoll() pour surveiller tous les FD (y compris ceux des CGI).
Fermer proprement les FD inutilisés.
Éviter les fuites mémoire (surtout avec les buffers de requêtes/réponses).




F. Compatibilité avec les Navigateurs


Problème :

Le serveur doit être compatible avec des navigateurs standards (Chrome, Firefox, etc.).
Les en-têtes HTTP doivent être correctement formatés.



Solutions :

Tester avec plusieurs navigateurs et outils (curl, telnet).
Comparer les réponses avec NGINX pour valider les en-têtes et comportements.




G. Gestion des Erreurs


Problème :

Le serveur ne doit jamais crasher (même en cas d’erreur mémoire ou de requête invalide).
Les erreurs doivent être loguées et renvoyées avec des pages d’erreur personnalisées.



Solutions :

Utiliser des blocs try/catch en C++ pour capturer les exceptions.
Vérifier les codes d’erreur système (errno) après les appels système (sauf pour read/write).
Prévoir des pages d’erreur par défaut (404, 500, etc.).




H. Stress Testing


Problème :

Le serveur doit rester disponible sous charge élevée.



Solutions :

Utiliser des outils comme ab (Apache Benchmark), wrk, ou des scripts Python pour simuler des requêtes concurrentes.
Vérifier l’absence de fuites mémoire avec valgrind.





2. Documentation Essentielle à Lire
A. RFC HTTP/1.1


Pourquoi :

Le projet repose sur HTTP/1.1. Il faut comprendre :

Le format des requêtes/réponses.
Les méthodes (GET, POST, DELETE).
Les en-têtes (Host, Content-Length, Transfer-Encoding, etc.).
La gestion des connexions persistantes.





RFC à lire :

RFC 7230 : Syntax and Routing
RFC 7231 : Semantics and Content
RFC 7232 : Conditional Requests
RFC 7233 : Range Requests
RFC 7235 : Authentication



Focus :

Format des messages HTTP.
Gestion des méthodes et codes de statut.
En-têtes obligatoires (Host, Content-Length).




B. Documentation sur poll()/epoll()/kqueue


Pourquoi :

Le serveur doit utiliser un mécanisme de multiplexage d’I/O non-bloquant.



Ressources :

Man pages : man 2 poll, man 7 epoll, man 2 kqueue.
Tutoriels sur la programmation asynchrone en C/C++.




C. Documentation sur les Sockets


Pourquoi :

Le serveur utilise des sockets pour communiquer avec les clients.



Ressources :

Man pages : man 2 socket, man 2 bind, man 2 listen, man 2 accept, man 2 recv, man 2 send.
Tutoriels sur la programmation réseau en C/C++.




D. Documentation sur les CGI


Pourquoi :

Le serveur doit interagir avec des CGI (ex: PHP, Python).



Ressources :

RFC 3875 : CGI Version 1.1
Documentation sur execve(), fork(), et les pipes.




E. Configuration NGINX


Pourquoi :

Le sujet suggère de s’inspirer de la section server de NGINX.



Ressources :

Documentation officielle NGINX
Exemples de fichiers de configuration NGINX.




F. Gestion des Fichiers en C++


Pourquoi :

Le serveur doit lire/écrire des fichiers (pages statiques, uploads).



Ressources :

Man pages : man 2 open, man 2 read, man 2 write, man 3 opendir.
Bonnes pratiques pour éviter les fuites de FD.





3. Étapes Recommandées avant de Coder

Lire les RFC HTTP/1.1 (surtout 7230 et 7231).
Tester manuellement avec telnet :

Se connecter à un serveur HTTP (ex: telnet example.com 80).
Envoyer une requête GET et observer la réponse.


Configurer NGINX :

Créer un serveur NGINX de test et comparer ses réponses avec celles de ton serveur.


Écrire un parseur de configuration :

Valider les ports, chemins, et options.


Implémenter un serveur minimal :

Gérer une seule route et une seule méthode (ex: GET).
Ajouter progressivement les autres fonctionnalités.


Tester avec des outils :

curl, telnet, navigateurs, et scripts de stress.





4. Outils Utiles

Debugging : gdb, valgrind, strace.
Testing : curl, telnet, ab, wrk, scripts Python.
Validation : Comparer les réponses avec NGINX.
