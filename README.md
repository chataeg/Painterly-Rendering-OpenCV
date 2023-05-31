# Painterly-Rendering-OpenCV
Painterly Rendering with Curved Brush Strokes of Multiple Sizes - Aaron Hertzmann

21-1 멀티미디어 프로그래밍 논문리뷰 및 코드 구현 + 소스코드

- 이미지 파일을 브러시를 이용한 유화 혹은 점묘화로 변환


# Preview

![Valorant_2023_05_20_-_23_38_47_04_AdobeExpress](https://github.com/chataeg/C-Game-Project/assets/38041722/1a143723-ba80-4e9d-b9c6-7052cbfe7b6d)


# 원본

![source1](https://github.com/chataeg/Painterly-Rendering-OpenCV/assets/38041722/225f19a6-633c-46f3-ad35-d11e543c4b39)



# 변환 후


유화

![result](https://github.com/chataeg/Painterly-Rendering-OpenCV/assets/38041722/5070c478-57b7-4728-b8a1-57697a90854c)


점묘화

![result1](https://github.com/chataeg/Painterly-Rendering-OpenCV/assets/38041722/3b224ab7-0c5d-46e6-9079-5a0e9c967448)



# OpenCV 오픈소스 라이브러리 설치 및 실행 방법

1. OpenCV 라이브러리 다운로드:

* OpenCV 2.3.0 버전을 다운로드하려면 다음 링크를 방문하세요: https://opencv.org/releases/ 페이지에서 "OpenCV 2.3.0 for Windows" 섹션을 찾고, "Download" 링크를 클릭하여 압축 파일을 다운로드합니다. 압축 파일 해제:

다운로드한 압축 파일을 원하는 위치에 해제합니다. 예를 들어, "C:\OpenCV2.3.0"에 압축을 해제할 수 있습니다.
시스템 환경 변수 설정:

Windows 탐색기를 열고 "내 컴퓨터"를 마우스 오른쪽 버튼으로 클릭한 다음, "속성"을 선택합니다.
"시스템 속성" 창에서 "고급 시스템 설정" 링크를 클릭합니다.
"시스템 속성" 창에서 "환경 변수" 버튼을 클릭합니다.
"시스템 변수" 섹션에서 "Path" 변수를 선택하고, "편집" 버튼을 클릭합니다.
"변수 값" 편집 창에서 다음 경로를 추가합니다: "C:\OpenCV2.3.0\bin" (OpenCV 설치 위치에 따라 경로를 조정해야 할 수도 있습니다.)
변경 사항을 저장하고 모든 창을 닫습니다.
프로젝트 설정:

비주얼 스튜디오를 열고 새로운 C++ 프로젝트를 만듭니다.
솔루션 탐색기에서 프로젝트를 마우스 오른쪽 버튼으로 클릭한 다음, "속성"을 선택합니다.
"속성 페이지" 창에서 "구성 속성"을 선택하고, "C/C++" 항목을 찾아 선택합니다.
"일반" 탭에서 "추가 포함 디렉터리"를 클릭한 다음, "..." 버튼을 클릭합니다.
"포함 디렉터리 편집" 창에서 다음 경로를 추가합니다: "C:\OpenCV2.3.0\include" (OpenCV 설치 위치에 따라 경로를 조정해야 할 수도 있습니다.)
"링커" 항목을 찾아 선택한 다음, "일반" 탭으로 이동합니다.
"추가 라이브러리 디렉터리"를 클릭한 다음, "..." 버튼을 클릭합니다.
"라이브러리 디렉터리 편집" 창에서 다음 경로를 추가합니다: "C:\OpenCV2.3.0\lib" (OpenCV 설치 위치에 따라 경로를 조정해야 할 수도 있습니다.)
"링커" 항목 아래에 있는 "입력"을 선택합니다.
"추가 종속성"을 클릭한 다음, 편집 상자에 다음 라이브러리를 추가합니다:
opencv_core230.lib
opencv_imgproc230.lib
opencv_highgui230.lib
opencv_ml230.lib
opencv_video230.lib
opencv_features2d230.lib
opencv_calib3d230.lib
opencv_objdetect230.lib
opencv_contrib230.lib
opencv_legacy230.lib
opencv_flann230.lib
코드 작성:

프로젝트에서 .cpp 파일을 만들고 OpenCV 함수를 사용할 수 있습니다. 예를 들어, 다음과 같은 코드를 사용하여 이미지를 로드하고 표시할 수 있습니다:
cpp
Copy code
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
    // 이미지 로드
    cv::Mat image = cv::imread("image.jpg");

    // 이미지 표시
    cv::namedWindow("Image");
    cv::imshow("Image", image);
    cv::waitKey(0);

    return 0;
}
빌드 및 실행:
코드 작성이 완료되면 프로젝트를 빌드하고 실행할 수 있습니다. OpenCV 2.3.0 함수가 포함된 코드가 실행되며, 이미지가 표시됩니다.
이제 Visual Studio에서 OpenCV 2.3.0을 설치하고 사용할 수 있어야 합니다. 주의할 점은 OpenCV 2.3.0은 오래된 버전이므로 최신 기능을 지원하지 않을 수 있으며, 보안 및 안정성 문제가 발생할 수 있습니다. 가능하면 최신 버전의 OpenCV를 사용하는 것이 좋습니다.
