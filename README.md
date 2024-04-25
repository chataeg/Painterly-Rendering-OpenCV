# Painterly-Rendering-OpenCV
Painterly Rendering with Curved Brush Strokes of Multiple Sizes - Aaron Hertzmann

여러 사이즈의 브러시로 연결된 점들로 사진을 유화 / 점묘화 처럼 변경하는 프로그램 입니다. 사용자는 사진 파일의 경로를 입력해 본인의 사진을 유화 / 점묘화 로 Transform 시킬 수 있습니다.

# 논문
https://mrl.cs.nyu.edu/publications/painterly98/hertzmann-siggraph98.pdf

21-1 멀티미디어 프로그래밍 논문리뷰 및 코드 구현 + 소스코드

- 이미지 파일을 브러시를 이용한 유화 혹은 점묘화로 변환

# 사용된 언어 및 라이브러리
* C
* OpenCV


# 원본 이미지

![source1](https://github.com/chataeg/Painterly-Rendering-OpenCV/assets/38041722/225f19a6-633c-46f3-ad35-d11e543c4b39)


# Preview [.GIF]
![Valorant_2023_05_20_-_23_38_47_04_AdobeExpress](https://github.com/chataeg/C-Game-Project/assets/38041722/1a143723-ba80-4e9d-b9c6-7052cbfe7b6d)

# 유투브 시연 영상 (클릭하면 유투브로 이동합니다.)
[![Video Label](http://img.youtube.com/vi/_qcZQU30bIY/0.jpg)](https://youtu.be/_qcZQU30bIY)



# 변환 후


유화

![result](https://github.com/chataeg/Painterly-Rendering-OpenCV/assets/38041722/5070c478-57b7-4728-b8a1-57697a90854c)


점묘화

![result1](https://github.com/chataeg/Painterly-Rendering-OpenCV/assets/38041722/3b224ab7-0c5d-46e6-9079-5a0e9c967448)



# OpenCV 오픈소스 라이브러리 설치 및 실행 방법

# 1. OpenCV 라이브러리 다운로드:

```
* OpenCV 최신버전을 다운로드하려면 다음 링크를 방문하세요: https://opencv.org/releases/ 
* 저장소에 포함되어 있는 OpenCV 2.3.0 파일을 사용하는 것을 추천합니다. 필자는 2.3.0 버전을 이용해 구현했고 컴파일러 설정이 최신 버전과 상이할 수 있습니다.
```

# 2. 비주얼 스튜디오 설정
```
* 비주얼 스튜디오를 열고 빈 C++ 프로젝트를 만듭니다. 솔루션 탐색기에서 프로젝트를 마우스 오른쪽 버튼으로 클릭한 다음, "속성"을 선택합니다. 
* "속성 페이지" 창에서 "구성 속성"을 선택하고, "C/C++" 항목을 찾아 선택합니다. "일반" 탭에서 "추가 포함 디렉터리"를 클릭한 다음, "..." 버튼을 클릭합니다. 
* "포함 디렉터리 편집" 창에서 include 폴더의 경로를 입력/추가 해줍니다. 
* "링커" 항목을 찾아 선택한 다음, "일반" 탭으로 이동합니다. "추가 라이브러리 디렉터리"를 클릭한 다음, "..." 버튼을 클릭합니다. 
* "라이브러리 디렉터리 편집" 창에서 lib 폴더 경로를 추가해 줍니다. 
* "링커" 항목 아래에 있는 "입력"을 선택합니다. "추가 종속성"을 클릭한 다음, 편집 상자에 다음 라이브러리를 추가합니다.
(OpenCV 설치 위치에 따라 경로를 조정해야 할 수도 있음 - 현재 글은 2.3.0 기준)

- opencv_core230.lib
- opencv_imgproc230.lib
- opencv_highgui230.lib
```

# 3. 코드 작성:

* 프로젝트에서 .cpp 파일을 만들고 OpenCV 함수를 사용할 수 있습니다. 예를 들어, 다음과 같은 코드를 사용하여 이미지를 로드하고 표시할 수 있습니다:

```
#include<opencv2/opencv.hpp>
int main()
{
	
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
	
	IplImage* src = cvLoadImage(filename); // 이미지 로드와 이미지 객체 포인터 할당
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3); // src 의 사이즈, 픽셀 값 비트 길이(8), 이미지의 컬러 채널 수 (3)
	
	
	cvShowImage("canvas", dst); // 'canvas' 라는 이름의 창에 이미지 표시
	cvWaitKey(); // 키 입력을 기다림
	return 0;
}
```

위 코드가 정상적으로 실행되고 이미지가 나타나면 OpenCV 설치가 완료된 것입니다.


# 4. 소스코드 실행:

* 빈 프로젝트를 만든 후 위의 일련의 설정들을 따라 가서 프로그램을 시작할 수 있지만, 저장소 다운로드자의 편의를 위해 프로젝트 파일, exe 파일 또한 추가하였음.



