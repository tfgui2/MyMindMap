# My Mind Map

-- simple mind map

run MyMindMap.exe and check help menu.

- 배포 파일들 모으기:
windeployqt --dir deploy --release release\MyMindMap.exe

- qt 5.15에서 windeployqt에 버그가 있는듯 : Unable to find the platform plugin. 이라는 에러를 낸다.
- 수동으로 dll을 복사함
1. C:\Qt\5.15.2\mingw81_64\bin 에 있는 전체 dll을 복사함
2. C:\Qt\5.15.2\mingw81_64\plugins\platforms 의 qwindows.dll 을 platforms 폴더를 만들어 복사함
자세한 정보는 여기에서 확인 : https://doc.qt.io/qt-5/windows-deployment.html
