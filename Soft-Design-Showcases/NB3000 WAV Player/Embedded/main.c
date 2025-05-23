/*****************************************************************************\
|*
|*  IN PACKAGE:         Software Platform Builder
|*
|*  COPYRIGHT:          Copyright (c) 2008, Altium
|*
|*  DESCRIPTION:        Shows how to use the generic Audio driver
|*
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fs.h>
#include <unistd.h>
#include <sys/stat.h>

// Application Stack interface
#include <devices.h>
#include <timing.h>
#include <audio.h>
#include <drv_sdhc.h>

#include "wave.h"
#include "form1.h"


#define DIR_SEP             "/"
#define FAILED()    { printf( "*** Failed ***\n" ); }


static void init( void );

audio_t * audio;
sdhc_t * sdcard;
agui_t * agui;
static uint8_t wavbuf[4096];


/**********************************************************************
|*
|*  FUNCTION    : main
|*
|*  PARAMETERS  : None
|*
|*  RETURNS     : None
|*
|*  DESCRIPTION : main program
 */

void main( void )
{
    int err;
    int handle = 0;
    int size = -1;
    int progress;
    bool start = true;

    init();

    for (;;)
    {
        puts( "Please insert SD card 'PB02 Sample Data' in NB3000..." );
        while( !sdhc_card_detect( sdcard ) )   // Wait for card to be inserted
            ;

        puts( "Card detected..." );

        for ( int i = 0; i < 10; i++ )
        {
            err = sdhc_card_init( sdcard, SDHC_INIT_POWERON );
            if ( sdhc_is_memcard(err) ) break;
        }

        if ( !sdhc_is_memcard(err) )
        {
            puts( "Not a valid memory card, init failed" );

            continue;
        }
        else
        {
            puts( "Card init OK" );
        }

        /* Try to mount partition #1 */
        err = mount( "/dev/BLOCKIO_1" ,"/sdcard", "fatfs", 1, MOUNT_FLAG_RDONLY);
        if (err != 0)
        {
            /* ... and if that fails try the entire disk */
            err = mount("/dev/BLOCKIO_1", "/sdcard", "fatfs", 0, MOUNT_FLAG_RDONLY);
        }
        if ( err != 0 )
        {
            puts( "Mount failed, please remove card!" );
        }
        else
        {
            // Success!
            puts( "Filesystem mounted at \"/sdcard\"" );
            break;
        }

        while( sdhc_card_detect( sdcard ) )
            ;
        puts( "Card removed" );
    }

    dirlisting("/sdcard");
    listbox_sort(&form1_dirs, false);

    for (;;)
    {
        agui_service(agui);

        if (play && start)
        {
            printf( "Playing file \"%s\"\n", dir_file );
            handle = open(dir_file, O_RDONLY);
        }
        if (play)
        {
            size = read(handle, wavbuf, sizeof(wavbuf));
            progress = wav_playfile(audio, wavbuf, size, handle, start);
            progressbar_set_percentage(AGUI_HANDLE(form1_progress), progress);
        }
        if ((!play && !start) || (size == 0))
        {
            close(handle);
            obj_set_enabled(AGUI_HANDLE(form1_play), true);
            obj_set_enabled(AGUI_HANDLE(form1_stop), false);
            obj_set_enabled(AGUI_HANDLE(form1_files), true);
            obj_set_enabled(AGUI_HANDLE(form1_dirs), true);
            play = false;
            size = -1;
        }
        start = !play;
    }
}

/**********************************************************************
|*
|*  FUNCTION    : init
|*
|*  PARAMETERS  : None
|*
|*  RETURNS     : None
|*
|*  DESCRIPTION : Initialize hardware and drivers
 */

static void init( void )
{
    // Say hello to the user
    puts( "Audio service example, " __FILE__ " compiled " __DATE__ ", " __TIME__ );

    // Initialize audio
    printf( "OK\nInitializing audio... " );
    if ( audio = audio_open( AUDIO_1 ), !audio )
    {
        puts( "Failed to initialize Audio Service" );
        abort();
    }
    if ( sdcard = sdhc_open( DRV_SDHC_1 ), !sdcard )
    {
        puts( "Unable to open SDHC driver" );
        abort();
    }
    agui = agui_open(AGUI_1);
    agui_show_form(AGUI_HANDLE(form1));

    puts( "OK\nInit Ready." );
}


/**********************************************************************
|*
|*  FUNCTION    : dirlisting
|*
|*  PARAMETERS  : path: path of the dir
|*
|*  RETURNS     : None
|*
|*  DESCRIPTION : List the contents of the dir
 */

void dirlisting( const char *path )
{
    DIR             *dir;
    struct dirent   *dirent;
    struct stat     buf;
    char            path_to_file[PATH_MAX];

    chdir(path);
    dir = opendir(path);
    if ( dir != NULL )
    {
        printf( "Reading folder \"%s\"\n\n", path );
        listbox_clear(&form1_dirs);
        listbox_clear(&form1_files);
        while ( 1 )
        {
            dirent = readdir(dir);
            if (dirent == NULL)
            {
                break;
            }
            sprintf(path_to_file, "%s%s%s", path, DIR_SEP, dirent->d_name);
            if (stat(path_to_file, &buf) == 0)
            {
                if (S_ISDIR(buf.st_mode))
                {
                    if (strcmp(dirent->d_name, ".") == 0)
                    {
                        /* skip */
                    }
                    else
                    {
                        listbox_add(&form1_dirs, dirent->d_name, NULL);
                    }
                }
                else if (S_ISREG(buf.st_mode))
                {
                    listbox_add(&form1_files, dirent->d_name, NULL);
                }
                else
                {
                    printf("[UNK]\t%s\n", dirent->d_name);
                }
            }
            else
            {
                FAILED();
            }
        }

        if (closedir(dir) != 0)
        {
            FAILED();
        }
    }
}



