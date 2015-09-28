#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <3ds.h>
#include <sf2d.h>

//TODO
// Cambiar Jourdonnais por Apache Kid
#define NAME_LENGTH 30
#define NUM_JUGADORES 5
#define DEBUG_TRES_VIDAS 0
#define DEBUG_ALIADOS_Y_ENEMIGOS 0
#define DEBUG_VER_ROLES 0
/**TYPEDEFS**/

//SHERIFF,OUTLAW,DEPUTY,RENEGADE

typedef enum {
    SHERIFF, FORAJIDO, ALGUACIL, RENEGADO, NINGUNO
} rol;

typedef enum {
    FONDO_BOTTOM, FONDO_BOTTOM_0, FONDO_BOTTOM_1, FONDO_BOTTOM_2,
    FONDO_BOTTOM_3, FONDO_BOTTOM_4, DADO_BLANCO, DADO_INTERIOR,
    CANDADO, FUENTE, NUM_GRAFICOS_BOTTOM
} numGraficoBottomScreen;

typedef enum {
    FONDO_TOP, COWBOY_0, COWBOY_1, COWBOY_2, COWBOY_3, COWBOY_4,
    NOMBRES_PERSONAJES_NEGRO, NOMBRES_PERSONAJES_ROJO, NOMBRES_ROLES,
    NUMEROS, CORAZON, FLECHITA, NUM_GRAFICOS_TOP
} numGraficosTopScreen;
//COWBOY_0_WALKING, COWBOY_1_WALKING, COWBOY_2_WALKING, COWBOY_3_WALKING, COWBOY_4_WALKING,

typedef enum {
    FLECHA, DINAMITA, UNO, DOS, CERVEZA, GATLING, VACIO
} dado;

typedef enum {
    PRESENTANDO, TIRANDO, APLICANDO, VICTORIA_RENEGADO, VICTORIA_FORAJIDOS, VICTORIA_SHERIFF
} estado;

typedef enum {
    BART_CASSIDY, BLACK_JACK, CALAMITY_JANET, EL_GRINGO,
    JESSE_JONES, JOURDONNAIS, KIT_CARLSON, LUCKY_DUKE,
    PAUL_REGRET, PEDRO_RAMIREZ, ROSE_DOOLAN, SID_KETCHUM,
    SLAB_THE_KILLER, SUZY_LAFAYETTE, VULTURE_SAM, WILLY_THE_KID
} personaje;

typedef struct {
    int numJugador;
    rol rolJugador;
    char nombreJugador[NAME_LENGTH + 1];
    int numVidas;
    int numVidasMaximas;
    int numVidasTrasAplicarDados;
    int numFlechas;
    int numFlechasTrasAplicarDados;
    int listaJugadoresAliados[NUM_JUGADORES - 1];
    int numAliados;
    int listaJugadoresEnemigos[NUM_JUGADORES - 1];
    int numEnemigos;
    personaje nombrePersonaje;
} jugador;

typedef struct {
    int numJugadorActual; // numero del jugador que está tirando (0-4)
    int numTiradaActual; //0,1, 2 o 3 (o 4, según el personaje)
    dado resultadoTirada[4][5]; // (num tirada, num dado)
    int dadoGuardado[3][5]; //(num tirada, num dado) 0 indica que le repite, 1 que lo guarda.
    int numFlechasRestantes;
    estado estadoTurno;
    int posicionDados[5][2]; // guarda la pos de los dados q se mueven a la hora de elegir a quien aplicaas cada dado
    int posicionFinalDados[5][2]; // Para la IA, guarda la posición final de los dados
    int dadoSeleccionado;
    int jugadoresElegir[NUM_JUGADORES]; //El dado seleccionado puede repartirse entre todos los jugadores
    int numJugadoresElegir;
    int estadoDado[5]; //Incorrecto = 0, Correcto 1
    int objetivoDado[5]; //A quien se le aplica el dado i
    int posicionGraficoJugador[5][2]; //Vértice donde esta el vaquero
    int presentandoJugador; //De 0 a 4, indica cual es el jugador q se está presentando en este momento
    sf2d_texture *graficosBottom[NUM_GRAFICOS_BOTTOM];
    sf2d_texture *graficosTop[NUM_GRAFICOS_TOP];
    sf2d_texture *graficoCaraPersonaje;
    int numJugadorCaraCargada;
} modelo;

/**IMAGENES PANTALLA INFERIOR **/
extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_bottom_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_bottom_0_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_bottom_1_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_bottom_2_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_bottom_3_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_bottom_4_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} dice_white_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} dice_inside_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} lock_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} font_img;

/**IMAGENES DE PANTALLA SUPERIOR**/

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} fondo_top_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cowboy_0_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cowboy_1_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cowboy_2_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cowboy_3_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cowboy_4_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} nombres_personajes_negro_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} nombres_personajes_rojo_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} nombres_roles_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} numeros_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} heart_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} arrow_img;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_00;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_01;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_02;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_03;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_04;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_05;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_06;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_07;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_08;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_09;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_10;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_11;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_12;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_13;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_14;

extern const struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel;
    unsigned char pixel_data[];
} cara_15;


/**Prototipos de funciones**/

void interfazTopScreen(jugador jugadores[], modelo miModelo);

void interfazBottomScreen(jugador jugadores[], modelo miModelo);

void initPlayers(jugador jugadores[], modelo *miModelo);

void logicaDelJuego(jugador jugadores[], modelo *miModelo, u32 kDown, u32 kHeld, touchPosition touch);

void actualizarPosiblesObjetivos(jugador jugadores[], modelo *miModelo, int numDado);

void lanzarDados(jugador jugadores[], modelo *miModelo);

void finalizarTurno(jugador jugadores[], modelo *miModelo);

void indios(jugador jugadores[], modelo *miModelo);

void guardarDados(jugador jugadores[], modelo *miModelo);

void cambioTirarAplicar(jugador jugadores[], modelo *miModelo);

void aplicarDados(jugador jugadores[], modelo *miModelo);

void asignarDadosIA(jugador jugadores[], modelo *miModelo);

int primeroPorDerecha(jugador jugadores[], int posicionJugador);

int primeroPorIzquierda(jugador jugadores[], int posicionJugador);

int dadoEnRegion(int x, int y, int numJugador);

void jugadorMuerto(jugador jugadores[], modelo *miModelo, int numJugadorMuerto);

void meterEnemigo(jugador jugadores[], int numJugador, int numJugadorEnemigo);

void meterAliado(jugador jugadores[], int numJugador, int numJugadorAliado);

int esAliado(jugador jugadores[], int numJugador, int numJugadorPresuntoAliado);

int esEnemigo(jugador jugadores[], int numJugador, int numJugadorPresuntoEnemigo);

void actualizarAlianzasTrasDanar(jugador jugadores[], int numJugadorAtacante, int numJugadorAtacado);

void actualizarAlianzasTrasCurar(jugador jugadores[], int numJugadorCurandero, int numJugadorCurado);

void cargarGraficoCaraPersonaje(modelo *miModelo, int numPersonaje);

void descargarGraficoCaraPersonaje(modelo *miModelo);
/**MAIIN**/

//sf2d_texture *graficosBottom[NUM_GRAFICOS_BOTTOM];
//sf2d_texture *graficosTop[NUM_GRAFICOS_TOP]; 

int main() {
    /**VARIABLES**/
    int i;
    jugador jugadores[NUM_JUGADORES];
    modelo miModelo;

    //INITIALIZATION
    sf2d_init();
    srand(time(NULL));

    /**INICIALZACIÓN DE VARIABLES DE LA LGICA DEL JUEGO*/
    initPlayers(jugadores, &miModelo);

    // TEXTURAS QUE VAN A ESTAR SIEMPRE CARGADAS

    //	FONDO_BOTTOM, FONDO_BOTTOM_0, FONDO_BOTTOM_1, FONDO_BOTTOM_2, FONDO_BOTTOM_3, FONDO_BOTTOM_4, DADO_BLANCO, DADO_INTERIOR, CANDADO,  NUM_GRAFICOS_BOTTOM
    miModelo.graficosBottom[FONDO_BOTTOM] = sf2d_create_texture_mem_RGBA8(fondo_bottom_img.pixel_data, fondo_bottom_img.width, fondo_bottom_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[FONDO_BOTTOM_0] = sf2d_create_texture_mem_RGBA8(fondo_bottom_0_img.pixel_data, fondo_bottom_0_img.width, fondo_bottom_0_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[FONDO_BOTTOM_1] = sf2d_create_texture_mem_RGBA8(fondo_bottom_1_img.pixel_data, fondo_bottom_1_img.width, fondo_bottom_1_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[FONDO_BOTTOM_2] = sf2d_create_texture_mem_RGBA8(fondo_bottom_2_img.pixel_data, fondo_bottom_2_img.width, fondo_bottom_2_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[FONDO_BOTTOM_3] = sf2d_create_texture_mem_RGBA8(fondo_bottom_3_img.pixel_data, fondo_bottom_3_img.width, fondo_bottom_3_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[FONDO_BOTTOM_4] = sf2d_create_texture_mem_RGBA8(fondo_bottom_4_img.pixel_data, fondo_bottom_4_img.width, fondo_bottom_4_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[DADO_BLANCO] = sf2d_create_texture_mem_RGBA8(dice_white_img.pixel_data, dice_white_img.width, dice_white_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[DADO_INTERIOR] = sf2d_create_texture_mem_RGBA8(dice_inside_img.pixel_data, dice_inside_img.width, dice_inside_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[CANDADO] = sf2d_create_texture_mem_RGBA8(lock_img.pixel_data, lock_img.width, lock_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosBottom[FUENTE] = sf2d_create_texture_mem_RGBA8(font_img.pixel_data, font_img.width, font_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);

    //FONDO_TOP, COWBOY_0, COWBOY_0_WALKING, COWBOY_1, COWBOY_2, COWBOY_3, COWBOY_4, NOMBRES_PERSONAJES_NEGRO, NOMBRES_PERSONAJES_ROJO, NOMBRES_ROLES, NUMEROS, CORAZON, FLECHITA, NUM_GRAFICOS_TOP
    miModelo.graficosTop[FONDO_TOP] = sf2d_create_texture_mem_RGBA8(fondo_top_img.pixel_data, fondo_top_img.width, fondo_top_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[COWBOY_0] = sf2d_create_texture_mem_RGBA8(cowboy_0_img.pixel_data, cowboy_0_img.width, cowboy_0_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[COWBOY_1] = sf2d_create_texture_mem_RGBA8(cowboy_1_img.pixel_data, cowboy_1_img.width, cowboy_1_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[COWBOY_2] = sf2d_create_texture_mem_RGBA8(cowboy_2_img.pixel_data, cowboy_2_img.width, cowboy_2_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[COWBOY_3] = sf2d_create_texture_mem_RGBA8(cowboy_3_img.pixel_data, cowboy_3_img.width, cowboy_3_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[COWBOY_4] = sf2d_create_texture_mem_RGBA8(cowboy_4_img.pixel_data, cowboy_4_img.width, cowboy_4_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[NOMBRES_PERSONAJES_NEGRO] = sf2d_create_texture_mem_RGBA8(nombres_personajes_negro_img.pixel_data, nombres_personajes_negro_img.width, nombres_personajes_negro_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[NOMBRES_PERSONAJES_ROJO] = sf2d_create_texture_mem_RGBA8(nombres_personajes_rojo_img.pixel_data, nombres_personajes_rojo_img.width, nombres_personajes_rojo_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[NOMBRES_ROLES] = sf2d_create_texture_mem_RGBA8(nombres_roles_img.pixel_data, nombres_roles_img.width, nombres_roles_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[NUMEROS] = sf2d_create_texture_mem_RGBA8(numeros_img.pixel_data, numeros_img.width, numeros_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[CORAZON] = sf2d_create_texture_mem_RGBA8(heart_img.pixel_data, heart_img.width, heart_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    miModelo.graficosTop[FLECHITA] = sf2d_create_texture_mem_RGBA8(arrow_img.pixel_data, arrow_img.width, arrow_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
    
    u32 kHeld;
    u32 kDown;
    touchPosition touch;

    while (aptMainLoop()) {

        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidCircleRead(&circle);
        kHeld = hidKeysHeld();
        kDown = hidKeysDown();
        hidTouchRead(&touch);

        if (kDown & KEY_START) {
            break;
        }

        //Es donde se cogen los eventos y se hacen las modificaciones necesarias
        logicaDelJuego(jugadores, &miModelo, kDown, kHeld, touch);

        interfazTopScreen(jugadores, miModelo); // <- Start frame GFX_TOP

        interfazBottomScreen(jugadores, miModelo); //<- Start_frame (GFX_BOTTOm)

        // Se cambian los buffers y se espera al VBlank con la siguiente llamada
        sf2d_swapbuffers();
    }


    // Liberación de texturas
    for (i = 0; i < NUM_GRAFICOS_BOTTOM; i++) {
        sf2d_free_texture(miModelo.graficosBottom[i]);
    }
    for (i = 0; i < NUM_GRAFICOS_TOP; i++) {
        sf2d_free_texture(miModelo.graficosTop[i]);
    }
    // Finalización de las librerías

    sf2d_fini();

    return 0;
}

void interfazTopScreen(jugador jugadores[], modelo miModelo) {

    sf2d_start_frame(GFX_TOP, GFX_LEFT);
    static int reloj = 0;
    reloj++;
    reloj %= 60;

    //Pintado de las cajitas
    //int vertices[NUM_JUGADORES][2] = {{166,11},{23,25},{56,140},{241,140},{295,28}}; Para los gorditos
    //int vertices[NUM_JUGADORES][2] = {{166,11},{40,26},{70,141},{262,141},{301,29}}; //Para los flaquitos
    //int sizeSprite[NUM_JUGADORES][2]= {{63,98},{88,104},{81,106},{88,100},{80,100}}; Para los gorditos
    int sizeSprite[NUM_JUGADORES][2] = {
        {63, 98},
        {50, 103},
        {60, 96},
        {51, 96},
        {60, 98}
    }; //Para los flaquitos
    int i;
    //Pintams el fondo
    sf2d_draw_texture(miModelo.graficosTop[FONDO_TOP], 0, 0);
    for (i = 0; i < NUM_JUGADORES; i++) {
        //El monigote del personaje parado (se corresponde al primer sprite)
        sf2d_draw_texture_part(miModelo.graficosTop[COWBOY_0 + i], miModelo.posicionGraficoJugador[i][0], miModelo.posicionGraficoJugador[i][1], 0, 0, sizeSprite[i][0], sizeSprite[i][1]);

        if (miModelo.estadoTurno == APLICANDO || TIRANDO) {
            //El nombre del jugador actual parpadeará en rojo, el rsto en negro
            if (miModelo.numJugadorActual == i && (reloj % 20) < 10)
                sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_PERSONAJES_ROJO], miModelo.posicionGraficoJugador[i][0]-(122 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 74, 0, 10 * jugadores[i].nombrePersonaje, 122, 10);
            else
                sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_PERSONAJES_NEGRO], miModelo.posicionGraficoJugador[i][0]-(122 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 74, 0, 10 * jugadores[i].nombrePersonaje, 122, 10);
        } else if (miModelo.estadoTurno == PRESENTANDO) {
            //El nombre del jugador actual presentandose en rojo, el rsto en negro
            if (miModelo.presentandoJugador == i && (reloj % 20) < 10)
                sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_PERSONAJES_ROJO], miModelo.posicionGraficoJugador[i][0]-(122 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 74, 0, 10 * jugadores[i].nombrePersonaje, 122, 10);
            else
                sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_PERSONAJES_NEGRO], miModelo.posicionGraficoJugador[i][0]-(122 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 74, 0, 10 * jugadores[i].nombrePersonaje, 122, 10);
        }
        //PARA SABER COMO VA LA IA
        if (DEBUG_VER_ROLES) {
            sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_ROLES], miModelo.posicionGraficoJugador[i][0]-(66 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 86, 0, 10 * jugadores[i].rolJugador, 66, 10);
        }


        //El rol del personaje, solo pintamos el nuestro y el del sheriff y el de los que han palmao
        if (i == 0) //Jugador Humano
            sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_ROLES], miModelo.posicionGraficoJugador[i][0]-(66 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 86, 0, 10 * jugadores[i].rolJugador, 66, 10);
        if (jugadores[i].numVidas <= 0) //Muertos
            sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_ROLES], miModelo.posicionGraficoJugador[i][0]-(66 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 86, 0, 10 * jugadores[i].rolJugador, 66, 10);
        if (jugadores[i].rolJugador == SHERIFF) //Sheriff
            sf2d_draw_texture_part(miModelo.graficosTop[NOMBRES_ROLES], miModelo.posicionGraficoJugador[i][0]-(66 - sizeSprite[i][0]) / 2, miModelo.posicionGraficoJugador[i][1] + 86, 0, 10 * jugadores[i].rolJugador, 66, 10);


        sf2d_draw_texture(miModelo.graficosTop[CORAZON], miModelo.posicionGraficoJugador[i][0] + 10, miModelo.posicionGraficoJugador[i][1] - 7);
        sf2d_draw_texture(miModelo.graficosTop[FLECHITA], miModelo.posicionGraficoJugador[i][0] + 10, miModelo.posicionGraficoJugador[i][1] + 10);

        //Mostrar una animacion para ver el efecto tras aplicar los dados
        if (miModelo.estadoTurno == APLICANDO && (reloj % 60) < 30) {
            sf2d_draw_texture_part(miModelo.graficosTop[NUMEROS], miModelo.posicionGraficoJugador[i][0] + 26, miModelo.posicionGraficoJugador[i][1] - 4, 0, 10 * jugadores[i].numVidasTrasAplicarDados, 26, 10);
            sf2d_draw_texture_part(miModelo.graficosTop[NUMEROS], miModelo.posicionGraficoJugador[i][0] + 26, miModelo.posicionGraficoJugador[i][1] + 13, 0, 10 * jugadores[i].numFlechasTrasAplicarDados, 26, 10);
        }//Mostramos la vida y flechas normales
        else {
            sf2d_draw_texture_part(miModelo.graficosTop[NUMEROS], miModelo.posicionGraficoJugador[i][0] + 26, miModelo.posicionGraficoJugador[i][1] - 4, 0, 10 * jugadores[i].numVidas, 26, 10);
            sf2d_draw_texture_part(miModelo.graficosTop[NUMEROS], miModelo.posicionGraficoJugador[i][0] + 26, miModelo.posicionGraficoJugador[i][1] + 13, 0, 10 * jugadores[i].numFlechas, 26, 10);
        }
    }

    //Vamos a mostrar el número de flechas restantes
    sf2d_end_frame();
}

void interfazBottomScreen(jugador jugadores[], modelo miModelo) {

    int i, j;
    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

    //DIBUJAMOS EL FONDO

    sf2d_draw_texture(miModelo.graficosBottom[FONDO_BOTTOM], 0, 0);
    /*Nolaria escribir mensajes de lo que dirian el resto de jugadores en alguna parte de la pantalla de abajo..
    }*/

    //Supongamos que está tirando, con lo cual
    if (miModelo.estadoTurno == PRESENTANDO) {
        //Habria que mostrar la imagen del pavo abajo
        sf2d_draw_texture(miModelo.graficoCaraPersonaje, 150, 30);
    }
    if (miModelo.estadoTurno == TIRANDO) {
        for (i = 0; i < miModelo.numTiradaActual; i++) {
            for (j = 0; j < 5; j++) {
                //Pintamos los dados (40x40)
                //sf2d_draw_texture(graficos[DADO_BLANCO],miModelo.posicionDados[i][0],miModelo.posicionDados[i][1]);
                sf2d_draw_texture_part(miModelo.graficosBottom[DADO_BLANCO], 20 + 60 * j, 8 + 48 * i, 0, 0, 40, 40);
                //Pintamos el interior de los dados (32x32). Son las coord de antes + 4 en cada eje para centrar la img 
                sf2d_draw_texture_part(miModelo.graficosBottom[DADO_INTERIOR], 24 + 60 * j, 12 + 48 * i, 0, 32 * miModelo.resultadoTirada[i][j], 32, 32);

                //Vamos a indicar cuales son los dados guardados:
                if (i != 3) //Para la última tirada de lucky duke no hacemos nada
                    if (miModelo.dadoGuardado[i][j])
                        sf2d_draw_texture(miModelo.graficosBottom[CANDADO], 20 + 60 * j, 8 + 48 * i);
            }
        }
    }
    if (miModelo.estadoTurno == APLICANDO) {
        //Dibujamos el tablero coloreado para los jugadores a los que puede ir el dado q esta siendo seleccionado
        if (miModelo.dadoSeleccionado != -1) {
            int vertices[5][2] = {
                {84, 0},
                {0, 0},
                {0, 149},
                {169, 151},
                {206, 0}
            };
            for (i = 0; i < miModelo.numJugadoresElegir; i++) {
                j = miModelo.jugadoresElegir[i];
                sf2d_draw_texture(miModelo.graficosBottom[FONDO_BOTTOM_0 + j], vertices[j][0], vertices[j][1]);
            }
        }
        //Dibujamos cada uno de los da2
        for (i = 0; i < 5; i++) {
            if (i != miModelo.dadoSeleccionado) {
                sf2d_draw_texture_part(miModelo.graficosBottom[DADO_BLANCO], miModelo.posicionDados[i][0], miModelo.posicionDados[i][1], 0, 40 + 40 * miModelo.estadoDado[i], 40, 40);
                sf2d_draw_texture_part(miModelo.graficosBottom[DADO_INTERIOR], miModelo.posicionDados[i][0] + 4, miModelo.posicionDados[i][1] + 4
                        , 0, 32 * miModelo.resultadoTirada[miModelo.numTiradaActual - 1][i], 32, 32);
            }
        }
        if (miModelo.dadoSeleccionado != -1) { //si ha elegido algun dado, lo pintamos el último para que quede encima del resto
            i = miModelo.dadoSeleccionado;
            sf2d_draw_texture_part(miModelo.graficosBottom[DADO_BLANCO], miModelo.posicionDados[i][0], miModelo.posicionDados[i][1], 0, 40 + 40 * miModelo.estadoDado[i], 40, 40);
            sf2d_draw_texture_part(miModelo.graficosBottom[DADO_INTERIOR], miModelo.posicionDados[i][0] + 4, miModelo.posicionDados[i][1] + 4
                    , 0, 32 * miModelo.resultadoTirada[miModelo.numTiradaActual - 1][i], 32, 32);
        }
    }

    /*if(DEBUG_ALIADOS_Y_ENEMIGOS){ //Vamos a pintar abajo a la izquierda los nombres de los aliados, abajo centro los nombres de los enemigos,y abajo derecha los posibles objetivos para el dado seleccionado
            for(i = 0; i< jugadores[miModelo.numJugadorActual].numAliados; i++){
                    int numJugador2 = jugadores[miModelo.numJugadorActual].listaJugadoresAliados[i];
                    sf2d_draw_texture_part(graficos[NOMBRES_PERSONAJES_BOTTOM],0,230-10*i,0,8*jugadores[numJugador2].nombrePersonaje,120,8);
            }
            for(i = 0; i< jugadores[miModelo.numJugadorActual].numEnemigos; i++){
                    int numJugador2 = jugadores[miModelo.numJugadorActual].listaJugadoresEnemigos[i];
                    sf2d_draw_texture_part(graficos[NOMBRES_PERSONAJES_BOTTOM],120,230-10*i,0,8*jugadores[numJugador2].nombrePersonaje,120,8);
            }
            if(miModelo.dadoSeleccionado!=-1){
                    for(i = 0; i< miModelo.numJugadoresElegir; i++){
                            int numJugador2 = miModelo.jugadoresElegir[i];
                            sf2d_draw_texture_part(graficos[NOMBRES_PERSONAJES_BOTTOM],240,230-10*i,0,8*jugadores[numJugador2].nombrePersonaje,120,8);
                    }
            }
    }*/
    sf2d_end_frame();
}

void initPlayers(jugador jugadores[], modelo *miModelo) {
    int i, j;

    // Init null roles
    for (i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i].numJugador = i + 1;
        jugadores[i].rolJugador = NINGUNO;
        jugadores[i].numFlechas = 0;
        jugadores[i].numAliados = 0;
        jugadores[i].numEnemigos = 0;
    }

    //Randomly distribute them
    rol distrib[NUM_JUGADORES] = {FORAJIDO, FORAJIDO, ALGUACIL, RENEGADO, SHERIFF}; //Roles a distribuir

    int random;
    for (i = 0; i < NUM_JUGADORES; i++) {
        do { // We choose a player that hasn't been given a role yet
            random = rand() % 5;
        } while (jugadores[random].rolJugador != NINGUNO);
        jugadores[random].rolJugador = distrib[i];
    }

    //En random está guardado el número del jugador que es el Sheriff.
    int jugadorSheriff = random;

    //Podemos crear el primer turno aquí

    miModelo->numJugadorActual = jugadorSheriff;
    miModelo->numTiradaActual = 0;
    miModelo->numFlechasRestantes = 9;

    //Presentariamos a los personajes uno a uno, empezando por el sheriff
    miModelo->estadoTurno = PRESENTANDO;
    miModelo->presentandoJugador = jugadorSheriff;
    miModelo->numJugadorCaraCargada = -1; //De momento su grafico no está cargado, habríua que cargarlo

    //Sus posiciones iniciales son fuera de la pantalla, y luego han de ir entrando uno a uno
    miModelo->posicionGraficoJugador[0][0] = 166;
    miModelo->posicionGraficoJugador[0][1] = -98;

    miModelo->posicionGraficoJugador[1][0] = -80; //40; //<- AJUSTAR
    miModelo->posicionGraficoJugador[1][1] = -34; //26; //<-AJUSTAR

    miModelo->posicionGraficoJugador[2][0] = -60; //<- AJUSTAR
    miModelo->posicionGraficoJugador[2][1] = 205; //<-AJUSTAR

    miModelo->posicionGraficoJugador[3][0] = 462; //<- AJUSTAR
    miModelo->posicionGraficoJugador[3][1] = 240; //<-AJUSTAR

    miModelo->posicionGraficoJugador[4][0] = 460; // 301<- AJUSTAR
    miModelo->posicionGraficoJugador[4][1] = -50; //29; //<-AJUSTAR

    for (i = 0; i < 5; i++)
        for (j = 0; j < 3; j++)
            miModelo->dadoGuardado[j][i] = 0;

    //Actualizamos aliados y enemigos de los personajes:
    for (i = 0; i < NUM_JUGADORES; i++) {
        if (jugadores[i].rolJugador == FORAJIDO) {
            jugadores[i].listaJugadoresEnemigos[0] = jugadorSheriff;
            jugadores[i].numEnemigos = 1;
        }
        //Para el renegado no me queda del todo claro si ha de caer en este saco jaja
        if (jugadores[i].rolJugador == ALGUACIL || jugadores[i].rolJugador == RENEGADO) {
            jugadores[i].listaJugadoresAliados[0] = jugadorSheriff;
            jugadores[i].numAliados = 1;
        }
    }

    //Reparto de personajes
    //0 - Bart Cassidy 8        4 - Jesse Jones 9 		8 - Paul Regret 9		12 - Slab the Killer 8 
    //1 - Black Jack 8			5 - Jourdonnais 7		9 - Pedro Ramirez 8		13 - Suzy Lafayette 8
    //2 - Calamity Janet 8		6 - Kit Carlson 7		10 -  Rose Doolan 9		14 - Vulture Sam 9
    //3 - El Gringo 7			7 - Lucky Duke 8 		11 - Sid Ketchum 8	 	15 - Willy the Kid 8

    int vidas[16] = {8, 8, 8, 7, 9, 7, 7, 8, 9, 8, 9, 8, 8, 8, 9, 8};
    char nombres[16][18] = {"Bart Cassidy", "Black Jack", "Calamity Janet", "El Gringo",
        "Jesse Jones", "Jourdonnais", "Kit Carlson", "Lucky Duke",
        "Paul Regret", "Pedro Ramirez", "Rose Doolan", "Sid Ketchum",
        "Slab the Killer", "Suzy Lafayette", "Vulture Sam", "Willy the Kid"};
    char nombresRoles[5][10] = {"None", "Sheriff", "Outlaw", "Deputy", "Renegade"};
    int salir;
    for (i = 0; i < NUM_JUGADORES; i++) {
        salir = 0;
        while (!salir) {
            random = rand() % 16;
            salir = 1;
            for (j = 0; j < i; j++)
                if (jugadores[j].nombrePersonaje == random)
                    salir = 0;
        }
        jugadores[i].numVidas = vidas[random];
        jugadores[i].numVidasMaximas = vidas[random];
        if (jugadores[i].rolJugador == SHERIFF) {
            jugadores[i].numVidas += 2;
            jugadores[i].numVidasMaximas += 2;
        }
        if (DEBUG_TRES_VIDAS) {
            jugadores[i].numVidas = 3;
            jugadores[i].numVidasMaximas = 3;
        }
        jugadores[i].nombrePersonaje = random;

        if (jugadores[i].rolJugador != SHERIFF)
            if (i != 0)
                snprintf(jugadores[i].nombreJugador, NAME_LENGTH, "P%d - %s", i + 1, nombres[random]);
            else
                snprintf(jugadores[i].nombreJugador, NAME_LENGTH, "You - %s - %s", nombres[random], nombresRoles[jugadores[i].rolJugador]);
        else
            if (i != 0)
            snprintf(jugadores[i].nombreJugador, NAME_LENGTH, "P%d - %s - Sheriff", i + 1, nombres[random]);
        else
            snprintf(jugadores[i].nombreJugador, NAME_LENGTH, "You - %s - Sheriff", nombres[random]);
    }
}

void logicaDelJuego(jugador jugadores[], modelo *miModelo, u32 kDown, u32 kHeld, touchPosition touch) {
    int i, dinamitas, guardados;
    int difVertical, difHorizontal;
    static int desfaseX = 0, desfaseY = 0;

    if (miModelo->estadoTurno == PRESENTANDO) {
        // La cosa sería ir por orden metiendo los personajes en sus sitios y además en la pantalla inferior mostrar cual es su habilidad
        // despues esperar a q pulse A para mostrar el siguiente personaje		
        int vertices[NUM_JUGADORES][2] = {
            {166, 11},
            {40, 26},
            {70, 141},
            {262, 141},
            {301, 29}
        }; //El Objetivo
        //Pueden pasar dos cosas, o que el monigote al que se esté presentando esté en su posición o que no lo esté.
        //Si lo está, esperamos a que pulse A para pasar al siguiente
        if (miModelo->numJugadorCaraCargada == -1)
            cargarGraficoCaraPersonaje(miModelo, jugadores[miModelo->presentandoJugador].nombrePersonaje);

        if (miModelo->posicionGraficoJugador[miModelo->presentandoJugador][0] == vertices[miModelo->presentandoJugador][0]
                && miModelo->posicionGraficoJugador[miModelo->presentandoJugador][1] == vertices[miModelo->presentandoJugador][1]) {
            if (kDown & KEY_A) {
                (miModelo->presentandoJugador)++;
                (miModelo->presentandoJugador) %= NUM_JUGADORES;
                //Si este tmb está en su sitio, es q hemos terminado de presentar a todos
                if (miModelo->posicionGraficoJugador[miModelo->presentandoJugador][0] == vertices[miModelo->presentandoJugador][0]
                        && miModelo->posicionGraficoJugador[miModelo->presentandoJugador][1] == vertices[miModelo->presentandoJugador][1]) {
                    descargarGraficoCaraPersonaje(miModelo);
                    miModelo->estadoTurno = TIRANDO;
                } else {
                    cargarGraficoCaraPersonaje(miModelo, jugadores[miModelo->presentandoJugador].nombrePersonaje);
                }
            }
        } else {
            if (miModelo->presentandoJugador == 0) //Para el vaquero 0 solo hay que añadir uno a su posicion vertical
                miModelo->posicionGraficoJugador[0][1]++;
            if (miModelo->presentandoJugador == 1) {
                miModelo->posicionGraficoJugador[1][0]++;
                if (!(miModelo->posicionGraficoJugador[1][0] % 2) && miModelo->posicionGraficoJugador[1][1] != vertices[1][1])
                    miModelo->posicionGraficoJugador[1][1]++;
            }
            if (miModelo->presentandoJugador == 2) {
                miModelo->posicionGraficoJugador[2][0]++;
                if (!(miModelo->posicionGraficoJugador[2][0] % 2) && miModelo->posicionGraficoJugador[2][1] != vertices[2][1])
                    miModelo->posicionGraficoJugador[2][1]--;
            }
            if (miModelo->presentandoJugador == 3) {
                miModelo->posicionGraficoJugador[3][0]--;
                if (!(miModelo->posicionGraficoJugador[3][0] % 2) && miModelo->posicionGraficoJugador[3][1] != vertices[3][1])
                    miModelo->posicionGraficoJugador[3][1]--;
            }
            if (miModelo->presentandoJugador == 4) {
                miModelo->posicionGraficoJugador[4][0]--;
                if (miModelo->posicionGraficoJugador[4][0] % 2 && miModelo->posicionGraficoJugador[4][1] != vertices[4][1])
                    miModelo->posicionGraficoJugador[4][1]++;
            }
        }
    }
    if (miModelo->estadoTurno == VICTORIA_RENEGADO || miModelo->estadoTurno == VICTORIA_FORAJIDOS || miModelo->estadoTurno == VICTORIA_SHERIFF) {
        //Comprobar que pulse A, y en ese caso salir al menú.. y hacer init players
        if (kDown & KEY_B)
            initPlayers(jugadores, miModelo);
    }
    if (miModelo->estadoTurno == TIRANDO) {

        if (miModelo->numTiradaActual == 0) {
            if (kDown & KEY_A) {
                //En la primera tirada, tiras por cachabas.
                lanzarDados(jugadores, miModelo);
                //Ahora decidir como guardar los dados si se trata de la CPU
                if (miModelo->numJugadorActual != 0)
                    guardarDados(jugadores, miModelo);
                return;
            }
        }
        if (miModelo->numTiradaActual == 1) {
            if (kDown & KEY_A) {
                //Comprobamos si le han salido dinamitas, con lo cual terminaría su turno y si está muerto mas aun xD
                dinamitas = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->resultadoTirada[0][i] == DINAMITA)
                        dinamitas++;
                }
                if (dinamitas >= 3) {
                    jugadores[miModelo->numJugadorActual].numVidas--;
                    if (jugadores[miModelo->numJugadorActual].numVidas > 0) {
                        cambioTirarAplicar(jugadores, miModelo); //Se acaba su turno. Si no tiene vidas, mas abajo se le declara muerto y finalizamos su turno
                        return; // Ya no se hace nada mas
                    }
                }
                //Si ha guardado todos los dados, o ha perdido todas las vidas a causa de los indios, se acaba el turno.
                if (jugadores[miModelo->numJugadorActual].numVidas <= 0) { //Puede haber perdido las vidas a causa de las dinamitas o de las flechas
                    jugadorMuerto(jugadores, miModelo, miModelo->numJugadorActual);
                    finalizarTurno(jugadores, miModelo);
                    return;
                }
                guardados = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->dadoGuardado[0][i])
                        guardados++;
                }
                //Ha decidido guardar todos los dados, i.e, plantarse
                if (guardados == 5) {
                    cambioTirarAplicar(jugadores, miModelo);
                    return;
                }
                //Si llega hasta aquí es que sigue vivo y no se ha plantado
                lanzarDados(jugadores, miModelo);
                //Ahora decidir como guardar los dados si se trata de la CPU
                if (miModelo->numJugadorActual != 0)
                    guardarDados(jugadores, miModelo);
                return;
            }

            // Si es el turno del jugador, puede darle a guardar los dados.
            if (kDown & KEY_TOUCH && miModelo->numJugadorActual == 0) { //Puede que haya pulsado un dado, en cuyo caso se comprueba si no es una dinamita
                // y en ese caso se intercambia el valor de dado guardado
                for (i = 0; i < 5; i++) { // base 20+60*i,8+48*(numTirada actual -1)
                    if (touch.px >= 20 + 60 * i && touch.px <= 60 + 60 * i && touch.py >= 8 && touch.py <= 48)
                        if (miModelo->resultadoTirada[0][i] != DINAMITA) {
                            if (miModelo->dadoGuardado[0][i] == 0) {
                                miModelo->dadoGuardado[0][i] = 1;
                                miModelo->dadoGuardado[1][i] = 1;
                                miModelo->dadoGuardado[2][i] = 1;
                            } else {
                                miModelo->dadoGuardado[0][i] = 0;
                                miModelo->dadoGuardado[1][i] = 0;
                                miModelo->dadoGuardado[2][i] = 0;
                            }
                        }
                }
            }
        }

        if (miModelo->numTiradaActual == 2) {
            if (kDown & KEY_A) {
                //Comprobamos si le han salido dinamitas, con lo cual terminaría su turno y si está muerto mas aun xD
                dinamitas = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->resultadoTirada[1][i] == DINAMITA)
                        dinamitas++;
                }
                if (dinamitas > 2) {
                    jugadores[miModelo->numJugadorActual].numVidas--;
                    if (jugadores[miModelo->numJugadorActual].numVidas > 0) {
                        cambioTirarAplicar(jugadores, miModelo);
                        return; // Ya no se hace nada mas
                    }
                }
                //Si ha guardado todos los dados, o ha perdido todas
                //las vidas a causa de los indios, se acaba el turno.
                if (jugadores[miModelo->numJugadorActual].numVidas <= 0) { //Puede haber perdido las vidas a causa de las dinamitas o de las flechas
                    jugadorMuerto(jugadores, miModelo, miModelo->numJugadorActual);
                    finalizarTurno(jugadores, miModelo);
                    return;
                }
                guardados = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->dadoGuardado[1][i])
                        guardados++;
                }
                //Ha decidido guardar todos los dados, i.e, plantarse
                if (guardados == 5) {
                    cambioTirarAplicar(jugadores, miModelo);
                    return;
                }
                //Si llega hasta aquí es que sigue vivo y no se ha plantado
                lanzarDados(jugadores, miModelo);
                //Ahora decidir como guardar los dados si se trata de la CPU
                if (miModelo->numJugadorActual != 0)
                    guardarDados(jugadores, miModelo);
                return;
            }

            // Si es el turno del jugador, puede darle a guardar los dados.
            if (kDown & KEY_TOUCH && miModelo->numJugadorActual == 0) {
                // Puede que haya pulsado un dado, en cuyo caso se comprueba si no es una dinamita
                // y en ese caso se intercambia el valor de dado guardado
                for (i = 0; i < 5; i++) { // base 20+60*i,8+48*(numTirada actual -1)
                    if (touch.px >= 20 + 60 * i && touch.px <= 60 + 60 * i && touch.py >= 8 + 48 && touch.py <= 48 + 48)
                        if (miModelo->resultadoTirada[1][i] != DINAMITA) {
                            if (miModelo->dadoGuardado[1][i] == 0) {
                                miModelo->dadoGuardado[1][i] = 1;
                                miModelo->dadoGuardado[2][i] = 1;
                            } else {
                                miModelo->dadoGuardado[1][i] = 0;
                                miModelo->dadoGuardado[2][i] = 0;
                            }
                        }
                }
            }
        }

        if (miModelo->numTiradaActual == 3) {
            if (kDown & KEY_A) {
                //Por lo general, ésta es la última tirada, salvo que el jugador controle a Lucky Duke, que tiene una tirada adicional
                //Comprobamos si le han salido dinamitas, con lo cual terminaría su turno y si está muerto mas aun xD
                dinamitas = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->resultadoTirada[2][i] == DINAMITA)
                        dinamitas++;
                }
                if (dinamitas > 2) {
                    jugadores[miModelo->numJugadorActual].numVidas--;
                    if (jugadores[miModelo->numJugadorActual].numVidas > 0) {
                        cambioTirarAplicar(jugadores, miModelo);
                        return; // Ya no se hace nada mas
                    }
                }
                //Si ha guardado todos los dados, o ha perdido todas
                //las vidas a causa de los indios, se acaba el turno.
                if (jugadores[miModelo->numJugadorActual].numVidas <= 0) { //Puede haber perdido las vidas a causa de las dinamitas o de las flechas
                    jugadorMuerto(jugadores, miModelo, miModelo->numJugadorActual);
                    finalizarTurno(jugadores, miModelo);
                    return;
                }
                guardados = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->dadoGuardado[2][i])
                        guardados++;
                }
                //Ha decidido guardar todos los dados, i.e, plantarse
                if (guardados == 5) {
                    cambioTirarAplicar(jugadores, miModelo);
                    return;
                }
                // Si llega hasta aquí es que sigue vivo tras tirar los dados. Ahora si el jugador es Lucky Duke, tras pulsar
                // KEY_A vuelve a lanzar, y en caso contrario finaliza su turno.
                if (jugadores[miModelo->numJugadorActual].nombrePersonaje != LUCKY_DUKE) {
                    cambioTirarAplicar(jugadores, miModelo);
                    return;
                } else { // es Lucky Duke y va a por su cuarta tirada, la última
                    lanzarDados(jugadores, miModelo);
                    if (miModelo->numJugadorActual != 0)
                        guardarDados(jugadores, miModelo);
                    return;
                }
            }
            // Si es el turno del jugador y se trata de Lucky Duke, puede darle a guardar los dados tras la tercera tirada.
            if (kDown & KEY_TOUCH && miModelo->numJugadorActual == 0 && jugadores[miModelo->numJugadorActual].nombrePersonaje == LUCKY_DUKE) {
                //Suponiendo de que se trata de Lucky Duke
                //Puede que haya pulsado un dado, en cuyo caso se comprueba si no es una dinamita
                // y en ese caso se intercambia el valor de dado guardado
                for (i = 0; i < 5; i++) { // base 20+60*i,8+48*(numTirada actual -1)
                    if (touch.px >= 20 + 60 * i && touch.px <= 60 + 60 * i && touch.py >= 8 + 2 * 48 && touch.py <= 48 + 2 * 48)
                        if (miModelo->resultadoTirada[1][i] != DINAMITA) {
                            if (miModelo->dadoGuardado[2][i] == 0)
                                miModelo->dadoGuardado[2][i] = 1;
                            else
                                miModelo->dadoGuardado[2][i] = 0;
                        }
                }
            }
        }

        if (miModelo->numTiradaActual == 4) {
            if (kDown & KEY_A) {
                //Ahora si que si ésta es la última tirada para Lucky Duke, la única opción es finalizar el turno
                //Comprobamos si le han salido dinamitas, con lo cual terminaría su turno y si está muerto mas aun xD
                dinamitas = 0;
                for (i = 0; i < 5; i++) {
                    if (miModelo->resultadoTirada[3][i] == DINAMITA)
                        dinamitas++;
                }
                if (dinamitas > 2) {
                    jugadores[miModelo->numJugadorActual].numVidas--;
                    if (jugadores[miModelo->numJugadorActual].numVidas > 0) {
                        cambioTirarAplicar(jugadores, miModelo);
                        return; // Ya no se hace nada mas
                    }
                }
                //Si ha guardado todos los dados, o ha perdido todas
                //las vidas a causa de los indios, se acaba el turno.
                if (jugadores[miModelo->numJugadorActual].numVidas <= 0) { //Puede haber perdido las vidas a causa de las dinamitas o de las flechas
                    jugadorMuerto(jugadores, miModelo, miModelo->numJugadorActual);
                    finalizarTurno(jugadores, miModelo);
                    return;
                }
                cambioTirarAplicar(jugadores, miModelo);
                return;
            }
        }
    }
    if (miModelo->estadoTurno == APLICANDO) {
        // Calculamos con los dados asignados en las areas actuales las vidas y las flechas de todos los jugadores:
        for (i = 0; i < 5; i++) {
            jugadores[i].numVidasTrasAplicarDados = jugadores[i].numVidas;
            jugadores[i].numFlechasTrasAplicarDados = jugadores[i].numFlechas;
        }
        for (i = 0; i < 5; i++) {
            dado d = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i];
            if (miModelo->estadoDado[i] == 1) {
                if (d == UNO || d == DOS) {
                    jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados--;
                    if (jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados < 0)
                        jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados = 0;
                }
                if (d == CERVEZA) {
                    jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados++;
                    if (jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados > jugadores[miModelo->objetivoDado[i]].numVidasMaximas)
                        jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados--;
                }
            }
        }
        /**Todo esto que viene ahora es para el jugador 0, que es el q puede mover los dados**/
        // Dentro de este if viene como pinchar en un dado y moverlo
        if (miModelo->numJugadorActual == 0) {
            if (miModelo->dadoSeleccionado == -1) {
                for (i = 0; i < 5; i++) {
                    //Si sucede esto, ha tocado un dado. Calculamos a que jugadores se lo puede repartir
                    if ((kDown & KEY_TOUCH) && touch.px >= miModelo->posicionDados[i][0] && touch.px <= miModelo->posicionDados[i][0] + 40
                            && touch.py >= miModelo->posicionDados[i][1] && touch.py <= miModelo->posicionDados[i][1] + 40) {
                        //Solo vamos a permitir que seleccione un dado de los que se pueden mover...
                        dado d = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i];
                        if (d == UNO || d == DOS || d == CERVEZA) {
                            miModelo->dadoSeleccionado = i;
                            desfaseX = touch.px - miModelo->posicionDados[i][0];
                            desfaseY = touch.py - miModelo->posicionDados[i][1];
                            //Almacenamos en jugadoresElegir al juador de la izq y al de la derecha en caso de UNO
                            actualizarPosiblesObjetivos(jugadores, miModelo, i);
                        }
                    }
                }
            }//En cambio, está seleccionando un dado
            else {
                //Puede mover libremente el dado
                if ((kHeld & KEY_TOUCH)) {
                    miModelo->posicionDados[miModelo->dadoSeleccionado][0] = touch.px - desfaseX;
                    miModelo->posicionDados[miModelo->dadoSeleccionado][1] = touch.py - desfaseY;
                    //Aqui comprobamos si actualmente el dado está en una posicion en que queda bien asignado
                    dado d = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][miModelo->dadoSeleccionado];
                    if (d == UNO || d == DOS || d == CERVEZA)
                        miModelo->estadoDado[miModelo->dadoSeleccionado] = 0;
                    for (i = 0; i < miModelo->numJugadoresElegir; i++) {
                        //Pasar X e Y del centro del dado y devolver si pertenece a alguna de las regiones
                        //Si está dentro, entonces el dado pasa a estar bien colocado
                        if (dadoEnRegion(miModelo->posicionDados[miModelo->dadoSeleccionado][0] + 20, miModelo->posicionDados[miModelo->dadoSeleccionado][1] + 20, miModelo->jugadoresElegir[i])) {
                            miModelo->estadoDado[miModelo->dadoSeleccionado] = 1;
                            miModelo->objetivoDado[miModelo->dadoSeleccionado] = miModelo->jugadoresElegir[i];
                        }
                    }
                }//Si suelta, dejaremos de tener seleccionado ningún dado
                else {
                    miModelo->dadoSeleccionado = -1;
                    miModelo->numJugadoresElegir = 0;
                }
            }
        }//PARA LA IA, en cada VBLANK movemos uno de los dados hacia su posición final
        else {
            if (miModelo->dadoSeleccionado == -1) { //Si hemos acabado de mover un dado a su sitio, comprobamos el siguiente
                for (i = 0; i < 5; i++) {
                    if (miModelo->posicionFinalDados[i][0] != miModelo->posicionDados[i][0] || miModelo->posicionFinalDados[i][1] != miModelo->posicionDados[i][1]) {
                        miModelo->dadoSeleccionado = i;
                        actualizarPosiblesObjetivos(jugadores, miModelo, i);
                        break;
                    }
                }
            } else if (miModelo->dadoSeleccionado>-1) { //Movemos 3 pixeles acercando primero en vertical y luego en horizontal
                i = miModelo->dadoSeleccionado;
                difVertical = miModelo->posicionFinalDados[i][1] - miModelo->posicionDados[i][1];
                difHorizontal = miModelo->posicionFinalDados[i][0] - miModelo->posicionDados[i][0];
                if (difVertical != 0) {
                    if (difVertical > 3)
                        miModelo->posicionDados[i][1] += 3;
                    else if (difVertical<-3)
                        miModelo->posicionDados[i][1] -= 3;
                    else
                        miModelo->posicionDados[i][1] = miModelo->posicionFinalDados[i][1];
                } else if (difHorizontal != 0) {
                    if (difHorizontal > 3)
                        miModelo->posicionDados[i][0] += 3;
                    else if (difHorizontal<-3)
                        miModelo->posicionDados[i][0] -= 3;
                    else
                        miModelo->posicionDados[i][0] = miModelo->posicionFinalDados[i][0];
                } else { //El dado está en su sitio
                    miModelo->dadoSeleccionado = -1;
                }
                //Comprobamos si el estado del dado es el bueno
                if (dadoEnRegion(miModelo->posicionDados[miModelo->dadoSeleccionado][0] + 20,
                        miModelo->posicionDados[miModelo->dadoSeleccionado][1] + 20,
                        miModelo->objetivoDado[miModelo->dadoSeleccionado])) {
                    miModelo->estadoDado[miModelo->dadoSeleccionado] = 1;
                }
            }
        }
        //Si pulsa A comprobamos que todos los dados estén correctamente asignados, y en ese caso finalizariamos el turno
        if (kDown & KEY_A) {
            int dadosBien = 0;
            for (i = 0; i < 5; i++) {
                //Para el jugador humano, vemos que haya colocado todos los dados
                if (miModelo->numJugadorActual == 0) {
                    if (miModelo->estadoDado[i] == 1)
                        dadosBien++;
                }//Para el jugador CPU, vemos que los dados estén en su posicion fijada
                else {
                    if (miModelo->posicionDados[i][0] == miModelo->posicionFinalDados[i][0] && miModelo->posicionDados[i][1] == miModelo->posicionFinalDados[i][1])
                        dadosBien++;
                }
            }
            if (dadosBien == 5)
                aplicarDados(jugadores, miModelo);
        }
    }
}

//En esta funcion se le pasa el número de un jugador que acaba de morir, y se comprueba si la partida ha finalizado.

void jugadorMuerto(jugador jugadores[], modelo *miModelo, int numJugadorMuerto) {
    int i, j, numForajidosVivos, numRenegadosVivos;
    //Primero, se pasan las flechas del jugador a la reserva total
    miModelo->numFlechasRestantes += jugadores[numJugadorMuerto].numFlechas;
    jugadores[numJugadorMuerto].numFlechas = 0;
    //Después se comprueba que la partida no haya acabado
    numForajidosVivos = 0;
    numRenegadosVivos = 0;
    for (j = 0; j < NUM_JUGADORES; j++) {
        if (jugadores[j].rolJugador == FORAJIDO && jugadores[j].numVidas > 0)
            numForajidosVivos++;
        if (jugadores[j].rolJugador == RENEGADO && jugadores[j].numVidas > 0)
            numRenegadosVivos++;
    }
    //1- El sheriff ha muerto, por lo que ganan los forajidos o el renegado (renegado si es el último)
    for (i = 0; i < NUM_JUGADORES; i++) {
        if (jugadores[i].rolJugador == SHERIFF && jugadores[i].numVidas <= 0) {
            if (numRenegadosVivos && !numForajidosVivos)
                miModelo->estadoTurno = VICTORIA_RENEGADO;
            else
                miModelo->estadoTurno = VICTORIA_FORAJIDOS;
        }
    }
    //2- El renegado y los forajidos han muerto. En ese caso, gana el sheriff
    if (numForajidosVivos == 0 && numRenegadosVivos == 0)
        miModelo->estadoTurno = VICTORIA_SHERIFF;
}

void actualizarPosiblesObjetivos(jugador jugadores[], modelo *miModelo, int numDado) {
    int i, derecha, derecha2, izquierda, izquierda2;

    if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][numDado] == UNO) {
        derecha = primeroPorDerecha(jugadores, miModelo->numJugadorActual);
        miModelo->jugadoresElegir[0] = derecha;
        miModelo->numJugadoresElegir = 1;
        izquierda = primeroPorIzquierda(jugadores, miModelo->numJugadorActual);
        if (izquierda != derecha) {
            miModelo->jugadoresElegir[1] = izquierda;
            miModelo->numJugadoresElegir = 2;
        }
    }
    if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][numDado] == DOS) {
        derecha = primeroPorDerecha(jugadores, miModelo->numJugadorActual);
        derecha2 = primeroPorDerecha(jugadores, derecha);
        if (derecha2 == miModelo->numJugadorActual) //solo se da con dos vivos 
            derecha2 = derecha;
        miModelo->jugadoresElegir[0] = derecha2;
        miModelo->numJugadoresElegir = 1;
        izquierda = primeroPorIzquierda(jugadores, miModelo->numJugadorActual);
        izquierda2 = primeroPorIzquierda(jugadores, izquierda);
        if (izquierda2 != derecha2 && izquierda2 != miModelo->numJugadorActual) {
            miModelo->jugadoresElegir[1] = izquierda2;
            miModelo->numJugadoresElegir = 2;
        }
    }
    //En caso de CERVEZA es facil, todos los vivos pueden ser objetivo
    if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][numDado] == CERVEZA) {
        miModelo->numJugadoresElegir = 0;
        for (i = 0; i < NUM_JUGADORES; i++)
            if (jugadores[i].numVidas > 0) {
                miModelo->jugadoresElegir[miModelo->numJugadoresElegir] = i;
                miModelo->numJugadoresElegir++;
            }

    }
}

void lanzarDados(jugador jugadores[], modelo *miModelo) {
    int i, j, k;
    dado caras[6] = {FLECHA, DINAMITA, UNO, DOS, CERVEZA, GATLING};
    for (i = 0; i < 5; i++) {
        //Si el dado no está guardado se vuelve a lanzar
        //Antes de la primera tirada está a 0
        if (!miModelo->dadoGuardado[miModelo->numTiradaActual][i]) {
            j = rand() % 6;
            miModelo->resultadoTirada[miModelo->numTiradaActual][i] = caras[j];
            //Si sale una dinamita, ese dado ya no se puede modificar
            if (caras[j] == DINAMITA) {
                for (k = miModelo->numTiradaActual; k < 3; k++)
                    miModelo->dadoGuardado[k][i] = 1;
            }
            //Por cada flecha que obtenga se le suma una flecha.
            if (caras[j] == FLECHA) {
                jugadores[miModelo->numJugadorActual].numFlechas++;
                miModelo->numFlechasRestantes--;
                if (miModelo->numFlechasRestantes == 0) {
                    //LLAMAR A LOS INDIOS!
                    indios(jugadores, miModelo);
                }
            }
        }//En otro caso, el dado viene guardado de una tirada anterior
        else {
            miModelo->resultadoTirada[miModelo->numTiradaActual][i] = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i];
        }
    }
    //Por último, sumamos una tirada
    miModelo->numTiradaActual++;
}

void finalizarTurno(jugador jugadores[], modelo *miModelo) {
    int i, j;
    // Primero comprobamos que no haya acabado ya la partida, es decir, el estado de la partida viene de APLICANDO en lugar de VICTORIA_X
    /*u32 kDown;
    do{
            hidScanInput();
            kDown = hidKeysDown();
            sf2d_start_frame(GFX_TOP, GFX_LEFT);
            sf2d_draw_texture_part(graficosTop[NUMEROS], 380,200,0,17*miModelo->estadoTurno,12,17);
            sf2d_end_frame();
            sf2d_swapbuffers();
    } while(!(kDown & KEY_B));*/

    if (miModelo->estadoTurno == APLICANDO || miModelo->estadoTurno == TIRANDO) {

        i = miModelo->numJugadorActual;
        do {
            i++;
            if (i == NUM_JUGADORES)
                i = 0;
        } while (jugadores[i].numVidas <= 0);

        miModelo->numJugadorActual = i;
        miModelo->numTiradaActual = 0;
        miModelo->estadoTurno = TIRANDO;

        for (i = 0; i < 5; i++)
            for (j = 0; j < 3; j++)
                miModelo->dadoGuardado[j][i] = 0;
    }
    //Si no, en la logica del juego se pone que sucede cuando hay victoria!
}

//

void indios(jugador jugadores[], modelo *miModelo) {
    int i;
    for (i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i].numVidas -= jugadores[i].numFlechas;
        if (jugadores[i].numVidas < 0) jugadores[i].numVidas = 0;
        if (jugadores[i].numVidas <= 0 && i != miModelo->numJugadorActual)
            jugadorMuerto(jugadores, miModelo, i);
        jugadores[i].numFlechas = 0;
    }
    miModelo->numFlechasRestantes = 9;
}

//TO -DO : Analiza la tirada obtenida, decide que dados le gustan y cuales quiere volver a tirar, o si
// se planta porque hay mucho riesgo de meurte

void guardarDados(jugador jugadores[], modelo *miModelo) {

}

//Devuelve al primer jugador que está a la derecha (distancia 1) de otr.

int primeroPorDerecha(jugador jugadores[], int posicionJugador) {
    int derecha = posicionJugador;
    do {
        derecha--;
        if (derecha == -1)
            derecha = NUM_JUGADORES - 1;
    } while (jugadores[derecha].numVidas <= 0 || derecha == posicionJugador);
    return derecha;
}

//Devuelve al primer jugador que está a la izquierda (distancia 1) de otr.

int primeroPorIzquierda(jugador jugadores[], int posicionJugador) {
    int izquierda = posicionJugador;
    do {
        izquierda++;
        if (izquierda == NUM_JUGADORES)
            izquierda = 0;
    } while (jugadores[izquierda].numVidas <= 0 || izquierda == posicionJugador);
    return izquierda;
}

//Cambia el estado del turno de tirando a aplicando (i.e., elegir a quien mandas
// cada uno de los dados con los que dejaste de tirar)
// Además, es donde se llama a la IA, se decide que dado mandar a quien.

void cambioTirarAplicar(jugador jugadores[], modelo *miModelo) {
    int i;
    miModelo->estadoTurno = APLICANDO;
    //Posicion correcta para los dados, sin seleccionar ninguno
    miModelo->posicionDados[0][0] = 140;
    miModelo->posicionDados[0][1] = 62;
    miModelo->posicionDados[1][0] = 98;
    miModelo->posicionDados[1][1] = 92;
    miModelo->posicionDados[2][0] = 184;
    miModelo->posicionDados[2][1] = 92;
    miModelo->posicionDados[3][0] = 115;
    miModelo->posicionDados[3][1] = 135;
    miModelo->posicionDados[4][0] = 165;
    miModelo->posicionDados[4][1] = 135;
    miModelo->dadoSeleccionado = -1;

    for (i = 0; i < 5; i++) {
        jugadores[i].numVidasTrasAplicarDados = jugadores[i].numVidas;
        jugadores[i].numFlechasTrasAplicarDados = jugadores[i].numFlechas;
    }

    //Conocer cuales son los dados que están correctamente (no hay q asignarlos)
    for (i = 0; i < 5; i++) {
        dado d = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i];
        miModelo->estadoDado[i] = 1; //Correcto salvo q se demuestre lo conrario
        if (d == UNO || d == DOS || d == CERVEZA)
            miModelo->estadoDado[i] = 0;
    }
    //Por último asignar esos dados para los jugadores CPU
    if (miModelo->numJugadorActual != 0)
        asignarDadosIA(jugadores, miModelo);
}

// Eleccion de los objetivos a los que aplicar cada uno de los dados que hemos tirado. 

void asignarDadosIA(jugador jugadores[], modelo *miModelo) {
    int i, j, k, random, encontrado, encontradoEnAliados;
    int verdaderosObjetivos[miModelo->numJugadoresElegir];
    int numVerdaderosObjetivos;
    int numJugadorActual = miModelo->numJugadorActual;
    int numJugador2;
    int numForajidos;
    int prioridad, prioridadEncontrada;
    int numJugadorObjetivo;
    dado d;

    //ASIGNAR UNOS Y DOSES
    for (i = 0; i < 5; i++) {
        if (miModelo->estadoDado[i] == 0) {
            d = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i];
            actualizarPosiblesObjetivos(jugadores, miModelo, i);
            /*if(DEBUG_ALIADOS_Y_ENEMIGOS){
                    //Vamos a probar a pintar la pantalla para actualizar la lista de jugadores a elegir y poder ver q coño pasa
                    miModelo->dadoSeleccionado = i;
                    interfazTopScreen(jugadores,*miModelo,graficosTop); // <- Start frame GFX_TOP
                    interfazBottomScreen(jugadores,*miModelo,graficosBottom); //<- Start_frame (GFX_BOTTOm)
                    sf2d_swapbuffers();
                    miModelo->dadoSeleccionado = -1;
            }*/
            if (d == UNO || d == DOS) {
                // FORAJIDOS: Mata sheriff > mata enemigo > daña sheriff > daña enemigo
                if (jugadores[numJugadorActual].rolJugador == FORAJIDO) {
                    //Buscamos en su lista de enemigos
                    prioridad = 0;
                    for (j = 0; j < jugadores[numJugadorActual ].numEnemigos; j++) {
                        for (k = 0; k < miModelo->numJugadoresElegir; k++) {
                            //Le hemos encontrado!
                            if (jugadores[numJugadorActual].listaJugadoresEnemigos[j] == miModelo->jugadoresElegir[k]) {
                                numJugadorObjetivo = miModelo->jugadoresElegir[k];
                                prioridadEncontrada = 0;
                                //Si muere el sheriff, prioridad 4. Si muere un enemigo, prioridad 3. Si daña al sheriff, prioridad 2. Si daña al enemigo, prioridad 1.
                                if (jugadores[numJugadorObjetivo].rolJugador == SHERIFF && jugadores[numJugadorObjetivo].numVidasTrasAplicarDados == 1)
                                    prioridadEncontrada = 4;
                                if (jugadores[numJugadorObjetivo].rolJugador != SHERIFF && jugadores[numJugadorObjetivo].numVidasTrasAplicarDados == 1)
                                    prioridadEncontrada = 3;
                                if (jugadores[numJugadorObjetivo].rolJugador == SHERIFF && jugadores[numJugadorObjetivo].numVidasTrasAplicarDados > 1)
                                    prioridadEncontrada = 2;
                                if (jugadores[numJugadorObjetivo].rolJugador != SHERIFF && jugadores[numJugadorObjetivo].numVidasTrasAplicarDados > 1)
                                    prioridadEncontrada = 1;
                                if (prioridadEncontrada > prioridad) {
                                    prioridad = prioridadEncontrada;
                                    miModelo->objetivoDado[i] = numJugadorObjetivo;
                                    jugadores[numJugadorObjetivo].numVidasTrasAplicarDados--;
                                    if (jugadores[numJugadorObjetivo].numVidasTrasAplicarDados < 0)
                                        jugadores[numJugadorObjetivo].numVidasTrasAplicarDados = 0;
                                }
                            }
                        }
                    }
                    //Si sigue prioridad 0, es que no ha encontrado un enemigo con lo que tiene que disparar entre sus objetivos, al que no sea aliado
                    if (prioridad == 0) {
                        //Buscamos si hay algún aliado entre los objetivos, y en ese caso le quitamos de sus objetivos
                        numVerdaderosObjetivos = 0;
                        for (j = 0; j < miModelo->numJugadoresElegir; j++) {
                            encontradoEnAliados = 0;
                            for (k = 0; k < jugadores[numJugadorActual].numAliados; k++) {
                                if (jugadores[numJugadorActual].listaJugadoresAliados[k] == miModelo->jugadoresElegir[j])
                                    encontradoEnAliados = 1;
                            }
                            if (!encontradoEnAliados) {
                                verdaderosObjetivos[numVerdaderosObjetivos] = miModelo->jugadoresElegir[j];
                                numVerdaderosObjetivos++;
                            }
                        }
                        //Si al menos hay uno, elegimos al azar entre ellos
                        if (numVerdaderosObjetivos > 0) {
                            random = rand() % numVerdaderosObjetivos;
                            miModelo->objetivoDado[i] = verdaderosObjetivos[random];
                        } else {//Tenemos que elegir entre los aliados... como son una lista de prioridad, tomaremos al último posible que esté entre los aliados a elegir
                            encontrado = -1;
                            for (j = jugadores[numJugadorActual].numAliados - 1; j >= 0; j--) {
                                for (k = 0; k < miModelo->numJugadoresElegir; k++) {
                                    if (jugadores[numJugadorActual].listaJugadoresAliados[j] == miModelo->jugadoresElegir[k]) {
                                        if (encontrado == -1) //Nos quedamos con el primero que aparezca
                                            encontrado = miModelo->jugadoresElegir[k];
                                    }
                                }
                            }
                            miModelo->objetivoDado[i] = encontrado;
                        }
                    }
                }//Sheriff,alguaciles y renegados, dañaran a los enemigos 
                else {
                    prioridad = 0;
                    for (j = 0; j < jugadores[numJugadorActual].numEnemigos; j++) {
                        for (k = 0; k < miModelo->numJugadoresElegir; k++) {
                            //Le hemos encontrado!
                            if (jugadores[numJugadorActual].listaJugadoresEnemigos[j] == miModelo->jugadoresElegir[k]) {
                                numJugadorObjetivo = miModelo->jugadoresElegir[k];
                                prioridadEncontrada = 0;
                                //Si muere un enemigo, prioridad 2, si no, prioridad 1.
                                if (jugadores[numJugadorObjetivo].numVidasTrasAplicarDados == 1)
                                    prioridadEncontrada = 2;
                                if (jugadores[numJugadorObjetivo].numVidasTrasAplicarDados > 1)
                                    prioridadEncontrada = 1;
                                if (prioridadEncontrada > prioridad) {
                                    prioridad = prioridadEncontrada;
                                    miModelo->objetivoDado[i] = numJugadorObjetivo;
                                }
                            }
                        }
                    }
                    //Si sigue prioridad 0, es que no ha encontrado un enemigo con lo que tiene que disparar entre sus objetivos, al que no sea aliado
                    if (prioridad == 0) {
                        //Buscamos si hay algún aliado entre los objetivos, y en ese caso le quitamos de sus objetivos
                        numVerdaderosObjetivos = 0;
                        for (j = 0; j < miModelo->numJugadoresElegir; j++) {
                            encontradoEnAliados = 0;
                            for (k = 0; k < jugadores[numJugadorActual].numAliados; k++) {
                                if (jugadores[numJugadorActual].listaJugadoresAliados[k] == miModelo->jugadoresElegir[j])
                                    encontradoEnAliados = 1;
                            }
                            if (!encontradoEnAliados) {
                                verdaderosObjetivos[numVerdaderosObjetivos] = miModelo->jugadoresElegir[j];
                                numVerdaderosObjetivos++;
                            }
                        }
                        //Si al menos hay uno, elegimos al azar entre ellos
                        if (numVerdaderosObjetivos > 0) {
                            random = rand() % numVerdaderosObjetivos;
                            miModelo->objetivoDado[i] = verdaderosObjetivos[random];
                        } else {//Tenemos que elegir entre los aliados... como son una lista de prioridad, tomaremos al último posible que esté entre los aliados a elegir
                            encontrado = -1;
                            for (j = jugadores[numJugadorActual].numAliados - 1; j >= 0; j--) {
                                for (k = 0; k < miModelo->numJugadoresElegir; k++) {
                                    if (jugadores[numJugadorActual].listaJugadoresAliados[j] == miModelo->jugadoresElegir[k]) {
                                        if (encontrado == -1) //Nos quedamos con el primero que aparezca
                                            encontrado = miModelo->jugadoresElegir[k];
                                    }
                                }
                            }
                            miModelo->objetivoDado[i] = encontrado;
                        }
                    }
                }
                //Por último, bajamos su núm potencial de vidas
                jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados--;
                if (jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados < 0)
                    jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados = 0;
            }
        }
    }

    //ASIGNAR CERVEZAS
    for (i = 0; i < 5; i++) {
        if (miModelo->estadoDado[i] == 0) {
            d = miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i];
            if (d == CERVEZA) {
                //Forajidos y Sheriff, se curan ellos mismos y las restantes para aliados...
                if (jugadores[numJugadorActual].rolJugador == FORAJIDO || jugadores[numJugadorActual].rolJugador == SHERIFF) {
                    if (jugadores[numJugadorActual].numVidasTrasAplicarDados < jugadores[numJugadorActual].numVidasMaximas) {
                        miModelo->objetivoDado[i] = numJugadorActual;
                        jugadores[numJugadorActual].numVidasTrasAplicarDados++;
                    }//Si está a tope, busca al aliado mas débil y le cura
                    else {
                        prioridad = 0;
                        for (j = 0; j < jugadores[numJugadorActual].numAliados; j++) {
                            numJugador2 = jugadores[numJugadorActual].listaJugadoresAliados[j];
                            //Si no está ya a tope
                            if (jugadores[numJugador2].numVidasTrasAplicarDados < jugadores[numJugador2].numVidasMaximas)
                                if (11 - jugadores[numJugador2].numVidasTrasAplicarDados > prioridad) { //El que esté mas low
                                    prioridad = 11 - jugadores[numJugador2].numVidasTrasAplicarDados;
                                    miModelo->objetivoDado[i] = numJugador2;
                                }
                        }
                        if (prioridad == 0) { // No hay ningun aliado vivo, con lo que me guardo las cervezas
                            miModelo->objetivoDado[i] = numJugadorActual;
                        }
                        jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados++;
                        if (jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados > jugadores[miModelo->objetivoDado[i]].numVidasMaximas)
                            jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados--;
                    }
                }
                //Alguacil-> cura al sheriff salvo q este entero, en cuyo caso se cura a si mismo salvo q este entero, y si no cura a un aliado
                if (jugadores[numJugadorActual].rolJugador == ALGUACIL) {
                    numJugador2 = 0;
                    for (j = 0; j < NUM_JUGADORES; j++) {
                        if (jugadores[j].rolJugador == SHERIFF)
                            numJugador2 = j;
                    }
                    if (jugadores[numJugador2].numVidasTrasAplicarDados < jugadores[numJugador2].numVidasMaximas) {
                        miModelo->objetivoDado[i] = numJugador2;
                        jugadores[numJugador2].numVidasTrasAplicarDados++;
                    }//El sheriff está a tope, trata de curarse a si mismo
                    else if (jugadores[numJugadorActual].numVidasTrasAplicarDados < jugadores[numJugadorActual].numVidasMaximas) {
                        miModelo->objetivoDado[i] = numJugadorActual;
                        jugadores[numJugadorActual].numVidasTrasAplicarDados++;
                    }//Si está a tope, busca al aliado mas débil y le cura
                    else {
                        prioridad = 0;
                        for (j = 0; j < jugadores[numJugadorActual].numAliados; j++) {
                            numJugador2 = jugadores[numJugadorActual].listaJugadoresAliados[j];
                            //Si no está ya a tope
                            if (jugadores[numJugador2].numVidasTrasAplicarDados < jugadores[numJugador2].numVidasMaximas)
                                if (11 - jugadores[numJugador2].numVidasTrasAplicarDados > prioridad) { //El que esté mas low
                                    prioridad = 11 - jugadores[numJugador2].numVidasTrasAplicarDados;
                                    miModelo->objetivoDado[i] = numJugador2;
                                }
                        }
                        if (prioridad == 0) { // No hay ningun aliado vivo, con lo que me guardo las cervezas
                            miModelo->objetivoDado[i] = numJugadorActual;
                        }
                        jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados++;
                        if (jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados > jugadores[miModelo->objetivoDado[i]].numVidasMaximas)
                            jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados--;
                    }
                }
                //Renegado -> cura al sheriff salvo q los forajidos hayan muerto
                if (jugadores[numJugadorActual].rolJugador == RENEGADO) {
                    numForajidos = 0;
                    numJugador2 = 0; //para evitar warnings
                    for (j = 0; j < NUM_JUGADORES; j++) {
                        if (jugadores[j].rolJugador == SHERIFF)
                            numJugador2 = j;
                        if (jugadores[j].rolJugador == FORAJIDO)
                            numForajidos++;
                    }
                    //Cura al sheriff salvo q los forajidos hayan muerto
                    if (jugadores[numJugador2].numVidasTrasAplicarDados < jugadores[numJugador2].numVidasMaximas && numForajidos > 0) {
                        miModelo->objetivoDado[i] = numJugador2;
                        jugadores[numJugador2].numVidasTrasAplicarDados++;

                    }//El sheriff está a tope, se cura a si mismo
                    else {
                        miModelo->objetivoDado[i] = numJugadorActual;
                        jugadores[numJugadorActual].numVidasTrasAplicarDados++;
                        if (jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados > jugadores[miModelo->objetivoDado[i]].numVidasMaximas)
                            jugadores[miModelo->objetivoDado[i]].numVidasTrasAplicarDados--;
                    }
                }
            }
        }
    }

    //POSICIONAR LOS DADOS
    for (i = 0; i < 5; i++) {
        if (miModelo->estadoDado[i] == 0) {
            //Asignar aqui la posicion final del dado
            if (miModelo->objetivoDado[i] == 0) {
                int vertices[5][2] = {
                    {92, 0},
                    {134, 1},
                    {118, 24},
                    {184, 1},
                    {164, 28}
                };
                miModelo->posicionFinalDados[i][0] = vertices[i][0];
                miModelo->posicionFinalDados[i][1] = vertices[i][1];
            }

            if (miModelo->objetivoDado[i] == 1) {
                int vertices[5][2] = {
                    {6, 9},
                    {50, 38},
                    {5, 66},
                    {59, 83},
                    {15, 111}
                };
                miModelo->posicionFinalDados[i][0] = vertices[i][0];
                miModelo->posicionFinalDados[i][1] = vertices[i][1];
            }

            if (miModelo->objetivoDado[i] == 2) {
                int vertices[5][2] = {
                    {50, 161},
                    {102, 199},
                    {96, 157},
                    {46, 200},
                    {4, 179}
                };
                miModelo->posicionFinalDados[i][0] = vertices[i][0];
                miModelo->posicionFinalDados[i][1] = vertices[i][1];
            }

            if (miModelo->objetivoDado[i] == 3) {
                int vertices[5][2] = {
                    {230, 164},
                    {178, 199},
                    {184, 157},
                    {234, 200},
                    {276, 179}
                };
                miModelo->posicionFinalDados[i][0] = vertices[i][0];
                miModelo->posicionFinalDados[i][1] = vertices[i][1];
            }
            if (miModelo->objetivoDado[i] == 4) {
                int vertices[5][2] = {
                    {274, 9},
                    {230, 38},
                    {275, 66},
                    {221, 83},
                    {265, 111}
                };
                miModelo->posicionFinalDados[i][0] = vertices[i][0];
                miModelo->posicionFinalDados[i][1] = vertices[i][1];
            }
        } else {
            //Dejar quieto el dado que ya está correctamente (flechas, dinamitas y gatlings)
            miModelo->posicionFinalDados[i][0] = miModelo->posicionDados[i][0];
            miModelo->posicionFinalDados[i][1] = miModelo->posicionDados[i][1];
        }
    }
}

//Se comprueba si el dado de centro x e y esta dentro de la región de dados del jugador
//numJugador

int dadoEnRegion(int x, int y, int numJugador) {
    if (numJugador == 0) {
        if (y < 61 && 61 * x - 47 * y - 5185 > 0 && 47 * y + 61 * x - 14274 < 0)
            return 1;
    }
    if (numJugador == 1) {
        if (78 * x - 55 * y - 4290 < 0 && 18 * y + 52 * x - 7124 < 0 && 92 * y + 30 * x - 14720 < 0)
            return 1;
    }
    if (numJugador == 2) {
        if (x < 147 && 100 * y + 31 * x - 18500 > 0 && 31 * x - 47 * y + 4138 < 0)
            return 1;
    }
    if (numJugador == 3) {
        if (x > 172 && 47 * y + 31 * x - 14027 > 0 && 31 * x - 100 * y + 8611 < 0)
            return 1;
    }
    if (numJugador == 4) {
        if (55 * y + 78 * x - 20592 > 0 && 52 * x - 28 * y - 8684 > 0 && 30 * x - 92 * y + 5150 > 0)
            return 1;
    }
    return 0;
}

//Cada dado tiene asignado un objetivo, y se aplican los daños y las curas por orden.

void aplicarDados(jugador jugadores[], modelo *miModelo) {
    int i, numGatlings;
    //Primero aplicamos unos y doses, a la vez
    for (i = 0; i < 5; i++) {
        if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i] == UNO) {
            jugadores[miModelo->objetivoDado[i]].numVidas--;
            if (jugadores[miModelo->objetivoDado[i]].numVidas <= 0) {
                jugadores[miModelo->objetivoDado[i]].numVidas = 0;
                jugadorMuerto(jugadores, miModelo, miModelo->objetivoDado[i]);
            }
            actualizarAlianzasTrasDanar(jugadores, miModelo->numJugadorActual, miModelo->objetivoDado[i]);
        }
        if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i] == DOS) {
            jugadores[miModelo->objetivoDado[i]].numVidas--;
            if (jugadores[miModelo->objetivoDado[i]].numVidas <= 0) {
                jugadores[miModelo->objetivoDado[i]].numVidas = 0;
                jugadorMuerto(jugadores, miModelo, miModelo->objetivoDado[i]);
            }
            actualizarAlianzasTrasDanar(jugadores, miModelo->numJugadorActual, miModelo->objetivoDado[i]);
        }
    }
    //Comprobamos cervezas (fijémonos en que no esté muerto ya el personaje que se quiere curar)
    for (i = 0; i < 5; i++) {
        if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i] == CERVEZA) {
            if (jugadores[miModelo->objetivoDado[i]].numVidas > 0) {
                jugadores[miModelo->objetivoDado[i]].numVidas++;
                if (jugadores[miModelo->objetivoDado[i]].numVidas > jugadores[miModelo->objetivoDado[i]].numVidasMaximas)
                    jugadores[miModelo->objetivoDado[i]].numVidas--;
                else
                    actualizarAlianzasTrasCurar(jugadores, miModelo->numJugadorActual, miModelo->objetivoDado[i]);
            }
        }
    }
    // Comprobamos si hay gatling
    numGatlings = 0;
    //Para Willy the kid, basta con tener dos ametralladoras entre sus dados
    if (jugadores[miModelo->numJugadorActual].nombrePersonaje == WILLY_THE_KID)
        numGatlings++;
    for (i = 0; i < 5; i++) {
        if (miModelo->resultadoTirada[miModelo->numTiradaActual - 1][i] == GATLING)
            numGatlings++;
    }
    // Quitamos a todos los personaje suna vida salvo al jugador actual, al que le quitamos las flechas
    if (numGatlings >= 3) {
        for (i = 0; i < NUM_JUGADORES; i++) {
            if (i != miModelo->numJugadorActual) {
                if (jugadores[i].nombrePersonaje != PAUL_REGRET) //Paul Regret no pierde vidas por la ametralladora gatling
                    jugadores[i].numVidas--;
                if (jugadores[i].numVidas <= 0) {
                    jugadores[i].numVidas = 0;
                    jugadorMuerto(jugadores, miModelo, i);
                }
            } else {
                miModelo->numFlechasRestantes += jugadores[i].numFlechas;
                jugadores[i].numFlechas = 0;
            }
        }
    }
    //Finalizamos el turno
    finalizarTurno(jugadores, miModelo);
}
// Introduce a un jugador en la lista de enemigos de otro, y si era aliado, lo saca de su lsita de aliados

void meterEnemigo(jugador jugadores[], int numJugador, int numJugadorEnemigo) {
    int i, encontrado = 0;
    if (numJugador == numJugadorEnemigo)
        return; //Esto es para no meter como enemigo o como aliado a uno mismo
    //Lo buscamos en su lista de enemigos
    for (i = 0; i < jugadores[numJugador].numEnemigos; i++) {
        if (jugadores[numJugador].listaJugadoresEnemigos[i] == numJugadorEnemigo)
            encontrado = 1;
    }
    //Si no estaba ya, lo incluimos
    if (!encontrado) {
        jugadores[numJugador].listaJugadoresEnemigos[jugadores[numJugador].numEnemigos] = numJugadorEnemigo;
        jugadores[numJugador].numEnemigos++;
    }
    //Lo buscamos su posicion en la lista de aliados, para sacarle en caso de encontrarle
    if (esAliado(jugadores, numJugador, numJugadorEnemigo)) {
        for (i = 0; i < jugadores[numJugador].numAliados; i++) {
            if (jugadores[numJugador].listaJugadoresAliados[i] == numJugadorEnemigo)
                encontrado = i;
        }
        for (i = encontrado; i < jugadores[numJugador].numAliados - 1; i++) {
            jugadores[numJugador].listaJugadoresAliados[i] = jugadores[numJugador].listaJugadoresAliados[i + 1];
        }
        jugadores[numJugador].numAliados--;
    }
}
// Introduce a un jugador en la lsita de aliados de otro. Si era enemigo, lo saca de la lsita de enemigos

void meterAliado(jugador jugadores[], int numJugador, int numJugadorAliado) {
    int i, encontrado = 0;
    if (numJugador == numJugadorAliado)
        return; //Esto es para no meter como enemigo o como aliado a uno mismo
    //Lo buscamos en su lista de aliados
    for (i = 0; i < jugadores[numJugador].numAliados; i++) {
        if (jugadores[numJugador].listaJugadoresAliados[i] == numJugadorAliado)
            encontrado = 1;
    }
    //Si no estaba ya, lo incluimos
    if (!encontrado) {
        jugadores[numJugador].listaJugadoresAliados[jugadores[numJugador].numAliados] = numJugadorAliado;
        jugadores[numJugador].numAliados++;
    }
    //Lo buscamos su posicion en la lista de enemigos, para sacarle en caso de encontrarle
    if (esEnemigo(jugadores, numJugador, numJugadorAliado)) {
        for (i = 0; i < jugadores[numJugador].numEnemigos; i++) {
            if (jugadores[numJugador].listaJugadoresEnemigos[i] == numJugadorAliado)
                encontrado = i;
        }

        for (i = encontrado; i < jugadores[numJugador - 1].numEnemigos; i++) {
            jugadores[numJugador].listaJugadoresEnemigos[i] = jugadores[numJugador].listaJugadoresEnemigos[i + 1];
        }
        jugadores[numJugador].numEnemigos--;
    }
}

//Devuelve si otro jugador está en la lista de aliados d eun jugador

int esAliado(jugador jugadores[], int numJugador, int numJugadorPresuntoAliado) {
    int i, resp = 0;
    for (i = 0; i < jugadores[numJugador].numAliados; i++) {
        if (jugadores[numJugador].listaJugadoresAliados[i] == numJugadorPresuntoAliado)
            resp = 1;
    }
    return resp;
}

// Devuelve si otro jugador está en la lista de eneigos de otro jugador

int esEnemigo(jugador jugadores[], int numJugador, int numJugadorPresuntoEnemigo) {
    int i, resp = 0;
    for (i = 0; i < jugadores[numJugador].numEnemigos; i++) {
        if (jugadores[numJugador].listaJugadoresEnemigos[i] == numJugadorPresuntoEnemigo)
            resp = 1;
    }
    return resp;
}

//TO DO: El jugador atacado puede mirar si era el único objetivo del dado q le ha hecho daño.. Si hay un dado del mismo tipo q hubiera sido guardado, entonces malo, pero si el dado
// viene de no haber sido guardado , entonces es q no quedaba otro remedio y no se puede deducir q el jugador atacante sea enemigo

void actualizarAlianzasTrasDanar(jugador jugadores[], int numJugadorAtacante, int numJugadorAtacado) {
    int i;
    //Supongamos que disparan al sheriff (todos saben quien es el sheriff)
    if (jugadores[numJugadorAtacado].rolJugador == SHERIFF) {
        for (i = 0; i < 5; i++) {
            if (jugadores[i].rolJugador == FORAJIDO) //Los forajidos haran al jugador atacante aliado, y si estaba en enemigo lo saca de ahi
                meterAliado(jugadores, i, numJugadorAtacante);
            if (jugadores[i].rolJugador == ALGUACIL || jugadores[i].rolJugador == RENEGADO) //Los alguaciles y renegado haran al jugador atacante enemigo y lo sacan de aliados
                meterEnemigo(jugadores, i, numJugadorAtacante);
            if (jugadores[i].rolJugador == SHERIFF) //El sheriff le mete en su lista de enemigos y lo saca de la de aliados
                meterEnemigo(jugadores, i, numJugadorAtacante);
        }
    }
    //Supongamos que un forajido es disparado, con lo que piensa que el que le dispara no es otro forajido
    //El unico caso es que ya haya disparado al sheriff, con lo cual sabemos que será un forajido casi seguro
    if (jugadores[numJugadorAtacado].rolJugador == FORAJIDO) {
        if (!esAliado(jugadores, numJugadorAtacado, numJugadorAtacante))
            meterEnemigo(jugadores, numJugadorAtacado, numJugadorAtacante);
    }
    for (i = 0; i < 5; i++) {
        //Para los alguaciles, cada vez q un jugador dispare a un enemigo se le pondrá en el saco de aliados salvo q haya disparado al sheriff (i.e, sea enemigo)
        if (jugadores[i].rolJugador == ALGUACIL) {
            if (esEnemigo(jugadores, i, numJugadorAtacado))
                if (!esEnemigo(jugadores, i, numJugadorAtacante))
                    meterAliado(jugadores, i, numJugadorAtacante);
        }
        //Para los forajidos,comprobamos si el jugador atacado es aliado. En ese caso, el jugador atacante pasará a ser ahora enemigo, salvo q haya disparado al sheriff(otro forajdio)
        if (jugadores[i].rolJugador == FORAJIDO) {
            if (esAliado(jugadores, i, numJugadorAtacado))
                if (!esAliado(jugadores, i, numJugadorAtacante))
                    meterEnemigo(jugadores, i, numJugadorAtacante);
        }
    }

}

void actualizarAlianzasTrasCurar(jugador jugadores[], int numJugadorCurandero, int numJugadorCurado) {


}

void cargarGraficoCaraPersonaje(modelo *miModelo, int numPersonaje) {
    if (miModelo->numJugadorCaraCargada == numPersonaje) {
        return;
    }
    if (miModelo->numJugadorCaraCargada != -1) {
        descargarGraficoCaraPersonaje(miModelo);
    }
    miModelo->numJugadorCaraCargada = numPersonaje;
    switch (numPersonaje) {
        case 0:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_00.pixel_data, cara_00.width, cara_00.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 1:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_01.pixel_data, cara_01.width, cara_01.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 2:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_02.pixel_data, cara_02.width, cara_02.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 3:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_03.pixel_data, cara_03.width, cara_03.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 4:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_04.pixel_data, cara_04.width, cara_04.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 5:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_05.pixel_data, cara_05.width, cara_05.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 6:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_06.pixel_data, cara_06.width, cara_06.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 7:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_07.pixel_data, cara_07.width, cara_07.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 8:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_08.pixel_data, cara_08.width, cara_08.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 9:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_09.pixel_data, cara_09.width, cara_09.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 10:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_10.pixel_data, cara_10.width, cara_10.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 11:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_11.pixel_data, cara_11.width, cara_11.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 12:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_12.pixel_data, cara_12.width, cara_12.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 13:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_13.pixel_data, cara_13.width, cara_13.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 14:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_14.pixel_data, cara_14.width, cara_14.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
        case 15:
            miModelo->graficoCaraPersonaje = sf2d_create_texture_mem_RGBA8(cara_15.pixel_data, cara_15.width, cara_15.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
            break;
    }
}

void descargarGraficoCaraPersonaje(modelo *miModelo) {
    sf2d_free_texture(miModelo->graficoCaraPersonaje);
    miModelo->numJugadorCaraCargada = -1;
}