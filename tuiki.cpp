//---------------------------------------------------------
// �T�v      : �ǋL�v���O�����i��񖳂��j
// File Name : tuiki.cpp
// Library   : OpenCV 2.0
//---------------------------------------------------------

#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#pragma comment (lib,"cv200d.lib")
#pragma comment (lib,"cxcore200d.lib")
#pragma comment (lib,"highgui200d.lib")



#define WIDTH	640	//	�L���v�`���摜�̉���
#define HEIGHT	480	//	�L���v�`���摜�̏c��

#define THRESH_BOTTOM	200			//	���x������臒l
//#define THRESH_TOP	80			//	���x�l�̏����臒l
#define THRESHOLD_MAX_VALUE	255		//	2�l���̍ۂɎg�p����ő�l

#define CIRCLE_RADIUS	2		//	�~�̔��a
#define LINE_THICKNESS	3		//	���̑���
#define LINE_TYPE		8		//	���̎��
#define SHIFT			0		//	���W�̏����_�ȉ��̌���\���r�b�g��


int main( int argc, char **argv ){ 
	int key;							//	�L�[���͗p�̕ϐ�
	CvCapture *capture = NULL;			//	�J�����L���v�`���p�̍\����
	IplImage *frameImage;				//	�L���v�`���摜�pIplImage
	IplImage *frameImage2;				//	�L���v�`���摜�pIplImage2

	//	�摜�𐶐�����
	IplImage *backgroundImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//�w�i�摜�pIplImage
	IplImage *grayImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//�O���[�X�P�[���摜�pIplImage
	IplImage *differenceImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//�����摜�pIplImage

	IplImage *hsvImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );				//HSV�摜�pIplImage
	IplImage *hueImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//�F��(H)���pIplImage
	IplImage *saturationImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//�ʓx(S)���pIplImage
	IplImage *valueImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//���x(V)���pIplImage
	IplImage *thresholdImage1 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//���x��THRES_BOTTOM���傫���̈�pIplImage
//	IplImage *thresholdImage2 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//���x��THRES_TOP�ȉ��̗̈�pIplImage
//	IplImage *thresholdImage3 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );		//thresholdImage1��thresholdImage2��AND���Z���ʗpIplImage
	IplImage *lightImage = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1 );				//�����Ă��镔���̗̈�̒��o���ʗpIplImage
	
	char windowNameCapture[] = "Capture"; 			//�L���v�`�������摜��\������E�B���h�E�̖��O
	char windowNameLight[] = "Light";				//�����Ă��镔���̗̈��\������E�B���h�E�̖��O
	char windowNameCapture2[] = "Capture2"; 		//�L���v�`�������摜��\������E�B���h�E�̖��O
	char windowNameThreshold[] = "Threshold";		//thresholdImage1��\������E�B���h�E�̖��O

	CvMoments moment;
	double m_00;
	double m_10;
	double m_01;
	int gravityX;
	int gravityY;
	unsigned char h,s,v;
    
	//	�J����������������
	if ( ( capture = cvCreateCameraCapture( 0 ) ) == NULL ) {
		//	�J������������Ȃ������ꍇ
		printf( "�J������������܂���\n" );
		return -1;
	}

	//	�E�B���h�E�𐶐�����
	cvNamedWindow( windowNameCapture, CV_WINDOW_AUTOSIZE );
	cvNamedWindow( windowNameLight, CV_WINDOW_AUTOSIZE );
  	cvNamedWindow( windowNameCapture2, CV_WINDOW_AUTOSIZE );
	cvNamedWindow( windowNameThreshold, CV_WINDOW_AUTOSIZE );

  	//	�����w�i��ݒ肷�邽�߂ɃJ��������摜���擾
	frameImage = cvQueryFrame( capture );
	//	frameImage���O���[�X�P�[�������A�w�i�摜�Ƃ���
	cvCvtColor( frameImage, backgroundImage, CV_BGR2GRAY );
	frameImage2 = cvCreateImage( cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3 );
	cvSet (frameImage2, CV_RGB(0,0,0));  //���F�œh��Ԃ�

	//	���C�����[�v
	while( 1 ) {
		//	capture�̓��͉摜�t���[����frameImage�Ɋi�[����
		frameImage = cvQueryFrame( capture );
		//	frameImage���O���[�X�P�[�����������̂��AgrayImage�Ɋi�[����
		cvCvtColor( frameImage, grayImage, CV_BGR2GRAY );
		//	grayImage�Ɣw�i�摜�Ƃ̍������Ƃ�
		cvAbsDiff( grayImage, backgroundImage, differenceImage );
		
		//	frameImage��BGR����HSV�ɕϊ�����
		cvCvtColor( frameImage, hsvImage, CV_BGR2HSV );
		//	HSV�摜��H�AS�AV�摜�ɕ�����
		cvSplit( hsvImage, hueImage, saturationImage, valueImage, NULL );
		//	���x�����邢�����𒊏o�A���̕����̂ݏo�͂���
		cvThreshold( valueImage, thresholdImage1, THRESH_BOTTOM, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );
//		cvThreshold( hueImage, thresholdImage2, THRESH_TOP, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY_INV );
//		cvAnd( thresholdImage1, thresholdImage2, thresholdImage3, NULL );
		
		//	�w�i�����摜�Ɩ��邢�̈�Ƃ�AND���Ƃ�
		cvAnd( differenceImage, thresholdImage1, lightImage, NULL );
		
		//	�����Ă���̈�̏d�S���Z�o����
		cvMoments( lightImage, &moment, 0 );
		m_00 = cvGetSpatialMoment( &moment, 0, 0 );
		m_10 = cvGetSpatialMoment( &moment, 1, 0 );
		m_01 = cvGetSpatialMoment( &moment, 0, 1 );
		gravityX = m_10 / m_00;
		gravityY = m_01 / m_00;

		if (0<gravityX){
			h = hsvImage ->imageData[hsvImage ->widthStep * (gravityY) + gravityX * 3 ];     // B
			s = hsvImage ->imageData[hsvImage ->widthStep * gravityY + gravityX * 3 + 1];    // G
			v = hsvImage ->imageData[hsvImage ->widthStep * gravityY + gravityX * 3 + 2];    // R

			printf ("x= %d ,y= %d v= %d,s= %d,h= %d  \n" ,gravityX,gravityY,v,s,h);

		//	�摜��ɉ~��`�悷��
			if (v>200){
			cvCircle( frameImage2, cvPoint( gravityX, gravityY ), CIRCLE_RADIUS,
			 CV_RGB( 150, 0, 0 ), LINE_THICKNESS, LINE_TYPE, 0 );

			}
		}

		//	�摜��\������
		cvShowImage( windowNameCapture, frameImage );
		cvShowImage( windowNameLight, lightImage );
		cvShowImage( windowNameCapture2,   frameImage2);
		cvShowImage( windowNameThreshold, thresholdImage1);

		//	�L�[���͔���
		key = cvWaitKey( 10 );
		if( key == 'q' ) 
			//	'q'�L�[�������ꂽ�烋�[�v�𔲂���
			break;
		else if( key == 'b' ) {
			//	'b'�L�[�������ꂽ��A���̎��_�ł̉摜��w�i�摜�Ƃ���
		   frameImage = cvQueryFrame( capture );
		    cvCvtColor( frameImage, backgroundImage, CV_BGR2GRAY );
		}
		else if(key == 'c') {
			//	'c'�L�[�������ꂽ��摜��ۑ�
			cvSaveImage( "image/frame.bmp", frameImage );
			cvSaveImage( "image/light.bmp", lightImage );
		}
	}
	//	�L���v�`�����������
	cvReleaseCapture( &capture );
	//	���������������
	cvReleaseImage( &backgroundImage );
	cvReleaseImage( &grayImage );
	cvReleaseImage( &differenceImage );
	cvReleaseImage( &hsvImage );
	cvReleaseImage( &hueImage );
	cvReleaseImage( &saturationImage );
	cvReleaseImage( &valueImage );
	cvReleaseImage( &thresholdImage1 );
//	cvReleaseImage( &thresholdImage2 );
//	cvReleaseImage( &thresholdImage3 );
	cvReleaseImage( &lightImage );
	//	�E�B���h�E��j������
	cvDestroyWindow( windowNameCapture );
	cvDestroyWindow( windowNameLight );
	cvDestroyWindow( windowNameThreshold );
	cvDestroyWindow( windowNameCapture2 );

	return 0;
} 
