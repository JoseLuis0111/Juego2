#include<allegro.h>
#include<stdio.h>
#include<time.h>

// 26/06/2020

typedef struct _Jugador{
    char Nomb[30];
    long Puntuacion;
}Jugador;

int Menu(void);
void Juego(void);
void Puntuaciones(void);
void CrearPuntuaciones(char [],long);
int PuntoColision(int,int);
void OrdBurbInv(Jugador [],int);

void main(){
    srand(time(NULL));

    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    //****************************************************************************************

    set_color_depth(32); // Profundidad de color 8,16,24 o 32 bits
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0); //Iniciar modo grafico (en modo ventana)
    set_window_title("EL JUEGO");

    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
    set_volume(255,255);

    Menu();
}

END_OF_MAIN();

int Menu(void){

    BITMAP *PantallaInicio = load_bitmap("IMG/PantallaInicio.bmp",NULL);
    BITMAP *PantallaInicioOp1 = load_bitmap("IMG/PantallaInicioOp1.bmp",NULL);
    BITMAP *PantallaInicioOp2 = load_bitmap("IMG/PantallaInicioOp2.bmp",NULL);
    BITMAP *PantallaInicioOp3 = load_bitmap("IMG/PantallaInicioOp3.bmp",NULL);

    BITMAP *Temp = create_bitmap(800,600);

    BITMAP *Cursor = load_bitmap("IMG/Cursor.bmp",NULL);

    int op=0;

    //MIDI  *AudioMenu = load_midi("Audio/AudioMenu.midi");
    //play_looped_midi(AudioMenu,0,120);

    SAMPLE *AudioMenu = load_wav("Audio/SpaceTrip.wav");
    play_sample(AudioMenu,200,200,1000,1);

    set_mouse_sprite(Cursor);
    show_mouse(screen);
    set_mouse_range(0,0,800,600);

    while(!key[KEY_ESC]){

        blit(PantallaInicio,Temp,0,0,0,0,800,600);

        //***********************************************************************
        //MOUSE

        if( mouse_x>=333 && mouse_x<=470 && mouse_y>=247 && mouse_y<=275){
            op=0;
            blit(PantallaInicioOp1,Temp,0,0,0,0,800,600);
            if(mouse_b &1){
                stop_sample(AudioMenu);
                Juego();
            }
        }

        if( mouse_x>=240 && mouse_x<=557 && mouse_y>=314 && mouse_y<=340){
            op=0;
            blit(PantallaInicioOp2,Temp,0,0,0,0,800,600);
            if(mouse_b &1){
                stop_sample(AudioMenu);
                Puntuaciones();
            }
        }

        if( mouse_x>=344 && mouse_x<=457 && mouse_y>=382 && mouse_y<=408){
            op=0;
            blit(PantallaInicioOp3,Temp,0,0,0,0,800,600);
            if(mouse_b &1){
                allegro_exit();
                return 0;
            }
        }

        //***********************************************************************
        //TECLADO

        //textprintf(screen,font,290,200,0xFFFFFF,"%d",op,0x4286f4);

        if(key[KEY_DOWN] && op<60){
            op++;
        }

        if(key[KEY_UP] && op>1){
            op--;
        }

        if(op>0 && op<20){
            blit(PantallaInicioOp1,Temp,0,0,0,0,800,600);
                if(key[KEY_ENTER] || key[KEY_X]){
                    stop_sample(AudioMenu);
                    Juego();
                }
        }

        if(op>=20 && op<40){
            blit(PantallaInicioOp2,Temp,0,0,0,0,800,600);
                if(key[KEY_ENTER] || key[KEY_X]){
                    stop_sample(AudioMenu);
                    Puntuaciones();
                }
        }

        if(op>=40 && op<=60){
            blit(PantallaInicioOp3,Temp,0,0,0,0,800,600);
                if(key[KEY_ENTER] || key[KEY_X]){
                    allegro_exit();
                    return 0;
                }
        }

        //draw_sprite(screen,Temp,0,0);
        blit(Temp,screen,0,0,0,0,800,600);

        //***********************************************************************
    }
}

void Juego(void){

    show_mouse(NULL);

    //MIDI  *AudioJuego = load_midi("Audio/AudioJuego.midi");
    //play_looped_midi(AudioJuego,0,120);

    SAMPLE *AudioJuego = load_wav("Audio/SlowDrip.wav");
    play_sample(AudioJuego,100,100,1000,1);

    BITMAP *Temp = create_bitmap(800,600);

    BITMAP *Escenario = load_bitmap("IMG/Escenario.bmp",NULL);
    BITMAP *Pausa = load_bitmap("IMG/Pausa.bmp",NULL);
    BITMAP *MonoC1 = load_bitmap("IMG/MonoCuadro1.bmp",NULL);
    BITMAP *MonoC2 = load_bitmap("IMG/MonoCuadro2.bmp",NULL);
    BITMAP *Pelota = load_bitmap("IMG/Pelota.bmp",NULL);

    BITMAP *_500pC1 = load_bitmap("IMG/500pC1.bmp",NULL);
    BITMAP *_500pC2 = load_bitmap("IMG/500pC2.bmp",NULL);
    BITMAP *_1000pC1 = load_bitmap("IMG/1000pC1.bmp",NULL);
    BITMAP *_1000pC2 = load_bitmap("IMG/1000pC2.bmp",NULL);
    BITMAP *Obstaculo = load_bitmap("IMG/Obstaculo80x15.bmp",NULL);

    //BITMAP *Enemigo = load_bitmap("IMG/Enemigo.bmp",NULL);
    //BITMAP *Prueba = load_bitmap("IMG/Prueba300px.bmp",NULL);

    int x1=400,y1=380,x2=(rand()%591),y2=0,x3,y3,x4=(rand()%720),y4=150,izq=rand()%2,izqObs=rand()%2,vida=3,Bajar=0,tecla;
    //   x1 y1 , mono1  | x2 y2 , Enemigo (Lanza pelotas) | x3 y3 , Pelota | x4 y4 , Obstaculo

    int SubirP=0,BajarP=1,SubirPIzq=0,SubirPDer=0,BajarPIzq=0,BajarPDer=0,Lanzar=0,Subir=0,x5,y5,z=0,Pelotas=20; //x5 y5 , Ataque2
    long Puntuacion=0;

    int xp=250,yp=500,P=0,SubirPIzq2=0,i; char Nombre[30];

    x3=x2+19; y3=y2+26; x5=x2+19; y5=y2+26;

    while(!key[KEY_ESC]){

            //**********************************************************************
            //Dibujado en pantalla

            blit(Escenario,Temp,0,0,0,0,800,600);


            //**********************************************************************
            //Controles

            if(key[KEY_RIGHT]){
                if(x1<700){
                    if(!key[KEY_Z]){
                        x1=x1+2;
                    }else{
                        x1++;
                    }
                }
            }
            if(key[KEY_LEFT]){
                if(x1+55>0){
                    if(!key[KEY_Z]){
                        x1=x1-2;
                    }else{
                        x1--;
                    }
                }
            }
            if(key[KEY_DOWN]){
                if(y1<500){
                    if(!key[KEY_Z]){
                        y1=y1+2;
                    }else{
                        y1++;
                    }
                }
            }
            if(key[KEY_UP]){
                if(y1>240){
                    if(!key[KEY_Z]){
                        y1=y1-2;
                    }else{
                        y1--;
                    }
                }
            }

            if(key[KEY_ENTER]){
                draw_sprite(screen,Pausa,100,100);
                //textprintf(Temp,font,390,290,0xFFFFFF,"PAUSA",0x4286f4);
                clear_keybuf();
                tecla=readkey();
            }

            if(!key[KEY_X]){
                draw_sprite(Temp,MonoC1,x1,y1);
            }

            if(key[KEY_X]){
                draw_sprite(Temp,MonoC2,x1,y1);
            }

            if(key[KEY_SPACE]){
                SubirPIzq=0;
                SubirPDer=0;
                x3=x2+19;
                y3=y2+26;
                BajarP=1;
            }

            //**********************************************************************
            //Desplazamiento lateral enemigo (Lanza pelotas).

            if(izq==1){
                if(x2>0){
                    x2--;
                    if(x2==0)
                        izq=0;
                }
            }else{
                if(x2<750){
                    x2++;
                    if(x2==750)
                        izq=1;
                }
            }
            //**********************************************************************
            //Obstaculo

            draw_sprite(Temp,Obstaculo,x4,y4);

            if(izqObs==1){
                if(x4>0){
                    x4--;
                    if(x4==0)
                        izqObs=0;
                }
            }else{
                if(x4<720){
                    x4++;
                    if(x4==720)
                        izqObs=1;
                }
            }

            //****************************************************************************
            //Comportamiento Pelota  (x3,y3)

            draw_sprite(Temp,Pelota,x3,y3);

            textprintf(Temp,font,740,15,0xFFFFFF,"P = %d",Pelotas,0x4286f4);

            if( x3+6>=x1+48 && (x3+6)<x1+77 && (y3+12)>=y1+36 && (y3+12)<=y1+70 && key[KEY_X] ){
                P=28-PuntoColision(x1+48,x3+6);
                BajarP=0;
                BajarPIzq=0;
                BajarPDer=0;
                SubirPIzq=1;
            }

            if( x3+6>=x1+77 && x3+6<=x1+105 && (y3+12)>=y1+36 && (y3+12)<=y1+70 && key[KEY_X] ){
                P=PuntoColision(x1+77,x3+6);
                BajarP=0;
                BajarPIzq=0;
                BajarPDer=0;
                SubirPDer=1;
            }

            if(BajarP){
                y3++;
                if(y3>=600){
                    x3=x2+19;
                    y3=y2+26;
                    Pelotas--;
                }
                if(x3+6 >= x4 && x3+6 <= x4+80 && y3+12>=150 && y3<=160){
                    BajarP=0;
                    SubirPIzq=1;
                }
            }

            if(SubirPIzq){
                y3=y3-2;
                if(x3>0){
                    if(P<20){
                        x3=x3-(int)((P*2)*0.1);
                    }else{
                        x3=x3-(int)((P)*0.1);
                    }
                }
                if(x3<=0){
                    SubirPIzq=0;
                    SubirPDer=1;
                }
                if(y3<=0 || x3+6>=370 && x3+6<=430 && y3<=12 || x3+6>=125 && x3+6<=245 && y3<=12 || x3+6>=555 && x3+6<=675 && y3<=12 || x3+6 >= x4 && x3+6 <= x4+80 && y3<=165 && y3>=155){
                    SubirPIzq=0;
                    BajarPIzq=1;
                }
            }

            if(SubirPDer){
                y3=y3-2;
                if(x3+12<800){
                    if(P<20){
                        x3=x3+(int)((P*2)*0.1);
                    }else{
                        x3=x3+(int)((P)*0.1);
                    }
                }
                if(x3+12>=800){
                    SubirPDer=0;
                    SubirPIzq=1;
                }
                if(y3<=0 || x3+6>=370 && x3+6<=430 && y3<=12 || x3+6>=125 && x3+6<=245 && y3<=12 || x3+6>=555 && x3+6<=675 && y3<=12 || x3+6 >= x4 && x3+6 <= x4+80 && y3<=165 && y3>=155){
                    SubirPDer=0;
                    BajarPDer=1;
                }
            }

            if(BajarPIzq){
                y3=y3+2;
                if(x3>0){
                    x3=x3-(int)((P*10)*0.01);
                }
                if(x3<=0){
                    BajarPIzq=0;
                    BajarPDer=1;
                }
                if(y3>=600){
                    BajarPIzq=0;
                    BajarP=1;
                    Pelotas--;
                }
                if(x3+6 >= x4 && x3+6 <= x4+80 && y3+12>=150 && y3<=160){
                    BajarPIzq=0;
                    SubirPIzq=1;
                }
            }

            if(BajarPDer){
                y3=y3+2;
                if(x3+12<800){
                    x3=x3+(int)((P)*0.1);
                }
                if(x3+12>=800){
                    BajarPDer=0;
                    BajarPIzq=1;
                }
                if(y3>=600){
                    BajarPDer=0;
                    BajarP=1;
                    Pelotas--;
                }
                if(x3+6 >= x4 && x3+6 <= x4+80 && y3+12>=150 && y3<=160){
                    BajarPDer=0;
                    SubirPDer=1;
                }
            }

            //****************************************************************************
            //Objetivos

            //blit(_1000pC1,Escenario,0,0,0,0,800,600);
            if(x3+6>=370 && x3+6<=430 && y3<=15){
                masked_blit(_1000pC2,Escenario,0,0,370,0,60,15);
                Puntuacion+=500; //(+1000)
                //draw_sprite(screen,_1000pC2,370,0);
            }else{
                masked_blit(_1000pC1,Escenario,0,0,370,0,60,15);
                //draw_sprite(screen,_1000pC1,370,0);
            }

            if(x3+6>=125 && x3+6<=245 && y3<=15){
                masked_blit(_500pC2,Escenario,0,0,125,0,120,15);
                Puntuacion+=250; //(+1000)
                //draw_sprite(screen,_1000pC2,370,0);
            }else{
                masked_blit(_500pC1,Escenario,0,0,125,0,120,15);
                //draw_sprite(screen,_1000pC1,370,0);
            }

            if(x3+6>=555 && x3+6<=675 && y3<=15){
                masked_blit(_500pC2,Escenario,0,0,555,0,120,15);
                Puntuacion+=250; //(+1000)
                //draw_sprite(screen,_1000pC2,370,0);
            }else{
                masked_blit(_500pC1,Escenario,0,0,555,0,120,15);
                //draw_sprite(screen,_1000pC1,370,0);
            }

            //****************************************************************************
            //Dibujar en pantalla

            //draw_sprite(Fondo,Mono,x,y);
            //blit(Escenario,screen,0,0,0,0,800,600);

            textprintf(Temp,font,20,15,0xFFFFFF,"Puntuacion: %ld",Puntuacion,0x4286f4);
            //draw_sprite(Temp,Escenario,0,0);

            //draw_sprite(screen,Enemigo,x2,y2);

            draw_sprite(screen,Temp,0,0);

            //****************************************************************************

            if(Pelotas<1){
                textprintf(screen,font,360,260,0xFFFFFF,"Sin pelotas :(",0x4286f4);
                clear_keybuf();
                tecla=readkey();
                textprintf(screen,font,340,300,0xFFFFFF,"Ingresa tu nombre:",0x4286f4);

                for(i=0;i<30;i++){

                    clear_keybuf();
                    Nombre[i]=readkey();

                    if(key[KEY_ENTER]){
                        Nombre[i]='\0';
                        break;
                    }
                    /*
                    if(key[KEY_BACKSPACE]){
                        i--;
                    }*/
                    textprintf(screen,font,360+12*i,322,0xFFFFFF,"%c",Nombre[i],0x4286f4);
                }

                CrearPuntuaciones(Nombre,Puntuacion);
                stop_sample(AudioJuego);
                Menu();
            }
    }
    //****************************************************************************************
    allegro_exit();
}

void CrearPuntuaciones(char Nombre[],long Puntuacion){

    FILE *file,*TmpFile; Jugador Tmp, Vec[30]; int i=0,x,n,PrevFile=0;

    //*****************************
    //Leer archivo
    //Meterlo a un vector (30 espacios)
    //Ordenar vector
    //Comparar nueva puntuacion con el vector
    //Vector a archivo binario
    //*****************************

    TmpFile=fopen("Puntuaciones.dat","rb");

    if(TmpFile){

        while( fread(&Tmp,1,sizeof(Jugador),TmpFile) ){
            Vec[i]=Tmp;
            i++;
        }

        n=i;
        fclose(TmpFile);
        PrevFile=1;
    }

    if(PrevFile){

        OrdBurbInv(Vec,n);

        strcpy(Tmp.Nomb,Nombre);
        Tmp.Puntuacion=Puntuacion;

        for(i=0;i<n;i++){

            if(Tmp.Puntuacion>=Vec[i].Puntuacion){
                x=i;

                if(n<30)
                    n++;

                for(i=n-1;i>x;i--){
                    Vec[i]=Vec[i-1];
                }

                Vec[x]=Tmp;
                break;
            }
        }

        if( Tmp.Puntuacion < Vec[n-1].Puntuacion && n<30){
            Vec[n]=Tmp;
            n++;
        }

        file=fopen("Puntuaciones.dat","wb");

        if(file){

            for(i=0;i<n;i++){
                Tmp=Vec[i];
                fwrite(&Tmp,1,sizeof(Jugador),file);
            }

            fclose(file);
        }

    }else{

        file=fopen("Puntuaciones.dat","a+b");

        strcpy(Tmp.Nomb,Nombre);
        Tmp.Puntuacion=Puntuacion;

        if(file){
            fwrite(&Tmp,1,sizeof(Jugador),file);
            fclose(file);
        }

    }

}

void Puntuaciones(void){

    BITMAP *Puntuaciones = load_bitmap("IMG/Puntuaciones.bmp",NULL);
    BITMAP *PuntuacionesOp1 = load_bitmap("IMG/PuntuacionesOp1.bmp",NULL);
    BITMAP *Despl1 = load_bitmap("IMG/Desplazamiento1.bmp",NULL);
    BITMAP *Despl2 = load_bitmap("IMG/Desplazamiento2.bmp",NULL);
    BITMAP *Cursor = load_bitmap("IMG/Cursor.bmp",NULL);

    BITMAP *Temp = create_bitmap(800,600);
    BITMAP *Temp2 = create_bitmap(800,600);

    set_mouse_sprite(Cursor);
    show_mouse(screen);
    set_mouse_range(0,0,800,600);

    int op=0,y=150,i=0,NoPunt=1,CargPunt=0,n,yT=131,yB=131,yP=0,P,PC; Jugador Vec[30];


    FILE *BinFile; Jugador Tmp;

    BinFile=fopen("Puntuaciones.dat","rb");

    if(BinFile){

        while( fread(&Tmp,1,sizeof(Jugador),BinFile) ){
            //printf("%s: %ld\n",Tmp.Nomb,Tmp.Puntuacion);
            Vec[i]=Tmp;
            i++;
        }

        n=i;

        fclose(BinFile);
        NoPunt=0;
    }


    while(!key[KEY_ESC]){

        masked_blit(Puntuaciones,Temp,0,0,0,0,800,600);

        if( mouse_x>=25 && mouse_x<=180 && mouse_y>=560 && mouse_y<=582 ){
            masked_blit(PuntuacionesOp1,Temp,0,0,0,0,800,600);
            if(mouse_b &1)
                Menu();
        }

        if(key[KEY_LEFT] && op<30 ){
            op++;
        }

        if(op>0 && op<=30){
            masked_blit(PuntuacionesOp1,Temp,0,0,0,0,800,600);
            if(key[KEY_ENTER] || key[KEY_X])
                Menu();
        }

        //************************************************************************************************************

        if(!CargPunt){

            for(i=0;i<n;i++){
                textprintf(Temp2,font,300,y,0xFFFFFF,"%d %s: %ld",i+1,Vec[i].Nomb,Vec[i].Puntuacion,0x4286f4);
                y=y+15;
            }
            CargPunt=1;
        }

        //************************************************************************************************************

        if(NoPunt)
            textprintf(Temp,font,320,270,0xFFFFFF,"No hay puntuaciones",0x4286f4);


        //************************************************************************************************************
        //Teclado

        if( key[KEY_DOWN] && yT+150<450 )
            yT++; yB=yT; draw_sprite(Temp,Despl2,552,yT);

        if( key[KEY_UP] && yT>131 )
            yT--; yB=yT; draw_sprite(Temp,Despl2,552,yT);

        if( !key[KEY_UP] && !key[KEY_DOWN])
            draw_sprite(Temp,Despl1,552,yT);

        //************************************************************************************************************
        //Mouse

        if( mouse_x>=552 && mouse_x<=570 && mouse_y>=yB && mouse_y<=yB+150 && mouse_b &1 ){

            //textprintf(Temp,font,310,300,0xFFFFFF,"%d",PuntoColision(yB,mouse_y),0x4286f4);

            /*
            if( yB>130 && yB+150<450 ){
                yB=mouse_y-PuntoColision(yB,mouse_y);
            }*/

            if( mouse_y-75>130 && mouse_y+75<=450 ){
                //yB=mouse_y-PuntoColision(yB,mouse_y);
                yB=mouse_y-75;
            }

            if(mouse_y>280 && yP+600>450)
                yP=yP-(yB-130)*0.01;

            if(mouse_y<=280)
                yP=yP+(yB-130)*0.01;

            yT=yB;
            draw_sprite(Temp,Despl2,552,yB);
        }
        //************************************************************************************************************
        //Desplazamiento puntuaciones

        if( yP<0 && key[KEY_UP] && !mouse_b &1 )
            yP++;

        if( yP+600>450 && key[KEY_DOWN] && !mouse_b &1 )
            yP--;

        //************************************************************************************************************

        blit(Temp,screen,0,0,0,0,800,600);
        draw_sprite(Temp,Temp2,0,yP);
        //blit(Temp2,Temp,0,0,0,0,800,600);
    }
}

int PuntoColision(int x1,int x3){

    int x=0;

    while(x1!=x3){
        x1++;
        x++;
        if(x1==x3){
            return x;
        }
    }
    return x;
}

void OrdBurbInv(Jugador Vec[],int n){

    int i,j; Jugador Temp;

	for( i=0 ;i<n-1; i++ ){

		for( j=i+1 ;j<n; j++ ){

			if(Vec[j].Puntuacion>Vec[i].Puntuacion){
				Temp=Vec[j];
				Vec[j]=Vec[i];
				Vec[i]=Temp;
			}
		}
	}

}
