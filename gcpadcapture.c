#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#define DEV_NAME  "/dev/ttyS0"
#define BAUD_RATE B57600
#define BUFF_SIZE 1
#define INP_SIZE  10

void serial_init( int fd )
{
  struct termios tio;
  memset( &tio, 0, sizeof( tio ) );
  tio.c_cflag = CS8 | CLOCAL | CREAD;
  tio.c_cc[ VTIME ] = 100;
  cfsetispeed( &tio, BAUD_RATE );
  cfsetospeed( &tio, BAUD_RATE );
  tcsetattr( fd, TCSANOW, &tio );
}

int main( int argc, char *argv[] ) {
  int fd;
  int i;
  int frame = 0;
  int len = 0;
  int trigger_last = 0;
  int display_enable = 1;
  unsigned char buffer[ BUFF_SIZE ], inp[ INP_SIZE ];
  unsigned char Joystick_X, Joystick_Y, Cstick_X, Cstick_Y, L_throttle, R_throttle;
  unsigned char Start, Y, X, B, A, L, R, Z, D_up, D_down, D_right, D_left;

  fd = open( DEV_NAME, O_RDWR );
  if ( fd < 0 ) {
    perror( argv[ 1 ] );
    exit( 1 );
  }

  serial_init( fd );

  while( 1 ) {
    for ( i = 0; i < INP_SIZE; i++ ) {
      inp[ i ] = 0x00;
    }
    i = 0;

    while ( read( fd, buffer, BUFF_SIZE ) ) {
      if ( buffer[ 0 ] == 0xff ) {
        break;
      } else {
        inp[ i ] = buffer[ 0 ];
        i++;
      }
    }

    Joystick_X = 255 -
      ( ( ( inp[ 2 ] & 0x04 ) ? 1 : 0 ) << 7 ) +
      ( ( ( inp[ 2 ] & 0x08 ) ? 1 : 0 ) << 6 ) +
      ( ( ( inp[ 2 ] & 0x10 ) ? 1 : 0 ) << 5 ) +
      ( ( ( inp[ 2 ] & 0x20 ) ? 1 : 0 ) << 4 ) +
      ( ( ( inp[ 2 ] & 0x40 ) ? 1 : 0 ) << 3 ) +
      ( ( ( inp[ 2 ] & 0x80 ) ? 1 : 0 ) << 2 ) +
      ( ( ( inp[ 3 ] & 0x02 ) ? 1 : 0 ) << 1 ) +
      ( ( inp[ 3 ] & 0x04 ) ? 1 : 0 );
    Joystick_Y = 255 -
      ( ( ( inp[ 3 ] & 0x08 ) ? 1 : 0 ) << 7 ) +
      ( ( ( inp[ 3 ] & 0x10 ) ? 1 : 0 ) << 6 ) +
      ( ( ( inp[ 3 ] & 0x20 ) ? 1 : 0 ) << 5 ) +
      ( ( ( inp[ 3 ] & 0x40 ) ? 1 : 0 ) << 4 ) +
      ( ( ( inp[ 3 ] & 0x80 ) ? 1 : 0 ) << 3 ) +
      ( ( ( inp[ 4 ] & 0x02 ) ? 1 : 0 ) << 2 ) +
      ( ( ( inp[ 4 ] & 0x04 ) ? 1 : 0 ) << 1 ) +
      ( ( inp[ 4 ] & 0x08 ) ? 1 : 0 );
    Cstick_X = 255 -
      ( ( ( inp[ 4 ] & 0x10 ) ? 1 : 0 ) << 7 ) +
      ( ( ( inp[ 4 ] & 0x20 ) ? 1 : 0 ) << 6 ) +
      ( ( ( inp[ 4 ] & 0x40 ) ? 1 : 0 ) << 5 ) +
      ( ( ( inp[ 4 ] & 0x80 ) ? 1 : 0 ) << 4 ) +
      ( ( ( inp[ 5 ] & 0x02 ) ? 1 : 0 ) << 3 ) +
      ( ( ( inp[ 5 ] & 0x04 ) ? 1 : 0 ) << 2 ) +
      ( ( ( inp[ 5 ] & 0x08 ) ? 1 : 0 ) << 1 ) +
      ( ( inp[ 5 ] & 0x10 ) ? 1 : 0 );
    Cstick_Y = 255 -
      ( ( ( inp[ 5 ] & 0x20 ) ? 1 : 0 ) << 7 ) +
      ( ( ( inp[ 5 ] & 0x40 ) ? 1 : 0 ) << 6 ) +
      ( ( ( inp[ 5 ] & 0x80 ) ? 1 : 0 ) << 5 ) +
      ( ( ( inp[ 6 ] & 0x02 ) ? 1 : 0 ) << 4 ) +
      ( ( ( inp[ 6 ] & 0x04 ) ? 1 : 0 ) << 3 ) +
      ( ( ( inp[ 6 ] & 0x08 ) ? 1 : 0 ) << 2 ) +
      ( ( ( inp[ 6 ] & 0x10 ) ? 1 : 0 ) << 1 ) +
      ( ( inp[ 6 ] & 0x20 ) ? 1 : 0 );
    L_throttle = 255 -
      ( ( ( inp[ 6 ] & 0x40 ) ? 1 : 0 ) << 7 ) +
      ( ( ( inp[ 6 ] & 0x80 ) ? 1 : 0 ) << 6 ) +
      ( ( ( inp[ 7 ] & 0x02 ) ? 1 : 0 ) << 5 ) +
      ( ( ( inp[ 7 ] & 0x04 ) ? 1 : 0 ) << 4 ) +
      ( ( ( inp[ 7 ] & 0x08 ) ? 1 : 0 ) << 3 ) +
      ( ( ( inp[ 7 ] & 0x10 ) ? 1 : 0 ) << 2 ) +
      ( ( ( inp[ 7 ] & 0x20 ) ? 1 : 0 ) << 1 ) +
      ( ( inp[ 7 ] & 0x40 ) ? 1 : 0 );
    R_throttle = 255 -
      ( ( ( inp[ 7 ] & 0x80 ) ? 1 : 0 ) << 7 ) +
      ( ( ( inp[ 8 ] & 0x02 ) ? 1 : 0 ) << 6 ) +
      ( ( ( inp[ 8 ] & 0x04 ) ? 1 : 0 ) << 5 ) +
      ( ( ( inp[ 8 ] & 0x08 ) ? 1 : 0 ) << 4 ) +
      ( ( ( inp[ 8 ] & 0x10 ) ? 1 : 0 ) << 3 ) +
      ( ( ( inp[ 8 ] & 0x20 ) ? 1 : 0 ) << 2 ) +
      ( ( ( inp[ 8 ] & 0x40 ) ? 1 : 0 ) << 1 ) +
      ( ( inp[ 8 ] & 0x80 ) ? 1 : 0 );

    Start = ( inp[ 0 ] & 0x08 ) ? 1 : 0;
    Y = ( inp[ 0 ] & 0x10 ) ? 1 : 0;
    X = ( inp[ 0 ] & 0x20 ) ? 1 : 0;
    B = ( inp[ 0 ] & 0x40 ) ? 1 : 0;
    A = ( inp[ 0 ] & 0x80 ) ? 1 : 0;
    L = ( inp[ 1 ] & 0x04 ) ? 1 : 0;
    R = ( inp[ 1 ] & 0x08 ) ? 1 : 0;
    Z = ( inp[ 1 ] & 0x10 ) ? 1 : 0;
    D_up    = ( inp[ 1 ] & 0x20 ) ? 1 : 0;
    D_down  = ( inp[ 1 ] & 0x40 ) ? 1 : 0;
    D_right = ( inp[ 1 ] & 0x80 ) ? 1 : 0;
    D_left  = ( inp[ 2 ] & 0x02 ) ? 1 : 0;

    if( D_down && !trigger_last ) { display_enable = !display_enable; }
    if( D_down ) { trigger_last = 1; } else { trigger_last = 0; }
    if( display_enable ) {
      printf( "%04d\tStart: %01d  Y: %01d  X: %01d  B: %01d  A: %01d  L: %01d  R: %01d  Z: %01d\
\tD-up: %01d  D-down: %01d  D-right: %01d  D-left: %01d\
\tJoy-X: %03d  Joy-Y: %03d  C-X: %03d  C-Y: %03d  Lth: %03d  Rth: %03d\n",
        frame, Start, Y, X, B, A, L, R, Z, D_up, D_down, D_right, D_left,
        Joystick_X, Joystick_Y, Cstick_X, Cstick_Y, L_throttle, R_throttle );
    }

    frame = ( frame + 1 ) % 60;
  }

  return 0;
}
