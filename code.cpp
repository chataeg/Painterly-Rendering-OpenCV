#include<opencv2/opencv.hpp>
int dot_or_line; //Drawing Mode Index

double getDiff(CvScalar src, CvScalar dst) // RGB�� ���̸� ����� �Լ�
{
	return (src.val[0] - dst.val[0]) * (src.val[0] - dst.val[0]) 
		+ (src.val[1] - dst.val[1]) * (src.val[1] - dst.val[1])
		+ (src.val[2] - dst.val[2]) * (src.val[2] - dst.val[2]);  
	//����� ����� ���� �� ���̸� ���� �����ؼ� ���Ѵ�.
}
double getGradient(CvScalar src, CvScalar dst)
{
	//�� ���̸� ��ȯ�ϴ� �Լ� (gradient �� ������ �˱����� �������� ����.)
	return ((src.val[0] - dst.val[0])
		+ (src.val[1] - dst.val[1]) 
		+ (src.val[2] - dst.val[2])) ;
}
void makeSplineStroke(IplImage* canvas, IplImage* ref,  CvPoint** S, CvScalar** C, int R[], int Ri, int GX, int GY)
{
	//�ش� �׸����� ������ �湮 ���θ� ������ 2���� �迭
	int** V = (int**)malloc(sizeof(int*) * (GY + 1));
	for (int i = 0; i < (GY + 1); i++)
		V[i] = (int*)malloc(sizeof(int) * (GX));

	//0���� �迭 �ʱ�ȭ. 0 = �̹湮 1 = �湮
	for (int i = 0; i < (GY); i++) 
		for (int j = 0; j < (GX); j++)
			V[i][j] = 0;
		
	//Spline �� �̾��� ��ǥ �迭 ���� �Ҵ� (ũ��� maxstrokeLength �� ���� ������ �˳��ϰ� �Ҵ���.)
	CvPoint* SP = (CvPoint*)malloc(sizeof(CvPoint) * 1000);
	
	//�ּ�, �ִ� Spline ���̸� ������ ����
	int maxStrokeLength = 20;
	int minStrokeLength = 10;
	//previous ��ġ�� ������ ��ǥ ����
	int py = 0, px = 0;

	//�׸��� ������ŭ �ݺ�
	for (int k = 0; k < GX*GY; k++) {
		//Spline �� ������ ���� �������� ����.
		//ind�� �迭�� �ε���
		//y, x �� ���� �ε���
		int ind = 0;
		int y, x;
		
		while (1)
		{
			y = rand() % GY;
			x = rand() % GX;

			//���� �ε����� �湮���� ���� ���̶��
			if (V[y][x] == 0)
			{
				V[y][x] = 1; // �ε����� �����ϰ�
				break; // Ż��
			}
		}

		// ó�� ��ǥ�̹Ƿ� ������ �迭�� �����ϰ� �ε��� ����
		SP[ind] = S[y][x];
		ind++;
		//ó�� �湮������ Ȯ������ �� ���
		bool first = true;

		//�̵��ϴ� ��ǥ�� ������ ���� ����
		int X, Y;
		X = x;
		Y = y;
		//�ִ� ��Ʈ��ũ ���̱��� �ݺ�
		for (int i = 1; i < maxStrokeLength; i++)
		{

			//��ǥ ���� ����ó��
			if (Y < 0 || Y >= canvas->height) break;
			if (X < 0 || X >= canvas->width) break;
			if (Y + 1 < 0 || Y + 1 >= canvas->height) break;
			if (X + 1 < 0 || X + 1 >= canvas->width) break;

			//�ּ� ��Ʈ��ũ �̻��̰� ĵ������ ���۷��� �̹����� ���̰� 
			//�� ����� ���۷��� �̹����� ���̺��� �۴ٸ� Spline ����.
			if (i > minStrokeLength 
				&& 
				(getDiff(cvGet2D(canvas, Y, X), cvGet2D(ref, Y, X)) 
					<
				 getDiff(C[y][x], cvGet2D(ref, Y, X))))
				break;

			//gx gy ������ ����
			double gx, gy;
			 
			//�� ���̸� �����Ͽ� ���� ����
			gx = getGradient(cvGet2D(ref, Y, X + 1), cvGet2D(ref, Y, X));
			gy = getGradient(cvGet2D(ref, Y + 1, X), cvGet2D(ref, Y, X));
		
			//�� ���̰� ���ٸ� Spline ����
			if (getGradient(cvGet2D(ref, Y + 1, X), cvGet2D(ref, Y, X)) == 0
				&& 
				getGradient(cvGet2D(ref, Y, X + 1), cvGet2D(ref, Y, X)) == 0) 
				break;
		
			//�������ͷ� ����
			double dx = -gy;
			double dy = gx;

			//������ �Ͽ� 0���� �۴ٸ� ��Ʈ��ũ�� ���� ȸ���� �߻��� ���̹Ƿ�
			//�ݴ� ���� ���ͷ� ����
			if (px * dx + py * dy < 0)
			{
				dx = -dx;
				dy = -dy;
			}
			
			//�̵��� ��ǥ ���
			//u, v = ���ο� ��ǥ
			int u, v;
			double theta = atan2(dy, dx);

			if (first) //ù ��ǥ�̸� ù ��° control point ���� 
			{
				//���� ũ�⸸ŭ ��ǥ �̵�
				u = S[y][x].x + R[Ri] * cos(theta);
				v = S[y][x].y + R[Ri] * sin(theta);
			}
			else // �� ��° �̻��̸� ������ ��ǥ���� ������
			{
				//���� ũ�⸸ŭ ��ǥ �̵�
				u = X + R[Ri] * cos(theta);
				v = Y + R[Ri] * sin(theta);
			}
	
			//���� ������ ��ǥ ����ó��
			if (u < 0 || u > canvas->width) break;
			if (v < 0 || v > canvas->height) break;

			//���� ��ǥ �迭�� ���� �� �ε��� ����
			SP[ind] = cvPoint(u, v);
			ind++;

			//ù ��° ������ �������Ƿ� �� ��� ����
			if(first == true)
				first = false;
			//��ǥ ����
			X = u;
			Y = v;
			//���� ��ǥ ����
			px = u;
			py = v;
		}

		//cvLine �Լ��� �̿��� Spline ����
		for (int i = 0; i < ind - 1; i++)
			cvLine(canvas, SP[i], SP[i + 1], C[y][x], R[Ri]);
		
	}

	cvShowImage("canvas", canvas);
	cvWaitKey(1000);

	//�����Ҵ� �޸� ����
	for (int i = 0; i < (GY + 1); i++)
		free(V[i]);
	free(V);

	free(SP);
}
void paintStroke(IplImage* src, IplImage* dst, CvPoint** S, CvScalar** C, int R[],int Ri,int GX, int GY)
{
	//�ش� �׸����� ������ �湮 ���θ� ������ 2���� �迭
	int** V = (int **)malloc(sizeof(int*) * (GY+1));
	for (int i = 0; i < (GY+1); i++)
		V[i] = (int*)malloc(sizeof(int) * (GX));
	
	//0���� �迭 �ʱ�ȭ. 0 = �̹湮 1 = �湮
	for (int i = 0; i < (GY); i++) 
		for (int j = 0; j < (GX); j++)
			V[i][j] = 0;
	
	//�׸����� ũ�⸸ŭ �ݺ�
	for (int i = 0; i < GX*GY; i++)
	{
		int y, x;
		//�������� �׸��� ���� ������ �ε��� ����
		while (1)
		{
			y = rand() % GY;
			x = rand() % GX;

			//���� �ε����� �湮���� ���� ���̶��
			if (V[y][x] == 0)
			{
				V[y][x] = 1; // �ε����� �����ϰ�
				break; // Ż��
			}
		}
		//������ �ε����� ���� �������� Circle ����
		cvCircle(dst, S[y][x], R[Ri], C[y][x], -1);
	}
	
	cvShowImage("canvas", dst);
	cvWaitKey(1000);

	//�����Ҵ� �޸� ����
	for (int i = 0; i < (GY + 1); i++)
		free(V[i]);
	free(V);
}
void paintLayer(IplImage* canvas, IplImage* ref,IplImage* src, int R[], int i)
{
		//�׸����� X �� ����, Y �� ���� ���
		int gridX =  canvas->width / R[i]; 
		int gridY =  canvas->height / R[i];
		
		//��ǥ�� ������ 2���� �迭 �����Ҵ� 
		CvPoint** S = (CvPoint **)malloc(sizeof(CvPoint *) *(gridY+1));
		for (int i = 0; i < (gridY+1); i++)
			S[i] = (CvPoint*)malloc(sizeof(CvPoint) * (gridX+1));
		//�ش� ��ǥ�� �÷� ���� ������ 2���� �迭 �����Ҵ�
		CvScalar** C = (CvScalar **)malloc(sizeof(CvScalar*) * (gridY+1));
		for (int i = 0; i < (gridY+1); i++)
			C[i] = (CvScalar*)malloc(sizeof(CvScalar) * (gridX+1));
	
		//��ü �׸��忡 ���Ͽ� �ݺ�
		for(int u = 0 ; u < gridY ; u++)
			for (int v = 0; v < gridX; v++)
			{
			
				double max = -FLT_MAX;
				double avg = 0; //����� ������ ����

				//������ �׸����� ������ ���� �ݺ�
				for (int y = u * R[i]; y < (u + 1) * R[i]; y++)
					for (int x = v * R[i]; x < (v + 1) * R[i]; x++)
					{
						if (y < 0 || y > canvas->height - 1) continue;
						if (x < 0 || x > canvas->width - 1) continue;

						CvScalar a = cvGet2D(canvas, y, x);
						CvScalar b = cvGet2D(ref, y, x);

						//getDiff �Լ��� �̿��� ���۷��� �̹����� ĵ������
						//���̰��� ���� �� avg �� ������
						double diff = getDiff(a, b);
						avg += diff; 

						//������ ���ذ��� �����ϴ� ���ÿ� ���� ������ ū ���� ã�´�.
						if (max < diff) {
							max = diff;
							//���� �����ߴ� 2���� �迭�� ������ ����
							S[u][v] = cvPoint(x, y);
							C[u][v] = cvGet2D(ref, y, x);
						}
					}
			
				//�������� �����־� ��� ������ ���
				avg = avg / (R[i] * R[i]); 

				//�׸��� ��尡 0�϶� 
				if (dot_or_line == 0)
				{
					//������ ��� ������ 1200 �����̸�
					//ĵ������ �ٱ����� ��ǥ�� ������. 
					if (avg < 1200 ) {
						S[u][v] = cvPoint(-100,-100);
					}
				}
			}

		//�׸��� ��尡 0�̸� paintStroke ȣ��
		if (dot_or_line == 0)
 			paintStroke(ref, canvas, S, C, R, i,  gridX, gridY);
		//�׸��� ��尡 1�̸� makeSplineStroke ȣ��
		else if (dot_or_line == 1)
			makeSplineStroke(canvas, ref, S, C, R,i,gridX,gridY);

		//�����Ҵ� �޸� ����
		for (int i = 0; i < (gridY + 1); i++)
			free(S[i]);
		free(S);
		for (int i = 0; i < (gridY + 1); i++)
			free(C[i]);
		free(C);
}
void paint(IplImage* src,IplImage* dst, int R[])
{
	IplImage* ref = cvCreateImage(cvGetSize(src), 8, 3); //���۷��� �̹����� ������ ������
	IplImage* canvas = cvCreateImage(cvGetSize(src), 8, 3); //ĵ����

	cvSet(canvas, cvScalar(255, 255, 255)); //ĵ������ �Ͼ������ �ʱ�ȭ

	for (int i = 0; i < 5; i++)
	{
		cvSmooth(src, ref, CV_GAUSSIAN, R[i]); //���۷��� �̹����� �� ũ��� ����þ� ����
		paintLayer(canvas, ref, src, R, i); //���̾ ĥ�� �Լ� ȣ��
	}

	cvCopy(canvas, dst); // ���̾ ���� ĥ�� �� ĵ������ dst �̹��� �����Ϳ� ����
}
int main()
{
	srand(1000); //���� �Լ� ���� �õ�
	printf("=============================================\nDepartment of Software, Sejong University\nMultimedia Programming Homework #4\nPainterly Rendering\n=============================================\n"); 
	
	char filename[1000];
	while (true) {
		printf("���� ��θ� �Է��Ͻÿ�.(���� ����� �ִ� ���ڿ� ũ��� 1000 �Դϴ�.)\n->");
		scanf("%s", &filename);

		if (cvLoadImage(filename) == NULL)
		{
			printf("������ ã�� �� �����ϴ�! �ٽ� �Է��ϼ���.");
		}
		else break;
	}

	IplImage* src = cvLoadImage(filename); 
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3); 
	

	while (1)
	{
		printf("�׸��� ��带 �����ϼ���. 0 = Circle, 1 = Stroke:");
		scanf("%d", &dot_or_line);
		getchar();
		if (dot_or_line != 0 && dot_or_line != 1) {
			printf("�߸��� ��ȣ�Դϴ�! �ٽ� �����ϼ���.\n");
			continue;
		}
		else break;
	}

	int R[5]; //�� ũ�⸦ �����ϴ� �迭


		R[0] = 35;	R[1] = 15;	R[2] = 7;	R[3] = 5; R[4] = 3; // ������ �� ũ��


	paint(src,dst, R); // paint �Լ� ȣ��
	cvShowImage("canvas", dst);
	cvWaitKey();
	return 0;
}