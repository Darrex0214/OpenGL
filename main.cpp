//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================


#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include <FreeImage.h> //*** Para Textura: Incluir librer�a

//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader* shader1; //Para Textura: variable para abrir los shader de textura
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   GLMmodel* bunny;
   GLMmodel* objmodel_ptr1; //*** Para Textura: variable para objeto texturizado
   GLMmodel* spidey; //
   GLMmodel* mushrooms; //
   GLMmodel* ant;
   GLMmodel* mountain;//
   GLMmodel* ocean;//
   GLuint texid; //*** Para Textura: variable que almacena el identificador de textura
   GLuint texid2;
   GLuint texid3;


public:
	myWindow(){}

	//*** Para Textura: aqui adiciono un m�todo que abre la textura en JPG
	void loadTexture(const char* filePath, GLuint& texid, GLint magFilter, GLint minFilter)
	{
		int w, h;
		GLubyte* data = 0;

		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

		// Load the texture
		FIBITMAP* textureImage = FreeImage_Load(FreeImage_GetFileType(filePath, 0), filePath);
		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(textureImage);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);

		glEnable(GL_TEXTURE_2D);
	}

	void initialize_textures()
	{
		loadTexture("./Mallas/Mushrooms1.jpg", texid, GL_NEAREST, GL_NEAREST);
		loadTexture("./Mallas/montana.jpg", texid2, GL_LINEAR, GL_LINEAR);
		loadTexture("./Mallas/Ocean.png", texid3, GL_LINEAR, GL_LINEAR);
	}



	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      //timer010 = 0.09; //for screenshot!

      glPushMatrix();
	  glRotatef(timer010 * 360, 0.5, 1.0f, 0.1f);

      if (shader) shader->begin();
		  
		  glPushMatrix();
		  glTranslatef(-1.5, -0.5, 0.0);
		  glmDraw(bunny, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();


		  glPushMatrix();
		  glTranslatef(3, -0.5, 0.0);
		  glmDraw(ant, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();

		  glPushMatrix();
		  glTranslatef(0.0, -0.5, 3);
		  glmDraw(spidey, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();


		  


		  glPushMatrix();
		  glTranslatef(0.0f, -1.5f, 0.0f);
		  glScalef(3, 0.001, 3);
		  glutSolidCube(10);
		  glPopMatrix();



	      //glutSolidTeapot(1.0);
      if (shader) shader->end();

	  //*** Para Textura: llamado al shader para objetos texturizados
	  if (shader1) shader1->begin();


		  glPushMatrix();
		  glTranslatef(1.5, -0.5, 0.0);
		  glBindTexture(GL_TEXTURE_2D, texid);
		  glmDraw(mushrooms, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();


		  glPushMatrix();
		  glTranslatef(-10.0, 4.4, 7);
		  glScalef(27, 29, 8);
		  glBindTexture(GL_TEXTURE_2D, texid2);
		  glmDraw(mountain, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();

		  glPushMatrix();
		  glTranslatef(20.0, -2.0, 0.0);
		  glScalef(8, 29, 20);
		  glBindTexture(GL_TEXTURE_2D, texid3);
		  glmDraw(ocean, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();



		 



	  //glutSolidTeapot(1.0);
	  if (shader1) shader1->end();




      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

	 //*** Para Textura: abre los shaders para texturas
		shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
		if (shader1 == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		else
		{
			ProgramObject = shader1->GetProgramObject();
		}

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;

	  //Abrir mallas
	  bunny = NULL;

	  if (!bunny)
	  {
		  bunny = glmReadOBJ("./Mallas/bunny.obj");
		  if (!bunny)
			  exit(0);

		  glmUnitize(bunny);
		  glmFacetNormals(bunny);
		  glmVertexNormals(bunny, 90.0);
	  }



	  //*** Para Textura: abrir malla de objeto a texturizar

	  spidey = NULL;

	  if (!spidey)
	  {
		  spidey = glmReadOBJ("./Mallas/Spidey.obj");
		  if (!spidey)
			  exit(0);

		  glmUnitize(spidey);
		  glmFacetNormals(spidey);
		  glmVertexNormals(spidey, 90.0);
	  }
	  
	  mushrooms = NULL;

	  if (!mushrooms)
	  {
		  mushrooms = glmReadOBJ("./Mallas/Mushrooms1.obj");
		  if (!mushrooms)
			  exit(0);

		  glmUnitize(mushrooms);
		  glmFacetNormals(mushrooms);
		  glmVertexNormals(mushrooms, 90.0);
	  }

	  ant = NULL;

	  if (!ant)
	  {
		  ant = glmReadOBJ("./Mallas/Free_Ant_.obj");
		  if (!ant)
			  exit(0);

		  glmUnitize(ant);
		  glmFacetNormals(ant);
		  glmVertexNormals(ant, 90.0);
	  }


	  mountain = NULL;

	  if (!mountain)
	  {
		  mountain = glmReadOBJ("./Mallas/montana.obj");
		  if (!mountain)
			  exit(0);

		  glmUnitize(mountain);
		  glmFacetNormals(mountain);
		  glmVertexNormals(mountain, 90.0);
	  }

	  ocean = NULL;

	  if (!ocean)
	  {
		  ocean = glmReadOBJ("./Mallas/Ocean.obj");
		  if (!ocean)
			  exit(0);

		  glmUnitize(ocean);
		  glmFacetNormals(ocean);
		  glmVertexNormals(ocean, 90.0);
	  }


 
	  //*** Para Textura: abrir archivo de textura
	  initialize_textures();
      DemoLight();

	}

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(45,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   
   void DemoLight()
   {
	   // Habilitar iluminaci�n y luces
	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);

	   // Posici�n de la luz
	   GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	   // Dibujar objetos con diferentes colores y materiales
	   glPushMatrix();

	   // Material rojo
	   GLfloat material_Ka[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	   GLfloat material_Kd[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	   GLfloat material_Se = 128.0f;
	   glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
	   glMaterialf(GL_FRONT, GL_SHININESS, material_Se);
	   glTranslatef(-1.5f, 0.0f, 0.0f);
	   glutSolidSphere(1.0, 20, 20);

	   // Material verde
	   GLfloat material_Kd_green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd_green);
	   glTranslatef(3.0f, 0.0f, 0.0f);
	   glutSolidSphere(1.0, 20, 20);

	   // Material azul
	   GLfloat material_Kd_blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd_blue);
	   glTranslatef(-1.5f, -3.0f, 0.0f);
	   glutSolidSphere(1.0, 20, 20);

	   glPopMatrix();

	   // Deshabilitar iluminaci�n
	   glDisable(GL_LIGHTING);
	   glDisable(GL_LIGHT0);
   }






};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------