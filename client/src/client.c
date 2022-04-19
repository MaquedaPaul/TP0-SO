#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* gral_logger;
	t_config* configuracion;

	/* ---------------- LOGGING ---------------- */




	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	gral_logger = iniciar_logger();

	//log_info(gral_logger,"Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	configuracion = iniciar_config();
	//char* valorConf = config_get_string_value(configuracion, "CLAVE");
	//config_has_property("cliente.config", "CLAVE");

	//log_info(gral_logger,valorConf);
	//log_info(gral_logger,"prueba2");

	//log_info(gral_logger,readline("> "));


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(configuracion, "CLAVE");
	//log_info(gral_logger,valor);
	ip = config_get_string_value(configuracion, "IP");
	//log_info(gral_logger,ip);
	puerto= config_get_string_value(configuracion, "PUERTO");
	//log_info(gral_logger,puerto);

	// Loggeamos el valor de config
	log_info(gral_logger,"El valor de la CLAVE es %s\n",valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(gral_logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, gral_logger, configuracion);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log","tp0",1,LOG_LEVEL_INFO);

	if(nuevo_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("./cliente.config");

	if(nuevo_config == NULL) {
		printf("No se pudo leer la config\n");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	//free(leido);
	log_info(logger,leido);
	int flag = 0;

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(flag != 1){
		leido = readline("> ");
		if(strcmp(leido,"") == 0){
			flag = 1;
		}
		log_info(logger,leido);

	}

	free(leido);
	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	//t_paquete* paquete = crear_paquete();
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while(strcmp(leido,"") != 0) {
				agregar_a_paquete( paquete, leido, strlen(leido)+1);
				free(leido);
				leido = readline("> ");
			}
	enviar_paquete(paquete, conexion);
	free(leido);
	eliminar_paquete(paquete);


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger != NULL){
				log_destroy(logger);

			}

	if(config != NULL){
				config_destroy(config);

				}

	liberar_conexion(conexion);
}
