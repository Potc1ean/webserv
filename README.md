/* CHEMAT DE TRAVAIL :
*   -creer un endpoint (point unique de communication reseau) via socket()
*    On a besion d'un point d'entrer dans le systeme pour communiquer via TCP/IP
*    socket() dit au noyau de gerer cette communication et en retour a un fd pour envoyer nos infos
*   -On a un lien avec le noyau mais il faut indiquer une add IP et un port pour que le serveur
*    attendent les clients dessus autrement il ne sait pas ou le faire (il faut bind())
*    L'addresse IP sert a dire le type de reseau qu'on utilise. Les port t'en as un paquet,
*    ca permet a l'ordi de gerer plusieur applie qui communique. Du coup en gros le port 
*    sert a la michine pour savoir a qu'elle programme il parle.
*   -Actuellement notre socket peut envoyer des donnees mais il ne peut pas accepter de client.
*    il est dit passif. Avec listen() il passe en mode ecoute de demande de connexion au serv
*    Le noyau cree un fichier pour stocker les connections en attentent (backlog)
*   -Maintenant il faut accepter les clients en attente donc on utilise accept()
*    accept() cree un socket client.
*   -recv() et send() sert a recevoir et envoyer des infos avec un client via son socket
*   -close sert a fermer un socket client.
*/

/* SOCKET D'ECOUTE :
*   le socket d'ecoute se creer avec socket(), bind() et listen().
*   son but et de faire une liste d'attente pour les clients.
*   il n'evolue pas c'est le meme durant toute la vie du serveur.
*/

/* SOCKET CLIENT :
*   Le socket client se cree direct apres l'appelle d'accept()
*   il permet la communication avec le client liee au socket
*   Le serv peut avoir plusieur de ces sockets et il les utilise pour ecrire/lire avec le client
*/