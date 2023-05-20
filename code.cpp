#include<opencv2/opencv.hpp>
int dot_or_line; //Drawing Mode Index

double getDiff(CvScalar src, CvScalar dst) // RGB값 차이를 계산할 함수
{
	return (src.val[0] - dst.val[0]) * (src.val[0] - dst.val[0]) 
		+ (src.val[1] - dst.val[1]) * (src.val[1] - dst.val[1])
		+ (src.val[2] - dst.val[2]) * (src.val[2] - dst.val[2]);  
	//양수로 만들기 위해 색 차이를 각각 제곱해서 더한다.
}
double getGradient(CvScalar src, CvScalar dst)
{
	//색 차이를 반환하는 함수 (gradient 의 방향을 알기위해 제곱하지 않음.)
	return ((src.val[0] - dst.val[0])
		+ (src.val[1] - dst.val[1]) 
		+ (src.val[2] - dst.val[2])) ;
}
void makeSplineStroke(IplImage* canvas, IplImage* ref,  CvPoint** S, CvScalar** C, int R[], int Ri, int GX, int GY)
{
	//해당 그리드의 오차점 방문 여부를 저장할 2차원 배열
	int** V = (int**)malloc(sizeof(int*) * (GY + 1));
	for (int i = 0; i < (GY + 1); i++)
		V[i] = (int*)malloc(sizeof(int) * (GX));

	//0으로 배열 초기화. 0 = 미방문 1 = 방문
	for (int i = 0; i < (GY); i++) 
		for (int j = 0; j < (GX); j++)
			V[i][j] = 0;
		
	//Spline 을 이어줄 좌표 배열 동적 할당 (크기는 maxstrokeLength 를 넘지 않지만 넉넉하게 할당함.)
	CvPoint* SP = (CvPoint*)malloc(sizeof(CvPoint) * 1000);
	
	//최소, 최대 Spline 길이를 저장할 변수
	int maxStrokeLength = 20;
	int minStrokeLength = 10;
	//previous 위치를 저장할 좌표 변수
	int py = 0, px = 0;

	//그리드 개수만큼 반복
	for (int k = 0; k < GX*GY; k++) {
		//Spline 을 시작할 점을 랜덤으로 고른다.
		//ind는 배열의 인덱스
		//y, x 는 랜덤 인덱스
		int ind = 0;
		int y, x;
		
		while (1)
		{
			y = rand() % GY;
			x = rand() % GX;

			//랜덤 인덱스가 방문하지 않은 곳이라면
			if (V[y][x] == 0)
			{
				V[y][x] = 1; // 인덱스를 변경하고
				break; // 탈출
			}
		}

		// 처음 좌표이므로 무조건 배열에 저장하고 인덱스 증가
		SP[ind] = S[y][x];
		ind++;
		//처음 방문인지를 확인해줄 불 대수
		bool first = true;

		//이동하는 좌표를 저장할 변수 선언
		int X, Y;
		X = x;
		Y = y;
		//최대 스트로크 길이까지 반복
		for (int i = 1; i < maxStrokeLength; i++)
		{

			//좌표 범위 예외처리
			if (Y < 0 || Y >= canvas->height) break;
			if (X < 0 || X >= canvas->width) break;
			if (Y + 1 < 0 || Y + 1 >= canvas->height) break;
			if (X + 1 < 0 || X + 1 >= canvas->width) break;

			//최소 스트로크 이상이고 캔버스와 레퍼런스 이미지의 차이가 
			//붓 색깔과 레퍼런스 이미지의 차이보다 작다면 Spline 종료.
			if (i > minStrokeLength 
				&& 
				(getDiff(cvGet2D(canvas, Y, X), cvGet2D(ref, Y, X)) 
					<
				 getDiff(C[y][x], cvGet2D(ref, Y, X))))
				break;

			//gx gy 저장할 변수
			double gx, gy;
			 
			//색 차이를 저장하여 벡터 생성
			gx = getGradient(cvGet2D(ref, Y, X + 1), cvGet2D(ref, Y, X));
			gy = getGradient(cvGet2D(ref, Y + 1, X), cvGet2D(ref, Y, X));
		
			//색 차이가 없다면 Spline 종료
			if (getGradient(cvGet2D(ref, Y + 1, X), cvGet2D(ref, Y, X)) == 0
				&& 
				getGradient(cvGet2D(ref, Y, X + 1), cvGet2D(ref, Y, X)) == 0) 
				break;
		
			//법선벡터로 변경
			double dx = -gy;
			double dy = gx;

			//내적을 하여 0보다 작다면 스트로크의 급한 회전이 발생한 것이므로
			//반대 방향 벡터로 변경
			if (px * dx + py * dy < 0)
			{
				dx = -dx;
				dy = -dy;
			}
			
			//이동할 좌표 계산
			//u, v = 새로운 좌표
			int u, v;
			double theta = atan2(dy, dx);

			if (first) //첫 좌표이면 첫 번째 control point 에서 
			{
				//붓의 크기만큼 좌표 이동
				u = S[y][x].x + R[Ri] * cos(theta);
				v = S[y][x].y + R[Ri] * sin(theta);
			}
			else // 두 번째 이상이면 이전의 좌표에서 더해줌
			{
				//붓의 크기만큼 좌표 이동
				u = X + R[Ri] * cos(theta);
				v = Y + R[Ri] * sin(theta);
			}
	
			//새로 형성한 좌표 예외처리
			if (u < 0 || u > canvas->width) break;
			if (v < 0 || v > canvas->height) break;

			//다음 좌표 배열에 저장 후 인덱스 증가
			SP[ind] = cvPoint(u, v);
			ind++;

			//첫 번째 수행이 끝났으므로 불 대수 해제
			if(first == true)
				first = false;
			//좌표 갱신
			X = u;
			Y = v;
			//이전 좌표 갱신
			px = u;
			py = v;
		}

		//cvLine 함수를 이용해 Spline 형성
		for (int i = 0; i < ind - 1; i++)
			cvLine(canvas, SP[i], SP[i + 1], C[y][x], R[Ri]);
		
	}

	cvShowImage("canvas", canvas);
	cvWaitKey(1000);

	//동적할당 메모리 해제
	for (int i = 0; i < (GY + 1); i++)
		free(V[i]);
	free(V);

	free(SP);
}
void paintStroke(IplImage* src, IplImage* dst, CvPoint** S, CvScalar** C, int R[],int Ri,int GX, int GY)
{
	//해당 그리드의 오차점 방문 여부를 저장할 2차원 배열
	int** V = (int **)malloc(sizeof(int*) * (GY+1));
	for (int i = 0; i < (GY+1); i++)
		V[i] = (int*)malloc(sizeof(int) * (GX));
	
	//0으로 배열 초기화. 0 = 미방문 1 = 방문
	for (int i = 0; i < (GY); i++) 
		for (int j = 0; j < (GX); j++)
			V[i][j] = 0;
	
	//그리드의 크기만큼 반복
	for (int i = 0; i < GX*GY; i++)
	{
		int y, x;
		//랜덤으로 그리드 내의 범위의 인덱스 설정
		while (1)
		{
			y = rand() % GY;
			x = rand() % GX;

			//랜덤 인덱스가 방문하지 않은 곳이라면
			if (V[y][x] == 0)
			{
				V[y][x] = 1; // 인덱스를 변경하고
				break; // 탈출
			}
		}
		//랜덤한 인덱스를 토대로 랜덤으로 Circle 생성
		cvCircle(dst, S[y][x], R[Ri], C[y][x], -1);
	}
	
	cvShowImage("canvas", dst);
	cvWaitKey(1000);

	//동적할당 메모리 해제
	for (int i = 0; i < (GY + 1); i++)
		free(V[i]);
	free(V);
}
void paintLayer(IplImage* canvas, IplImage* ref,IplImage* src, int R[], int i)
{
		//그리드의 X 선 개수, Y 선 개수 계산
		int gridX =  canvas->width / R[i]; 
		int gridY =  canvas->height / R[i];
		
		//좌표를 저장할 2차원 배열 동적할당 
		CvPoint** S = (CvPoint **)malloc(sizeof(CvPoint *) *(gridY+1));
		for (int i = 0; i < (gridY+1); i++)
			S[i] = (CvPoint*)malloc(sizeof(CvPoint) * (gridX+1));
		//해당 좌표의 컬러 값을 저장할 2차원 배열 동적할당
		CvScalar** C = (CvScalar **)malloc(sizeof(CvScalar*) * (gridY+1));
		for (int i = 0; i < (gridY+1); i++)
			C[i] = (CvScalar*)malloc(sizeof(CvScalar) * (gridX+1));
	
		//전체 그리드에 대하여 반복
		for(int u = 0 ; u < gridY ; u++)
			for (int v = 0; v < gridX; v++)
			{
			
				double max = -FLT_MAX;
				double avg = 0; //평균을 저장할 변수

				//각각의 그리드의 면적에 대해 반복
				for (int y = u * R[i]; y < (u + 1) * R[i]; y++)
					for (int x = v * R[i]; x < (v + 1) * R[i]; x++)
					{
						if (y < 0 || y > canvas->height - 1) continue;
						if (x < 0 || x > canvas->width - 1) continue;

						CvScalar a = cvGet2D(canvas, y, x);
						CvScalar b = cvGet2D(ref, y, x);

						//getDiff 함수를 이용해 레퍼런스 이미지와 캔버스의
						//차이값을 구한 뒤 avg 에 더해줌
						double diff = getDiff(a, b);
						avg += diff; 

						//오차를 더해가며 저장하는 동시에 가장 오차가 큰 점을 찾는다.
						if (max < diff) {
							max = diff;
							//각각 선언했던 2차원 배열에 정보를 저장
							S[u][v] = cvPoint(x, y);
							C[u][v] = cvGet2D(ref, y, x);
						}
					}
			
				//면적으로 나눠주어 평균 오차를 계산
				avg = avg / (R[i] * R[i]); 

				//그리기 모드가 0일때 
				if (dot_or_line == 0)
				{
					//면적의 평균 오차가 1200 이하이면
					//캔버스의 바깥으로 좌표를 보낸다. 
					if (avg < 1200 ) {
						S[u][v] = cvPoint(-100,-100);
					}
				}
			}

		//그리기 모드가 0이면 paintStroke 호출
		if (dot_or_line == 0)
 			paintStroke(ref, canvas, S, C, R, i,  gridX, gridY);
		//그리기 모드가 1이면 makeSplineStroke 호출
		else if (dot_or_line == 1)
			makeSplineStroke(canvas, ref, S, C, R,i,gridX,gridY);

		//동적할당 메모리 해제
		for (int i = 0; i < (gridY + 1); i++)
			free(S[i]);
		free(S);
		for (int i = 0; i < (gridY + 1); i++)
			free(C[i]);
		free(C);
}
void paint(IplImage* src,IplImage* dst, int R[])
{
	IplImage* ref = cvCreateImage(cvGetSize(src), 8, 3); //레퍼런스 이미지를 저장할 포인터
	IplImage* canvas = cvCreateImage(cvGetSize(src), 8, 3); //캔버스

	cvSet(canvas, cvScalar(255, 255, 255)); //캔버스를 하얀색으로 초기화

	for (int i = 0; i < 5; i++)
	{
		cvSmooth(src, ref, CV_GAUSSIAN, R[i]); //레퍼런스 이미지를 붓 크기로 가우시안 블러링
		paintLayer(canvas, ref, src, R, i); //레이어를 칠할 함수 호출
	}

	cvCopy(canvas, dst); // 레이어를 전부 칠한 후 캔버스를 dst 이미지 포인터에 저장
}
int main()
{
	srand(1000); //랜덤 함수 고정 시드
	printf("=============================================\nDepartment of Software, Sejong University\nMultimedia Programming Homework #4\nPainterly Rendering\n=============================================\n"); 
	
	char filename[1000];
	while (true) {
		printf("파일 경로를 입력하시오.(파일 경로의 최대 문자열 크기는 1000 입니다.)\n->");
		scanf("%s", &filename);

		if (cvLoadImage(filename) == NULL)
		{
			printf("파일을 찾을 수 없습니다! 다시 입력하세요.");
		}
		else break;
	}

	IplImage* src = cvLoadImage(filename); 
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3); 
	

	while (1)
	{
		printf("그리기 모드를 선택하세요. 0 = Circle, 1 = Stroke:");
		scanf("%d", &dot_or_line);
		getchar();
		if (dot_or_line != 0 && dot_or_line != 1) {
			printf("잘못된 번호입니다! 다시 선택하세요.\n");
			continue;
		}
		else break;
	}

	int R[5]; //붓 크기를 저장하는 배열


		R[0] = 35;	R[1] = 15;	R[2] = 7;	R[3] = 5; R[4] = 3; // 각각의 붓 크기


	paint(src,dst, R); // paint 함수 호출
	cvShowImage("canvas", dst);
	cvWaitKey();
	return 0;
}