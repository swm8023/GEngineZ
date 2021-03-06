#include "Director.h"
#include "Input.h"
#include <Core/CoreHeader.h>
#include <Client/Main/App.h>
#include <Client/Scene/Camera.h>
#include <Client/Editor/CameraController.h>
#include <thread>
#include <chrono> 

namespace z {
Director* GDirector = nullptr;

Director::Director() {
	InitializeSingleton<Director>(GDirector, this);
	// size will changed when on resize called later
	mRenderer = new Renderer();

	LoadScene(GApp->GetContentPath() / "Test/Scene/test.scene");
	SetCameraController(new CameraController());
}

Director::~Director() {
	FinalizeSingleton<Director>(GDirector, this);
}

void Director::SetCameraController(CameraController* controller) {
	mCameraController = controller;
	controller->SetCamera(mCurScene->GetCamera());
}

void Director::LoadScene(const std::string& scnFile) {
	mCurScene = new Scene();
	mCurScene->Load(scnFile);
}


void Director::FrameTick() {
	// begin
	BeginFrame();
	// handle input
	GInput->Dispatch();
	// object tick
	mCameraController->Apply();
	mCurScene->Tick();

	// render tick
	mRenderer->Tick();
	mRenderer->Render();

	// end
	EndFrame();;
}

void Director::Update() {
	static uint64_t last_update_ms = ZTime::Now().TotalMs();
	uint64_t now = ZTime::Now().TotalMs();

	if (last_update_ms + mFrameInterval < now) {

		if (GApp->IsEditorUIEnable()) {
			GApp->EditorUINewFrame();
			GApp->GetUIManager()->NewFrame();
		}

		mFrameTime = (now - last_update_ms) / 1000.0f;
		last_update_ms = now;
		FrameTick();

		// render stats
		GDevice->GetAndClearStats(mRHIStats);

		mFramesForStatFps++;
		if (now > mFpsStatTime + 1000) {
			mCurFps = mFramesForStatFps * 1000.0f / (now - mFpsStatTime);
			mFpsStatTime = now;
			mFramesForStatFps = 0;
		}
	}

}


void Director::BeginFrame() {

}

void Director::EndFrame() {

}








}