// VRPN Imager Server example program.

#include <stdio.h>
#include "vrpn_Connection.h"
#include "vrpn_Imager.h"

static	vrpn_Connection	    *g_connection = NULL;
static	vrpn_Imager_Server  *g_is = NULL;
const	unsigned	    g_sizeX = 1024 , g_sizeY = 768;

// Fill a buffer of 8-bit integers with an image that has a diagonal
// line of growing brightness.  The size of the image is passed in, as
// well as an offset for the brightnesses to make the image change
// a bit over time.

static	void  fill_buffer(vrpn_uint8 *buffer,
			  const unsigned numX, const unsigned numY,
			  const unsigned offset)
{
  unsigned x,y;
  for (x = 0; x < numX; x++) {
    for (y = 0; y < numY; y++) {
      buffer[x + y*numX] = (x + y + offset) % 256;
    }
  }
}

#ifndef min
#define	min(a,b) ( (a) < (b) ? (a) : (b) )
#endif
int main (int argc, char * argv[])
{
  static  int frame_number = 0;
  static  vrpn_uint8 buffer[g_sizeX*g_sizeY];
  int	  channel_id;

  // Need to have a global pointer to it so we can shut it down
  // in the signal handler (so we can close any open logfiles.)
  //vrpn_Synchronized_Connection	connection;
  if ( (g_connection = vrpn_create_server_connection()) == NULL) {
    fprintf(stderr, "Could not open connection\n");
    return -1;
  }

  // Open the imager server and set up channel zero to send our data.
  if ( (g_is = new vrpn_Imager_Server("TestImage", g_connection, g_sizeX, g_sizeY)) == NULL) {
    fprintf(stderr, "Could not open imager server\n");
    return -1;
  }
  if ( (channel_id = g_is->add_channel("Slope")) == -1) {
    fprintf(stderr, "Could not add channel\n");
    return -1;
  }

  // Generate about ten frames per second by sending one and then sleeping.
  // Better would be to mainloop() the connection much faster, and then
  // only send when it is time.  This will do, but the connections might
  // not open as fast as they would otherwise.

  // It would be nice to let the user stop the server cleanly, but ^C
  // will do the trick.

  while (1) {
    fill_buffer(buffer, g_sizeX, g_sizeY, frame_number++ % 256);
    g_is->send_begin_frame(0, g_sizeX-1, 0, g_sizeY-1);
    g_is->mainloop();
    int nRowsPerRegion=vrpn_IMAGER_MAX_REGIONu8/g_sizeX;
    int y;
    for(y=0; y<g_sizeY; y+=nRowsPerRegion) {
      g_is->send_region_using_base_pointer(channel_id,0,g_sizeX-1,y,min(g_sizeY,y+nRowsPerRegion)-1, buffer, 1, g_sizeX, g_sizeY);
      g_is->mainloop();
    }
    g_is->send_end_frame(0, g_sizeX-1, 0, g_sizeY-1);

    g_is->mainloop();
    g_connection->mainloop();
    vrpn_SleepMsecs(10);
  }

  return 0;
}
