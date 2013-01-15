#include "util.h"
#include "bsp.h"
#include "camera.h"
#include "frustum.h"
#include "Logger.h"

#define WIDTH 1280
#define HEIGHT 720

myfrustum g_frustum;
bool g_noclip = true;

int main(int argc, char **argv)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    Logger::Log(Logger::DEBUG, "Die SDL konnte nicht initialisiert werden (%s)", SDL_GetError());
  }

  IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  SDL_WM_SetCaption("Test", "Test2");
  SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);

  // TODO: after initializing glew check if all needed functions are available.. fall back if not or just quit
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    Logger::Log(Logger::ERROR, "Error: %s", glewGetErrorString(err));
  } 
  Logger::Log(Logger::ERROR, "Status: Using GLEW %s", glewGetString(GLEW_VERSION));


  vec3f vEyePt( -10.0, 10.0, 20.0 );

  SDL_WarpMouse(WIDTH/2, HEIGHT/2);

  bsp *map = new bsp("maps\\q3dm6.bsp");


  camera g_cam(&vEyePt, map);

  glEnable(GL_DEPTH_TEST); 
  glDisable(GL_LIGHTING);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);

#ifndef __USE_SHADERS__
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
#endif

  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (float)WIDTH/HEIGHT, 1, 10000);

  glMatrixMode(GL_MODELVIEW);

  unsigned int ticks = 0;   
  unsigned int delta = 0;

  while (true)
  {
    delta = SDL_GetTicks() - ticks;

    g_cam.updateTime(delta);

    ticks = SDL_GetTicks(); 

    SDL_Event event;

    while (SDL_PollEvent(&event)) 
    {
      switch (event.type)
      {
      case SDL_QUIT:
        SDL_Quit();
        exit(0);
        break;
      case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
        {    
          SDL_Quit();
          exit(0);
        }
        if(event.key.keysym.sym == SDLK_n)
        {
          g_noclip = !g_noclip;
          Logger::Log(Logger::DEFAULT, "noclip is %i", g_noclip);
        }
        break;
      default:
        break;
      }
    }                      

    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_a]) g_cam.strafe(-1.0f);
    if (keystate[SDLK_d]) g_cam.strafe(1.0f);
    if (keystate[SDLK_w]) g_cam.move(-1.0f);
    if (keystate[SDLK_s]) g_cam.move(1.0f);

    int x,y;
    SDL_PumpEvents();
    SDL_GetMouseState(&x, &y);
    SDL_WarpMouse(400, 300);
    float rx = ((((float)x)-400.f)/100.f);
    float ry = ((((float)y)-300.f)/100.f);

    if (rx != 0 || ry != 0)
    {  
      g_cam.pitch(-ry);
      g_cam.yaw(-rx);
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear color and depth buffer

    glLoadIdentity(); // Reset orientation               

    glMultMatrixf(g_cam.GetMatrix());
    glMultMatrixf(quake2oglMatrix);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    mat4f mm, pm;
    float m[16], p[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, m); 
    glGetFloatv(GL_PROJECTION_MATRIX, p);

    mm = m;
    pm = p;

    g_frustum.extract_planes(mm, pm);

    // Graphical commands go here
    map->render(g_cam.position_, ((float)ticks)/1000.0f);
    SDL_GL_SwapBuffers();

    //if (delta != 0)	
    //  std::cout << "frametime in ms: " << delta << " fps: " << 1000 / delta << std::endl;  

    SDL_ShowCursor(SDL_DISABLE);
  }

  IMG_Quit();
  SDL_Quit();
  return 0;
}
