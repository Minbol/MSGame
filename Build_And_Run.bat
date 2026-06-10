@echo off
setlocal

:: ==========================================
:: 환경 변수 설정 (본인의 환경에 맞게 수정하세요)
:: ==========================================
set "ENGINE_PATH=D:\Epic\UE_5.7"
set "PROJECT_NAME=MG"
:: ==========================================

set "UBT_PATH=%ENGINE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
set "UPROJECT_PATH=%~dp0%PROJECT_NAME%.uproject"

echo ==========================================
echo [1/3] Unreal Project Files 생성 중...
echo ==========================================
"%UBT_PATH%" -projectfiles -project="%UPROJECT_PATH%" -game -engine -progress
if %ERRORLEVEL% neq 0 (
    echo.
    echo [오류] Project Files 생성 실패!
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ==========================================
echo [2/3] 프로젝트 빌드(Compile) 시작... (Editor Win64 Development)
echo ==========================================
"%UBT_PATH%" %PROJECT_NAME%Editor Win64 Development -Project="%UPROJECT_PATH%" -WaitMutex -FromMsBuild
if %ERRORLEVEL% neq 0 (
    echo.
    echo [오류] 빌드 실패! 소스 코드를 확인하세요.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ==========================================
echo [3/3] 빌드 성공! 언리얼 에디터 실행 중...
echo ==========================================
start "" "%UPROJECT_PATH%"

echo 모든 작업이 완료되었습니다.이 창은 닫으셔도 됩니다.
timeout /t 5
endlocal