//
// Created by dxing97 on 11/11/17.
//
#include <libdiscord/libdiscord.h>

/*
 * This bot will respond to every "ayy" with "lmao", or any other trigger-response pair you want
 */

#include <getopt.h>
#include <signal.h>
#include <libdiscord/REST.h>

static int bot_exit = 0; //0: no exit, 1: exit
static int bot_state = 0; //0: not connected/disconnected, 1: connect initiated
static int fail_mode = 1; //0: default, try recovering, 1: exit on error
CURL *handle;
char *trigger = "ayy", *response = "lmao";

void int_handler(int i) {
    bot_exit = 1;
}   

//---APPEL-------------------------
char liste_appel [255] [33]; //un pseudo ne peut avoir + de 32 caractères
int taille_liste_appel = 0;
int appel = 0; //boolean
//---------------------------------

//---SONDAGE-----------------------
int nb_choix;
char liste_choix [10][100]; //tableau de possibilités
char question [100];
int nb_participant;
char liste_participants [255] [33]; //liste des participants
int choix_participants [255]; //choix de chaque participants ; correspondance avec liste_participants
int vote_choix [10];
int sondage = 0;//boolean
//---------------------------------

/*
 * main way of user interaction with libdiscord
 * the user callback returns 0 if everything is OK
 *
 * ld_context contains info about the bot. User code shouldn't have to mess with it.
 * ld_callback_reason is the reason for the library calling the callback. See the enum declaration in libdiscord.h
 * data and len contain data that may be needed for the callback, their use depends on the reason for the callback.
 */
void substring(char s[], char sub[], int p, int l) {
       int c = 0;
          
          while (c < l) {
                    sub[c] = s[p+c-1];
                          c++;
                             }
             sub[c] = '\0';
}

int callback(struct ld_context *context, enum ld_callback_reason reason, void *data, int len) {
    
    struct ld_json_message message;
    struct ld_json_user user;
    
    char message_out[512] ;

    /*
    if(reason == LD_CALLBACK_TYPING_START ) {
        
        ld_json_message_init(&message);
        ld_json_pack_message(&message, (json_t *) data);
       
        sprintf( message_out, " mentions %p\n reactions %p", message.mentions, message.reactions ) ;

        ld_send_basic_message(context, message.channel_id, message_out );


        return 0 ;
    }
    */


    if (reason == LD_CALLBACK_MESSAGE_CREATE) {

        ld_json_message_init(&message);
        ld_json_pack_message(&message, (json_t *) data);

        if(message.author->id == context->current_user->id) {
            return 0;
        }

        if(message.author->bot == 1) {
            return 0;
        }

        if(message.channel_id == 0) {
            return 0; //realistically speaking, the channel ID will never be 0 (but you never know...)
        }
        
         

        //---POUR L'APPEL------------------------------------------------------------------------------------------------

        //Réponds Coucou suivi du pseudo à la personne présente
        if(strncasecmp(message.content, "!present", strlen("!présent")) == 0) {
            sprintf( message_out, "Coucou %s", message.author->username ) ;
            ld_send_basic_message(context, message.channel_id, message_out);

            if (appel) {
                //liste_appel [taille_liste_appel] = message.author->username;
                //strcpy(liste_appel [taille_liste_appel], message.author->username);
                
                //On verifie qu'il ne soit pas deja present
                int i;
                for (i = 0; i < taille_liste_appel ; i++){
                    if(strncasecmp(message.author->username, liste_appel[i], 32) == 0) {  
                        sprintf( message_out, "%s est déjà présent sur la liste d'appel", message.author->username ) ;
                        ld_send_basic_message(context, message.channel_id, message_out);
                        return 0;
                    }
                } 


                sprintf(liste_appel [taille_liste_appel],"%s", message.author->username);
                taille_liste_appel += 1;

                sprintf( message_out, "%s est présent", message.author->username ) ;
                ld_send_basic_message(context, message.channel_id, message_out);

            }

            return 0;
        }

        //Commence à faire l'appel si la personne à le role professeur : pas implémenté
        if(strncasecmp(message.content, "!appel", strlen("!appel")) == 0) {  
            if(strncasecmp(message.content, "!appel start", strlen("!appel start")) == 0) {  
                //if (prof) {
                    if (!appel) {
                        sprintf( message_out, "Début de l'appel");
                        ld_send_basic_message(context, message.channel_id, message_out);
                        appel = 1;
                        taille_liste_appel = 0;
                    } else {
                        sprintf( message_out, "Un appel est déja en cours" ) ;
                        ld_send_basic_message(context, message.channel_id, message_out);
                    }
                //} else {
                //  sprintf( message_out, "Action non autorisé", message.author->username ) ;
                //  ld_send_basic_message(context, message.channel_id, message_out);
                //}
                
                return 0;
            }

            if(strncasecmp(message.content, "!appel stop", strlen("!appel stop")) == 0) {
                if (appel == 1){
                    appel = 0;
                    sprintf( message_out, "Fin de l'appel") ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                } else {
                    sprintf( message_out, "Pas d'appel en cours") ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
            }  
            
            if(strncasecmp(message.content, "!appel list", strlen("!appel list")) == 0) {
                int i;
                char str_liste_appel [200] = "\0";
                char str_tmp [512] = "\0" ;
                
                if (appel == 0) { //Pas d'appel en cours
                    sprintf( message_out, "Pas d'appel en cours") ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                    return 0;
                }

                if (taille_liste_appel == 0){
                    sprintf( message_out, "Personne n'est présent :cry:" ) ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                    return 0;
                }

                for (i = 0; i < taille_liste_appel; i++){
                    //printf("\033[1;31m %s %s \n \033[0m ",str_liste_appel,liste_appel[i]);
                    printf("\033[1;31m");
                    printf("%d -> %s \n",i,liste_appel[i]);
                    sprintf(str_tmp,"%s%s",str_liste_appel,liste_appel[i]);
                    sprintf(str_liste_appel,"%s",str_tmp);
                    if (i < taille_liste_appel-1){
                        sprintf(str_tmp,"%s, ",str_liste_appel);
                        sprintf(str_liste_appel,"%s",str_tmp);
                    }
                    printf("%s \n",str_liste_appel);
                    printf("\033[0m");
                }
                sprintf( message_out, "Les présents sont : %s", str_liste_appel ) ;
                ld_send_basic_message(context, message.channel_id, message_out);
                return 0;
            } 
                sprintf( message_out, "Usage : ![commande] [action] option\n\
!appel start -> commence l'appel \n\
!present -> permet de dire que l'on est présent \n\
!appel list -> liste les personnes présentes \n\
!appel stop -> arrête l'appel") ;
                ld_send_basic_message(context, message.channel_id, message_out);
            }

        //----------------------------------------------------------------------------------------------------------------
        //---POUR LE SONDAGE-------------------------------------------------------------------------------------------------------------
        if(strncasecmp(message.content, "!sondage", strlen("!sondage")) == 0) {
            if(strncasecmp(message.content, "!sondage start", strlen("!sondage start")) == 0) {
                if (!sondage) {
                    sprintf( message_out, "Début du sondage");
                    ld_send_basic_message(context, message.channel_id, message_out);
                    sondage = 1;
                    nb_choix = 0;
                    nb_participant = 0;
                    /*if(strncasecmp(message.content, "!sondage start ", strlen("!sondage start ")) == 0) {
                        int i=0;
                        while (i<strlen(message.content)-strlen("!sondage start")){
                            question[i] = message.content[i+strlen("!sondage start")];
                            i++;
                        }                        
                        question[i] = "\0";
                    } else {
                        question[0] = "\0";
                    }*/
                    substring(message.content,question,strlen("!sondage start")+2,strlen(message.content)-strlen("!sondage start")-1);
                    
                } else {
                    sprintf( message_out, "Un sondage est déja en cours" ) ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
            }
            if(strncasecmp(message.content, "!sondage add", strlen("!sondage add")) == 0) {
                if (sondage){
                    if (nb_choix < 10 ){
                        substring(message.content,liste_choix[nb_choix],strlen("!sondage add")+2,strlen(message.content)-strlen("!sondage add")-1);
                        vote_choix[nb_choix] = 0;
                        sprintf( message_out, "Choix n°%d : \"%s\" ajouté",nb_choix+1,liste_choix[nb_choix] ) ;
                        ld_send_basic_message(context, message.channel_id, message_out);
                        nb_choix++;
                    } else {
                        sprintf( message_out, "Pas plus de 10 choix possibles" ) ;
                        ld_send_basic_message(context, message.channel_id, message_out);
                    }
                } else {
                    sprintf( message_out, "Pas de sondage en cours" ) ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
            }
            if(strncasecmp(message.content, "!sondage choose", strlen("!sondage choose")) == 0) {
/* 9l to del
int nb_choix;
char liste_choix [10][100]; //tableau de possibilités
char question [100];
int nb_participant;
char liste_participants [255] [33]; //liste des participants
int choix_participants [255]; //choix de chaque participants ; correspondance avec liste_participants
int vote_choix [10];
int sondage = 0;//boolean
*/
                if (sondage){
                    char tmp [2];
                    substring(message.content,tmp,strlen("!sondage choose")+2,strlen(message.content)-strlen("!sondage choose")-1);
                    //Voir table ASCII (add and sub str with int)
                    if ((strlen(tmp) < 2)&& ( "0" <= tmp[0] <= nb_choix+"0" )) { //Si tmp est un nombre entre 0 et nb_choix
                        int i;
                        for (i = 0; i<nb_participant; i++){
                            printf("%s ; %s\n",message.author->username,liste_participants[i]);
                            if (message.author->username == liste_participants [i]){
                                //Si a participé
                                if ( tmp[0] == "0" ) {
                                    //Annulation du vote
                                    vote_choix[choix_participants[i]]--;
                                    int j;
                                    for (j = i; j<nb_participant;j++){
                                        sprintf(liste_participants[j],"%s",liste_participants[j+1]);
                                        sprintf(liste_participants[j],"%s",liste_participants[j+1]);
                                        choix_participants[j]=choix_participants[j+1];
                                    }
                                    nb_participant --;
                                    sprintf( message_out, "Votre vote a été annulé") ;
                                } else {
                                    //Changement du vote
                                    vote_choix[choix_participants[i]]--;
                                    choix_participants[i] = tmp - 49;
                                    vote_choix[choix_participants[i]]++;
                                    sprintf( message_out, "Votre vote a été changé") ;
                                }
                                ld_send_basic_message(context, message.channel_id, message_out);
                                return 0;
                            } 
                        }
                        //Si n'a pas participé
                        if ( tmp[0] == "0" ) {
                            sprintf( message_out, "Vous n'avez pas encore voté") ;
                            ld_send_basic_message(context, message.channel_id, message_out);
                        } else {
                            sprintf(liste_participants[nb_participant],"%s", message.author->username);
                            choix_participants[nb_participant] = tmp[0] - 49;
                            vote_choix[tmp[0]-49]++;
                            sprintf( message_out, "%s a voté %d : \"%s\"", message.author->username, choix_participants[nb_participant]+1,liste_choix[choix_participants[nb_participant]]) ;
                            nb_participant++;
                            ld_send_basic_message(context, message.channel_id, message_out);
                        }
                    } else {
                        sprintf( message_out, "Veuillez choisir un nombre entre 0 et %d", nb_choix ) ;
                        ld_send_basic_message(context, message.channel_id, message_out);
                    }
                } else {
                    sprintf( message_out, "Pas de sondage en cours" ) ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
            }
            if(strncasecmp(message.content, "!sondage rm", strlen("!sondage rm")) == 0) {
                if (sondage){
                    if (nb_choix) {
                        char tmp [2];
                        substring(message.content,tmp,strlen("!sondage rm")+2,strlen(message.content)-strlen("!sondage rm")-1);
                        //Voir table ASCII (add and sub str with int)
                        if ((strlen(tmp) < 2)&& ( "1" <= tmp[0] <= nb_choix+"0" )) { //Si tmp est un nombre entre 0 et nb_choix
                            int choix_rm = tmp[0] - 49;
                            int i;
                            for (i=choix_rm ; i<nb_choix ; i++){
                                sprintf(liste_choix[i],"%s",liste_choix[i+1]);
                                vote_choix[i]=vote_choix[i+1];
                            }
                            nb_choix --;
                        } else {
                            sprintf( message_out, "Veuillez choisir un nombre entre 1 et %d", nb_choix ) ;
                            ld_send_basic_message(context, message.channel_id, message_out);
                        }
                    } else { 
                        sprintf( message_out, "Aucun choix a supprimer" ) ;
                        ld_send_basic_message(context, message.channel_id, message_out);
                    }
                } else {
                    sprintf( message_out, "Pas de sondage en cours" ) ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
            }
            if(strncasecmp(message.content, "!sondage show", strlen("!sondage show")) == 0) {
                if (sondage){
                    sprintf( message_out, "%s",question ) ;

                    char tmp [512];
                    sprintf(tmp,"%s",message_out);
                    
                    int i;
                    for (i = 0; i < nb_choix ; i ++){
                        sprintf(message_out,"%s\n%d : \"%s\", %d vote(s)", tmp, i+1, liste_choix[i], vote_choix[i]);
                        sprintf(tmp,"%s",message_out);
                    }
                    

                    ld_send_basic_message(context, message.channel_id, message_out);
                } else {
                    sprintf( message_out, "Pas de sondage en cours" ) ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
            } 
            if(strncasecmp(message.content, "!sondage stop", strlen("!sondage stop")) == 0) {
                if (sondage){
                    sondage = 0;
                    sprintf( message_out, "Fin du sondage") ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                } else {
                    sprintf( message_out, "Pas de sondage en cours") ;
                    ld_send_basic_message(context, message.channel_id, message_out);
                }
                return 0;
                return 0;
            }
            sprintf( message_out, "Usage : ![commande] [action] (option)\n\
!sondage start (question) -> commence le sondage \n\
!sondage add (choix) -> rajoute un choix au sondage \n\
!sondage choose (n°choix) -> permet de choisir une option du sondage \n\
!sondage rm (n°choix) -> rajoute un choix au sondage \n\
!sondage show -> montre l'etat du sondage \n\
!appel stop -> arrête le sondage") ;
            ld_send_basic_message(context, message.channel_id, message_out);
            
            return 0;
        }
        //----------------------------------------------------------------------------------------------------------------

    }

    return 0 ;
}

int main(int argc, char *argv[]) {

    /*
     * pass in arguments for parameters that we want to connect to discord with.
     *  this includes the bot token, debug verbosity, and so on
     *  use getopt
     * initialize a connection to the gateway
     *
     * listen for messages sent with "ayy" (with permutations)
     * if an "ayy" is detected, POST a message to the same channel with content "lmao"
     * continue ad infinitum (or until the bot is killed)
     */
    int c; // bot_exit: 0 for don't exit, 1 for exit
    char *bot_token = NULL;
    char *game = NULL;
    unsigned long log_level = 31;
    if(argc == 1) {
        goto HELP;
    }

    while(1) {
        //options: help, bot token
        //if bot token isn't specified, exit
        static struct option long_options[] = {
                {"bot-token",      required_argument, 0, 't'},
                {"help",           no_argument,       0, 'h'},
                {"log-level",      required_argument, 0, 'l'},
//                {"use-ulfius", no_argument, 0, 'u'},
                {"game",           required_argument, 0, 'g'},
                {"trigger",        required_argument, 0, 'r'},
                {"response",       required_argument, 0, 'R'},
                {"abort-on-error", no_argument,       0, 'a'},
                {0, 0,                                0, 0}
        };

        int option_index = 0;
        c = getopt_long(argc, argv, "ht:l:"
                                    //                                    "u"
                                    "g:r:R:a", long_options, &option_index);

        if(c == -1) {
            break;
        }

        switch(c) {
            case 'h':
            HELP:
                printf("libdiscord example bot: ayylmao - basic call and response bot\n"
                       "%s [-t bot_token]\n\n"
                       "Options: \n\t"
                       "-t, --bot-token [bot_token]\n\t\t"
                       "Required. Discord bot token. See Discord developer pages on how to obtain one.\n\t"
                       //                               "-u, --use-ulfius\n\t\t"
                       //                               "If set, uses ulfius to send messages instead of libcurl. \n\t\t"
                       //                               "Default is to use libcurl\n\t"
                       "-g, --game\n\t\t"
                       "Sets the initial value of the \"game\" field in the bot presence.\n\t"
                       "-r, --trigger [trigger_string]\n\t\t"
                       "Sets string that will trigger a response from the bot. Default is \"ayy\".\n\t"
                       "-R, --response [response_string]\n\t\t"
                       "Sets response that will be sent when the trigger is read. Default is \"lmao\".\n\t"
                       "-a --abort-on-error \n\t\t"
                       "If set, the bot will exit if the websocket connection is closed instead of trying to reconnect.\n\t"
                       "-h, --help\n\t\t"
                       "Displays this help dialog\n", argv[0]);
                return 0;
            case 't':
                bot_token = strdup(optarg);
                break;
            case 'l':
                log_level = strtoul(optarg, NULL, 10);
                break;
            case 'g':
                game = strdup(optarg);
                break;
            case 'r':
                trigger = strdup(optarg);
                break;
            case 'R':
                response = strdup(optarg);
                break;
            case 'a':
                fail_mode = 1;
            default:
                abort();
        }
    }

    printf("Example bot 1 \"ayylmao\" starting up using libdiscord v%s\n", LD_VERSION);

    if(bot_token == NULL) {
        printf("Bot token not set! See example-ayylmao -h for details.");
        return 1;
    }
    printf("Initializing libdiscord with log level %lu\n", log_level);

    ld_set_logging_level(log_level);

    signal(SIGINT, int_handler);
    ld_debug("set response to %s", response);
    //define context info, including bot token
    struct ld_context_info *info;
    info = malloc(sizeof(struct ld_context_info));
    ld_init_context_info(info);

    info->bot_token = strdup(bot_token);
    info->user_callback = callback;
    info->gateway_ringbuffer_size = 8;
    info->init_presence = NULL;

    free(bot_token);

    //initialize context with context info
    struct ld_context context;
    int retp;
    retp = ld_init_context(info, &context); //garbage in, garbage out
    if(retp != 0) {
        ld_error("error creating libdiscord context");
        return 1;
    }
    free(info);

    handle = curl_easy_init();

    int ret, i = 0;
    //while the bot is still alive
    while(!bot_exit) {
//        if(bot_state == 0) {
//            //bot isn't connected, so we should try connecting
//            ret = ld_connect(&context);
//            if(ret != 0) {
//                ld_warning("error connecting to discord (%d)", ret);
//                break;
//            }
//            bot_state = 1;
//        }
        ret = ld_service(&context, 20); //service the connection
        if(ret != LDS_OK) {
            if(fail_mode == 1) {
                ld_error("ld_service returned (%d), retrying", ret);
            } else {
                bot_exit = 1;
                ld_error("ld_service returned (%d), exiting", ret);
            }
        }
    }
    ld_info("disconnecting from discord");
    //destroy the context
    ld_cleanup_context(&context);
    curl_easy_cleanup(handle);
    return 0;
}
