#include "gl/glaux.h"
#include "gl/glut.h"
#include <cmath>
#include <stdio.h>
#include <tchar.h>

#define BALL_RECT	20.0
#define BALL_NUM	7
#define HOLE_RECT	22.0
#define HOLE_NUM	6
#define WIDTH		1000
#define HEIGHT		600
#define PIE	3.141592653589793238462643383279
#define G	9.8065

GLuint tex[1]; //�ؽ��� ���� �ҽ��� �ĺ���

			   //�� Ŭ����
class hole {
public:
	float holeColor[3];	// Ȧ�� ������ ���� �迭
	double holeCoord[3];	// Ȧ�� ��ġ�� ���� �迭
public:
	// Ȧ ������
	hole(float *color, double *coord) {
		for (int i = 0; i<3; i++) {
			holeColor[i] = color[i];
			holeCoord[i] = coord[i];
		}
	}
};
hole *holeList[HOLE_NUM];
double hole1[3] = { 110.0f,110.0f,0.0f };
double hole2[3] = { 490.0f,110.0f,0.0f };
double hole3[3] = { 890.0f,110.0f,0.0f };
double hole4[3] = { 110.0f,490.0f,0.0f };
double hole5[3] = { 490.0f,490.0f,0.0f };
double hole6[3] = { 890.0f,490.0f,0.0f };

//����
float chole1[3] = { 0.0f, 0.5f, 0.0f };
float chole2[3] = { 0.0f, 0.5f, 0.0f };
float chole3[3] = { 0.0f, 0.5f, 0.0f };
float chole4[3] = { 0.0f, 0.5f, 0.0f };
float chole5[3] = { 0.0f, 0.5f, 0.0f };
float chole6[3] = { 0.0f, 0.5f, 0.0f };

class ball {
public:
	float ballColor[3];		//���� ������ ���� �迭
	double ballCoord[3];	//���� ��ġ�� ���� �迭
	double ballRect;		//���� ������ ����
	double ballSpd;			//���� �ӵ� �뷫 0~5���� ��
	double ballFric;		//���� �������

public:
	//�� ������.
	ball(float *color, double *coord, double rect) {
		for (int i = 0; i<3; i++) {
			ballColor[i] = color[i];
			ballCoord[i] = coord[i];
		}
		ballRect = rect;
		ballSpd = 0;
		ballFric = 0.0001;
	}
	// ���۰� �������� �ƴ��� üũ
	void holechk()
	{
		for (int i = 0; i<HOLE_NUM; i++) {
			double x = holeList[i]->holeCoord[0] - ballCoord[0];
			double y = holeList[i]->holeCoord[1] - ballCoord[1];
			double z = holeList[i]->holeCoord[2] - ballCoord[2];
			if ((x*x + y*y + z*z) < BALL_RECT*BALL_RECT - 2) {
				if (ballColor[0] == 1.0f && ballColor[1] == 1.0f && ballColor[2] == 1.0f)	// ����̶��
				{
					// ���ۿ� ������� �ٽ� ������ġ�� ��������
					ballCoord[0] = holeList[i]->holeCoord[0];
					ballCoord[1] = holeList[i]->holeCoord[1];
					ballCoord[2] = holeList[i]->holeCoord[2];
					ballCoord[0] = 650.0f;
					ballCoord[1] = 300.0f;
					ballCoord[2] = 0.0f;
					ballSpd = 0;
				}
				else {	// �ٸ������� ���ۿ� �����
					for (int j = 0; j<3; j++) {
						ballColor[j] = 0.0f;	// ������ ����������
						ballSpd *= 0.1;	// �ڿ������� �����̰�
						ballCoord[j] = 1050.0f;	// â �ٱ� ��ǥ�� ��
					}
				}
			}
		}
	}
	//���� ��� ���� ���� �浹�ߴ��� üũ
	bool check(const ball& b) const {

		//�̸� ���� ��ġ�� �̵����Ѻ� ������ �浹�ϴ��� Ȯ��
		double move1 = (ballCoord[0] + cos((ballRect*PIE) / 180.0)*ballSpd)
			- (b.ballCoord[0] + cos((b.ballRect*PIE) / 180.0)*b.ballSpd);
		double move2 = (ballCoord[1] + sin((ballRect*PIE) / 180.0)*ballSpd)
			- (b.ballCoord[1] + sin((b.ballRect*PIE) / 180.0)*b.ballSpd);

		if (pow(move1, 2) + pow(move2, 2) <= pow(BALL_RECT * 2, 2) + 1)
			return true;
		else
			return false;
	}

	//���� ���� ������ ���� ������ġ ���
	void move() {
		if (ballSpd>ballFric*G) {
			ballSpd -= ballFric*G;
			ballCoord[0] += cos((ballRect*PIE) / 180.0)*ballSpd;
			ballCoord[1] += sin((ballRect*PIE) / 180.0)*ballSpd;
		}
		else
			ballSpd = 0;
	}

	//���� �浹�ߴ��� üũ
	void collisionWall() {
		//�̸� ������ ������ ��ġ�� ����غ���.
		double move1 = ballCoord[0] + cos((ballRect*PIE) / 180.0)*ballSpd;
		double move2 = ballCoord[1] + sin((ballRect*PIE) / 180.0)*ballSpd;

		if (move1 - 75 <= BALL_RECT + 1 || move1 >= WIDTH - 75 - BALL_RECT - 1) {		//���� or ������ �浹
			ballRect = 180 - ballRect;
			if (ballRect<0)
				ballRect += 360;
			ballSpd *= 0.55;			//����� ������ �����ؼ� �ӵ��� 20% �ٿ���
		}
		else if (move2 - 75 <= BALL_RECT + 1 || move2 >= HEIGHT - 75 - BALL_RECT - 1) {		//�� or �Ʒ� �浹
			ballRect = 360 - ballRect;
			if (ballRect<0)
				ballRect += 360;
			ballSpd *= 0.55;
		}
	}

	//�浹üũ�� �� ������ִ� �κ�
	void collision(ball& b) {

		//���Ͱ���� ���� ���� �������� ������ ã����
		double rect = atan2(this->ballCoord[1] - b.ballCoord[1]
			, this->ballCoord[0] - b.ballCoord[0]);
		rect *= 180;
		rect /= PIE;

		//�浹�� ���� �ΰ��� ���� �ӵ��� ���� 45%�� �ٿ���
		ballSpd *= 0.55;
		b.ballSpd *= 0.55;

		double bx1, by1, bx2, by2;		//���Ϳ�

		bx1 = cos(((ballRect - rect)*PIE) / 180.0)*ballSpd;
		by1 = sin(((ballRect - rect)*PIE) / 180.0)*ballSpd;
		bx2 = cos(((b.ballRect - rect)*PIE) / 180.0)*b.ballSpd;
		by2 = sin(((b.ballRect - rect)*PIE) / 180.0)*b.ballSpd;

		printf("%lf\n", rect);
		printf("SPD : ball1 %lf, ball2 %lf\n", ballSpd, b.ballSpd);
		printf("Rect : ball1 %lf, ball2 %lf\n", ballRect, b.ballRect);
		printf("ball1 x_vector : %lf, y_vector : %lf\n", bx1, by1);
		printf("ball2 x_vector : %lf, y_vector : %lf\n", bx2, by2);

		//ź���浹�̹Ƿ�
		//�� ���� x���͸� ���� ��ȯ���ش��� �ٽ� �ӵ��� ������
		ballSpd = sqrt(pow(bx2, 2) + pow(by1, 2));
		b.ballSpd = sqrt(pow(bx1, 2) + pow(by2, 2));

		//������ �ٽ� ������
		printf("ball1 SPD : %lf, ball2 SPD : %lf\n", ballSpd, b.ballSpd);
		ballRect = rect + (atan2(by1, bx2) * 180 / PIE);
		b.ballRect = rect + (atan2(by2, bx1) * 180 / PIE);

		printf("newRect ball1 : %lf, ball2 : %lf\n", ballRect, b.ballRect);
		printf("\n");

	}
};

GLUquadricObj *obj[6];
ball *ballList[BALL_NUM];
bool mouseClick = false;
int lineX1, lineY1, lineX2, lineY2;

void init() {

	//���� ��ġ��
	double ball1[3] = { 350.0f,300.0f,0.0f };
	double ball2[3] = { 300.0f,330.0f,0.0f };
	double ball3[3] = { 300.0f,270.0f,0.0f };
	double ball4[3] = { 250.0f,350.0f,0.0f };
	double ball5[3] = { 250.0f,300.0f,0.0f };
	double ball6[3] = { 250.0f,250.0f,0.0f };
	//���
	double ball7[3] = { 650.0f,300.0f,0.0f };
	//����

	//���� �����...
	float cball1[3] = { 1.0f,0.0f,0.0f };
	float cball2[3] = { 0.0f,1.0f,0.0f };
	float cball3[3] = { 0.0f,0.0f,1.0f };
	float cball4[3] = { 1.0f,0.5f,0.0f };
	float cball5[3] = { 1.0f,1.0f,0.0f };
	float cball6[3] = { 1.0f,0.0f,1.0f };
	//���
	float cball7[3] = { 1.0f,1.0f,1.0f };

	//���� �������
	ballList[0] = new ball(cball1, ball1, 170.0);
	ballList[1] = new ball(cball2, ball2, 10.0);
	ballList[2] = new ball(cball3, ball3, 270.0);
	ballList[3] = new ball(cball4, ball4, 90.0);
	ballList[4] = new ball(cball5, ball5, 180.0);
	ballList[5] = new ball(cball6, ball6, 44.0);
	//��� �����
	ballList[6] = new ball(cball7, ball7, 0.0f);

	//Ȧ �����
	holeList[0] = new hole(chole1, hole1);
	holeList[1] = new hole(chole2, hole2);
	holeList[2] = new hole(chole3, hole3);
	holeList[3] = new hole(chole4, hole4);
	holeList[4] = new hole(chole5, hole5);
	holeList[5] = new hole(chole6, hole6);

}

void check() {

	//���� ���� �浹�ߴ��� üũ�Ѵ��� ���� �浹������ ���� �ٲ��ش�
	for (int i = 0; i<BALL_NUM; i++)
		ballList[i]->collisionWall();
	//���� ���� ��ó�� ���� ���۾����� ���� ����
	for (int i = 0; i<BALL_NUM; i++)
		ballList[i]->holechk();

	//���� ���� �浹�� üũ�Ѵ���
	//���� �浹������ �浹�۾����� ���� ƨ�ܳ��� ����� ���ش�.
	for (int i = 0; i<BALL_NUM - 1; i++) {
		for (int j = i + 1; j<BALL_NUM; j++) {
			if (ballList[i]->check(*ballList[j]))
				ballList[i]->collision(*ballList[j]);

		}
	}
}

//�̹��� ������ ���� GLuint������ ���� 
AUX_RGBImageRec* LoadBMPFile(TCHAR* filename)
{
	FILE *hFile = NULL;
    errno_t err = NULL;

	if (!filename) return NULL;

	err = _tfopen_s(&hFile, filename, _T("r")); //�̹��� ���� ����

	if (err == 0) {
		fclose(hFile);
		return auxDIBImageLoad(filename); //���� ������ AUX_RGBImageRec���·� ����
	}

	return NULL;
}
//�̹��� �����а� �׽��� ���� �ҽ��� ��ȯ�ϰ� ����
void LoadImage()
{
	AUX_RGBImageRec* texRec = 0; //�̹��� ������ �����ϱ� ���� ����(�ؽ��� ���� �ҽ�)     

	glGenTextures(1, &tex[0]); //�ؽ��� ���� �ҽ��� �ĺ��ڸ� ����
	if (texRec = LoadBMPFile("BilliardsBackground.bmp")) { //�̹��� ������ �о� �ؽ��� ���� �ҽ��� ����
		glBindTexture(GL_TEXTURE_2D, tex[0]); //����� �ؽ��� ���� �ҽ��� �ĺ��ڸ� ����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // �ؽ��� ���� �ҽ��� ũ�� > ���εǾ��� ��ü�� ũ���϶� ���ó��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // �ؽ��� ���� �ҽ��� ũ�� < ���εǾ��� ��ü�� ũ���϶� Ȯ��ó��  
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec->sizeX, texRec->sizeY, 0,
			GL_RGB, GL_UNSIGNED_BYTE, texRec->data); //���� �ҽ��� ���� Ư���� ���� 
	}
	else return;

	if (texRec) {
		if (texRec->data) free(texRec->data); //�ʿ���� �޸� ����
		free(texRec); //�ʿ���� �޸� ����  
	}
	glEnable(GL_TEXTURE_2D); //���� �ؽ��� ������ ����ϰڳ��� OpenGL���� �˸�   
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //ȭ���� ���� �ؽ��� ���� �ҽ��� ���� ��� �����Ͽ� ǥ���ϴ��� ���� 
}

void display() {
	glClearColor(0.50, 0.3, 0.0, 0.12);	// �ٴڻ�
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//����� �����̵��� ���콺 Ŭ���ϰ� ���������� �� ���� �׷���
	if (mouseClick && lineX2 != 0) {
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex2f(lineX1, lineY1);
		glVertex2f(lineX2, lineY2);
		glEnd();
	}

	glDisable(GL_LIGHTING);
	glColor3f(0.1f, 0.2f, 0.1f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(50.0f, 50.0f, 0.0f);
	glVertex3f(950.0f, 50.0f, 0.0f);
	glVertex3f(950.0f, 550.0f, 0.0f);
	glVertex3f(50.0f, 550.0f, 0.0f);
	glVertex3f(50.0f, 50.0f, 0.0f);
	glVertex3f(75.0f, 75.0f, 0.0f);
	glVertex3f(925.0f, 75.0f, 0.0f);
	glVertex3f(950.0f, 50.0f, 0.0f);
	glVertex3f(925.0f, 75.0f, 0.0f);
	glVertex3f(925.0f, 525.0f, 0.0f);
	glVertex3f(950.0f, 550.0f, 0.0f);
	glVertex3f(925.0f, 525.0f, 0.0f);
	glVertex3f(75.0f, 525.0f, 0.0f);
	glVertex3f(50.0f, 550.0f, 0.0f);
	glVertex3f(75.0f, 525.0f, 0.0f);
	glVertex3f(75.0f, 75.0f, 0.0f);
	glEnd();

	//���� �׸��� ������ �迭�� ����Ѵ��� Pop�� ����
	for (int i = 0; i<BALL_NUM; i++) {
		GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //�����
		GLfloat mat_shininess[] = { 127.0 }; // �ݻ�Ǿ� ��ġ�� ���� ���ݻ����� ������� ����, ���õ��ö�ƽ��
		GLfloat light_position[] = { -0.5f, 0.5f, 0.5f, 0.0f }; // �������� 0.0�� ������ǥ��(2�̸� 1.0 1.0 1.0�� ���� 0.5���ȴ�)
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //����������
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);//����������
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ballList[i]->ballColor);//����������
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);//������ Ư�� GL_LIGHT0�� 0���� 7���� ��밡��
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glEnable(GL_LIGHTING); //������ �Ѵ°�
		glPushMatrix();
		glTranslatef(ballList[i]->ballCoord[0],
			ballList[i]->ballCoord[1], ballList[i]->ballCoord[2]);
		glutSolidSphere(BALL_RECT, 24, 24);
		glPopMatrix();
	}
	for (int i = 0; i<HOLE_NUM; i++) {
		GLfloat mat_specular[] = { 0.0f, 0.5f, 0.0f, 1.0f }; //�����
		GLfloat mat_shininess[] = { 2.0 }; // �ݻ�Ǿ� ��ġ�� ���� ���ݻ����� ������� ����, ���õ��ö�ƽ��
		GLfloat light_position[] = { -0.5f, 0.6f, -0.8f, 0.0f }; // �������� 0.0�� ������ǥ��(2�̸� 1.0 1.0 1.0�� ���� 0.5���ȴ�)
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //����������
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);//����������
		glMaterialfv(GL_FRONT, GL_DIFFUSE, holeList[i]->holeColor);//����������
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);//������ Ư�� GL_LIGHT0�� 0���� 7���� ��밡��
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glEnable(GL_LIGHTING); //������ �Ѵ°�
		glPushMatrix();
		glTranslatef(holeList[i]->holeCoord[0], holeList[i]->holeCoord[1], holeList[i]->holeCoord[2]);
		glutSolidSphere(HOLE_RECT, 24, 24);
		glPopMatrix();
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[0]); //� �ؽ��� ���� �ҽ��� ������� ����
	glColor3f(0.0f, 0.5f, 0.0f); //�׸� ���� ����
								 /*������ �׸� �׸�*/
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS); // �׸� ��Ÿ���� ���λ� ĥ��
	glTexCoord2f(0.0f, 0.0f); /*������ �ؽ��� ���� �ҽ��� ������ ����*/ glVertex3f(75.0f, 75.0f, 0.0f); //��������
	glTexCoord2f(0.0f, 1.0f); glVertex3f(75.0f, 525.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(925.0f, 525.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(925.0f, 75.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D); //�ؽ��� ���� �ҽ��� ��� ���ϰڴٰ� OpenGL���� �˸�
	glPopMatrix();

	glColor3f(0.45f, 0.2f, 0.1f); //�׸� ���� ����
	glRectf(50.0f, 50.0f, 950.0f, 550.0f); //�簢��
										   //���
	glutSwapBuffers();
}

void MyTimer(int value) {

	//1ms���� ������ ������ �ӵ��� ���� ������ ������
	for (int i = 0; i<BALL_NUM; i++)
		ballList[i]->move();

	check();
	glutPostRedisplay();			//ȭ���� ��������
	glutTimerFunc(1, MyTimer, 1);		//�ڱ� �ڽ��� �ٽ� �θ����ν� 1ms���� ��� ���ŵ�
}

//���콺 �Է��� �޾����� �ݹ�Ǵ� �Լ�
//button�� ����Ŭ��,������Ŭ������
//state�� ������������ ���������
//x,y�� Ŭ���� ��ǥ
void mouse(int button, int state, int x, int y) {

	//���콺�� ��ǥ�� ���� ���� 0,0�̹Ƿ�
	//���� �Ʒ��� 0,0�� �׸�����ǥ�� ��ȯ���ش�.
	y = HEIGHT - y;

	//���콺 ���ʹ�ư ��������
	//���� �츮�� ������ �� �ִ� ���(���� ��������)�� �� �ȿ� x,y��ǥ�� ����ִٸ�
	//����üũ�� true�� ���༭ mouseomove���� �˼� �ֵ��� �Ѵ�.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ballList[BALL_NUM - 1]->ballSpd == 0) {
		if (pow(x - ballList[BALL_NUM - 1]->ballCoord[0], 2) + pow(y - ballList[BALL_NUM - 1]->ballCoord[1], 2) < pow(BALL_RECT, 2)) {
			mouseClick = true;
			//�׸��⸦ �̻ڰ� �ϱ� ���ؼ� ���� �߽����κ��� ���� �׷������� �Ѵ�.
			lineX1 = ballList[BALL_NUM - 1]->ballCoord[0];
			lineY1 = ballList[BALL_NUM - 1]->ballCoord[1];
		}
	}
	//���콺 ���ʹ�ư �鶧
	//����� �ӵ��� 0�̸� ���� ĥ �� �ִ� �����̹Ƿ�..
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && ballList[BALL_NUM - 1]->ballSpd == 0) {
		mouseClick = false;
		printf("%d %d\n", lineX1 - lineX2, lineY1 - lineY2);
		printf("%lf\n", atan2(double(lineY1 - lineY2), double(lineX1 - lineX2)) * 180 / PIE);
		ballList[BALL_NUM - 1]->ballRect = atan2(double(lineY1 - lineY2), double(lineX1 - lineX2)) * 180 / PIE;
		//atan2�� �̿��Ͽ� ���� ���ư� ������ ������ش�.

		ballList[BALL_NUM - 1]->ballSpd = sqrt(pow(lineX1 - lineX2, 2.0) + pow(lineY1 - lineY2, 2.0)) / 20;
		//����� �ӵ��� ������ 20�� �������ν� ������ �Ҵ����ش�.

		lineX1 = lineY1 = lineX2 = lineY2 = 0;
		//���׸��� ���������� ��� 0����
	}
}

//���콺 �̵� �Լ�(���ʹ�ư�� ���� ���¿����� �۵���)
void mousemove(int x, int y) {

	//���콺 ������ �������ִٸ�
	if (mouseClick) {
		y = HEIGHT - y;
		lineX2 = x;
		lineY2 = y;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("�籸");		//������ �̸�
	glMatrixMode(GL_PROJECTION);	// ���� ��� ��ȯ
	glLoadIdentity();	// ������� �ʱ�ȭ
	glViewport(0, 0, WIDTH, HEIGHT);		//����Ʈ ������
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -100.0, 100.0);		//����Ʈ�� ���� ��ǥ���� 1000,600���� ����
	init();
	LoadImage(); //�̹��� �ҷ���
	glutMouseFunc(mouse);			//���콺 �Է��Լ� ���
	glutMotionFunc(mousemove);		//���콺 �̵��Լ� ���
	glutTimerFunc(1, MyTimer, 1);		//Ÿ�̸Ӽ�ġ
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}