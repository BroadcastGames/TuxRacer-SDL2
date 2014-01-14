/* 
 * Tux Racer 
 * Copyright (C) 1999-2001 Jasmin F. Patry
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "tuxracer.h"
#include "fonts.h"
#include "gl_util.h"
#include "textures.h"
#include "fps.h"
#include "phys_sim.h"
#include "multiplayer.h"
#include "ui_mgr.h"
#include "racing.h"
#include "game_logic_util.h"
#include "course_load.h"
#include "bonus.h"
#include "hud.h"
#include "platform.h"

#define HUD_SCALE(x)		winsys_scale(x)

#define SECONDS_IN_MINUTE 60

#define TIME_LABEL_X_OFFSET 12.0
#define TIME_LABEL_Y_OFFSET 18.0

#define TIME_X_OFFSET 8.0
#define TIME_Y_OFFSET 8.0

#define SCORE_X_OFFSET 12.0
#define SCORE_Y_OFFSET 12.0

#define HERRING_ICON_HEIGHT HUD_SCALE(30.0)
#define HERRING_ICON_WIDTH HUD_SCALE(48.0)
#define HERRING_ICON_IMG_SIZE HUD_SCALE(64.0)
#define HERRING_ICON_X_OFFSET HUD_SCALE(160.0)
#define HERRING_ICON_Y_OFFSET 25.0
#define HERRING_COUNT_Y_OFFSET 25.0

#define GAUGE_IMG_SIZE HUD_SCALE(128)

#define ENERGY_GAUGE_BOTTOM HUD_SCALE(1.0)
#define ENERGY_GAUGE_HEIGHT HUD_SCALE(103.0)
#define ENERGY_GAUGE_CENTER_X (GAUGE_IMG_SIZE / 2.0 + HUD_SCALE(7.0))
#define ENERGY_GAUGE_CENTER_Y (GAUGE_IMG_SIZE / 2.0 - HUD_SCALE(9.0))

#define GAUGE_WIDTH (GAUGE_IMG_SIZE - 1.0)
#define SPEED_UNITS_Y_OFFSET HUD_SCALE(4.0)

#define SPEEDBAR_OUTER_RADIUS (GAUGE_IMG_SIZE / 2 + HUD_SCALE(6))
#define SPEEDBAR_BASE_ANGLE 225
#define SPEEDBAR_MAX_ANGLE 45
#define SPEEDBAR_GREEN_MAX_SPEED ( MAX_PADDLING_SPEED * M_PER_SEC_TO_KM_PER_H )
#define SPEEDBAR_YELLOW_MAX_SPEED 100
#define SPEEDBAR_RED_MAX_SPEED 160
#define SPEEDBAR_GREEN_FRACTION 0.5
#define SPEEDBAR_YELLOW_FRACTION 0.25
#define SPEEDBAR_RED_FRACTION 0.25

#define FPS_X_OFFSET 12
#define FPS_Y_OFFSET 12

#define USE_TIME_ICON
#define TIME_ICON_SIZE (0.1*getparam_y_resolution())

static GLfloat energy_background_color[] = { 0.2, 0.2, 0.2, 0.5 };
static GLfloat energy_foreground_color[] = { 0.54, 0.59, 1.00, 0.5 };
static GLfloat speedbar_background_color[] = { 0.2, 0.2, 0.2, 0.5 };
static GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };

static void draw_time(player_data_t* plyr)
{
    /*
    font_t *font;
    int minutes, seconds, hundredths;
    char *string;
    int w, asc, desc;
    char *binding;
    char buff[BUFF_LEN];
    scalar_t time_y_refval;
    GLuint texobj;
	int x_org, y_org;
	
	scalar_t time;
    //depending of the calculation mode, we want to display either the time spendt or the time remaining
    //Half_Pipe mode
    if (!strcmp(get_calculation_mode(),"Half_Pipe")) 
    {
		scalar_t par_time = g_game.race.time_req[DIFFICULTY_LEVEL_EASY];
        time = (par_time-g_game.time);
    }
    else if (!strcmp(get_calculation_mode(),"jump") )
    {
        time = (plyr->control.is_flying) ? (g_game.time-plyr->control.fly_start_time+plyr->control.fly_total_time) : plyr->control.fly_total_time;
        time = (time > FLYING_TIME_LIMIT) ? time : 0.0;
    }
    //default mode
    else {
        time = g_game.time;
    }
    
    get_time_components( time, &minutes, &seconds, &hundredths );

    x_org = OVERSCAN_MARGIN_X + TIME_X_OFFSET;
    y_org = getparam_y_resolution() - OVERSCAN_MARGIN_Y - TIME_Y_OFFSET - TIME_ICON_SIZE;

    if ( !get_texture_binding( "time_icon", &texobj ) ) {
        texobj = 0;
    }
    
    glBindTexture( GL_TEXTURE_2D, texobj );

    {
		GLfloat vertices[]={
			x_org, y_org, 0,
			x_org + TIME_ICON_SIZE, y_org, 0,
			x_org + TIME_ICON_SIZE, y_org + TIME_ICON_SIZE, 0,
			x_org, y_org + TIME_ICON_SIZE, 0
		};
		GLfloat texcoords[]={
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};
        
		GLubyte indices[]={0, 1, 2, 2, 3, 0};
		
        /*glEnableClientState (GL_VERTEX_ARRAY);
        glEnableClientState (GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT , 0, vertices);	
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
		
		glDisableClientState (GL_VERTEX_ARRAY);
        glDisableClientState (GL_TEXTURE_COORD_ARRAY);

    }

    time_y_refval = getparam_y_resolution() - TIME_LABEL_Y_OFFSET;

    binding = "time_value";

    if ( ! get_font_binding( binding, &font ) ) {
	print_warning( IMPORTANT_WARNING,
		       "Couldn't get font for binding %s", binding );
	return;
    }

    bind_font_texture( font );

    sprintf( buff, "%02d:%02d", minutes, seconds );

    string = buff;

    get_font_metrics( font, string, &w, &asc, &desc );

    glPushMatrix();
    {
	glTranslatef( TIME_X_OFFSET + OVERSCAN_MARGIN_X + TIME_ICON_SIZE, 
		      time_y_refval - TIME_Y_OFFSET - OVERSCAN_MARGIN_Y - asc, 
		      0 );
	draw_string( font, string );
    }
    glPopMatrix();

    binding = "time_hundredths";

    if ( ! get_font_binding( binding, &font ) ) {
	print_warning( IMPORTANT_WARNING,
		       "Couldn't get font for binding %s", binding );
	return;
    }

    bind_font_texture( font );

    sprintf( buff, "%02d", hundredths );
    string = buff;

    glPushMatrix();
    {
	glTranslatef( TIME_X_OFFSET + OVERSCAN_MARGIN_X + w + 5 + TIME_ICON_SIZE, 
		      time_y_refval - TIME_Y_OFFSET - OVERSCAN_MARGIN_Y,  
		      0 );
	get_font_metrics( font, string, &w, &asc, &desc );
	glTranslatef( 0, -asc-2, 0 );
	draw_string( font, string );
    }
    glPopMatrix();
     */
}

static void draw_herring_count( int herring_count )
{
    char *string;
    char buff[BUFF_LEN];
    GLuint texobj;
    font_t *font;
    char *binding;
    int w, asc, desc;

	GLfloat verticesItem []=
    {
        0, 0, 0,
        HERRING_ICON_WIDTH, 0, 0,
        HERRING_ICON_WIDTH, HERRING_ICON_HEIGHT, 0,
        0, HERRING_ICON_HEIGHT, 0,
        0, 0, 0,
        HERRING_ICON_WIDTH, HERRING_ICON_HEIGHT, 0
    };
        
    GLfloat texCoordsItem []=
    {
        0.0, 0.0 ,
        (GLfloat) HERRING_ICON_WIDTH / HERRING_ICON_IMG_SIZE, 0,
        (GLfloat)HERRING_ICON_WIDTH / HERRING_ICON_IMG_SIZE, (GLfloat)HERRING_ICON_HEIGHT / HERRING_ICON_IMG_SIZE ,
        0, (GLfloat)HERRING_ICON_HEIGHT / HERRING_ICON_IMG_SIZE,
        0.0, 0.0,
        (GLfloat)HERRING_ICON_WIDTH / HERRING_ICON_IMG_SIZE, (GLfloat)HERRING_ICON_HEIGHT / HERRING_ICON_IMG_SIZE ,
    };

	GLubyte indices[]={0, 1, 2, 2, 3, 0};

    set_gl_options( TEXFONT );

    binding = "herring_icon";

    if ( !get_texture_binding( binding, &texobj ) ) {
	print_warning( IMPORTANT_WARNING,
		       "Couldn't get texture for binding %s", binding );
	return;
    }

    binding = "herring_count";

    if ( !get_font_binding( binding, &font ) ) {
	print_warning( IMPORTANT_WARNING,
		       "Couldn't get font for binding %s", binding );
	return;
    }

    sprintf( buff, " x %03d", herring_count ); 

    string = buff;

    get_font_metrics( font, string, &w, &asc, &desc );

    glBindTexture( GL_TEXTURE_2D, texobj );

    /*
    glPushMatrix();
	{        
        glTranslatef( getparam_x_resolution() - HERRING_ICON_X_OFFSET - OVERSCAN_MARGIN_X,
                     getparam_y_resolution() - HERRING_ICON_Y_OFFSET - OVERSCAN_MARGIN_Y - asc, 
                     0 );
        
        glEnableClientState (GL_VERTEX_ARRAY);
        glEnableClientState (GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT , 0, verticesItem);	
        glTexCoordPointer(2, GL_FLOAT, 0, texCoordsItem);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
		
		glDisableClientState (GL_VERTEX_ARRAY);
        glDisableClientState (GL_TEXTURE_COORD_ARRAY);

		bind_font_texture( font );

		glTranslatef( HERRING_ICON_WIDTH, HERRING_ICON_Y_OFFSET -  HERRING_COUNT_Y_OFFSET, 0 );

		draw_string( font, string );

	}

    glPopMatrix();    
     */
}

static void draw_score( player_data_t *plyr )
{
    char buff[BUFF_LEN];
    char *string;
    char *binding;
	int asc, desc, w;
    font_t *font;
    
    /* score calculation */
    int score = calculate_player_score(plyr);
    
    if ( !get_font_binding( "fps", &font ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get font for binding %s", binding );
        return;
    }

	string="Score: ";

	get_font_metrics(font, string, &w, &asc, &desc);

    bind_font_texture( font );
    set_gl_options( TEXFONT );
    /*
    glPushMatrix();
    {
        glTranslatef( SCORE_X_OFFSET + OVERSCAN_MARGIN_X,
                     SCORE_Y_OFFSET + OVERSCAN_MARGIN_Y,
                     0 );
        draw_string( font, string );
    }
    glPopMatrix();
     */

    if ( !get_font_binding( "fps_big", &font ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get font for binding %s", binding );
        return;
    }
    
    bind_font_texture( font );
    set_gl_options( TEXFONT );
    
    sprintf( buff, "%d",score );
    string = buff;
    
    /*
    glPushMatrix();
    {
        glTranslatef( SCORE_X_OFFSET + OVERSCAN_MARGIN_X + w,
                     SCORE_Y_OFFSET + OVERSCAN_MARGIN_Y,
                     0 );
        draw_string( font, string );
    }
    glPopMatrix();
     */
}

#define CIRCLE_DIVISIONS 20

point2d_t calc_new_fan_pt( scalar_t angle )
{
    point2d_t pt;
    pt.x = ENERGY_GAUGE_CENTER_X + cos( ANGLES_TO_RADIANS( angle ) ) *
	SPEEDBAR_OUTER_RADIUS;
    pt.y = ENERGY_GAUGE_CENTER_Y + sin( ANGLES_TO_RADIANS( angle ) ) *
	SPEEDBAR_OUTER_RADIUS;
    
    return pt;
}

point2d_t calc_new_fan_tex_pt( scalar_t angle )
{
    point2d_t pt;
    pt.x = (cos(ANGLES_TO_RADIANS(angle))*SPEEDBAR_OUTER_RADIUS+ENERGY_GAUGE_CENTER_X)/GAUGE_IMG_SIZE;
    pt.y = (sin(ANGLES_TO_RADIANS(angle))*SPEEDBAR_OUTER_RADIUS+ENERGY_GAUGE_CENTER_Y)/GAUGE_IMG_SIZE;
    
    return pt;
}

void draw_partial_tri_fan( scalar_t fraction )
{
    /*
#define DIVS (CIRCLE_DIVISIONS+2)
    scalar_t angle, angle_incr, cur_angle;
    int i;
    point2d_t pt;
	GLfloat vertices[DIVS*3];
	GLfloat texcoords[DIVS*2];
	
    angle = SPEEDBAR_BASE_ANGLE + ( SPEEDBAR_MAX_ANGLE - SPEEDBAR_BASE_ANGLE ) * fraction;
    
    cur_angle = SPEEDBAR_BASE_ANGLE;
    
    angle_incr = fraction * ( SPEEDBAR_BASE_ANGLE - SPEEDBAR_MAX_ANGLE ) / CIRCLE_DIVISIONS;
    
    vertices[0]=ENERGY_GAUGE_CENTER_X;
    vertices[1]=ENERGY_GAUGE_CENTER_Y;
    vertices[2]=0;

	texcoords[0]=ENERGY_GAUGE_CENTER_X/GAUGE_IMG_SIZE;
	texcoords[1]=ENERGY_GAUGE_CENTER_Y/GAUGE_IMG_SIZE;

    for (i=1; i<DIVS-1; i++) {
        pt = calc_new_fan_pt( cur_angle );
        
        vertices[i*3]=pt.x;
		vertices[i*3+1]=pt.y;
		vertices[i*3+2]=0;

        pt = calc_new_fan_tex_pt( cur_angle );
        
        texcoords[i*2]=pt.x;
		texcoords[i*2+1]=pt.y;

        cur_angle -= angle_incr;
    }

    pt = calc_new_fan_pt( angle );
    vertices[sizeof(vertices)/sizeof(GLfloat)-3]=pt.x;
	vertices[sizeof(vertices)/sizeof(GLfloat)-2]=pt.y;
	vertices[sizeof(vertices)/sizeof(GLfloat)-1]=0;

    pt = calc_new_fan_tex_pt( angle );
    texcoords[sizeof(texcoords)/sizeof(GLfloat)-2]=pt.x;
	texcoords[sizeof(texcoords)/sizeof(GLfloat)-1]=pt.y;
        
    glVertexPointer(3, GL_FLOAT , 0, vertices);
    glTexCoordPointer(2, GL_FLOAT , 0, texcoords);
    glDrawArrays(GL_TRIANGLE_FAN, 0, DIVS);

	return;
     */
}

void draw_gauge( scalar_t speed, scalar_t energy )
{
    char *binding;
    GLfloat xplane[4] = { 1.0/GAUGE_IMG_SIZE, 0.0, 0.0, 0.0 };
    GLfloat yplane[4] = { 0.0, 1.0/GAUGE_IMG_SIZE, 0.0, 0.0 };
    GLuint energymask_texobj, speedmask_texobj, outline_texobj;
    font_t *speed_font;
    font_t *units_font;
    int w, asc, desc;
    char *string;
    char buff[BUFF_LEN];
    scalar_t y;
    scalar_t speedbar_frac;
    
    set_gl_options( GAUGE_BARS );
    
    binding = "gauge_energy_mask";
    if ( !get_texture_binding( binding, &energymask_texobj ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get texture for binding %s", binding );
        return;
    }
    
    binding = "gauge_speed_mask";
    if ( !get_texture_binding( binding, &speedmask_texobj ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get texture for binding %s", binding );
        return;
    }
    
    binding = "gauge_outline";
    if ( !get_texture_binding( binding, &outline_texobj ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get texture for binding %s", binding );
        return;
    }
    
    binding = "speed_digits";
    if ( !get_font_binding( binding, & speed_font ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get font for binding %s", speed_font );
    }
    
    binding = "speed_units";
    if ( !get_font_binding( binding, &units_font ) ) {
        print_warning( IMPORTANT_WARNING,
                      "Couldn't get font for binding %s", speed_font );
    }
    
    
    //glTexGenfv( GL_S, GL_OBJECT_PLANE, xplane );
    //glTexGenfv( GL_T, GL_OBJECT_PLANE, yplane );
    
    y = ENERGY_GAUGE_BOTTOM + energy * ENERGY_GAUGE_HEIGHT;

    /*
    glPushMatrix();
    {
        const GLfloat verticesItem []=
        {
            0.0, y, 0,
            0.0, GAUGE_IMG_SIZE, 0,
            GAUGE_IMG_SIZE, GAUGE_IMG_SIZE, 0,
            GAUGE_IMG_SIZE, GAUGE_IMG_SIZE, 0,
            GAUGE_IMG_SIZE, y, 0,
            0.0, y, 0,
            
            0.0, 0.0, 0,
            0.0, y, 0,
            GAUGE_IMG_SIZE, y, 0,
            GAUGE_IMG_SIZE, y, 0,
            GAUGE_IMG_SIZE, 0.0, 0,
            0.0, 0.0, 0,
            
            0.0, 0.0, 0,
            0.0, GAUGE_IMG_SIZE, 0,
            GAUGE_IMG_SIZE, GAUGE_IMG_SIZE, 0,
            GAUGE_IMG_SIZE, GAUGE_IMG_SIZE, 0,
            GAUGE_IMG_SIZE, 0.0, 0,
            0.0, 0.0, 0,
        };

        const GLfloat texcoordItem []=
        {
            0, 0,
            0, 1,
			1, 1,
			1, 1,
			1, 0,
			0, 0,

			0, 0,
			0, y/GAUGE_IMG_SIZE,
			1, y/GAUGE_IMG_SIZE,
			1, y/GAUGE_IMG_SIZE,
			1, 0,
			0, 0
        };

        glTranslatef( getparam_x_resolution() - GAUGE_WIDTH - OVERSCAN_MARGIN_X,
                     OVERSCAN_MARGIN_Y,
                     0 );

		/*glEnableClientState (GL_VERTEX_ARRAY);
        glEnableClientState (GL_TEXTURE_COORD_ARRAY);
        
        glBindTexture( GL_TEXTURE_2D, energymask_texobj );

        glColor4f(energy_background_color[0], energy_background_color[1], energy_background_color[2], energy_background_color[3]);

        glVertexPointer (3, GL_FLOAT , 0, verticesItem+36);
        glTexCoordPointer (2, GL_FLOAT , 0, texcoordItem);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glBindTexture( GL_TEXTURE_2D, energymask_texobj );
        glColor4f(energy_foreground_color[0], energy_foreground_color[1], energy_foreground_color[2], energy_foreground_color[3]);
        
        glVertexPointer (3, GL_FLOAT , 0, verticesItem+18);
        glTexCoordPointer (2, GL_FLOAT , 0, texcoordItem+12);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    
        speedbar_frac = 0.0;
        
        if ( speed > SPEEDBAR_GREEN_MAX_SPEED ) {
            speedbar_frac = SPEEDBAR_GREEN_FRACTION;
            
            if ( speed > SPEEDBAR_YELLOW_MAX_SPEED ) {
                speedbar_frac += SPEEDBAR_YELLOW_FRACTION;
                
                if ( speed > SPEEDBAR_RED_MAX_SPEED ) {
                    speedbar_frac += SPEEDBAR_RED_FRACTION;
                } else {
                    speedbar_frac +=
                    ( speed - SPEEDBAR_YELLOW_MAX_SPEED ) /
                    ( SPEEDBAR_RED_MAX_SPEED - SPEEDBAR_YELLOW_MAX_SPEED ) *
                    SPEEDBAR_RED_FRACTION;
                }
                
            } else {
                speedbar_frac += 
                ( speed - SPEEDBAR_GREEN_MAX_SPEED ) /
                ( SPEEDBAR_YELLOW_MAX_SPEED - SPEEDBAR_GREEN_MAX_SPEED ) *
                SPEEDBAR_YELLOW_FRACTION;
            }
            
        } else {
            speedbar_frac +=  speed/SPEEDBAR_GREEN_MAX_SPEED * 
            SPEEDBAR_GREEN_FRACTION;
        }
        
        
        glBindTexture( GL_TEXTURE_2D, speedmask_texobj );


		glColor4f(speedbar_background_color[0], speedbar_background_color[1], speedbar_background_color[2], speedbar_background_color[3]);
        draw_partial_tri_fan( 1.0 );
        glColor4f(white[0], white[1], white[2], white[3]);
        draw_partial_tri_fan( min( 1.0, speedbar_frac ) );
        
        glBindTexture( GL_TEXTURE_2D, outline_texobj );
        glVertexPointer(3, GL_FLOAT , 0, verticesItem+36);
        glTexCoordPointer(2, GL_FLOAT , 0, texcoordItem);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        sprintf( buff, "%d", (int)speed );
        string = buff;
        
        get_font_metrics( speed_font, string, &w, &asc, &desc );
        
        bind_font_texture( speed_font);
        set_gl_options( TEXFONT );
        glColor4f( 1, 1, 1, 1 );
        
        glPushMatrix();
        {
            glTranslatef( ENERGY_GAUGE_CENTER_X - w/2.0,
                         ENERGY_GAUGE_BOTTOM + ENERGY_GAUGE_HEIGHT / 2.0,
                         0 );
            draw_string( speed_font, string );
            
        }
        glPopMatrix();
        
        string = "km/h";
        
        get_font_metrics( units_font, string, &w, &asc, &desc );
        
        bind_font_texture( units_font );
        
        glPushMatrix();
        {
            glTranslatef( ENERGY_GAUGE_CENTER_X - w/2.0,
                         ENERGY_GAUGE_BOTTOM + ENERGY_GAUGE_HEIGHT / 2.0
                         - asc - SPEED_UNITS_Y_OFFSET,
                         0 );
            draw_string( units_font, string );
            
        }
        glPopMatrix();
        
    }
    glPopMatrix();
     */

	//glDisableClientState (GL_VERTEX_ARRAY);
    //glDisableClientState (GL_TEXTURE_COORD_ARRAY);
}
void print_fps()
{
    /*
    char buff[BUFF_LEN];
    char *string;
    char *binding;
    font_t *font, *font_big;
	int x, y;
	int w, asc, desc;

    ui_setup_display();

    if ( ! getparam_display_fps() ) {
	return;
    }

    if ( !get_font_binding( "fps", &font ) ) {
	print_warning( IMPORTANT_WARNING,
		       "Couldn't get font for binding %s", binding );
	return;
    }

    if ( !get_font_binding( "fps_big", &font_big ) ) {
	print_warning( IMPORTANT_WARNING,
		       "Couldn't get font for binding %s", binding );
	return;
    }

    bind_font_texture( font_big );
    set_gl_options( TEXFONT );
    glColor4f( 1, 1, 1, 1 );

    sprintf( buff, "%.1f", get_fps() );
	get_font_metrics(font_big, buff, &w, &asc, &desc);
	y=asc+desc;
	get_font_metrics(font, "FPS: ", &w, &asc, &desc);
	x=w;
    string = buff;

    glPushMatrix();
    {
	glTranslatef( FPS_X_OFFSET+OVERSCAN_MARGIN_X,
		      FPS_Y_OFFSET+OVERSCAN_MARGIN_Y+y,
		      0 );
	draw_string( font, "FPS: " );
    }
    glPopMatrix();

    glPushMatrix();
    {
	glTranslatef( FPS_X_OFFSET+OVERSCAN_MARGIN_X+x,
		      FPS_Y_OFFSET+OVERSCAN_MARGIN_Y+y,
		      0 );
	draw_string( font_big, string );
    }
    glPopMatrix();
     */
}

void draw_hud( player_data_t *plyr )
{
    vector_t vel;
    scalar_t speed;

    vel = plyr->vel;
    speed = normalize_vector( &vel );

    ui_setup_display();
	
    if (game_has_herring()) {
        draw_herring_count( plyr->herring );
        draw_score( plyr );
    }

    draw_gauge( speed * M_PER_SEC_TO_KM_PER_H, plyr->control.jump_amt );
    draw_time(plyr);

    print_fps();
}

