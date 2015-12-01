static GLfloat gl_trans[6][3] = {
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0},
    {1.0, 0.0, 0.0},
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, -1.0, 0.0}
};
static GLfloat gl_rots[6][4] = {
    {0.0, 0.0, 1.0, 0.0},
    {180.0, 0.0, 1.0, 0.0},
    {90.0, 0.0, 1.0, 0.0},
    {-90.0, 0.0, 1.0, 0.0},
    {-90.0, 1.0, 0.0, 0.0},
    {90.0, 1.0, 0.0, 0.0}
};
static GLfloat gl_cols[6][3] = {
    {1, 0, 0},
    {0, 1, 1},
    {0, 1, 0},
    {1, 0, 1},
    {0, 0, 1},
    {1, 1, 0}
};

void glTests(const int test_number)
{
    switch(test_number)
    {
        case 1:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glColor3f(1,1,1);
            glBegin(GL_TRIANGLES);
            glVertex2f(0.25, 0.25);
            glVertex2f(0.75, 0.25);
            glVertex2f(0.75, 0.75);
            glEnd();
            break;
        case 2:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glColor3f(1, 1, 1);
            glBegin(GL_QUADS);
            glVertex3f(-1.0, -1.0, -5.0);
            glVertex3f(1.0, -1.0, -2.0);
            glVertex3f(1.0, 1.0, -2.0);
            glVertex3f(-1.0, 1.0, -5.0);
            glEnd();
            break;
        case 3:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glPushMatrix();

            glTranslatef(0.25, 0.5, 0.0);
            glRotatef(-45, 0.0, 1.0, 0.0);
            glScalef(0.25, 0.25, 1.0);

            glColor3f(0, 0, 1);
            glBegin(GL_QUADS);
            glVertex2f(-1.0, -1.0);
            glVertex2f(1.0, -1.0);
            glVertex2f(1.0, 1.0);
            glVertex2f(-1.0, 1.0);
            glEnd();

            glPopMatrix();

            glColor3f(1, 0, 0);
            glBegin(GL_TRIANGLES);
            glVertex3f(0.5, 0.25, 0.5);
            glVertex3f(0.75, 0.25, -0.5);
            glVertex3f(0.75, 0.75, -0.5);
            glEnd();
            break;
        case 4:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glColor3f(0, 0, 1);
            glBegin(GL_QUADS);
            glVertex2f(0.4, 0.2);
            glVertex2f(2.0, 0.2);
            glVertex2f(2.0, 0.8);
            glVertex2f(0.4, 0.8);
            glEnd();

            glColor3f(1, 0, 0);
            glBegin(GL_TRIANGLES);
            glVertex3f(0.2, 0.2, -0.5);
            glVertex3f(0.8, 0.5, 0.5);
            glVertex3f(0.2, 0.8, -0.5);
            glEnd();
            break;
        case 5:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glTranslatef(0.0, 0.0, -5.0);
            glRotatef(-60, -1.0, 1.0, -1.0);
            
            for (int i = 0; i < 6; ++i) {
                GLfloat* tran = gl_trans[i];
                GLfloat* rot = gl_rots[i];
                GLfloat* col = gl_cols[i];
                
                glPushMatrix();

                glTranslatef(tran[0], tran[1], tran[2]);
                glRotatef(rot[0], rot[1], rot[2], rot[3]);

                glColor3f(col[0], col[1], col[2]);

                glBegin(GL_QUADS);
                glVertex2f(-1.0, -1.0);
                glVertex2f(1.0, -1.0);
                glVertex2f(1.0, 1.0);
                glVertex2f(-1.0, 1.0);
                glEnd();

                glPopMatrix();
            }

            break;
        case 6:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glTranslatef(0.1667, 0.5, 0.0);
            glBegin(GL_TRIANGLES);
            glColor3f(1, 0, 0);
            for( int i = 0; i < 3; i++ )
            {
                glVertex2f(0 + 0.33*i, 0.25);
                glVertex2f(-0.1667 + 0.33*i, -0.25);
                if( i == 0 ) glColor3f(0, 1, 0);
                else if( i == 1) glColor3f(0, 0, 1);
                else if( i == 2) glColor3f(1, 0, 0);
                glVertex2f(0.1667 + 0.33*i, -0.25);
            }
            glEnd();
            break;
        default:
            std::cerr<<"Invalid test number"<<std::endl;
            break;
    }
}

static MGLfloat mgl_trans[6][3] = {
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0},
    {1.0, 0.0, 0.0},
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, -1.0, 0.0}
};
static MGLfloat mgl_rots[6][4] = {
    {0.0, 0.0, 1.0, 0.0},
    {180.0, 0.0, 1.0, 0.0},
    {90.0, 0.0, 1.0, 0.0},
    {-90.0, 0.0, 1.0, 0.0},
    {-90.0, 1.0, 0.0, 0.0},
    {90.0, 1.0, 0.0, 0.0}
};
static MGLbyte mgl_cols[6][3] = {
    {255, 0, 0},
    {0, 255, 255},
    {0, 255, 0},
    {255, 0, 255},
    {0, 0, 255},
    {255, 255, 0}
};

void mglTests(const int test_number)
{
    switch(test_number)
    {
        case 1:
            mglMatrixMode(MGL_PROJECTION);
            mglLoadIdentity();
            mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            mglMatrixMode(MGL_MODELVIEW);
            mglLoadIdentity();

            mglColor(255, 255, 255);
            mglBegin(MGL_TRIANGLES);
            mglVertex2(0.25, 0.25);
            mglVertex2(0.75, 0.25);
            mglVertex2(0.75, 0.75);
            mglEnd();
            break;
        case 2:
            mglMatrixMode(MGL_PROJECTION);
            mglLoadIdentity();
            mglFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
            mglMatrixMode(MGL_MODELVIEW);
            mglLoadIdentity();

            mglColor(255, 255, 255);
            mglBegin(MGL_QUADS);
            mglVertex3(-1.0, -1.0, -5.0);
            mglVertex3(1.0, -1.0, -2.0);
            mglVertex3(1.0, 1.0, -2.0);
            mglVertex3(-1.0, 1.0, -5.0);
            mglEnd();
            break;
        case 3:
            mglMatrixMode(MGL_PROJECTION);
            mglLoadIdentity();
            mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            mglMatrixMode(MGL_MODELVIEW);
            mglLoadIdentity();

            mglPushMatrix();

            mglTranslate(0.25, 0.5, 0.0);
            mglRotate(-45, 0.0, 1.0, 0.0);
            mglScale(0.25, 0.25, 1.0);

            mglColor(0, 0, 255);
            mglBegin(MGL_QUADS);
            mglVertex2(-1.0, -1.0);
            mglVertex2(1.0, -1.0);
            mglVertex2(1.0, 1.0);
            mglVertex2(-1.0, 1.0);
            mglEnd();

            mglPopMatrix();

            mglColor(255, 0, 0);
            mglBegin(MGL_TRIANGLES);
            mglVertex3(0.5, 0.25, 0.5);
            mglVertex3(0.75, 0.25, -0.5);
            mglVertex3(0.75, 0.75, -0.5);
            mglEnd();
            break;
        case 4:
            mglMatrixMode(MGL_PROJECTION);
            mglLoadIdentity();
            mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            mglMatrixMode(MGL_MODELVIEW);
            mglLoadIdentity();

            mglColor(0, 0, 255);
            mglBegin(MGL_QUADS);
            mglVertex2(0.4, 0.2);
            mglVertex2(2.0, 0.2);
            mglVertex2(2.0, 0.8);
            mglVertex2(0.4, 0.8);
            mglEnd();

            mglColor(255, 0, 0);
            mglBegin(MGL_TRIANGLES);
            mglVertex3(0.2, 0.2, -0.5);
            mglVertex3(0.8, 0.5, 0.5);
            mglVertex3(0.2, 0.8, -0.5);
            mglEnd();
            break;
        case 5:
            mglMatrixMode(MGL_PROJECTION);
            mglLoadIdentity();
            mglFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
            mglMatrixMode(MGL_MODELVIEW);
            mglLoadIdentity();

            mglTranslate(0.0, 0.0, -5.0);
            mglRotate(-60, -1.0, 1.0, -1.0);

            for (int i = 0; i < 6; ++i) {
                MGLfloat* tran = mgl_trans[i];
                MGLfloat* rot = mgl_rots[i];
                MGLbyte* col = mgl_cols[i];

                mglPushMatrix();

                mglTranslate(tran[0], tran[1], tran[2]);
                mglRotate(rot[0], rot[1], rot[2], rot[3]);
                
                mglColor(col[0], col[1], col[2]);
                mglBegin(MGL_QUADS);
                mglVertex2(-1.0, -1.0);
                mglVertex2(1.0, -1.0);
                mglVertex2(1.0, 1.0);
                mglVertex2(-1.0, 1.0);
                mglEnd();

                mglPopMatrix();
            }
            break;
        case 6:
            mglMatrixMode(MGL_PROJECTION);
            mglLoadIdentity();
            mglOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
            mglMatrixMode(MGL_MODELVIEW);
            mglLoadIdentity();

            mglTranslate(0.1667, 0.5, 0.0);
            mglBegin(MGL_TRIANGLES);
            mglColor(255, 0, 0);
            for( int i = 0; i < 3; i++ )
            {
                mglVertex2(0 + 0.33*i, 0.25);
                mglVertex2(-0.1667 + 0.33*i, -0.25);
                if( i == 0 ) mglColor(0, 255, 0);
                else if( i == 1) mglColor(0, 0, 255);
                else if( i == 2) mglColor(255, 0, 0);
                mglVertex2(0.1667 + 0.33*i, -0.25);
            }
            mglEnd();
            break;
        default:
            std::cerr<<"Invalid test number"<<std::endl;
            break;
    }
}
