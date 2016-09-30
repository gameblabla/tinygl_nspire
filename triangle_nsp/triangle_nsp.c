// adapted from GLFW, glfw.sf.net
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <zbuffer.h>
#include "n2DLib.h"

static volatile unsigned *value;
static volatile unsigned *control;
unsigned long tick_sum = 0;
unsigned long start = 0;

/*
Uses the first timer (0x900C0000)
TC: http://hackspire.unsads.com/wiki/index.php/Memory-mapped_I/O_ports#900C0000_-_First_timer
CX: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0271d/Babehiha.html  
*/

void StartTicks(void)
{
	*(volatile unsigned *)0x900B0018 &= ~(1 << 11); /* Enable bus access */
	value = (unsigned *)0x900C0004;
	control = (unsigned *)0x900C0008;
	*(volatile unsigned *)0x900C0080 = 0xA; /* "[...] on the CX to specify the 32768Hz clock as the source for the First Timer" */
	*control = 0b10000010; /* Wrapping; 32-bit; divider to 1; interrupt disabled; free-running; start */
	start = *value;
}

unsigned long GetTicks (void)
{
	return((start - *value) / 33);
}

int main(int argc, char **argv) 
{
    // main loop:
    int frames=0;
    int x=0;
    float  t, t0;
    int running = GL_TRUE;
    t0 = GetTicks()/1000.0;
    // initialize SDL video:
    float winSizeX=320.0f;
    float winSizeY=240.0f;
    float precalculate_resolution;
    
    precalculate_resolution = winSizeX/winSizeY;
    
    initBuffering();
    
    // initialize TinyGL:
    unsigned int pitch;
    int	mode;
    
	pitch = 640;
	mode = ZB_MODE_5R6G5B;
	
    ZBuffer *frameBuffer = ZB_open( winSizeX, winSizeY, mode, 0, 0, 0, 0);
    glInit( frameBuffer );

    // set viewport
    glViewport( 0, 0, winSizeX, winSizeY);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );


    while( running ) 
    {
		t += 0.1f;

        // Clear color buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Select and setup the projection matrix
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluPerspective( 65.0f, precalculate_resolution, 1.0f, 100.0f );

        // Select and setup the modelview matrix
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glRotatef(-90, 1,0,0);
        glTranslatef(0,0,-1.0f);

        // Draw a rotating colorful triangle
        glTranslatef( 0.0f, 14.0f, 0.0f );
        glRotatef( 0.3*(GLfloat)x + (GLfloat)t*100.0f, 0.0f, 0.0f, 1.0f );
        glBegin( GL_TRIANGLES );
        glColor3f( 1.0f, 0.0f, 0.0f );
        glVertex3f( -5.0f, 0.0f, -4.0f );
        glColor3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( 5.0f, 0.0f, -4.0f );
        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex3f( 0.0f, 0.0f, 6.0f );
        glEnd();

        // swap buffers:
        clearBufferB();
        ZB_copyFrameBuffer(frameBuffer, BUFF_BASE_ADDRESS, pitch);
        updateScreen();
        
        if (isKeyPressed(KEY_NSPIRE_ESC)) 
        {
			running = 0;
		}
    }
    // cleanup:
    ZB_close(frameBuffer);
    clearBufferB();
    deinitBuffering();
    return 0;
}
