#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include "ImageData.h"
#include "ImageIO.h"
#include "GenericTimer.h"

int g_WindowWidth = 360;    /* ウィンドウの横幅 */
int g_WindowHeight = 360;   /* ウィンドウの縦幅 */
int g_MessageStartX = 20;
int g_MessageStartY = 260;
int panel[16]; /*パネル位置判定に使用*/
int F =0; /* タイトル画面は0,ゲーム中は1,クリア画面は2 で表す*/
#define MESSAGE_FILENAME "message.txt"
#define BUFSIZE 8192
ImageData g_Image[16]; /*画像データ*/
ImageData restart;
char g_MessageBuffer[BUFSIZE];



/*テキストファイル読込*/
int LoadStringFromFile(const char *filename, char *buf, int bufsize)
{
  int nReadTotal = 0;
  FILE *fp = fopen(filename, "r");

  if (fp == NULL){
    fprintf(stderr, "LoadStringFromFile: cannot open \"%s\"\n", filename);
    return 0;
  }

  while ( ! feof(fp) && nReadTotal < bufsize){
    fgets(&buf[nReadTotal], bufsize-nReadTotal, fp);
    nReadTotal += strlen(&buf[nReadTotal]);
  }

  fclose(fp);
  return 1;
}

/* 初期化処理 */
void init(void)
{
  for(int i=0;i<16;i++){
    /*InitImageData(&g_Image[i]);   画像データを初期化 */
    glClearColor(1.0, 1.0, 1.0, 1.0);  /* ウィンドウを消去するときの色を設定 */

  char file[16];
  sprintf(file,"%d.ppm",i+1);
    if (LoadPPMImage(file, &g_Image[i]) ){  /* 画像データの読み込みに成功 */
      FlipImageData(&g_Image[i]);  /* 画像の上下を反転する */
    }
  }
  if (LoadPPMImage("restart.ppm", &restart)){  /* 画像データの読み込みに成功 */
    FlipImageData(&restart);  /* 画像の上下を反転する */
  }

  if ( ! LoadStringFromFile(MESSAGE_FILENAME, g_MessageBuffer, BUFSIZE)){
    sprintf(g_MessageBuffer, "Error: cannot open \"%s\"", MESSAGE_FILENAME);
  }
}

void DisplayString(const char *str, int xi, int yi){
  int i, yStart = yi;

  glRasterPos2i(xi,yStart);
  for (i=0; str[i] != '\0'; i++){
    if (str[i] == '\n'){
      yStart -= 24;
      glRasterPos2i(xi,yStart);
    } else {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
    }
  }

  char a[100];
  double b =GetTime()/1000;
  sprintf(a,"%f",b);
  for(i=0;a[i]!='\0';i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, a[i]);
  }

  glRasterPos2i(0, 0);
}

/* 表示処理のためのコールバック関数 */
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);    /* ウィンドウを消去 … glClearColor で指定した色で塗りつぶし */

  if(F==0){
    for(int i=0;i<16;i++){
      if ( IsImageDataAllocated(&g_Image[i])){
      DrawImageData(&g_Image[i], (i%4)*64, 192-(i/4)*64);
      }
    }

  } else if(F==1){
    for(int i=0;i<16;i++){
      if ( IsImageDataAllocated(&g_Image[panel[i]])){
        DrawImageData(&g_Image[panel[i]], (i%4)*64, 192-(i/4)*64);
      }
    }
    if ( IsImageDataAllocated(&restart)){
      DrawImageData(&restart, 270, 328);
    }
  } else if(F==2){
    glColor3ub(0,0,0);
    DisplayString(g_MessageBuffer, g_MessageStartX, g_MessageStartY);
    if ( IsImageDataAllocated(&restart)){
      DrawImageData(&restart, 270, 328);
    }
  }

  glutSwapBuffers();
}

/* ウィンドウサイズが変更されたときのためのコールバック関数 */
void reshape(int w, int h)
{
  if (h < 1) return;

  g_WindowWidth = w;
  g_WindowHeight = h;

  glViewport(0, 0, w, h); /* ウィンドウの描画範囲を指定 */

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);

  glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}

/*i番目とj番目のパネルを入れ替える関数*/
void change(int i, int j)
{
  int a = j*4 +i;
  int b = -1;
  if (i > 0 && panel[a - 1] == 15) b = a - 1;
  if (i < 3 && panel[a + 1] == 15) b = a + 1;
  if (j > 0 && panel[a - 4] == 15) b = a - 4;
  if (j < 3 && panel[a + 4] == 15) b = a + 4;
  if (b != -1) {
    panel[b] = panel[a];
    panel[a] = 15;
  }
}

/* マウス入力時の関数*/
void mouse(int button,int state, int x, int _y) {
  if(x>=270 && x<=350 && _y>=16 && _y<=32) F=0; /*マウスやキーボードの入力で、最初から実行し直すことができる*/

  if(F==0){ /*タイトル*/
    if (state==GLUT_DOWN && button==GLUT_LEFT_BUTTON) {  /*初期パネルのシャッフル*/
      for (int i=0;i<16;i++) panel[i] = i; /*ひとまずパネル16個設定*/ 
      srand((unsigned)time( NULL ));  /*乱数生成*/
      for (int i=0;i<1000;i++){change((rand())%4, (rand())%4);}
      F =1;
      StartTimer();
    }

  } else if(F==1){ /*ゲーム進行*/
    int y = g_WindowHeight - _y;
    if (button==GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      change(x/64, 3-(y/64));
    }
   
    int clear =1; /*クリアなら1,違うなら0*/ 
    for (int i=0; i<16; i++) {if (panel[i] != i) clear = 0;}
    if(clear ==1) F=2; 

  } else if(F==2){ /*クリア画面*/
    printf(" %lf ",GetTime()/1000);
  }

  glutPostRedisplay();     /*ウィンドウ描画関数を呼ぶ */  
}

void keyboard(unsigned char key, int x, int y){
  switch (key){
    case 'q':   /* キーボードの 'q' 'Q' 'ESC' を押すとプログラム終了 */
    case 'Q':
    case '\033':
      exit(-1);
      break;
  }
  glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(g_WindowWidth, g_WindowHeight);
  glutCreateWindow("15puzzle");

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutReshapeFunc(reshape);

  init();
  glutMainLoop();

  return 0;
}
